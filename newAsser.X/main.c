/*
 * File:   main.c
 * Author: nschoe
 *
 * Created on December 16, 2013
 */


#include <xc.h>
#include <p33FJ128MC802.h>

#include "configurationBits.h"
#include "constants.h"
#include "tools.h"

#include "motors.h"

// Function signatures that are defined below in the file
void initIO( void );
void initTimer23( void );
void startTimer23( const unsigned int );

int main ( void )
{
  initIO();
  initTimer23();
  initPWM1();

  PWM1CON1bits.PEN1H = 0;
  PWM1CON1bits.PEN1L = 1;
  P1DC1 = 0x172 / 4;

  PWM1CON1bits.PEN2H = 0;
  PWM1CON1bits.PEN2L = 1;
  P1DC2 = 0x172 / 4;
  
  startTimer23 ( 500 );

  while(1);
  
  return 0;
}

// Define the ISR for the Timer3 interruption
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void)
{
  toggleDebugLed();
  IFS0bits.T3IF = 0;
}

// PWM1 interrupt
void __attribute__((__interrupt__, no_auto_psv)) _MPWM1Interrupt(void)
{
  IFS3bits.PWM1IF = 0;
}

// Initialize I/O ports
void initIO( void )
{
  // Initialize the debug led as output
  TRISB = 0b11001111; // Debug Led (RB5) and Motor Enable (RB4) in output mode
}

void initTimer23( void )
{
  // Stops 16-bit timers actions on Timer3 and Timer2
  T3CONbits.TON = 0;
  T2CONbits.TON = 0;

  // Activates Timer2 and Timer3 32-bit mode
  T2CONbits.T32 = 1;

  // Set to use Internal clock (Fosc / 2)
  T2CONbits.TCS = 0;

  // Disable Gated time
  T2CONbits.TGATE = 0;

  // Set prescaler to 1:1
  T2CONbits.TCKPS = 0b00;

  // Set priority level to 1
  IPC2bits.T3IP = 0x01;
}

void startTimer23( const unsigned int nbMiliseconds )
{
  unsigned long count;

  // Clear interrupt flag
  IFS0bits.T3IF = 0;

  // Enable timer interrupt
  IEC0bits.T3IE = 1;

  // Clear the current value of the counter
  TMR3 = 0x00;
  TMR2 = 0x00;

  // Compute the corresponding values to put in MSW and LSW
  count = (FCY / 1000) * nbMiliseconds; // Parentheses matter to ensure we don't overflow

  PR3 = count >> 16;             // MSW
  PR2 = (count << 16 ) >> 16;    // LSW

  // Starts the timer
  T2CONbits.TON = 1;
}