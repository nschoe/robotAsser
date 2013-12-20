/*
 * File:   io.c
 * Author: nschoe
 *
 * Created on December 18, 2013
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 *
 */

#include <xc.h>
#include <p33FJ128MC802.h>

#include "io.h"

/* Initialize I/O ports
 * Initialize Motor and Debug Led pins
 */
void initIO( void )
{
  // Initialize the debug led as output
  TRISB = 0b11001111; // Debug Led (RB5) and Motor Enable (RB4) in output mode
}
