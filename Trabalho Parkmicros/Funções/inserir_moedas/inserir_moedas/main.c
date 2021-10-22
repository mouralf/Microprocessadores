/*
 * inserir_moedas.c
 *
 * Created: 22/10/2021 00:40:26
 * Author : Tio will
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7

void Buttons_config(){
	DDRB &= ~(1 << DDB3);	//set D11 as INPUT
	DDRB &= ~(1 << DDB4);	//set D12 as INPUT
}

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

float Buttons_validation(){
	/*Valida se algum bot�o foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	int contagem = 0;
	
	while(count != BOUNCE && contagem != 15){		//enquanto count n�o atingir 7 e espera sem toque no bot�o passar de determinado tempo com contagem
		_delay_ms(50);								//delay para corrigir o debouncaa
		contagem++;									// vari�vel para determinar tempo de espera dentro desse while
		if(PINB >> BOTAO1 == 1){					//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o bot�o D11/PB3
			botaoAtual = (PINB >> BOTAO1);			//le o estado atual do bot�o D11
			if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
				count++;}else{count = 0;}						//incrementa o contador			//sen�o, zera o contador
				
				if (count == BOUNCE){						//se o contador atingir o valor de 7
					valor = valor + 0.50;					//seta o valor a ser retornado para 0.50
					contagem = 0;							//reinicia contagem
					}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
				}
				
				if(PINB >> BOTAO2 == 1){					//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o bot�o D12/PB4
					botaoAtual = (PINB >> BOTAO2);			//le o estado atual do bot�o D12
					if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
						count++;							//incrementa o contador de bounce
						}else{count = 0;}					//sen�o, zera o contador de bounce
						
						if (count == BOUNCE){				//se o contador atingir o valor de 7
							valor = valor + 1;				//seta o valor a ser retornado para 1
							contagem = 0;					//reinicia contagem  do while
							}else{botaoAntes = botaoAtual;}	//se nao tiver atingido 7, volta a contar
						}
					}
					
					return valor;									// retorna o valor inserido pelas moedas
					
				}

void escreve_moedas(){
	//fun��o para escrever no lcd quando o usu�rio terminar de inserir o dinheiro 
					
	float val = 0;													// valor recebido para cada moeda inserida, iniciado com valor zero para ser somada
	int contagem = 0;												// contagem para informar ap�s um tempo sem apertar um bot�o o dinheiro total 
																	//inserido
					
	while(1){
						
		contagem++;												 // inicia contagem
						
		val = val + Buttons_validation();						// verifica moeda inserida na fun��o e soma ao valor anterior do total de dinheiro j� inserido 
						
		if(PINB >> BOTAO2 == 1 || PINB >> BOTAO1 == 1){			// se bot�o apertado, reinicia contagem
				contagem = 0;
		}
						
		if(contagem == 5){										//ap�s certo tempo sem tocar em um bot�o, informa o dinheiro total colocado
				contagem = 0;									// reinicia contagem
				LCD_control(LCD_CLR, CNFG);						
				enviaString("   ");
				enviaInt(((int)val));							// fun��es para printar o valor em reais e centavos no lcd
				enviaString(",");
				enviaInt(((int)(val*10)%2)*5);
				enviaInt(0);
				val = 0;
				break;
		}
						
		if(val > 4.50){
				contagem = 0;									// reinicia contagem
				LCD_control(LCD_CLR, CNFG);						// informa que o usu�rio n�o poder� inserir mais moedas, pois chegou ao limite 
				enviaString("   ");
				enviaString("4,50");
				LCD_control(LCD_SEG_LINHA, CNFG);
				enviaString("   ");
				enviaString("VALOR MAXIMO!");
				val = 0;
				break;
						}
						
	}
					
					
}

int main(void){	
	
			Buttons_config();
					
			while(1){
						
						escreve_moedas();
						
					}
					
					
	return 0;
}

