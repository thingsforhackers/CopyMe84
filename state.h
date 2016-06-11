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

#define STATE_INVALID 255

typedef uint8_t (*SMFunc)(struct StateM* sm, uint32_t currentTime);

struct StateM
{
    uint8_t current;
    uint8_t previous;
    uint8_t next;
    uint32_t enterTime;
    SMFunc handlerFunc;
};

void initStateM(struct StateM* state, SMFunc handlerFunc, uint32_t currentTime);

uint8_t runStateM(struct StateM* state, uint32_t currentTime);

uint32_t getStateDuration(struct StateM* state, uint32_t currentTime);

uint8_t isStateEntered(struct StateM* state);

#endif /* STATE_H_ */
