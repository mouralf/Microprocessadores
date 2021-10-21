    /*
 * GccApplication1.c
 *
 * Created: 01/10/2021 10:00:40
 * Author : luiz-
 */ 

#include <avr/io.h>
#include <math.h>

void atraso(float tempo_ms){
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer
	
	unsigned char n_contagem = round(tempo_ms/64*1000);
	OCR0A = n_contagem;			//Valor de comparação 3 contagens (1,66 ms)
	
	TIFR0 = (1 << 1); //limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


int main(void)
{
	
	//LED no pino PB5
	DDRB = (1 << 5);	//pino PB5 é saída (LED)
	
	
    float delay_ms = 2;
	/* Replace with your application code */
    while (1) 
    {
		PORTB |= (1 << 5);	//Ativa LED
		atraso(delay_ms);
		PORTB &= ~(1 << 5);	//apaga LED
		atraso(delay_ms);
    }
}

