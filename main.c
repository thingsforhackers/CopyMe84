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
#define MAX_LEVEL 8
static uint8_t sequence[MAX_LEVEL];

//Keeps track of time since
static volatile uint32_t ticksSincePOR; //Power on time in ms. ~49 Days max

//Snap of ticksSincePOR at time of wakeup
static uint32_t _currentTime;
static uint8_t currentLevel;
static uint8_t currentStep;
#define STEP_DURATION 333
#define REPEAT_STEP_TO 5000

static struct StateM mainSM;


ISR(TIM0_COMPA_vect)
{
  ticksSincePOR++;
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
    GAME_STATE_GAME_OVER                      //8
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
    {&PINA, PA2, BUTTON_UP, 0},
    //BLUE_BUTTON_IDX,
    {&PINA, PA3, BUTTON_UP, 0},
    //YELLOW_BUTTON_IDX,
    {&PINA, PA4, BUTTON_UP, 0},
    //GREEN_BUTTON_IDX,
    {&PINA, PA5, BUTTON_UP, 0},
};



static void indicateFail()
{
  //tone(SPEAKER_PIN, 150);
//  digitalWrite(RED_LED_PIN, HIGH);
//  digitalWrite(GREEN_LED_PIN, HIGH);
//  digitalWrite(BLUE_LED_PIN, HIGH);
//  digitalWrite(YELLOW_LED_PIN, HIGH);
  PORTB = _BV(RED_LED) | _BV(BLUE_LED) | _BV(YELLOW_LED) | _BV(GREEN_LED);
}

static void stepEnd()
{
  //noTone(SPEAKER_PIN);
  PORTB = 0;
}

static void doIndicateStep(uint8_t step)
{

  switch(sequence[step])
  {
    case INDICATE_RED:
    {
      //tone(SPEAKER_PIN, NOTE_A4);
      //digitalWrite(RED_LED_PIN, HIGH);
      PORTB |= _BV(RED_LED);
      break;
    }

    case INDICATE_BLUE:
    {
      //tone(SPEAKER_PIN, NOTE_G4);
      //digitalWrite(BLUE_LED_PIN, HIGH);
      PORTB |= _BV(BLUE_LED);
      break;
    }

    case INDICATE_YELLOW:
    {
      //tone(SPEAKER_PIN, NOTE_C4);
      //digitalWrite(YELLOW_LED_PIN, HIGH);
      PORTB |= _BV(YELLOW_LED);
      break;
    }

    case INDICATE_GREEN:
    {
      //tone(SPEAKER_PIN, NOTE_G5);
      //digitalWrite(GREEN_LED_PIN, HIGH);
      PORTB |= _BV(GREEN_LED);
      break;
    }
  }
}

void generateSequence()
{
//  Serial.println("Sequence");
  for(int idx = 0; idx < MAX_LEVEL; idx++)
  {
    sequence[idx] = (uint8_t)(rand() % INDICATE_CNT);
//    Serial.print(idx);
//    Serial.print(" ");
//    Serial.println(sequence[idx]);
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

static uint8_t mainStateFunc(struct StateM* sm, uint32_t currentTime)
{
  uint8_t retValue = 0;

  switch(sm->current)
  {
    case GAME_STATE_POR:
    {
      currentLevel = 1;
      generateSequence();
      sm->next = GAME_STATE_PRE_GAME;
      break;
    }

    case GAME_STATE_PRE_GAME:
    {
      static uint8_t leds = 0;
      if(isStateEntered(sm))
      {
        //?
      }
      else
      {
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
      }
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INIT:
    {
      if(getStateDuration(sm, currentTime) > 2000)
      {
//        Serial.print("Level: ");
//        Serial.println(currentLevel);
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
          sm->next = GAME_STATE_CHECK_SEQUENCE_PAUSE;
        }
        else
        {
          //Play next step in this sequence
          sm->next = GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP;
        }
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE_PAUSE:
    {
      if( getStateDuration(sm, currentTime) > 1000 )
      {
        currentStep = 0;
        sm->next = GAME_STATE_CHECK_SEQUENCE;
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE:
    {
      if( getStateDuration(sm, currentTime) > REPEAT_STEP_TO )
      {
        //Serial.println("Too slow!!!");
        sm->next = GAME_STATE_GAME_OVER;
      }
      else
      {
        uint8_t playerKey = getInput();

        if(playerKey != NO_KEY)
        {
//          Serial.print("Key: ");
//          Serial.println(playerKey);

          if(sequence[currentStep] == playerKey)
          {
            doIndicateStep(currentStep);
            sm->next = GAME_STATE_CHECK_SEQUENCE_INDICATE;
          }
          else
          {
//            Serial.println("Wrong Key!!!");
//            Serial.print("Excpected: ");
//            Serial.println(sequence[currentStep]);
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
//            Serial.println("You won!!");
            sm->next = GAME_STATE_GAME_OVER;
          }
          else
          {
//            Serial.println("Level complete!!");
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
  OCR0A = 4;
  TCCR0A = _BV(WGM01); //CTC mode
  TCNT0 = 0;
  TIFR0 = _BV(TOV0) | _BV(OCF0A) | _BV(OCF0B); //Clear TOV0
  TIMSK0 = _BV(OCIE0A); //Enable A match int
  TCCR0B = _BV(CS02);  // CLK/256
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
