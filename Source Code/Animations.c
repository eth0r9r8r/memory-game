/* 
 * File:   Animations.c
 * Author: Ethan Reed
 *
 * Created on November 4, 2022, 5:11 PM
 */

#include "stdlib.h"
#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include "UART2.h"

extern int CNflag;

void osc_wait(void)
{
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;
    CNflag = 0;
    while(1){
        LATBbits.LATB8 = 1;
        delay_ms(200);
        LATBbits.LATB8 = 0;
        if(CNflag == 1) break;
        LATBbits.LATB9 = 1;
        delay_ms(40);
        LATBbits.LATB9 = 0;
        if(CNflag == 1) break;
        LATBbits.LATB7 = 1;
        delay_ms(200);
        LATBbits.LATB7 = 0;
        if(CNflag == 1) break;
        LATBbits.LATB9 = 1;
        delay_ms(40);
        LATBbits.LATB9 = 0;
        if(CNflag == 1) break;
    }
    CNflag = 0;
    IEC1bits.CNIE = 0;
    return;
}

void disp_s(uint8_t *s, uint8_t l)
{
    int delay = 750 - 50 * l;
    if(delay < 100) delay = 100;
    for(uint8_t i = 0; i < l; i++){
        delay_ms(delay);
        if(s[i]==0){
            XmitUART2('1',1);
            LATBbits.LATB8 = 1;
            delay_ms(delay);
            clearTerm(1);
            LATBbits.LATB8 = 0;
            continue;
        }
        if(s[i]==1){
            XmitUART2('2',1);
            LATBbits.LATB9 = 1;
            delay_ms(delay);
            clearTerm(1);
            LATBbits.LATB9 = 0;
            continue;
        }
        if(s[i]==2){
            XmitUART2('3',1);
            LATBbits.LATB7 = 1;
            delay_ms(delay);
            clearTerm(1);
            LATBbits.LATB7 = 0;
            continue;
        }
    }
}

void blink(void)
{
    for(int i = 0; i < 5; i++){
        LATBbits.LATB7 = 1 - LATBbits.LATB7;
        LATBbits.LATB8 = 1 - LATBbits.LATB8;
        LATBbits.LATB9 = 1 - LATBbits.LATB9;
        delay_ms(200);
    }
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
}

void you_died(void)
{
    LATBbits.LATB7 = 1;
    LATBbits.LATB8 = 1;
    LATBbits.LATB9 = 1;
    delay_ms(300);
    LATBbits.LATB9 = 0;
    delay_ms(300);
    for(int i = 0; i < 5; i++){
        LATBbits.LATB7 = 1 - LATBbits.LATB7;
        LATBbits.LATB8 = 1 - LATBbits.LATB8;
        LATBbits.LATB9 = 1 - LATBbits.LATB9;
        delay_ms(300);
    }
    LATBbits.LATB7 = 0;
    LATBbits.LATB8 = 0;
    LATBbits.LATB9 = 0;
    delay_ms(1000);
}