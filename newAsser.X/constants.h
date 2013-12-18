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

    // Motors (PWM)
#define MOTOR_MAX_SPEED 366


#ifdef	__cplusplus
}
#endif

#endif	/* CONSTANTS_H */

