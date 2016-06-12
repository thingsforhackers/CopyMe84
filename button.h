/*
 * button.h
 *
 *  Created on: 5 Jun 2016
 *      Author: njohn
 *
 *  A simple button debounce module
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include <stdint.h>

typedef enum
{
  BUTTON_UP,      //Button not pressed
  BUTTON_PRESSED, //Initial press
  BUTTON_DEBOUCE, //debounce period
  BUTTON_DOWN,    //After debounce
  BUTTON_HELD,     //After hold time,
  BUTTON_RELEASED
} ButtonState;

typedef struct
{
    volatile uint8_t * portAddr;
    uint8_t bitMask;
    ButtonState state;
    uint32_t time;
} ButtonCtx;

void initButtons(ButtonCtx* ctx, uint8_t count);
void updateButtons(ButtonCtx* ctx, uint8_t count);


#endif /* BUTTON_H_ */
