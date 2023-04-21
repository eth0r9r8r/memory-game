/* 
 * File:   TimeDelay.c
 * Author: Ethan Reed
 *
 * Created on October 12, 2022, 4:30 PM
 */

#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>

// Global variables
int TMR2flag = 0;

// Sets timer 2 to run in a loop until interrupted by input. When interrupted,
// the function returns the current value of the TMR2 register, to be used as a
// seed for a random number generator.
unsigned int get_seed(void)
{
    TMR2flag = 1;
    while(TMR2flag){
        TMR2flag = 0; // Reset flag
        TMR2 = 0; // Reset TMR2
        PR2 = 15625; // Set PR2
        T2CONbits.TON = 1;  // Activate timer 2
        Idle();
    }
    unsigned int seed = TMR2;
    return seed;
}


// Sets a timer for (time_ms) milliseconds, then enters idle mode.
// Returns 1 if idle was interrupted by timer, 0 if interrupted by input change.
int delay_ms(uint16_t time_ms)
{
    TMR2flag = 0;
    
    // Reset TMR2 and calculate PR2
    TMR2 = 0;
    PR2 = 15.625 * time_ms;
    T2CONbits.TON = 1;  // Activate timer 2
    
    Idle();  // Wait for interrupt
    
    // Return 0 if timer was interrupted before finishing, else return 1
    if(TMR2flag == 1){
        TMR2flag = 0;
        return 1;
    }
    return 0;
}

// Initialize Timer 2
void TMR2init(void)
{
    // Configure Timer 2
    T2CONbits.T32   = 0;  // Use timer 2 as 16-bit timer
    T2CONbits.TCKPS = 3;  // Set pre-scaler to 1:256
    T2CONbits.TCS   = 0;  // Use internal clock
    T2CONbits.TGATE = 0;  // Disable gated time accumulation 
    T2CONbits.TSIDL = 0;  // Set to continue in idle mode
    
    // Configure interrupts
    IPC1bits.T2IP = 4;  // Set timer 2 interrupt priority to 4
    IFS0bits.T2IF = 0;  // Clear timer 2 interrupt flag
    IEC0bits.T2IE = 1;  // Enable timer 2 interrupts
}

void __attribute__((interrupt, no_auto_psv))_T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 0;
    TMR2flag = 1;
}
