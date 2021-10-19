/*
 * debounce.c
 *
 * Created: 18/10/2021 18:12:55
 * Author : pbatm
 */ 


//MUDAR AS FUNÇÕES PARA O ARDUINO :)


#include <avr/io.h>

#define BOUNCE 7

void delay_1ms(){
	TCCR0A = 0x2;        //modo CTC para o timer 0
	TCCR0B = 0x4;		//clock/256
	TCNT0 = 0;            //Zera timer
	OCR0A = 63;        //Valor de comparação 63 contagens (1 ms)
	
	TIFR0 = (1 << 1); //limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


char debounce(char teclaPressionada){
	//avalia o estado da tecla para verificar se parou de oscilar o sinal
	unsigned char count = 0; //utilizado para verificar se estabilizou (deve ser = 7)
	unsigned char tecla_antes = 0;
	unsigned char tecla_atual;
	
	//inicia o algoritmo para verificação do debounce
	while (count != BOUNCE){
		delay_1ms(); 													//espera 1 ms para fazer a leitura
		tecla_atual = teclaPressionada;									//captura o estado atual da tecla
		
		if (tecla_atual == tecla_antes)				//verifica se é igual ao estado anterior
		count++;														//incrementa o contador se for
		else
		count = 0;													//zera o contador se não for
		
		if (count == BOUNCE)									//se chegar no valor de debounce
		break;															//sai do if para finalizar o while
		else
		tecla_antes = tecla_atual;					//se nao atingiu o BOUNCE, pega o estado atual da tecla
	}
	return tecla_atual;
}

int main(void)
{
     DDRB = (1 << 5);    //pino PB5 é saída (LED)
     while (1)
     {
	     PORTB |= (1 << 5);    //Ativa LED
	     delay_1ms();
	     PORTB &= ~(1 << 5);    //apaga LED
	     delay_1ms();
     }
}

