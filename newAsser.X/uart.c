#include "uart.h"

#include <xc.h>
#include <p33FJ128MC802.h>

extern fifo * g_UART_Buffer;

void init_uart()
{
    AD1PCFGLbits.PCFG5 = 1; // deactivate ADC : cause port mapping
    RPOR1bits.RP2R = 3;							// Tx1 -> RP2
    RPINR18bits.U1RXR = 3;						// RP3 -> Rx1

    U1BRG = 1; // 9600 bauds at 7.37 MHz

    U1STAbits.UTXISEL1 = 0;	// Interrupt on empty FIFO, last byte is being sent
    U1STAbits.UTXISEL0 = 0;	//

    IEC0bits.U1TXIE = 1;

    U1MODEbits.UARTEN = 1; 	// Enable UART
    U1STAbits.UTXEN = 1; 	// Enable UART TX
}

int UART_putc ( unsigned char c )
{
    /*if(U1STAbits.UTXBF) // If a transmission is engaged
    {
        addFifo( c );
        IFS0bits.U1TXIF = 0;
        IEC0bits.U1TXIE = 1;	// Send interruption

        return -1;
    }
    else
    {*/
    U1TXREG = c;
    return 0;
}

int UART_send_32 ( int32_t message )
{
    if (UART_putc(message >> 24)) return 0;
    blockPauseMs ( 2 );
    if (UART_putc(message >> 16)) return 1;
    blockPauseMs ( 2 );
    if (UART_putc(message >> 8)) return 2;
    blockPauseMs ( 2 );
    if (UART_putc(message)) return 3;
    blockPauseMs ( 2 );
    return 4;
}
