/*
 * MudaLED.c
 *
 * Created: 06/10/2021 18:57:05
 * Author : pbatm
 */ 
#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>

#define led PB2

void acendeLed(){
	PORTB |= (1<<led);
}

void apagaLed(){
	PORTB &= ~(1<<led);
}

int main(void)
{
	DDRB |= 0x04;     
	
    while (1) 
    {
		acendeLed();
		_delay_ms(100);
		apagaLed();
		_delay_ms(100);
    }
}

