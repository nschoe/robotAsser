/*
 * File:   motors.c
 * Author: nschoe
 *
 * Created on December 16, 2013
 * Handles motors initialization, start and speed of the motors
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * December 18
 * Worked on PWM1 : trying to find the right settings to make it work.
 * Added setMotorsSpeed
 */


#include <xc.h>
#include <p33FJ128MC802.h>
#include <stdint.h>

#include "constants.h"
#include "motors.h"

// Configure registers value, running mode, frequency, etc
void initPWM1( void )
{
	P1TCONbits.PTEN = 0; // Timer enable should be 0 when initialization phase

	PORTBbits.RB4 = 1; // Enable Motor

	P1TCONbits.PTMOD = 0b00; // PWM runs in Free Running Mode (PWM timer counts up then starts again from 0)
	P1TCONbits.PTCKPS = 0b00; // Prescaler 1:1
	P1TCONbits.PTOPS = 0b00; // Postcaler 1:1

	PWM1CON1bits.PMOD1 = 1; // PWM1H1 and PWM1L1 independant mode
	PWM1CON1bits.PMOD2 = 1; // PWM1H2 and PWM1L2 independant mode

	// P1TPER = FCY / (FPWM * P1TMR Prescaler) - 1
	P1TPER = MOTOR_MAX_SPEED / 2 - 1; //

	P1DC1 = 0; // Duty Cycle to 0%
	P1DC2 = 0; // Duty Cycle to 0%

	// Disable PWM1 output pins
	PWM1CON1bits.PEN1L = 0;
	PWM1CON1bits.PEN1H = 0;
	PWM1CON1bits.PEN2L = 0;
	PWM1CON1bits.PEN2H = 0;

	P1TCONbits.PTEN = 1; // Start PWN timer
}

/* Set each motor's speed with verification of max speed.
 * Positive values used to go forward
 * Negative values used to go backward
 * ABSOLUTE mode : values are interpreted as values to be passed directly to the motors
 * PERCENTAGE mode : values are interpreted as 1 / 1000 ("PER THOUSANDS")
 * 
 */
