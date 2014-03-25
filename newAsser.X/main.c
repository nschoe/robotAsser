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
int32_t g_DistL, g_DistR;// Distance traveled by left and right wheel (number of rotations)
int32_t g_Dist_Last, g_POS_Last; // Distance traveled at last position calculation (number of rotations, number of ticks)
char g_PauseBlock; // Used for the blocking pause functions
char g_END;
char g_NewOrder; // Set to 1 when received a new order from the PSoC (prevents repeated send of DONE_ASSER)
uint32_t g_Loop;
int16_t g_LeftSpeed, g_RightSpeed;
uint32_t g_X, g_Y, g_Alpha, g_Cons_X, g_Cons_Y, g_Cons_Alpha;
uint32_t g_Alpha_Last; // Alpha value when last correction was computed

// Interupts functions are defined at the end of the file

int main ( void )
{
    /*
     TO DO
     =====
     * Set up another timer than 2-3 for blocking pause functions
     */

	initIO( );
        initAsser( );
	initTimer23( );
	initPWM1( );
        initQEI( );

        g_PauseBlock = FALSE;
        g_END = FALSE;
        g_Loop = 1001;

        // Set previous speed values to 0
        g_LeftSpeed = 0;
        g_RightSpeed = 0;

        g_DistL = 0;
        g_DistR = 0;

        // No new order at the beginning
        g_NewOrder = FALSE;

        // No previous correction
        g_Alpha_Last = 0; // Start facing EAST
        
        // Define start position
        g_Alpha = 0;
        
        blockPauseS( 1 );

        //g_FunctionTimer23 = FUNCTION_STOP;
        //startTimer23( 12000 ); // In match : should be 90 000

        //setMotorsSpeed(600, 600, MOTOR_MODE_PERCENTAGE);
        //while(1);

        g_Cons_Alpha = 180000;

        g_NewOrder = TRUE; // TEST
        
        while ( !g_END ) // 90 seconds not finished
        {
            if ( asser() == DONE_ASSER ) // We finished executing the order
            {
                // Send DONE_ASSER to PSoC
                if ( g_NewOrder ) // If the asser is done and it was a new order
                {
                    g_NewOrder = FALSE;

                    // Effectively send the DONE_ASSER to PSoC
                    stopMotors(); // For tests only
                    g_END = TRUE; // For tests only
                }
            }
        }

        // After 90 seconds, everything must be stopped
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