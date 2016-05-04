#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>

//Keeps track of time since
static volatile uint32_t ticksSincePOR; //Power on time in ms. ~49 Days max

//Snap of ticksSincePOR at time of wakeup
static uint32_t currentTime;

ISR(TIM0_COMPA_vect)
{
  ticksSincePOR++;
}

//Main task code goes here
void runTask()
{

}


int main (void)
{
  DDRA = _BV(PINA6);
  ACSR = _BV(ACD);

  OCR0A = 4;
  TCCR0A = _BV(WGM11);
  TIFR0 = _BV(TOV0);
  TIMSK0 = _BV(OCIE0A);
  TCCR0B = _BV(CS02);  // CLK/256

  sei();

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
