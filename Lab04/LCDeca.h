#include <reg51.h>
//define os pinos de enable e registrer select
sbit RS = P1^3;
sbit EN = P1^2;

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
#define LCD_HOME 			0x02							//0000 0010 -> volta o cursor para o inicio
#define LCD_SHIFT 		0x1D							//0001 1101 -> incrementa o display pra direita

void LCD_control(unsigned char c, unsigned char control_type);
void LCD_init();
void enviaString(char* str);
void delay_lcd();
void delay();

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
	
	RS = control_type; 										//0 ou 1
	
	EN = 1;																//seta EN como 1
	P1 &= 0x0F; 													//0000 1111	
	P1 |= (c & 0xF0); 										//Envia o primeiro nibble
	EN = 0; 															//seta EN como 0
	
	delay_lcd();
	
	EN = 1; 															//seta EN como 1
	c = c<<4;															//desloca 4 bits para a esquerda
	P1 &= 0x0F; 													//1111 0000 
	P1 |= c; 															//Envia o segundo nibble
	EN = 0; 															//seta EN como 0
	
	delay_lcd();
}



void LCD_init(){
	P1 = 0x20;
	RS = 0;
	EN = 1;
	EN = 0;
	delay();
	LCD_control(LCD_SET, CNFG); 					//function set
	LCD_control(LCD_DSP_CTR, CNFG); 			//display control
	LCD_control(LCD_ENT_MODE, CNFG); 			//entry mode

}

void enviaString(char* str){
	int index = 0;
	while (str[index] != 0){
		LCD_control(str[index], 1);
		index++;
	}

}

void delay(){
	unsigned char c;
	for (c = 0; c<150; c++);
}


