
#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <avr/io.h>

#ifndef BAUD
#define BAUD 9600
#endif

FILE * uart_out;
void serial_init(void);
void uart_init(void);
int uart_putChar(char c, FILE *stream);
int uart_getChar(FILE *stream);
char* ftoa(double val, char * s, uint8_t base = 10, uint8_t float_dig = 1, uint8_t integ_dig = 0);
char* itoa(long val, char * s, uint8_t base = 10, uint8_t minimum_digits = 0);


#ifdef __cplusplus
}
#endif

#endif
