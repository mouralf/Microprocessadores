/*
 * validaPlaca.c
 *
 * Created: 19/10/2021 20:01:59
 * Author : pbatm
 */ 

#include <avr/io.h>


int validaPlaca(char* str){  //retorna 0 se a placa � inv�lida, e 1 se a placa � v�lida
	int i = 0;
	
	while(i != 7){
		
		if(i < 3){
			if(str[i] < 65){   //na tabela ascii o numero decimal equivalente em que come�a a ter letras � 65
				return 0;		//caso seja menor que 65 n�o � uma letra ou seja � invalida a placa, nas 3 primeiras letras
			}
		}
		
		if(i >= 3){
			if(str[i] < 48 || str[i] > 57){		//verifica se os ultimos 4 caracteres sao numeros
				if(str[i] >= 65){				//verifica se � uma letra
					if(i != 4){					//caso seja uma letra, verifica se � no quinto caracter por causa das novas placas
						return 0;				//caso n�o seja retorna placa invalida
					}
				}
				else {							//caso n�o seja uma letra nem n�mero retorna placa invalida
					return 0;
				}				
			}
		}
		i++;
	}
	return 1;									//retorna 1 se a placa for v�lida
}


int main(void)
{
    char placa[] = "ABC1D34";
	int val;
	
	val = validaPlaca(placa);
		
    while (1) 
    {
    }
}

