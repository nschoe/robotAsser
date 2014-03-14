/*
 * File:   io.c
 * Author: nschoe
 *
 * Created on January, 9, 2014
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * Created initialization function
 *
 * February, 6
 * Tests with the speed ramp
 */

#include <xc.h>
#include <p33FJ128MC802.h>
#include <stdint.h>

#include "asser.h"
#include "constants.h"
#include "tools.h"
#include "motors.h"

// Defined in main.gc
extern int32_t g_DistL;
extern int32_t g_DistR;
extern uint32_t g_Loop;
extern int16_t g_OldLeftSpeed, g_OldRightSpeed;

/*
 * Asser main loop function
 */
unsigned char asser( const int16_t cSpeedL, const int16_t cSpeedR, int32_t * comS, const int32_t cAlpha )
{
    int32_t ticksL, ticksR, rAlpha, errAlpha;
    int16_t deltaTicks, newSpeedL, newSpeedR;

    // Increment asser loop number
    g_Loop++;

    if( g_Loop >= 1000 )
    {
        // Reset asser loop number
        g_Loop = 0;

        // Test of the ramp speed
        newSpeedL = speedRamp( cSpeedL, g_OldLeftSpeed );
        newSpeedR = speedRamp( cSpeedR, g_OldRightSpeed );

        // Update previous speed values
        g_OldLeftSpeed = newSpeedL;
        g_OldRightSpeed = newSpeedR;

        setMotorsSpeed( newSpeedL, newSpeedR, MOTOR_MODE_PERCENTAGE );

        /*
        ticksL = g_DistL * TICKS_PER_ROTATION + POS1CNT;
        ticksR = g_DistR * TICKS_PER_ROTATION + POS2CNT;
        deltaTicks = ticksR - ticksL; // trigonometric rotation -> right - left
        
        rAlpha = ANGLE_PER_TICK * deltaTicks;

        errAlpha = cAlpha - rAlpha; // in mili-degrees

        *comS = errAlpha / K;

        setMotorsSpeed( cSpeedL - (*comS / 2 ), cSpeedR + (*comS / 2), MOTOR_MODE_PERCENTAGE );
        */
    }

    return( ASSER_RUNNING );
}

/*
 * Initializes QEI modules.
 */
void initQEI( void )
{
    QEI1CONbits.QEIM = 0b000; // Disable QEI for initialization
    QEI2CONbits.QEIM = 0b000; // Disable QEI for initialization

    // Set remapable pins for QEI
    RPINR14bits.QEA1R = 10;
    RPINR14bits.QEB1R = 11;
    RPINR16bits.QEA2R = 7;
    RPINR16bits.QEB2R = 6;

    /* Set Digital Filter clock prescaler to 16.
     * This means that he Digital Filter clock is FCY / 16 = 230kHz.
     * We estimated the encode wheel to turn about 10 rotations per second
     * So the encoder yields pics at about 60Khz.
     */
    DFLT1CONbits.QECK = 0b011;
    DFLT2CONbits.QECK = 0b011;
    DFLT1CONbits.CEID = 0b1; // Disable Count Error interrupts
    DFLT2CONbits.CEID = 0b1; // Disable Count Error interrupts
    DFLT1CONbits.QEOUT = 0b1; // Use the filtered output (instead of raw)
    DFLT2CONbits.QEOUT = 0b1; // Use the filtered output (instead of raw)

    MAX1CNT = TICKS_PER_ROTATION - 1; // Load the comparison value with the number of ticks to perform 1 rotation
    MAX2CNT = TICKS_PER_ROTATION - 1; // Load the comparison value with the number of ticks to perform 1 rotation
    
    POS1CNT = 0; // Reset the current position counter value
    POS2CNT = 0; // Reset the current position counter value

    IEC3bits.QEI1IE = 0b1; // Allow QEI to yield interupts
    IEC4bits.QEI2IE = 0b1; // Allow QEI to yield interupts

    QEI1CONbits.QEIM = 0b111; // Enables QEI mode, 4x resolution and reset on MAXCNT
    QEI2CONbits.QEIM = 0b111; // Enables QEI mode, 4x resolution and reset on MAXCNT
}

void initAsser( void )
{
    g_DistL = 0;
    g_DistR = 0;
}

// Returns a speed value to apply to the motors (as percentage) based on the max acceleration values
int16_t speedRamp( const int16_t targetSpeed, const int16_t previousSpeed )
{
    /*
     * This method of detecting the acceleration is weak :
     * in case we are going forward at a certain speed and set a very high negative value,
     * it is detected as an acceleration.
     * TO FIX : use absolute values AND signs to detect
     */

    // "Acceleration"
    if( abs( targetSpeed ) >= abs( previousSpeed ) )
    {
        if( (targetSpeed - previousSpeed) >= ACCELERATION_MAX )
            return( previousSpeed + ACCELERATION_MAX );
        else
            return( targetSpeed );
    }
    else
        return 0;
}