/*
 * validaPlaca.c
 *
 * Created: 19/10/2021 20:01:59
 * Author : pbatm
 */ 

#include <avr/io.h>






//define os pinos de enable e registrer select
#define RS PB0
#define EN PB1

//variaveis para controle do LCD (valor pro RS)
#define CNFG 0													//para configurar o display
#define DADO 1													//para envio de dados

//definições para inicializar o LCD :)
#define LCD_4BIT_MODE 0x02							// Modo 4-bits
#define LCD_SET 0x28										//dois nibbles: 0010 1000 -> function set
#define LCD_DSP_CTR 0x0C 								//dois nibbles: 0000 1100 -> display control
#define LCD_ENT_MODE 0x06 							//dois nibbles: 0000 0110 -> entry mode
#define LCD_CLR 0x01										//dois nibbles: 0000 0001 -> display clear



//configurações visuais do LCD
#define LCD_SEG_LINHA 0xC0							//1100 0000 -> envia o cursor pra segunda linha
#define LCD_HOME 	0x02							//0000 0010 -> volta o cursor para o inicio
#define LCD_SHIFT 	0x1D							//0001 1101 -> incrementa o display pra direita

void LCD_control(unsigned char c, unsigned char control_type);
void LCD_init();
void enviaString(char* str);
void delay_lcd();
void delay();

void delay_lcd(){
	char t;
	t = 70;
	while (t != 0)
	t--;
}


void LCD_control(unsigned char c, unsigned char control_type){
	/*Função para controle e envio de dados do/pro LCD  */
	//c -> numero hexadecimal para setar os pinos do LCD
	// control_type -> pode ser para configurações do LCD (0) ou envio de dados (1)
	
	if(control_type == 1){
		PORTB |= (1<<RS);
	}
	else{ if(control_type == 0){
		PORTB &= ~(1<<RS);
	}
}

PORTD &= 0x0F; 						//0000 1111
PORTD |= (c & 0xF0);				//Envia o primeiro nibble

PORTB |= (1<<EN);															//seta EN como 1
PORTB &= ~(1<<EN);															//seta EN como 0

delay_lcd();

c = c<<4;															//desloca 4 bits para a esquerda
PORTD &= 0x0F; 													    //1111 0000
PORTD |= (c & 0xF0); 													//Envia o segundo nibble

PORTB |= (1<<EN);																//seta EN como 1
PORTB &= ~(1<<EN);	 															//seta EN como 0

delay_lcd();
}



void LCD_init(){
	DDRB |= 0x03;								// Define os pinos PB0 e PB1 como output
	DDRD |= 0xF0;								// Define os pinos PD4, PD5, PD6 e PD7 como output
	delay_lcd();
	LCD_control(LCD_HOME, CNFG); //TESTANDO
	LCD_control(LCD_SET, CNFG); 				//function set
	LCD_control(LCD_DSP_CTR, CNFG); 			//display control
	LCD_control(LCD_ENT_MODE, CNFG); 			//entry mode
	LCD_control(LCD_CLR, CNFG);					//Limpa a tela
	LCD_control(0x80, CNFG);	// Sets cursor to (0,0). SE DER CERTO TESTAR SEM DEP
}

void enviaString(char* str){
	int index = 0;
	while (str[index] != 0){
		LCD_control(str[index], 1);
		index++;
	}
}

void enviaChar(char c){
	LCD_control(c, 1);    //envia o char c
}

void enviaInt(int c){
	LCD_control(c+48, 1); //soma 48 para ser o numero em char da tabela ascii
}

void delay(){
	unsigned char c;
	for (c = 0; c<150; c++);
}








//A PARTIR DE AGORA 

char const placas_idosos_PNE[3][7] = {
	"IDO1020",
	"IDS0089",
	"PNE0102"
};	

int comparaString(char* str1, char* str2){ //compara duas strings de 7 algarismos para verificar se sao iguais
	int i;
	
	for(i=0; i<7; i++){
		if(str1[i] != str2[i])				//caso sejam diferentes, retorna 0
			return 0;
	}
	return 1;								//caso sejam iguais, retorna 1
}


int validaPlaca(char* str){  //retorna 0 se a placa é inválida, e 1 se a placa é válida
	int j, k, i = 0;
	char aux[7];
	
	
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
	
	
	for(j=0; j<3; j++){
		for(k=0; k<7; k++){
			aux[k] = placas_idosos_PNE[j][k];		//copia a placa do idoso para uma string auxiliar
		}
		if(comparaString(str, aux))				//compara as duas placas
			return 2;							//retorna 2 se for caso especial, ex. idoso
	}
	return 1;									//retorna 1 se a placa for válida
}


int main(void)
{
    char placa[] = "PNE0102";
	int val, especial;
	
	DDRB = 0x03;
	DDRD = 0xF0;
	delay_lcd();
	
	LCD_init();
	delay_lcd();
	
	val = validaPlaca(placa);
	
	if(val==1)
		enviaString("Placa valida");
		
	if(val==0)
		enviaString("Placa invalida");
		
	if(val==2){
		enviaString("Placa preferencial");
		especial = 1;
	}
		
    while (1) 
    {
    }
}

