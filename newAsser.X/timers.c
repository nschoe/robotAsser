/*
 * File:   timers.c
 * Author: nschoe
 *
 * Created on December 18, 2013
 * Timers setup
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * 
 */

#include <xc.h>
#include <p33FJ128MC802.h>

#include "timers.h"
#include "constants.h"

/*
 * 32-bit timers using 16-bit Timer2 and Timer3
 */
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

/*
 * Start the 32-bit timer to count for nbMiliseconds miliseconds and yields an Timer3 interupt
 * Automatically adjusted to the frequency (FCY)
 */
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