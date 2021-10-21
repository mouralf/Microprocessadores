    /*
 * GccApplication1.c
 *
 * Created: 01/10/2021 10:00:40
 * Author : luiz-
 */ 

#include <avr/io.h>
#include <math.h>		//utilizado para arredondar o número de contagens

void delay_ms(float tempo_ms){
	/*Função que recebe o tempo em ms como parâmetro e proporciona um delay correspondente ao tempo solicitado*/
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer
	
	//Sendo a frequência do timer de 16 MHz e um prescaler de 1024 temos: 16 MHz/1024 = 15625
	//Desse modo, o tempo do timer é dado por 1/15625 = 64 us
	//Assim, o número de contagens é dado por tempo_ms/64 * 1000
	unsigned char n_contagem = round(tempo_ms/64*1000);		//número de contagens arredondado
	OCR0A = n_contagem;										//Valor de comparação 3 contagens (1,66 ms)
	
	TIFR0 = (1 << 1);										//limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


//tudo na main é só pra testar se a função ta funcionando direitinho
int main(void)
{
	
	//LED no pino PB5
	DDRB = (1 << 5);	//pino PB5 é saída (LED)
	
	
    float tempo_ms = 10;
	/* Replace with your application code */
    while (1) 
    {
		PORTB |= (1 << 5);	//Ativa LED
		delay_ms(tempo_ms);
		PORTB &= ~(1 << 5);	//apaga LED
		delay_ms(tempo_ms);
    }
}

