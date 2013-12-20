/* 
 * File:   motors.h
 * Author: nschoe
 *
 * Created on December 16, 2013
 */

#ifndef MOTORS_H
#define	MOTORS_H

#ifdef	__cplusplus
extern "C" {
#endif

    void initPWM1( void );
    char setMotorsSpeed( const int16_t, const int16_t, const char );
    void stopMotors( void );


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORS_H */