char setMotorsSpeed( const int16_t speedL, const int16_t speedR, const char mode )
{
	char answer;
	uint16_t realSpeedL, realSpeedR;

	// Check if we interpret the values as absolute or 1/1000 ("percentage")
	if( MOTOR_MODE_ABSOLUTE == mode )
	{
		// The speed values are to be passed directly to the motors

		// left wheel
		if( 0 == speedL ) // stop left wheel
		{
			PWM1CON1bits.PEN1L = 0; // deactivate PWM1 output
			PWM1CON1bits.PEN1H = 0; // idem
			realSpeedL = 0;
		}
		else if( speedL > 0) // left wheel go forward
		{
			PWM1CON1bits.PEN1L = 1; // left wheel go forward
			PWM1CON1bits.PEN1H = 0;

			// Check if we are faster than max speed
			if( speedL > MOTOR_MAX_SPEED )
			{
				realSpeedL = MOTOR_MAX_SPEED;
				answer = MOTOR_LEFT_POSITIVE_OVERSPEED;
			}
			else
			{
				realSpeedL = speedL;
				answer = MOTOR_OK;
			}
		}
		else if( speedL < 0) // left wheel go backward
		{
			PWM1CON1bits.PEN1L = 0; // left wheel go backward
			PWM1CON1bits.PEN1H = 1;

			// Check if we are faster than max speed
			if( -speedL > MOTOR_MAX_SPEED )
			{
				realSpeedL = MOTOR_MAX_SPEED;
				answer = MOTOR_LEFT_NEGATIVE_OVERSPEED;
			}
			else
			{
				realSpeedL = -speedL;
				answer = MOTOR_OK;
			}
		}
		else
		{
			answer = IMPOSSIBLE;
		}

		// right wheel
		if( 0 == speedR ) // stop right wheel
		{
			PWM1CON1bits.PEN2L = 0; // deactivate PWM1 output
			PWM1CON1bits.PEN2H = 0; // idem
			realSpeedR = 0;
		}
		else if( speedR > 0) // right wheel go forward
		{
			PWM1CON1bits.PEN2L = 1; // right wheel go forward
			PWM1CON1bits.PEN2H = 0;

			// Check if we are faster than max speed
			if( speedR > MOTOR_MAX_SPEED )
			{
				realSpeedR = MOTOR_MAX_SPEED;
				answer = MOTOR_RIGHT_POSITIVE_OVERSPEED;
			}
			else
			{
				realSpeedR = speedR;
				answer = MOTOR_OK;
			}
		}
		else if( speedR < 0) // right wheel go backward
		{
			PWM1CON1bits.PEN2L = 0; // right wheel go backward
			PWM1CON1bits.PEN2H = 1;

			// Check if we are faster than max speed
			if( -speedR > MOTOR_MAX_SPEED )
			{
				realSpeedR = MOTOR_MAX_SPEED;
				answer = MOTOR_RIGHT_NEGATIVE_OVERSPEED;
			}
			else
			{
				realSpeedR = -speedR;
				answer = MOTOR_OK;
			}
		}
		else
		{
			answer = IMPOSSIBLE;
		}
	}
	else if( MOTOR_MODE_PERCENTAGE == mode )
	{
		// The speed values are 1/1000 and must be converted to direct values

		// left wheel
		if( 0 == speedL ) // stop left wheel
		{
			PWM1CON1bits.PEN1L = 0; // deactivate PWM1 output
			PWM1CON1bits.PEN1H = 0; // idem
			realSpeedL = 0;
		}
		else if( speedL > 0) // left wheel go forward
		{
			PWM1CON1bits.PEN1L = 1; // left wheel go forward
			PWM1CON1bits.PEN1H = 0;

			// Check if we are faster than max speed
			if( speedL > 1000 )
			{
				realSpeedL = MOTOR_MAX_SPEED;
				answer = MOTOR_LEFT_POSITIVE_OVERSPEED;
			}
			else
			{
				realSpeedL = (speedL * MOTOR_MAX_SPEED) / 1000;
				answer = MOTOR_OK;
			}
		}
		else if( speedL < 0) // left wheel go backward
		{
			PWM1CON1bits.PEN1L = 0; // left wheel go backward
			PWM1CON1bits.PEN1H = 1;

			// Check if we are faster than max speed
			if( -speedL > 1000 )
			{
				realSpeedL = MOTOR_MAX_SPEED;
				answer = MOTOR_LEFT_NEGATIVE_OVERSPEED;
			}
			else
			{
				realSpeedL = (-speedL * MOTOR_MAX_SPEED) / 1000;
				answer = MOTOR_OK;
			}
		}
		else
		{
			answer = IMPOSSIBLE;
		}

		// right wheel
		if( 0 == speedR ) // stop right wheel
		{
			PWM1CON1bits.PEN2L = 0; // deactivate PWM1 output
			PWM1CON1bits.PEN2H = 0; // idem
			realSpeedR = 0;
		}
		else if( speedR > 0) // right wheel go forward
		{
			PWM1CON1bits.PEN2L = 1; // right wheel go forward
			PWM1CON1bits.PEN2H = 0;

			// Check if we are faster than max speed
			if( speedR > 1000 )
			{
				realSpeedR = MOTOR_MAX_SPEED;
				answer = MOTOR_RIGHT_POSITIVE_OVERSPEED;
			}
			else
			{
				realSpeedR = (speedR * MOTOR_MAX_SPEED) / 1000;
				answer = MOTOR_OK;
			}
		}
		else if( speedR < 0) // right wheel go backward
		{
			PWM1CON1bits.PEN2L = 0; // right wheel go backward
			PWM1CON1bits.PEN2H = 1;

			// Check if we are faster than max speed
			if( -speedR > 1000 )
			{
				realSpeedR = MOTOR_MAX_SPEED;
				answer = MOTOR_RIGHT_NEGATIVE_OVERSPEED;
			}
			else
			{
				realSpeedR = (-speedR * MOTOR_MAX_SPEED) / 1000;
				answer = MOTOR_OK;
			}
		}
		else
		{
			answer = IMPOSSIBLE;
		}
	}
	else
	{
		answer = MOTOR_ERROR_MODE;
	}

	// Check if we are in mode error
	if( MOTOR_ERROR_MODE != answer )
	{
		P1DC1 = realSpeedL;
		P1DC2 = realSpeedR;
	}

	return( answer );
}

// Stop the motors by putting the duty cycle to 0 and disabling the pins
void stopMotors( void )
{
	PWM1CON1bits.PEN1L = 0; // deactivate PWM1 output
	PWM1CON1bits.PEN1H = 0; // idem
	P1DC1 = 0;
	P1DC2 = 0;
}
