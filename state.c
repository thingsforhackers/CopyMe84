/*
 * state.c
 *
 *  Created on: 8 Jun 2016
 *      Author: njohn
 */
#include "state.h"


void initStateM(StateM* state, SMFunc* handlerFunc, uint32_t currentTime)
{
  state->handlerFunc = handlerFunc;
  state->current = STATE_INVALID;
  state->previous = STATE_INVALID;
  runStateM(state, currentTime);
}

uint8_t runStateM(StateM* state, uint32_t currentTime)
{
  uint8_t retVal;

  state->next = STATE_INVALID;
  retVal = state->handlerFunc(state);
  state->previous = state->current;
  if( STATE_INVALID !=  state->next)
  {
    state->previous = state->current;
    state->current = state->next;
    state->enterTime = currentTime;
  }
  return retVal;
}
