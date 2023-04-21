/* 
 * File:   main.c
 * Author: Ethan Reed
 *
 * Created on October 9, 2022, 7:46 PM
 */

#include "time.h"
#include "Animations.h"
#include "stdlib.h"
#include "string.h"
#include "xc.h"
#include "IOs.h"
#include "TimeDelay.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include "UART2.h"


//// CONFIGURATION BITS - PRE-PROCESSOR DIRECTIVES ////

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = OFF // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 

// GLOBAL VARIABLES
extern int CNflag;

// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}
      
int main(void)
{
    // Initialize IO
    IOinit();
    
    // Initialize Timer 2
    TMR2init();
    
    // Initialize UART2
    InitUART2();
    
    unsigned int carriage;
    
    // Menu screen
    clearTerm(41);
    carriage = Disp2String("-- ECHO -- (Press any button to continue)");
    
    // Generate seed
    srand(get_seed());
    CNflag = 0;
    
    // Clear terminal
    clearTerm(carriage);
    
    // Wait for player to release button
    if(!CNflag) Idle();
    while(IOcheck());
   
    
    IEC1bits.CNIE = 0;  // Disable CN interrupts
    
    uint8_t sequence[100];
    uint8_t i, level;
    uint8_t guess;
    while(1)
    {
        // Generate sequence
        for(i = 0; i < 100; i++){
            sequence[i] = rand() % 3;
        }
        // Wait for input then pause
        clearTerm(carriage);
        carriage = Disp2String("Press any button to begin");
        osc_wait();
        clearTerm(carriage);
        delay_ms(1000);
        // Indicate start of game
        carriage = Disp2String("Here we go!!!");
        blink();
        
        // Start game
        for(level = 1; level <= 100; level++){
            clearTerm(carriage);
            // display level sequence
            disp_s(sequence, level);
            // Enable CN interrupts
            CNflag = 0;
            IFS1bits.CNIF = 0;
            IEC1bits.CNIE = 1;
            
            // Checks player's input
            for(i = 0; i < level; i++){
                while(1){
                    Idle();
                    if(!CNflag) continue;
                    guess = IOcheck();
                    if(!guess) continue;
                    break;
                }
                // if right, check next input
                if(guess - 1 == sequence[i])
                    continue;
                // else:
                IEC1bits.CNIE = 0; // Disable CN interrupts
                clearTerm(carriage);
                carriage = Disp2String("YOU DIED.");
                you_died(); // Display failure animation
                break;
            }
            if(IEC1bits.CNIE == 0) break;
            // Disable CN interrupts
            IEC1bits.CNIE = 0;
            clearTerm(carriage);
            carriage = Disp2String("Nice one!!!");
            // Display success animation
            blink();
        }
    }
    return 0;
}
