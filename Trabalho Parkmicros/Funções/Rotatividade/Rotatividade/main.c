/*
 * Rotatividade.c
 *
 * Created: 21/10/2021 18:51:41
 * Author : Tio will
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


// Essa função insere o tempo utilizado pelo usuário a partir do dinheiro inserido e o tipo de usuário(idoso, normal)
// A função tem entradas para o tipo de usuário, o tempo global que verifica quando exatamente o usuário solicitou a
// utilização do parkimetro e o dinheiro inserido através da função de inserir moedas

int rotatividade(int tipo, int tempo, float dinheiro){
	
	int tempo_pagamento = 0;
	
	if(tipo == 1){								// condição para que seja utilzido as condições de cobrança para o usuário padrão
		
		switch ((int)dinheiro)					// entrada da variável dinheiro
		{
			case 1:								// para cada case temos uma condição para quantidade de dinheiro inserido
			tempo_pagamento = 8;				// a variável de tempo é dado pelo TIMER 1, ela conta a cada 4 segundos, logo para cada unidade dessa variável temos 4 segundos 
			break;
			
			case 2:
			tempo_pagamento = 16;
			break;
			
			case 3:
			tempo_pagamento = 24;
			break;
			
			case 4:
			tempo_pagamento = 32;
			break;
		}
		
		
		
	}
	
	if(tipo == 2){								// condição para que o usuário do tipo idoso
		
		switch ((int)dinheiro)					// mesma ideia para o usuário padrão acima 
		{
			case 0:
			tempo_pagamento = 8;
			break;
			
			case 1:
			tempo_pagamento = 16;
			break;
			
			case 2:
			tempo_pagamento = 24;
			break;
			
			case 3:
			tempo_pagamento = 32;
			break;
			
			case 4:
			tempo_pagamento = 9000;
			break;
		}
		
		
	}

	return (tempo +  tempo_pagamento);
}


int main(void)
{
	
	int tempo = 0;
	float dinheiro = 2.5;
	int tipo = 2;
	int tempo_estouro;
	
	tempo_estouro = rotatividade(tipo, tempo, dinheiro-0.5);
	
    /* Replace with your application code */
    while (1) 
    {
		
	 }
	 
	
}

