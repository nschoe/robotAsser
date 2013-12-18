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
 */


#include <xc.h>
#include <p33FJ128MC802.h>

#include "constants.h"
#include "motors.h"

// Configure registers value, running mode, frequency, etc
void initPWM1( void )
{
  P1TCONbits.PTEN = 0;          // Timer enable should be 0 when initialization phase

  PORTBbits.RB4 = 1;         // Enable Motor

  P1TCONbits.PTMOD = 0b00;      // PWM runs in Free Running Mode (PWM timer counts up then starts again from 0)
  P1TCONbits.PTCKPS = 0b00;     // Prescaler 1:1
  P1TCONbits.PTOPS = 0b00;      // Postcaler 1:1

  PWM1CON1bits.PMOD1 = 1;       // PWM1H1 and PWM1L1 independant mode
  PWM1CON1bits.PMOD2 = 1;

  // P1TPER = FCY / (FPWM * P1TMR Prescaler) - 1
  P1TPER = 0xB8;//MOTOR_MAX_SPEED / 2;

  P1DC1 = 0;                    // Duty Cycle to 0%
  P1DC2 = 0;

  // Disable PWM1 output pins
  PWM1CON1bits.PEN1L = 0;
  PWM1CON1bits.PEN1H = 0;
  PWM1CON1bits.PEN2L = 0;
  PWM1CON1bits.PEN2H = 0;

  P1TCONbits.PTEN = 1;          // Start PWN timer
  

  // last year's code
  /*
  P1TPER = 0xF5; // Période du timer
  P1DC1 = 0; // Duty-cycle PWM1H1 = 0%
  P1DC2 = 0; // Duty-cycle PWM1H2 = 0%

  PWM1CON1bits.PMOD1 = 1; // Sorties PWM1H1 et PWM1L1 indépendantes
  PWM1CON1bits.PMOD2 = 1; // Sorties PWM1H2 et PWM1L2 indépendantes

  PWM1CON1bits.PEN1H = 0;
  PWM1CON1bits.PEN2H = 0;
  PWM1CON1bits.PEN1L = 0;
  PWM1CON1bits.PEN2L = 0;

  P1TCONbits.PTEN = 1; // Active le Timer des PWMs
  P1TCONbits.PTOPS = 5 - 1; // Reset automatique aux modif de P1TCON
   */
}