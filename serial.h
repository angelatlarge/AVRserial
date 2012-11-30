
#ifndef PID_H
#define PID_H

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
char* dftoa(double val, char * s, uint8_t base, uint8_t float_dig, uint8_t integ_dig);
char* ditoa(long val, char * s, uint8_t base, uint8_t minimum_digits);


#ifdef __cplusplus
}
#endif

#endif
