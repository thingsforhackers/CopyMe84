/*
 * CopyMe84A
 *
 * Pin assignments
 *
 * Colour		LED			Switch   PCINT
 * ======   ======  =======  =====
 * Red			PB0(2)	PA2(11)  PCINT2
 * Blue			PB1(3)	PA3(10)  PCINT3
 * Yellow		PB3(4)	PA4(9)   PCINT4
 * Green		PB2(5)	PA5(8)   PCINT5
 *
 * 	Speaker: PA6(7)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <avr/pgmspace.h> // needed for PROGMEM
#include <util/delay.h>
#include <avr/wdt.h>
#include "button.h"
#include "state.h"
#include "tune.h"
#include "notes.h"
#include "tmr.h"

static const uint8_t RED_LED = PB0;
static const uint8_t BLUE_LED = PB1;
static const uint8_t YELLOW_LED = PB3;
static const uint8_t GREEN_LED = PB2;

#define RED_SWITCH PA2
#define BLUE_SWITCH PA3
#define YELLOW_SWITCH PA4
#define GREEN_SWITCH PA5

//Blue
#define NOTE_G4 ((uint16_t)392)
//Yellow
#define NOTE_C4 ((uint16_t)277)
//Red
#define NOTE_A4 ((uint16_t)440)
//Green
#define NOTE_G5 ((uint16_t)784)

static const uint8_t SPEAKER = PA6;
#define MAX_LEVEL 64
static uint8_t sequence[MAX_LEVEL];



//Snap of ticksSincePOR at time of wakeup
static uint8_t currentLevel;
static uint8_t currentStep;

#define MAX_DURATION ((uint32_t)398)
#define STEP_DURATION ((uint32_t)400)
#define REPEAT_STEP_TO 5000

uint16_t noteIdx;
uint32_t noteLen;
uint32_t noteDly;

static struct StateM mainSM;

volatile uint32_t toneStopTime;

volatile uint32_t toggle_count;

uint16_t noteArray3[] = {
    C4, E4, G4, C5, E5, G4, C5, E5
};


volatile uint16_t led22;

ISR(TIM1_COMPA_vect)
{
  if(millis() >= toneStopTime)
  {
    TCCR1B = 0; //Stop Timer1
  }
}

void enterDeepSleep(void)
{
  PCMSK0 |= _BV(PCINT2) | _BV(PCINT3) | _BV(PCINT4) | _BV(PCINT5);
  GIMSK |= _BV(PCIE0);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei();
  sleep_cpu();
  cli();
  GIMSK &= ~_BV(PCIE0);
  wdt_enable(WDTO_15MS);
  while(1);
}
EMPTY_INTERRUPT(PCINT0_vect);

uint8_t isNoteFinished(void)
{
  return TCCR1B == 0;
}

// 1MHz clk, preScale 8, /2 for toggle
#define TMR1_FREQ ((uint16_t)( (F_CPU / 8) / 2 ))
// note in Hz
// duration in ms
void playNote(uint16_t note, uint32_t duration)
{
  uint16_t cmpValue = TMR1_FREQ / note;

  uint8_t valueHigh = (cmpValue >> 8) & 0xFF;
  uint8_t valueLow = cmpValue & 0xFF;

  cli();
  TIFR1 = _BV(OCF1A);
  toneStopTime = millis() + duration;
  toggle_count = duration;
  OCR1AH = valueHigh;
  OCR1AL = valueLow;
  TCCR1B = _BV(WGM12) | _BV(CS11);
  sei();
}

void stopNote()
{
  cli();
  TCCR1B = 0;
  TIFR1 = _BV(OCF1A);
  sei();
}

typedef enum
{
    GAME_STATE_POR,                           //0 - Power on reset

    GAME_STATE_PRE_GAME,                      //1
    GAME_STATE_PLAY_SEQUENCE_INIT,            //2
    GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP,   //3
    GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY,  //4

    GAME_STATE_CHECK_SEQUENCE_PAUSE,          //5 - Delay before start
    GAME_STATE_CHECK_SEQUENCE,                //6 - Player plays back sequence
    GAME_STATE_CHECK_SEQUENCE_INDICATE,       //7
    GAME_STATE_GAME_OVER,                      //8

    GAME_STATE_TITLE_TUNE_START,
    GAME_STATE_TITLE_TUNE_PLAY_NOTE,
    GAME_STATE_TITLE_TUNE_HOLD_NOTE


} GameState;

typedef enum INDICATION_ID
{
  INDICATE_RED,
  INDICATE_BLUE,
  INDICATE_YELLOW,
  INDICATE_GREEN,

  INDICATE_CNT

} INDICATION_ID;
static const uint8_t NO_KEY = INDICATE_CNT;

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
    {&PINA, RED_SWITCH, BUTTON_UP, 0},
    //BLUE_BUTTON_IDX,
    {&PINA, BLUE_SWITCH, BUTTON_UP, 0},
    //YELLOW_BUTTON_IDX,
    {&PINA, YELLOW_SWITCH, BUTTON_UP, 0},
    //GREEN_BUTTON_IDX,
    {&PINA, GREEN_SWITCH, BUTTON_UP, 0},
};

static void indicateFail()
{
  playNote(150, 3000);
  PORTB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
}

static void stepEnd()
{
  stopNote();
  PORTB = 0;
}

static void doIndicateStep(uint8_t step)
{

  switch(sequence[step])
  {
    case INDICATE_RED:
    {
      playNote(NOTE_A4, MAX_DURATION);
      PORTB |= _BV(RED_LED);
      break;
    }

    case INDICATE_BLUE:
    {
      playNote(NOTE_G4, MAX_DURATION);
      PORTB |= _BV(BLUE_LED);
      break;
    }

    case INDICATE_YELLOW:
    {
      playNote(NOTE_C4, MAX_DURATION);
      PORTB |= _BV(YELLOW_LED);
      break;
    }

    case INDICATE_GREEN:
    {
      playNote(NOTE_G5, MAX_DURATION);
      PORTB |= _BV(GREEN_LED);
      break;
    }
  }
}

static void generateSequence(void)
{
  for(int idx = 0; idx < MAX_LEVEL; idx++)
  {
    sequence[idx] = (uint8_t)(rand() % INDICATE_CNT);
  }
}

static uint8_t getInput()
{
  uint8_t pressed;

  if(buttons[RED_BUTTON_IDX].state == BUTTON_DOWN)
  {
      pressed = INDICATE_RED;
  }
  else if(buttons[BLUE_BUTTON_IDX].state == BUTTON_DOWN)
  {
      pressed = INDICATE_BLUE;
  }
  else if(buttons[YELLOW_BUTTON_IDX].state == BUTTON_DOWN)
  {
      pressed = INDICATE_YELLOW;
  }
  else if(buttons[GREEN_BUTTON_IDX].state == BUTTON_DOWN)
  {
      pressed = INDICATE_GREEN;
  }
  else
  {
    pressed = NO_KEY;
  }

  return pressed;
}

static uint8_t mainStateFunc(struct StateM* sm)
{
  uint8_t retValue = 0;

  switch(sm->current)
  {
    case GAME_STATE_POR:
    {
      if(isStateEntered(sm))
      {
        currentLevel = 1;
      }
      else if(getStateDuration(sm) > 2000)
      {
        //sm->next = GAME_STATE_PRE_GAME;
        sm->next = GAME_STATE_TITLE_TUNE_START;
      }
      break;
    }

    case GAME_STATE_PRE_GAME:
    {
      static uint8_t leds = 0;
      if(getInput() != NO_KEY)
      {
        srand(millis());
        generateSequence();
        sm->next = GAME_STATE_PLAY_SEQUENCE_INIT;
        leds = 0;
      }
      else if(getStateDuration(sm) > 500)
      {
        sm->enterTime = millis();
        leds = leds ? 0 : _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
      }
      PORTB = leds;
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INIT:
    {
      if(getStateDuration(sm) > 2000)
      {
        currentStep = 0;
        sm->next = GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP;
      }
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP:
    {
      doIndicateStep(currentStep);
      sm->next = GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY;
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY:
    {
      if(getStateDuration(sm) > STEP_DURATION)
      {
        stepEnd();
        currentStep++;
        if(currentStep >= currentLevel)
        {
          //All done, now handle player repeating this level
#if 0
          sm->next = GAME_STATE_CHECK_SEQUENCE_PAUSE;
#else
          currentStep = 0;
          sm->next = GAME_STATE_CHECK_SEQUENCE;
//          currentLevel++;
//          sm->next = GAME_STATE_PLAY_SEQUENCE_INIT;
#endif
        }
        else
        {
          //Play next step in this sequence
          sm->next = GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP;
        }
      }
      break;
    }

#if 0
    case GAME_STATE_CHECK_SEQUENCE_PAUSE:
    {
      if( getStateDuration(sm, currentTime) > 1000 )
      {
        currentStep = 0;
        sm->next = GAME_STATE_CHECK_SEQUENCE;
      }
      break;
    }
#endif

    case GAME_STATE_CHECK_SEQUENCE:
    {
      if( getStateDuration(sm) > REPEAT_STEP_TO )
      {
        sm->next = GAME_STATE_GAME_OVER;
      }
      else
      {
        uint8_t playerKey = getInput();

        if(playerKey != NO_KEY)
        {
          if(sequence[currentStep] == playerKey)
          {
            doIndicateStep(currentStep);
            sm->next = GAME_STATE_CHECK_SEQUENCE_INDICATE;
          }
          else
          {
            sm->next = GAME_STATE_GAME_OVER;
          }
        }
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE_INDICATE:
    {
      if( getStateDuration(sm) > STEP_DURATION )
      {
        stepEnd();
        currentStep++;
        if(currentStep >= currentLevel)
        {
          //All done, now handle player repeating this level
          currentLevel++;
          if(currentLevel > MAX_LEVEL )
          {
            sm->next = GAME_STATE_GAME_OVER;
          }
          else
          {
            sm->next = GAME_STATE_PLAY_SEQUENCE_INIT;
          }
        }
        else
        {
          //Play next step in this sequence
          sm->next = GAME_STATE_CHECK_SEQUENCE;
        }
      }
      break;
    }

    case GAME_STATE_GAME_OVER:
    {
      if(isStateEntered(sm))
      {
        indicateFail();
      }
      else if(getStateDuration(sm) > 4000)
      {
        stepEnd();
        sm->next = GAME_STATE_POR;
      }
      break;
    }

    case GAME_STATE_TITLE_TUNE_START:
    {
      noteIdx = 0;
      noteLen = 150;
      noteDly = 190;
      sm->next = GAME_STATE_TITLE_TUNE_PLAY_NOTE;
      break;
    }

    case GAME_STATE_TITLE_TUNE_PLAY_NOTE:
    {
      if(getInput() == NO_KEY)
      {
        uint16_t noteFreq = pgm_read_word(&(noteArray[noteIdx]));
        playNote(noteFreq, noteFreq * noteLen / 500);
        sm->next = GAME_STATE_TITLE_TUNE_HOLD_NOTE;
      }
      else
      {
        stopNote();
        sm->next = GAME_STATE_PRE_GAME;
      }
      break;
    }

    case GAME_STATE_TITLE_TUNE_HOLD_NOTE:
    {
      if(isNoteFinished() && getStateDuration(sm) > noteDly)
      {
        if((noteIdx > 512) && (noteIdx < 540)) // slow down (rit.) at end.
        {
          noteLen += 3;
          noteDly += 5;
        }
        noteIdx++;
        if (noteIdx == TUNE_LENGTH)
        {
          sm->next = GAME_STATE_POR;
          enterDeepSleep();
        }
        else
        {
          sm->next = GAME_STATE_TITLE_TUNE_PLAY_NOTE;
        }
      }

      break;
    }

    case STATE_INVALID:
    default:
    {
      sm->next = GAME_STATE_POR;
      break;
    }
  }

  return retValue;
}

int main (void)
{
  MCUSR = 0;
  wdt_disable();

  //Setup Pins
  DDRA = _BV(SPEAKER); //Unused as inputs
  PORTA = ~(_BV(SPEAKER)); //Unused pull high as well
  DDRB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
  PORTB = 0;

  startTmr();

  ACSR = _BV(ACD); //Don't need ADC

  TCCR1B = 0;
  TIFR1 = _BV(OCF1A);
  TCCR1A = _BV(COM1A0);
  TCCR1B = _BV(WGM12) | _BV(CS11);
  TIMSK1 |= _BV(OCIE1A);

  sei();

  initButtons(buttons, BUTTON_CNT);
  initStateM(&mainSM, mainStateFunc);

  while(1)
  {
    sleep_mode();
    updateButtons(buttons, BUTTON_CNT);
    runStateM(&mainSM);
  }

  return 0;
}
