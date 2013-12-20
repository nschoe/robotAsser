/*
 * File:   main.c
 * Author: nschoe
 *
 * Created on December 16, 2013
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * December 18
 * Enabled Motor EN pin in initIO()
 */


#include <xc.h>
#include <p33FJ128MC802.h>
#include <stdint.h>

#include "configurationBits.h"
#include "constants.h"
#include "tools.h"

#include "timers.h"
#include "motors.h"
#include "io.h"

// Global variables (TO BE LIMITED IN NUMBER)
unsigned char g_FunctionTimer23;

// Interupts functions are defined at the end of the file

int main ( void )
{

	initIO( );
	initTimer23( );
	initPWM1( );

	blockPauseS( 1 );

	setMotorsSpeed( 100, 100, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 200 );
	setMotorsSpeed( 250, 250, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 200 );
	setMotorsSpeed( 400, 400, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 200 );
	setMotorsSpeed( 550, 550, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 200 );

	setMotorsSpeed(750, 750, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 200 );

	setMotorsSpeed(1000, 1000, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 700 );

	setMotorsSpeed(750, 1000, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 1000 );
	
	setMotorsSpeed(1000, 750, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 1000 );
	setMotorsSpeed(1000, 1000, MOTOR_MODE_PERCENTAGE );
	blockPauseMs( 3000 );

	stopMotors();

	g_FunctionTimer23 = FUNCTION_DEBUG_LED;
	startTimer23( 200 );

	//setMotorsSpeed( 250, 250, MOTOR_MODE_PERCENTAGE );
	
	while( 1 );

	return 0;
}

/********************* INTERUPTS *******************/

// Define the ISR for the Timer3 interruption
void __attribute__((__interrupt__, no_auto_psv)) _T3Interrupt(void )
{
	// The behavior is determined by the gFunctionTimer23 varaible
	if( FUNCTION_DEBUG_LED == g_FunctionTimer23 )
	{
		toggleDebugLed( );
		IFS0bits.T3IF = 0; // Reset the interupt flag
	}
	else if( FUNCTION_BLOCKING == g_FunctionTimer23 )
	{
		IFS0bits.T3IF = 0; // Reset the interupt flag
	}
}

// PWM1 interrupt
void __attribute__( ( __interrupt__, no_auto_psv ) ) _MPWM1Interrupt( void )
{
	IFS3bits.PWM1IF = 0;
}
