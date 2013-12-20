/* 
 * File:   constants.h
 * Author: nschoe
 *
 * Created on December 16, 2013
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * December 18
 * Worked on PWM1 : adjusting frequency to make it work
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FOSC 7370000       // Oscillator frequency (in Hertz)
#define FCY  FOSC / 2      // Instruction frequency (in Hertz)

#define IMPOSSIBLE -99    // Used to catch unreachable 'else' or 'case' statements

    /************************************************ Timer23 handler function*/
#define FUNCTION_DEBUG_LED 1
#define FUNCTION_BLOCKING 2

    /*********************************************************** Motors (PWM) */
#define FPWM 20000         // PWM Frequency (in Hertz)
#define PWM_PRESCALER 1    // Value of P1TCON<PTCKPS>
#define MOTOR_MAX_SPEED 2 * (FCY / (FPWM * PWM_PRESCALER))

#define MOTOR_OK 0
#define MOTOR_MODE_ABSOLUTE 1
#define MOTOR_MODE_PERCENTAGE 2
#define MOTOR_ERROR_MODE 3
#define MOTOR_LEFT_POSITIVE_OVERSPEED 4
#define MOTOR_LEFT_NEGATIVE_OVERSPEED 5
#define MOTOR_RIGHT_POSITIVE_OVERSPEED 6
#define MOTOR_RIGHT_NEGATIVE_OVERSPEED 7

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

