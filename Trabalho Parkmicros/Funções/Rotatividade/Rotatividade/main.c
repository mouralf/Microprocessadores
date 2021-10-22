/*
 * Rotatividade.c
 *
 * Created: 21/10/2021 18:51:41
 * Author : Tio will
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


// Essa fun��o insere o tempo utilizado pelo usu�rio a partir do dinheiro inserido e o tipo de usu�rio(idoso, normal)
// A fun��o tem entradas para o tipo de usu�rio, o tempo global que verifica quando exatamente o usu�rio solicitou a
// utiliza��o do parkimetro e o dinheiro inserido atrav�s da fun��o de inserir moedas

int rotatividade(int tipo, int tempo, float dinheiro){
	
	int tempo_pagamento = 0;
	
	if(tipo == 1){								// condi��o para que seja utilzido as condi��es de cobran�a para o usu�rio padr�o
		
		switch ((int)dinheiro)					// entrada da vari�vel dinheiro
		{
			case 1:								// para cada case temos uma condi��o para quantidade de dinheiro inserido
			tempo_pagamento = 8;				// a vari�vel de tempo � dado pelo TIMER 1, ela conta a cada 4 segundos, logo para cada unidade dessa vari�vel temos 4 segundos 
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
	
	if(tipo == 2){								// condi��o para que o usu�rio do tipo idoso
		
		switch ((int)dinheiro)					// mesma ideia para o usu�rio padr�o acima 
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

