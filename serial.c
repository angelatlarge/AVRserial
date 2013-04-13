
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


#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined (__AVR_ATmega644__) || defined (__AVR_ATmega644A__)
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
#elif defined (__AVR_ATmega16__) || defined (__AVR_ATmega32__)
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
	#define USE_URSEL
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
#	if defined (USE_URSEL)
		kUCSRC = _BV(URSEL) | _BV(kUCSZ1) | _BV(kUCSZ0); 
#	else
		kUCSRC = _BV(kUCSZ1) | _BV(kUCSZ0);   
#	endif	
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



