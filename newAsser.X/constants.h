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
 *
 * February, 6
 * Tuned the coefficients of the speed ramp
 *
 * March, 20
 * Implemented the functions to update the (X, Y, alpha) position
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
#define FUNCTION_OTHER 1
#define FUNCTION_DEBUG_LED 2
#define FUNCTION_BLOCKING 3
#define FUNCTION_STOP 4

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

    /******************************************************** ROTARY ENCODERS */
#define TICKS_PER_ROTATION 1440  // Nb of ticks the rotary encoder generates in a one rotation

    /**************************************************************** BOOLEAN */
#define FALSE 0
#define TRUE 1

    /********************************************************* ASSER CONTANTS */
#define ASSER_RUNNING 1
#define END_ASSER 2

#define PI 3.141592

#define DIAM_ENCODER 41100 // Diameter (um) of the rotary encoders
#define DIST_WHEELS 247000 // The distance (um) between the rotary encoders
#define ANGLE_PER_TICK 21 // For a 1 tick difference between wheels, it represents 22 milidegrees orientation
#define DIST_PER_TICK // Distance the robot moves when the rotsry encoders move for 1 tick

#define K 11
#define K_D 0
#define K_I 5

    /************************************************************* SPEED RAMP */
#define ACCELERATION_MAX 10 // 1%

#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

