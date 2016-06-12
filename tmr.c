/*
 * tmr.c
 *
 *  Created on: 12 Jun 2016
 *      Author: njohn
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tmr.h"

//Keeps track of time since
static volatile uint32_t tmr0Mills; //Power on time in ms. ~49 Days max

ISR(TIM0_COMPA_vect)
{
  tmr0Mills++;
}

void startTmr(void)
{
  //Timer below, assuming a 1MHz sys-clock, will give 976Hz TimrA int. Close
  //enough for a 1ms time base
  OCR0A = 128;
  TCCR0A = _BV(WGM01); //CTC mode
  TCNT0 = 0;
  TIFR0 = _BV(TOV0) | _BV(OCF0A) | _BV(OCF0B); //Clear TOV0
  TIMSK0 = _BV(OCIE0A); //Enable A match int
  TCCR0B = _BV(CS01);  // CLK/8
}


uint32_t millis(void)
{
  uint32_t m;
  uint8_t oldSREG = SREG;
  cli();
  m = tmr0Mills;
  SREG = oldSREG;
  return m;
}
