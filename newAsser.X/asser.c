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
#include "math.h"

// Defined in main.gc
extern int32_t g_DistL, g_DistR;
extern uint32_t g_Loop;
extern int16_t g_LeftSpeed, g_RightSpeed;
extern uint32_t g_X, g_Y, g_Alpha, g_Cons_X, g_Cons_Y, g_Cons_Alpha;
extern uint32_t g_Alpha_Last;

/*
 * Asser main loop function
 */
unsigned char asser()
{
    int16_t speedCor;
    uint32_t alpha, d;
    uint32_t dX, dY;

    // Increment asser loop number
    g_Loop++;

    // Position calculation
    alpha = ((g_DistR - g_DistL) * TICKS_PER_ROTATION + POS2CNT - POS1CNT) * ANGLE_PER_TICK;
    //d = ((g_DistR + g_DistL) * TICKS_PER_ROTATION + POS2CNT + POS1CNT) * DIST_PER_TICK / 2;

    //dY = (uint32_t) d * sin ( (alpha + g_Alpha) / 2 );
    //dX = (uint32_t) d * cos ( (alpha + g_Alpha) / 2 );

    // Speed regulation
    if( g_Loop >= 1000 )
    {
        // Reset asser loop number
        g_Loop = 0;

        //*
        speedCor = (int16_t) ((K_I * (g_RightSpeed - g_LeftSpeed) + K *((1 + K_D) * (g_Cons_Alpha - alpha) - K_D * (g_Cons_Alpha - g_Alpha_Last)))/(1 + K_I));
        g_LeftSpeed = - speedCor / 2; //speedRamp( 0 - speedCor / 2, g_LeftSpeed );
        g_RightSpeed = speedCor / 2; //speedRamp( 0 + speedCor / 2, g_RightSpeed );

        // We just did a correction, so we update g_Alpha_Last
        g_Alpha_Last = alpha;

        //*/
    }
    setMotorsSpeed(g_LeftSpeed, g_RightSpeed, MOTOR_MODE_PERCENTAGE );


    // Really update position
    //g_X += dX;
    //g_Y += dY;
    g_Alpha = alpha;


    //*
    if( g_Alpha >= g_Cons_Alpha )
        return( DONE_ASSER );
    else
        return( ASSER_RUNNING );
    //*/
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