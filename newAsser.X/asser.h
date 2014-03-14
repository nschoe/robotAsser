/* 
 * File:   asser.h
 * Author: nschoe
 *
 * Created on January 9, 2014
 * =============================================================================
 *                                  CHANGELOG
 * =============================================================================
 * January, 9
 * 
 */

#ifndef ASSER_H
#define	ASSER_H

#ifdef	__cplusplus
extern "C" {
#endif

    void initQEI( void );
    void initAsser( void );
    unsigned char asser( const int16_t, const int16_t, int32_t *, const int32_t );
    int16_t speedRamp( const int16_t, const int16_t );


#ifdef	__cplusplus
}
#endif

#endif	/* ASSER_H */

