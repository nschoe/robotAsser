/* 
 * File:   fifo.h
 * Author: feltrin
 *
 * Created on March 30, 2014, 2:15 PM
 */

#ifndef FIFO_H
#define	FIFO_H

#ifdef	__cplusplus
extern "C" {
#endif


struct s_fifo
{
    char value;
    struct s_fifo * next;
};

typedef struct s_fifo fifo;

int addFifo( char value );

#ifdef	__cplusplus
}
#endif

#endif	/* FIFO_H */

