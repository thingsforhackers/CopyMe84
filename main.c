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
#include <stdlib.h>
#include <avr/pgmspace.h> // needed for PROGMEM
#include <util/delay.h>
#include "button.h"
#include "state.h"
#include "tune.h"
#include "notes.h"


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
#define MAX_LEVEL 8
static uint8_t sequence[MAX_LEVEL];

//Keeps track of time since
static volatile uint32_t ticksSincePOR; //Power on time in ms. ~49 Days max

//Snap of ticksSincePOR at time of wakeup
static volatile uint32_t _currentTime;
static uint8_t currentLevel;
static uint8_t currentStep;

#define MAX_DURATION ((uint32_t)398)
#define STEP_DURATION ((uint32_t)400)
#define REPEAT_STEP_TO 5000

uint16_t noteIdx;
uint32_t noteLen;
uint32_t noteDly;

static struct StateM mainSM;

uint32_t volatile toneStopTime;

volatile uint32_t toggle_count;



uint16_t noteArray3[] = {
    C4, E4, G4, C5, E5, G4, C5, E5
};



ISR(TIM1_COMPA_vect)
{
//  if(_currentTime >= toneStopTime)
//  {
//    TCCR1B = 0; //Stop Timer1
//  }
  if( toggle_count )
  {
    toggle_count--;
  }
  else
  {
    TCCR1B = 0; //Stop Timer1
  }
}

