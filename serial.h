
#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <avr/io.h>


FILE * uart_out;
void serial_init(void);
void uart_init(void);
int uart_putChar(char c, FILE *stream);
int uart_getChar(FILE *stream);


#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H */
