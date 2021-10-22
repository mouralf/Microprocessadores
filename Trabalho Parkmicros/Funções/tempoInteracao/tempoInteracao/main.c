/*
 * tempoInteracao.c
 *
 * Created: 20/10/2021 00:58:07
 * Author : pbatm
 */ 

#include <avr/io.h>

int tempoInteracao(int tempo_atual, int tempo_click){
	
	if((tempo_atual - tempo_click) >= 24){		//verifica se nenhuma interação foi feita nos ultimos 24 seg
		piscaLed();								//CRIAR FUNCAO PISCA LED
		if((tempo_atual - tempo_click) >=40)
			return 0;							//caso estoure o tempo de 40 segundos retorna 0 
	}
	return 1									//caso a interação tenha sido feita a pouco tempo retorna 1
}




int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

