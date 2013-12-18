/*
 * File:   tools.h
 * Author: nschoe
 *
 * Created on December 16, 2013
 * Define useful functions to be used in the whole project.
 * Contains functions to be useful for debug.
 */

#include <xc.h>
#include <p33FJ128MC802.h>

#include "tools.h"
#include "constants.h"
/*
 * The debug led is on RB5 which is in PORTB
 */
void turnONDebugLed( void )
{
  TRISBbits.TRISB5 = 0;  // Active on level low
}

void turnOFFDebugLed( void )
{
  TRISBbits.TRISB5 = 1;  // Active on level low
}

void toggleDebugLed( void )
{
  TRISBbits.TRISB5 = 1 - TRISBbits.TRISB5;
}
