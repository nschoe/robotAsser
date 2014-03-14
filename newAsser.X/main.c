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
#include "asser.h"

// Global variables (TO BE LIMITED IN NUMBER)
unsigned char g_FunctionTimer23;
int32_t g_DistL, g_DistR; // Distance traveled by left and right wheel
char g_PauseBlock; // Used for the blocking pause functions
char g_END;
uint32_t g_Loop;
int16_t g_OldLeftSpeed, g_OldRightSpeed;

// Interupts functions are defined at the end of the file

int main ( void )
{
    int32_t comS;

	initIO( );
        initAsser( );
	initTimer23( );
	initPWM1( );
        initQEI( );

        g_PauseBlock = FALSE;
        g_END = FALSE;
        g_Loop = 0;
        comS = 0;

        // Set previous speed values to 0
        g_OldLeftSpeed = 0;
        g_OldRightSpeed = 0;
        
        blockPauseS( 1 );

        g_FunctionTimer23 = FUNCTION_STOP;
        startTimer23( 12000 );

        while( !g_END && (asser( 800, 800, &comS, 0 ) != END_ASSER) );
        stopMotors();

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
            g_PauseBlock = FALSE; // Tells the variable that tells us we are in block mode
		IFS0bits.T3IF = 0; // Reset the interupt flag
	}
        else if( FUNCTION_STOP == g_FunctionTimer23 )
        {
            g_END = TRUE;
            IFS0bits.T3IF = 0; // Reset the interupt flag
        }
        else
        {
                IFS0bits.T3IF = 0; // Reset the interupt flag
        }
}

// PWM1 interrupt
void __attribute__( ( __interrupt__, no_auto_psv ) ) _MPWM1Interrupt( void )
{
	IFS3bits.PWM1IF = 0;
}


// QEI 1 interrupt : left wheel
void __attribute__( ( __interrupt__, no_auto_psv ) ) _QEI1Interrupt( void )
{
    IFS3bits.QEI1IF = 0; // Reset flag

    //  Update distance
    if( 1 == QEI1CONbits.UPDN ) // going forward
        g_DistL++;
    else
        g_DistL--;
}

// QEI 2 interrupt : right wheel
void __attribute__( ( __interrupt__, no_auto_psv ) ) _QEI2Interrupt( void )
{
    IFS4bits.QEI2IF = 0; // Reset flag

    //  Update distance
    if( 1 == QEI2CONbits.UPDN ) // going forward
        g_DistR++;
    else
        g_DistR--;
}