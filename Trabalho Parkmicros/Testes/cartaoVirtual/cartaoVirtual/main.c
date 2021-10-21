/*
 * cartaoVirtual.c
 *
 * Created: 21/10/2021 00:54:23
 * Author : pbatm
 */ 

#include <avr/io.h>

char const placas_cartao[10][7] = {
	"IAC3719",
	"IMN0123",
	"ISD1A07",
	"ICB8720",
	"IWE9103",
	"IOP1931",
	"-------",
	"-------",
	"-------",
	"-------"
};
char const creditos_cartao[10][2] = {
	30, 50, 	// R$30,5
	8,  50,		// R$8,5
	20,  0,		// R$20,0
	30,  0,		// R$30,0
	20,  0,		// R$20,0
	10, 50,		// R$10,5
	0, 	0,
	0, 	0,
	0, 	0,
	0, 	0
};

int comparaString(char* str1, char* str2){ //compara duas strings de 7 algarismos para verificar se sao iguais
	int i;
	
	for(i=0; i<7; i++){
		if(str1[i] != str2[i])				//caso sejam diferentes, retorna 0
		return 0;
	}
	return 1;								//caso sejam iguais, retorna 1
}

int verificaCartao(char* placa){
	int i,k;
	char aux[7];
	
	for(i=0; i<10; i++){
		for(k=0; k<7; k++){
			aux[k] = placas_cartao[i][k];		//copia a placa com cartao para uma string auxiliar
		}
		if(comparaString(placa, aux))			//compara as duas placas
		{
			return i;							//caso tenha cartao registrado, retorna o indice do cartao
		}				
	}
	return 15;									//caso não tenha cartao registrado retorna 15
}

int verificaSaldo(int ind, int valor){			//recebe o valor a ser pago e o indice do cartao
	int aux = 0;
	
	aux = ((creditos_cartao[ind][0])*100) + creditos_cartao[ind][1]; //armazena em aux o valor do cartao , se for 3,50 reais, armazena 350
	
	if(aux >= valor){							//verifica se o valor do cartao é suficiente
		creditos_cartao[ind][0] = (aux-valor)/100;	//atualiza a conta do cartao o valor pago.
		creditos_cartao[ind][1] = (aux-valor)%100;
	}
	else{
		return 0;								//retorna 0 caso o saldo seja insuficiente
	}
	return 1;									//retorna 1 caso o valor tenha sido pago
}

int cadastraCartao(char* placa){
	int j, ind = 15;
	
	for(j=0; j<10; j++){
		if(placas_cartao[j][1] == '-'){		//procura no array de strings se não atingiu o limite de cadastro
			ind = j;
			j = 10;
		}
	}
	
	if(ind != 15){							//caso nao tenha atinjido o limite, registra a placa no próximo espaço para placa no array
		for(j=0; j<7; j++){
			placas_cartao[ind][j] = placa[j];
		}
	}
	return ind;							//retorna o indice para poder ser incrementado o valor inserido depois, 
										//se for igual a 15, atinjiu o limite e nao pode ser cadastrado
}

int cartaoVirtual(char* placa){				//FUNÇÃO NÃO FINALIZADA
	int val, custo;
	
	val = verificaCartao(placa);
	
	if(val == 15){				//se a placa nao tiver um cartao registrado, retorna 0
		return 0
	}
	
	custo = selecionaCusto();
	
	if(!verificaSaldo(val, custo)){
										//exibir "SEM CREDITO"
	}
	
}

int main(void)
{
    
    while (1) 
    {
    }
}

