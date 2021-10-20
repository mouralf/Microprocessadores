/*
 * validaPlaca.c
 *
 * Created: 19/10/2021 20:01:59
 * Author : pbatm
 */ 

#include <avr/io.h>


int validaPlaca(char* str){  //retorna 0 se a placa é inválida, e 1 se a placa é válida
	int i = 0;
	
	while(i != 7){
		
		if(i < 3){
			if(str[i] < 65){   //na tabela ascii o numero decimal equivalente em que começa a ter letras é 65
				return 0;		//caso seja menor que 65 não é uma letra ou seja é invalida a placa, nas 3 primeiras letras
			}
		}
		
		if(i >= 3){
			if(str[i] < 48 || str[i] > 57){		//verifica se os ultimos 4 caracteres sao numeros
				if(str[i] >= 65){				//verifica se é uma letra
					if(i != 4){					//caso seja uma letra, verifica se é no quinto caracter por causa das novas placas
						return 0;				//caso não seja retorna placa invalida
					}
				}
				else {							//caso não seja uma letra nem número retorna placa invalida
					return 0;
				}				
			}
		}
		i++;
	}
	return 1;									//retorna 1 se a placa for válida
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

