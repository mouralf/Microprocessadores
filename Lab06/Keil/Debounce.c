#include <REG52.H>

#define BOUNCE 7

void delay_1ms(){
	/*
	frequencia da cpu = 10 MHz
	frequencia do timer = 10 MHz / 4 = 2.5 MHz -> período de 0.4 us
	
	Para o período de 1 ms, temos que ter um tempo de meia onda de 0.5 ms.
	Assim,  0.5ms / 0.4 us = 1.25 * 1000 = 1250. Para definir o prescaler, devemos dividir pelo menor valor que o resultado seja <256
	prescaler = 8 e contagem = 156 */
	
	OPTION_REG = 0x02;											// 0010
	TMR0 = 100; 														//255 + 1 - (n_contagem)
	
	while ((INTCON & (1 <<2)) == 0); 				//aguarda o estouro do timer
	INTCON &= (1<<2);												//limpa a flag de estouro
	
}

int debounce(){
	//avalia o estado da tecla para verificar se parou de oscilar o sinal
	unsigned char count = 0; //utilizado para verificar se estabilizou (deve ser = 7)
	unsigned char tecla_antes = 0;
	unsigned char tecla_atual;
	
	
	//inicia o algoritmo para verificação do debounce
	
	while (count != 7){
		
		delay_1ms(); 													//espera 1 ms para fazer a leitura
		tecla_atual = TECLA;									//captura o estado atual da tecla
		
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

void main (){
	TRISA = 0xF0;
	PORTA = 0;
	
	while (1){
		PORTA |= (1 <<2); //LIGA RA2
		delay_1ms();
		
		PORTA &= ~(1 <<2); //LIMPA RA2;
		delay_1ms();
}