ISR(TIM0_COMPA_vect)
{
  ticksSincePOR++;
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
  //uint32_t stopTime = _currentTime + duration;
  toggle_count = duration;

  OCR1AH = valueHigh;
  OCR1AL = valueLow;
  TCCR1B = _BV(WGM12) | _BV(CS11);
//  //cli();
//  TCCR1B = 0;
//  TIFR1 = _BV(OCF1A);
//  OCR1AH = valueHigh;
//  OCR1AL = valueLow;
//  TCCR1A = _BV(COM1A0);
//  TCCR1B = _BV(WGM12) | _BV(CS11);
//  TIMSK1 |= _BV(OCIE1A);
//  toneStopTime = stopTime;
//  //sei();
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

    GATE_STATE_TITLE_TUNE,
    GATE_STATE_TITLE_TUNE2,

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

void generateSequence()
{
  for(int idx = 0; idx < MAX_LEVEL; idx++)
  {
    sequence[idx] = (uint8_t)(rand() % INDICATE_CNT);
  }

//  sequence[0] = INDICATE_GREEN;
//  sequence[1] = INDICATE_BLUE;
//  sequence[2] = INDICATE_BLUE;
//  sequence[3] = INDICATE_GREEN;
//  sequence[4] = INDICATE_RED;
//  sequence[5] = INDICATE_YELLOW;
//  sequence[6] = INDICATE_GREEN;
//  sequence[7] = INDICATE_RED;
}

#if 1
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
#else
static uint8_t getInput()
{
  uint8_t pressed;

  if((PINA & _BV(PA2)) == 0)
  {
      pressed = INDICATE_RED;
  }
  else if((PINA & _BV(PA3)) == 0)
  {
      pressed = INDICATE_BLUE;
  }
  else if((PINA & _BV(PA4)) == 0)
  {
      pressed = INDICATE_YELLOW;
  }
  else if((PINA & _BV(PA5)) == 0)
  {
      pressed = INDICATE_GREEN;
  }
  else
  {
    pressed = NO_KEY;
  }

  return pressed;
}
#endif
uint16_t led22;

static uint8_t mainStateFunc(struct StateM* sm, uint32_t currentTime)
{
  uint8_t retValue = 0;

  switch(sm->current)
  {
    case GAME_STATE_POR:
    {
      if(isStateEntered(sm))
      {
        currentLevel = 1;
        generateSequence();
      }
      else if(getStateDuration(sm, currentTime) > 2000)
      {
        sm->next = GAME_STATE_PRE_GAME;
        //sm->next = GATE_STATE_TITLE_TUNE;
      }
      break;
    }

    case GAME_STATE_PRE_GAME:
    {
      static uint8_t leds = 0;
      if(getInput() != NO_KEY)
      {
          sm->next = GAME_STATE_PLAY_SEQUENCE_INIT;
          leds = 0;
      }
      else if(getStateDuration(sm, currentTime) > 500)
      {
        sm->enterTime = currentTime;
        leds = leds ? 0 : _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
      }
      PORTB = leds;
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INIT:
    {
      if(getStateDuration(sm, currentTime) > 2000)
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
      if(getStateDuration(sm, currentTime) > STEP_DURATION)
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
      if( getStateDuration(sm, currentTime) > REPEAT_STEP_TO )
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
      if( getStateDuration(sm, currentTime) > STEP_DURATION )
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
      else if(getStateDuration(sm, currentTime) > 4000)
      {
        stepEnd();
        sm->next = GAME_STATE_POR;
      }
      break;
    }

//    case GATE_STATE_TITLE_TUNE2:
//    {
//      if(isStateEntered(sm))
//      {
//        noteIdx = 0;
//        noteLen = 150;
//        noteDly = 190;
//      }
//      for (unsigned int idx = 0; idx < TUNE_LENGTH; idx++)
//      {
//        uint16_t noteFreq = pgm_read_word(&(noteArray[idx]));
//        playNote(noteFreq, noteFreq * noteLen / 500);
//
//        targetT = ticksSincePOR + noteDly;
//        while(ticksSincePOR < targetT);
//
//        //_delay_ms(190);
//        if((noteIdx > 512) && (noteIdx < 540)) // slow down (rit.) at end.
//        {
//          noteLen += 3;
//          noteDly += 5;
//        }
//      }
//      sm->next = GAME_STATE_POR;
//      break;
//    }

    case GATE_STATE_TITLE_TUNE:
    {
      led22++;
      PORTB = (led22 >> 8) & 0xF;

      if(isStateEntered(sm))
      {
        noteIdx = 0;
        noteLen = 150;
        noteDly = 190;
      }
      else if(getStateDuration(sm, currentTime) > noteDly)
      {
        uint16_t noteFreq = pgm_read_word(&(noteArray[noteIdx]));
        //uint16_t noteFreq = noteArray[noteIdx];
        playNote(noteFreq, noteFreq * noteLen / 500);
        //if(getStateDuration(sm, currentTime) > noteDly)
        if((noteIdx > 512) && (noteIdx < 540)) // slow down (rit.) at end.
        {
          noteLen += 3;
          noteDly += 5;
        }
        noteIdx++;
        if (noteIdx == TUNE_LENGTH)
        {
          sm->next = GAME_STATE_POR;
        }
        else
        {
          sm->next = GATE_STATE_TITLE_TUNE;
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

//Main task code goes here
static void runTask()
{
  updateButtons(buttons, BUTTON_CNT, _currentTime);

#if 0
  uint8_t playerKey = getInput();

  uint8_t leds = 0;

  if(INDICATE_RED == playerKey)
  {
    leds = _BV(RED_LED);
  }
  else if(INDICATE_BLUE == playerKey)
  {
    leds = _BV(BLUE_LED);
  }
  else if(INDICATE_YELLOW == playerKey)
  {
    leds = _BV(YELLOW_LED);
  }
  else if(INDICATE_GREEN == playerKey)
  {
    leds = _BV(GREEN_LED);
  }

  PORTB = leds;
#endif

  runStateM(&mainSM, _currentTime);
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
  OCR0A = 128;
  TCCR0A = _BV(WGM01); //CTC mode
  TCNT0 = 0;
  TIFR0 = _BV(TOV0) | _BV(OCF0A) | _BV(OCF0B); //Clear TOV0
  TIMSK0 = _BV(OCIE0A); //Enable A match int
  TCCR0B = _BV(CS01);  // CLK/8

  TCCR1B = 0;
  TIFR1 = _BV(OCF1A);
  TCCR1A = _BV(COM1A0);
  TCCR1B = _BV(WGM12) | _BV(CS11);
  TIMSK1 |= _BV(OCIE1A);

  sei();

  //PORTB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);

  initStateM(&mainSM, mainStateFunc, _currentTime);

  initButtons(buttons, BUTTON_CNT);

  while(1)
  {
    sleep_mode();
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
      _currentTime = ticksSincePOR;
    }
    runTask();
  }

  return 0;
}
