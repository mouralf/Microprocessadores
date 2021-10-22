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

float Buttons_validation(){
	/*Valida se algum botão foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	int contagem = 0;
	
	while(count != BOUNCE && contagem != 15){		//enquanto count não atingir 7 e espera sem toque no botão passar de determinado tempo com contagem
		_delay_ms(50);								//delay para corrigir o debouncaa
		contagem++;									// variável para determinar tempo de espera dentro desse while
		if(PINB >> BOTAO1 == 1){					//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o botão D11/PB3
			botaoAtual = (PINB >> BOTAO1);			//le o estado atual do botão D11
			if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
				count++;}else{count = 0;}						//incrementa o contador			//senão, zera o contador
				
				if (count == BOUNCE){						//se o contador atingir o valor de 7
					valor = valor + 0.50;					//seta o valor a ser retornado para 0.50
					contagem = 0;							//reinicia contagem
					}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
				}
				
				if(PINB >> BOTAO2 == 1){					//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o botão D12/PB4
					botaoAtual = (PINB >> BOTAO2);			//le o estado atual do botão D12
					if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
						count++;							//incrementa o contador de bounce
						}else{count = 0;}					//senão, zera o contador de bounce
						
						if (count == BOUNCE){				//se o contador atingir o valor de 7
							valor = valor + 1;				//seta o valor a ser retornado para 1
							contagem = 0;					//reinicia contagem  do while
							}else{botaoAntes = botaoAtual;}	//se nao tiver atingido 7, volta a contar
						}
					}
					
					return valor;									// retorna o valor inserido pelas moedas
					
				}

void escreve_moedas(){
	//função para escrever no lcd quando o usuário terminar de inserir o dinheiro 
					
	float val = 0;													// valor recebido para cada moeda inserida, iniciado com valor zero para ser somada
	int contagem = 0;												// contagem para informar após um tempo sem apertar um botão o dinheiro total 
																	//inserido
					
	while(1){
						
		contagem++;												 // inicia contagem
						
		val = val + Buttons_validation();						// verifica moeda inserida na função e soma ao valor anterior do total de dinheiro já inserido 
						
		if(PINB >> BOTAO2 == 1 || PINB >> BOTAO1 == 1){			// se botão apertado, reinicia contagem
				contagem = 0;
		}
						
		if(contagem == 5){										//após certo tempo sem tocar em um botão, informa o dinheiro total colocado
				contagem = 0;									// reinicia contagem
				LCD_control(LCD_CLR, CNFG);						
				enviaString("   ");
				enviaInt(((int)val));							// funções para printar o valor em reais e centavos no lcd
				enviaString(",");
				enviaInt(((int)(val*10)%2)*5);
				enviaInt(0);
				val = 0;
				break;
		}
						
		if(val > 4.50){
				contagem = 0;									// reinicia contagem
				LCD_control(LCD_CLR, CNFG);						// informa que o usuário não poderá inserir mais moedas, pois chegou ao limite 
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

