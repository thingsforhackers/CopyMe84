/*
 * state.h
 *
 *  Created on: 8 Jun 2016
 *      Author: njohn
 */

#ifndef STATE_H_
#define STATE_H_
#include <stdint.h>


struct StateM;

static const uint8_t STATE_INVALID = 255;

typedef void *(SMFunc)(struct StateM* sm);

typedef struct
{
    uint8_t current;
    uint8_t previous;
    uint8_t next;
    uint32_t enterTime;
    SMFunc* handlerFunc;
} StateM;

void initStateM(StateM* state, SMFunc* handlerFunc, uint32_t currentTime);

uint8_t runStateM(StateM* state, uint32_t currentTime);

#endif /* STATE_H_ */
