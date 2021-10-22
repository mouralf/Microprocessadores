#include "biblioteca.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void delay_ms(float tempo_ms){
	/*Fun��o que recebe o tempo em ms como par�metro e proporciona um delay correspondente ao tempo solicitado*/
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer
	
	//Sendo a frequ�ncia do timer de 16 MHz e um prescaler de 1024 temos: 16 MHz/1024 = 15625
	//Desse modo, o tempo do timer � dado por 1/15625 = 64 us
	//Assim, o n�mero de contagens � dado por tempo_ms/64 * 1000
	unsigned char n_contagem = round(tempo_ms/64*1000);		//n�mero de contagens arredondado
	OCR0A = n_contagem;										//Valor de compara��o 3 contagens (1,66 ms)
	
	TIFR0 = (1 << 1);										//limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}

void timer_ms(float tempo_ms){
	/*Fun��o que recebe o tempo em ms como par�metro e proporciona um timer correspondente ao tempo solicitado com interrup��o*/
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer
	
	//Sendo a frequ�ncia do timer de 16 MHz e um prescaler de 1024 temos: 16 MHz/1024 = 15625
	//Desse modo, o tempo do timer � dado por 1/15625 = 64 us
	//Assim, o n�mero de contagens � dado por tempo_ms/64 * 1000
	unsigned char n_contagem = round(tempo_ms/64*1000);		//n�mero de contagens arredondado
	OCR0A = n_contagem;										//Valor de compara��o 3 contagens (1,66 ms)
	
	TIMSK0 = (1 << OCIE0A);	//habilita interrup��o por atingir o tempo
}
ISR(TIMER0_COMPA_vect){	//interrup��o por compara��o
	enviaChar('M');	//envia uma mssg dizendo
}


int main(void)
{
	PartsConfig();
	enviaChar('a');
	
	TCNT0 = 0;
	
	
	sei();	//habilita interrup��es
	
	char enviaTecla;
	
	while (1)
	{
		enviaTecla = TecladoMatricial();
		enviaChar(enviaTecla);
		
		timer_ms(100);
	} //while
	
}//main
	
