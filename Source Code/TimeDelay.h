/* 
 * File:   TimeDelay.h
 * Author: Ethan Reed
 *
 * Created on October 12, 2022, 4:30 PM
 */

#ifndef TIMEDELAY_H
#define	TIMEDELAY_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include "stdint.h"

int delay_ms(uint16_t);
void TMR2init(void);
unsigned int get_seed(void);


#endif	/* TIMEDELAY_H */

