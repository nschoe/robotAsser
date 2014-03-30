/* 
 * File:   uart.h
 * Author: feltrin
 *
 * Created on March 27, 2014, 4:50 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdint.h>
    #include "fifo.h"

    void init_uart();
    int UART_putc ( unsigned char c );
    int UART_send_32 ( int32_t );

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

