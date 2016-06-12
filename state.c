/*
 * state.c
 *
 *  Created on: 8 Jun 2016
 *      Author: njohn
 */
#include "state.h"
#include "tmr.h"


void initStateM(struct StateM* state, SMFunc handlerFunc)
{
  state->handlerFunc = handlerFunc;
  state->current = STATE_INVALID;
  state->previous = STATE_INVALID;
  state->enterTime = millis();
  runStateM(state);
}

uint8_t runStateM(struct StateM* state)
{
  uint8_t retVal;

  state->next = STATE_INVALID;
  retVal = state->handlerFunc(state);
  state->previous = state->current;
  if( STATE_INVALID !=  state->next)
  {
    state->previous = state->current;
    state->current = state->next;
    state->enterTime = millis();
  }
  return retVal;
}

uint32_t getStateDuration(struct StateM* state)
{
  return millis() - state->enterTime;
}

uint8_t isStateEntered(struct StateM* state)
{
  return state->current != state->previous;
}
