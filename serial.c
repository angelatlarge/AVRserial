
#include <serial.h>
#include <math.h>
#include <string.h>

#ifndef F_CPU
#       error Must define F_CPU or pass it as compiler argument
#endif


#ifdef __cplusplus
extern "C"{
#endif
	FILE * uart_out;
#ifdef __cplusplus
}
#endif

//~ #ifndef BAUD
//~ #define BAUD 9600
//~ #endif

#include <util/setbaud.h>


#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
	#define kUBRRL	UBRR0L
	#define kUBRRH	UBRR0H
	#define kUCSRA	UCSR0A
	#define kUCSRB	UCSR0B
	#define kUCSRC	UCSR0C
	#define kUCSZ1	UCSZ01
	#define kU2X	U2X0
	#define kUCSZ0	UCSZ00
	#define kRXEN	RXEN0
	#define kTXEN	TXEN0
	#define kUDRE	UDRE0
	#define kRXC	RXC0
	#define kUDR	UDR0
#elif defined (__AVR_ATmega16__)
	#define kUBRRL	UBRRL
	#define kUBRRH	UBRRH
	#define kUCSRA	UCSRA
	#define kUCSRB	UCSRB
	#define kUCSRC	UCSRC
	#define kUCSZ1	UCSZ1
	#define kU2X	U2X
	#define kUCSZ0	UCSZ0
	#define kRXEN	RXEN
	#define kTXEN	TXEN
	#define kUDRE	UDRE
	#define kRXC	RXC
	#define kUDR	UDR	
#else
	#error No known CPU defined
#endif

void serial_init(void) {
	uart_out = fdevopen(uart_putChar, uart_getChar);
	stdout = stdin = uart_out;
    uart_init();
}

void uart_init(void) {
    kUBRRH = UBRRH_VALUE;
    kUBRRL = UBRRL_VALUE;

#if USE_2X
    kUCSRA |= _BV(kU2X);
#else
    kUCSRA &= ~(_BV(kU2X));
#endif

	// 8-bit data
#if defined (__AVR_ATmega16__)
    kUCSRC = _BV(URSEL) | _BV(kUCSZ1) | _BV(kUCSZ0); 
#else
    kUCSRC = _BV(kUCSZ1) | _BV(kUCSZ0);   
#endif	
    kUCSRB = _BV(kRXEN) | _BV(kTXEN);   // Enable RX and TX 
}

int uart_putChar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putChar('\r', stream);
    }
    loop_until_bit_is_set(kUCSRA, kUDRE);
    kUDR = c;
	return 0;
}

int uart_getChar(FILE *stream) {
    loop_until_bit_is_set(kUCSRA, kRXC); // Wait until data exists. 
    return kUDR;
}



char* dftoa(double val, char * s, uint8_t base, uint8_t float_dig, uint8_t integ_dig) {
	if (base < 2) base = 10;
	char buf[24 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];
	uint32_t integral = (uint32_t)val;
	uint32_t fract = (uint32_t)((val - integral) * pow(10, float_dig));
	*str = '\0';
	uint8_t nLen = 0;
	uint8_t nFloatLen = 0;
	int i;
	for (i = 0; i<2; i++) {
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

char* ditoa(long val, char * s, uint8_t base, uint8_t minimum_digits) {
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
