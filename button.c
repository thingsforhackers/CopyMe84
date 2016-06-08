/*
 * button.c
 *
 *  Created on: 5 Jun 2016
 *      Author: njohn
 */
#include <avr/io.h>
#include "button.h"

static const uint32_t DEBOUNCE_TIME = 20; //ms
static const uint32_t HOLD_TIME = 2000; //ms

void initButtons(ButtonCtx* ctx, uint8_t count)
{
  for( uint8_t idx = 0; idx < count; idx++)
  {
    ctx[idx].state = BUTTON_UP;
  }
}

void updateButtons(ButtonCtx* buttons, uint8_t count, uint32_t currentTime)
{
  for( uint8_t idx = 0; idx < count; idx++)
  {
    ButtonCtx* button = &buttons[idx];
    const uint8_t pressed = *button->portAddr & _BV(button->bitMask) ? 0 : 1;
    switch(button->state)
    {
      case BUTTON_UP:
      {
        if(pressed)
        {
          button->time = currentTime;
          button->state = BUTTON_PRESSED;
        }
        break;
      }

      case BUTTON_PRESSED:
      {
        button->state = BUTTON_DEBOUCE;
        break;
      }

      case BUTTON_DEBOUCE:
      {
          if( (currentTime - button->time) > DEBOUNCE_TIME )
          {
              if(pressed)
              {
                  button->time = currentTime;
                  button->state = BUTTON_DOWN;
              }
              else
              {
                  button->state = BUTTON_UP;
              }
          }
          break;
      }

      case BUTTON_DOWN:
      {
          if(pressed)
          {
              if( (currentTime - button->time) > HOLD_TIME )
              {
                  button->state = BUTTON_HELD;
              }
          }
          else
          {
              button->state = BUTTON_RELEASED;
          }
          break;
      }

      case BUTTON_HELD:
      {
          if(!pressed)
          {
              button->state = BUTTON_UP;
          }
          break;
      }

      case BUTTON_RELEASED:
      {
          button->state = BUTTON_UP;
          break;
      }
    }
  }
}
