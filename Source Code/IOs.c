/* 
 * File:   IOs.c
 * Author: Ethan Reed
 *
 * Created on October 12, 2022, 4:28 PM
 */

#include "xc.h"
#include "TimeDelay.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>

// Global variables
int CNflag = 0;

// Initializes IO
void IOinit(void)
{
    AD1PCFG = 0xFFFF;  // Turn all analog pins to digital
    
    TRISAbits.TRISA4  = 1;  // Set RA4(CN0) to input
    CNPU1bits.CN0PUE  = 1;  // Enable pull-up resistor on CN0
    CNEN1bits.CN0IE   = 1;  // Enable interrupts on CN0
    
    TRISAbits.TRISA2  = 1;  // Set RA2(CN30) to input
    CNPU2bits.CN30PUE = 1;  // Enable pull-up resistor on CN30
    CNEN2bits.CN30IE  = 1;  // Enable interrupts on CN30
    
    TRISBbits.TRISB4  = 1;  // Set RB4(CN1) to input
    CNPU1bits.CN1PUE  = 1;  // Enable pull-up resistor on CN1
    CNEN1bits.CN1IE   = 1;  // Enable interrupts on CN1
    
    TRISBbits.TRISB8  = 0;  // Set RB8 to output;
    TRISBbits.TRISB9  = 0;  // Set RB9 to output;
    TRISBbits.TRISB7  = 0;  // Set RB7 to output;
    
    IPC4bits.CNIP = 5;  // Set CN interrupt priority to 5
    IFS1bits.CNIF = 0;  // Clear CN interrupt flag
    IEC1bits.CNIE = 1;  // Enable CN interrupts
}

// Returns 1 if a CN interrupt is a result of a switch bounce or noise
int isBounce(void)
{
    CNflag = 0;
    
    // If inputs are not stable throughout loop return 1
    if(delay_ms(100)) return 0;
    
    // If inputs are unchanged throughout loop return 0
    return 1;
}

// Checks status of inputs after an input change is detected.
// Pulses output LED accordingly.
uint8_t IOcheck(void)
{
    if(isBounce()) return IOcheck();
    
    if(PORTAbits.RA2 == 0) return 1;
    if(PORTAbits.RA4 == 0) return 2;
    if(PORTBbits.RB4 == 0) return 3;
    return 0;
}

//ISR
void __attribute__((interrupt, no_auto_psv))_CNInterrupt(void)
{
    CNflag = 1;
    T2CONbits.TON = 0;
    IFS1bits.CNIF = 0;
}