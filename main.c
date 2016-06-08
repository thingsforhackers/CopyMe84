/*
 * CopyMe84A
 *
 * Pin assignments
 *
 * Colour		LED			Switch
 * ======       ======      =======
 * Red			PB0(2)		PA2(11)
 * Blue			PB1(3)		PA3(10)
 * Yellow		PB3(4)		PA4(9)
 * Green		PB2(5)		PA5(8)
 *
 * 	Speaker: PA6(7)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include "button.h"
#include "state.h"


static const uint8_t RED_LED = PB0;
static const uint8_t BLUE_LED = PB1;
static const uint8_t YELLOW_LED = PB3;
static const uint8_t GREEN_LED = PB2;

static const uint8_t RED_SWITCH = PA2;
static const uint8_t BLUE_SWITCH = PA3;
static const uint8_t YELLOW_SWITCH = PA4;
static const uint8_t GREEN_SWITCH = PA5;

static const uint8_t SPEAKER = PA6;

//Keeps track of time since
static volatile uint32_t ticksSincePOR; //Power on time in ms. ~49 Days max

//Snap of ticksSincePOR at time of wakeup
static uint32_t currentTime;

ISR(TIM0_COMPA_vect)
{
  ticksSincePOR++;
}

typedef enum
{
  RED_BUTTON_IDX,
  BLUE_BUTTON_IDX,
  YELLOW_BUTTON_IDX,
  GREEN_BUTTON_IDX,

  BUTTON_CNT

} ButtonIdx;
static ButtonCtx buttons[BUTTON_CNT] =
{
    //RED_BUTTON_IDX
    {&PINA, PA2, BUTTON_UP, 0},
    //BLUE_BUTTON_IDX,
    {&PINA, PA3, BUTTON_UP, 0},
    //YELLOW_BUTTON_IDX,
    {&PINA, PA4, BUTTON_UP, 0},
    //GREEN_BUTTON_IDX,
    {&PINA, PA5, BUTTON_UP, 0},
};

//Main task code goes here
static void runTask()
{
  updateButtons(buttons, BUTTON_CNT, currentTime);

  if(buttons[RED_BUTTON_IDX].state == BUTTON_DOWN)
  {
    PORTB |= _BV(RED_LED);
  }
  else
  {
    PORTB &= ~_BV(RED_LED);
  }
}

int main (void)
{

  //Setup Pins
  DDRA = _BV(SPEAKER); //Unused as inputs
  PORTA = ~(_BV(SPEAKER)); //Unused pull high as well
  DDRB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
  PORTB = 0;

  ACSR = _BV(ACD); //Don't need ADC

  //Timer below, assuming a 1MHz sys-clock, will give 976Hz TimrA int. Close
  //enough for a 1ms time base
  OCR0A = 4;
  TCCR0A = _BV(WGM01); //CTC mode
  TCNT0 = 0;
  TIFR0 = _BV(TOV0) | _BV(OCF0A) | _BV(OCF0B); //Clear TOV0
  TIMSK0 = _BV(OCIE0A); //Enable A match int
  TCCR0B = _BV(CS02);  // CLK/256
  sei();

  //PORTB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);

  initButtons(buttons, BUTTON_CNT);

  while(1)
  {
    sleep_mode();
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      currentTime = ticksSincePOR;
    }
    runTask();
  }

  return 0;
}
