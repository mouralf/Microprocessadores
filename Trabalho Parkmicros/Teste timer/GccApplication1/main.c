/*
 * GccApplication1.c
 *
 * Created: 01/10/2021 10:00:40
 * Author : luiz-
 */ 

#include <avr/io.h>


void atraso(){
	TIFR0 = (1 << 1); //limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


int main(void)
{
	
	//LED no pino PB5
	
	DDRB = (1 << 5);	//pino PB5 é saída (LED)
	
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x4;
	TCNT0 = 0;			//Zera timer
	OCR0A = 104;		//Valor de comparação 104 contagens (1,66 ms)
    
	/* Replace with your application code */
    while (1) 
    {
		PORTB |= (1 << 5);	//Ativa LED
		atraso();
		PORTB &= ~(1 << 5);	//apaga LED
		atraso();
    }
}

