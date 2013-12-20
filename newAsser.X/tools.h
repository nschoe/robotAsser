/* 
 * File:   tools.h
 * Author: nschoe
 *
 * Created on December 16, 2013, 2:53 PM
 */

#ifndef TOOLS_H
#define	TOOLS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

    void turnONDebugLed( void );
    void turnOFFDebugLed( void );
    void toggleDebugLed( void );
    void blockPauseMs( const uint16_t );
    void blockPauseS( const uint16_t );


#ifdef	__cplusplus
}
#endif

#endif	/* TOOLS_H */

