/*
 * Buttons.c
 *
 * Created: 20/10/2021 10:11:43
 * Author : luiz-
 */ 

#include <avr/io.h>
#include <math.h>

#define BOTAO1 PB3 //D11 -> referente a R$ 0,50
#define BOTAO2 PB4 //D12 -> referente a R$ 1,00

#define BOUNCE 7

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

void Buttons_config(){
	/*configura as portas referentes aos botões como INPUT*/
	DDRB &= ~(1 << DDB3);	//set D11 as INPUT
	DDRB &= ~(1 << DDB4);	//set D12 as INPUT
}



//void Debounce_validation()

float Buttons_validation(){
	/*Valida se algum botão foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	
	while(count != BOUNCE){						//enquanto count não atingir 7
		delay_ms(1);							//delay para corrigir o debounce
		if(PINB >> BOTAO1 == 1){				//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o botão D11/PB3
			botaoAtual = (PINB >> BOTAO1);		//le o estado atual do botão D11
			if (botaoAtual == botaoAntes)		//se for igual ao estado anterior
			count++;							//incrementa o contador
			else
			count = 0;							//senão, zera o contador
			
			if (count == BOUNCE){				//se o contador atingir o valor de 7
				valor = 0.50;					//seta o valor a ser retornado para 0.50
				break;
			}
			else
			botaoAntes = botaoAtual;			//se nao tiver atingido 7, volta a contar
		}
		if(PINB >> BOTAO2 == 1){				//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o botão D12/PB4
			botaoAtual = (PINB >> BOTAO2);		//le o estado atual do botão D12
			if (botaoAtual == botaoAntes)		//se for igual ao estado anterior
			count++;							//incrementa o contador
			else
			count = 0;							//senão, zera o contador
			
			if (count == BOUNCE){				//se o contador atingir o valor de 7
				valor = 1;						//seta o valor a ser retornado para 1
				break;
			}
			else
			botaoAntes = botaoAtual;			//se nao tiver atingido 7, volta a contar
		}
	}
	return valor;
}

int main(void)
{
	Buttons_config(); //chama a função que configura as portas dos botões como INPUT
	/* Replace with your application code */
	while (1)
	{
		Buttons_validation(); //chama a função que verifica se um botão foi pressionado
	}
}

  
 
