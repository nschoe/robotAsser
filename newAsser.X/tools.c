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
#include "timers.h"

extern unsigned char g_FunctionTimer23; // defiend in main.c

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

// Pause for nbMiliseconds miliseconds, blocking function
void blockPauseMs( const uint16_t nbMiliseconds )
{
	g_FunctionTimer23 = FUNCTION_BLOCKING;
	startTimer23( nbMiliseconds );
	while( 0 == IFS0bits.T3IF );
}

void blockPauseS( const uint16_t nbSeconds )
{
	uint16_t i;

	for( i = 0; i < nbSeconds; i++ )
	{
		blockPauseMs( 1000 );
	}
}
