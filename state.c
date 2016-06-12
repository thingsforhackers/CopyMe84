/*
 * state.c
 *
 *  Created on: 8 Jun 2016
 *      Author: njohn
 */
#include "state.h"


void initStateM(struct StateM* state, SMFunc handlerFunc, uint32_t currentTime)
{
  state->handlerFunc = handlerFunc;
  state->current = STATE_INVALID;
  state->previous = STATE_INVALID;
  state->enterTime = currentTime;
  runStateM(state, currentTime);
}

uint8_t runStateM(struct StateM* state, uint32_t currentTime)
{
  uint8_t retVal;

  state->next = STATE_INVALID;
  retVal = state->handlerFunc(state, currentTime);
  state->previous = state->current;
  if( STATE_INVALID !=  state->next)
  {
    state->previous = state->current;
    state->current = state->next;
    state->enterTime = currentTime;
  }
  return retVal;
}

uint32_t getStateDuration(struct StateM* state, uint32_t currentTime)
{
  return currentTime - state->enterTime;
}

uint8_t isStateEntered(struct StateM* state)
{
  return state->current != state->previous;
}
