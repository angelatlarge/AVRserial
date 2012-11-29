
#include <serial.h>

#ifndef F_CPU
#       error Must define F_CPU or pass it as compiler argument
#endif

extern "C"{
 FILE * uart_out;
}

#include <stdio.h>
#define BAUD 9600

#include <util/setbaud.h>

void serial_init(void) {
	uart_out = fdevopen(uart_putChar, uart_getChar);
	stdout = stdin = uart_out;
    uart_init();
}

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8-bit data  
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // Enable RX and TX 
}

int uart_putChar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putChar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
	return 0;
}

int uart_getChar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); // Wait until data exists. 
    return UDR0;
}



char* ftoa(double val, char * s, uint8_t base = 10, uint8_t float_dig = 1, uint8_t integ_dig = 0) {
	if (base < 2) base = 10;
	char buf[24 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];
	uint32_t integral = (uint32_t)val;
	uint32_t fract = (uint32_t)((val - integral) * pow(10, float_dig));
	*str = '\0';
	uint8_t nLen = 0;
	uint8_t nFloatLen = 0;
	for (int i = 0; i<2; i++) {
		uint32_t num = (i==0)?fract:integral;
		do {
			unsigned long m = num;
			num /= base;
			char c = m - base * num;
			*--str = c < 10 ? c + '0' : c + 'A' - 10;
			nLen++;
		} while (
				num 									// Still non-zero
			|| 
				(( nLen < float_dig ) && !i)			// Floating point precision has not been reached
			||
				(( nLen-nFloatLen < integ_dig ) && i)	// Integral precision has not been reached
			);
		if (i==0) {
			*--str = '.';
			nFloatLen = ++nLen;
		}			
	}
	s[nLen] = '\0';
	memcpy(s, str, nLen);
	return s;
}

char* itoa(long val, char * s, uint8_t base = 10, uint8_t minimum_digits = 0) {
	char buf[16 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];
	*str = '\0';
	long n = val;

	// prevent crash if called with base == 1
	if (base < 2) base = 10;
	uint8_t nLen = 0;
	do {
		unsigned long m = n;
		n /= base;
		char c = m - base * n;
		*--str = c < 10 ? c + '0' : c + 'A' - 10;
		nLen++;
	} while(n || (nLen<minimum_digits));

	s[nLen] = '\0';
	memcpy(s, str, nLen);
	return s;

}
