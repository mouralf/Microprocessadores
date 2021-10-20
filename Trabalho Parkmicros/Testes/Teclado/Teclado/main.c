/*
 * TecladoMatricial_Teste.c
 *
 * Created: 19/10/2021 12:45:56
 * Author : luiz-
 */ 

#include <avr/io.h>

//definições para as linhas
#define LINHA1 PD0	
#define LINHA2 PD1
#define LINHA3 PD2
#define LINHA4 PD3

//definições para as colunas
#define COLUNA1 PC1
#define COLUNA2 PC2
#define COLUNA3 PC3
	
void config(){
	//configura as linhas como saída
	DDRD |= (1 << DDD0); //set PD0 as OUTPUT
	DDRD |= (1 << DDD1); //set PD1 as OUTPUT
	DDRD |= (1 << DDD2); //set PD2 as OUTPUT
	DDRD |= (1 << DDD3); //set PD3 as OUTPUT
	
	//configura as colunas como entrada
	DDRC &= ~(1 << DDC1); //set PC1 as INPUT
	DDRC &= ~(1 << DDC2); //set PC2 as INPUT
	DDRC &= ~(1 << DDC3); //set PC3 as INPUT
	
	//configura as resistências de  pull_up das colunas
	PORTC = (1 << COLUNA1); //set PC1 as INPUT_PULLUP
	PORTC = (1 << COLUNA2); //set PC2 as INPUT_PULLUP
	PORTC = (1 << COLUNA3); //set PC3 as INPUT_PULLUP
}

uint8_t verificaTecla(){	
	uint8_t contador = 1;
	
	//coloca as linhas em estado HIGH (exceto a LINHA1)
	PORTD = (1 << LINHA1);		//PD0 = 1
	PORTD = (1 << LINHA2);		//PD1 = 1
	PORTD = (1 << LINHA3);		//PD2 = 1
	PORTD = (1 << LINHA4);		//PD3 = 1	
	PORTD &= ~(1 << LINHA1);	//PD0 = 0	
	
	
	
	if((PINC & (1 << COLUNA1)) == 0){  //Se a leitura do pino referente a coluna 1 (PC1) for 0
		while((PINC & (1 << COLUNA1)) == 0);

		/*
		//testes no Tinkercad
		delay(100);
		Serial.println("Linha 1 Coluna 1");
		*/
		
		return contador;
	}
	
	contador++;
	
	if((PINC & (1 << COLUNA2)) == 0){  //Se a leitura do pino referente a coluna 1 (PC1) for 0
		while((PINC & (1 << COLUNA2)) == 0);

		/*
		//testes no Tinkercad
		delay(100);
		Serial.println("Linha 1 Coluna 2");
		*/
		
		return contador;
	}
		
}

int main(void)
{
	config();
	
    /* Replace with your application code */
    while (1) 
    {
		verificaTecla();
    }
}




/*


void setup(){
	Serial.begin(9600);
	config();
}

void loop(){
	verificaTecla();
}


*/