/*
//RS = 0: configurações; RS = 1: dados
//Enable - 0 1 0 -> informa que os pinos de dados devem ser lidos


Forma de comunicação

###### Inicizalização do display #########
//E = 1 -> Envio do primeiro nibble -> E = 0 -> E = 1 - >Envio do segundo nibble

----- Envio do primeiro nibble ------
RS		R/W		D7		D6		D5		D4		E
0			 0		0			0			1			0			1

RS		R/W		D7		D6		D5		D4		E
0			 0		0			0			1			0			0

Aguardar >4 ms

----- Envio do segundo nibble -------

RS		R/W		D7		D6		D5		D4		E
0			 0		0			0			1			0			1

RS		R/W		D7		D6		D5		D4		E
0			 0		0			0			1			0			0

RS		R/W		D7		D6		D5		D4		E
0			 0		1			0			1			0			1

RS		R/W		D7		D6		D5		D4		E
0			 0		1			0			1			0			0

Aguardar >4 ms
*/

#include <reg51.h>

//seta os pinos de dados
sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
//seta os pinos de enable e registrer select
sbit RS = P1^3;
sbit E = P1^2;

//#define EN 4	//PB4 -> EN
//#define RS 7	//PD7 -> RS

//variaveis para controle do LCD (valor pro RS)
#define CNFG 0	//para configurar o display
#define DADO 1		//para envio de dados

//definições para inicializar o LCD (pinos D7 D6 D5 D4 e D3 D2 D1 D0) :)
#define LCD_SET 0x28	//dois nibbles: 0010 1000 -> function set
#define LCD_DSP_CTR 0x0C //dois nibbles: 0000 1100 -> liga o display e cursor off
#define LCD_ENT_MODE 0x06 //dois nibbles: 0000 0110 -> incrementa o cursor

//0010 0000
//0000 0010

void LCD_control(unsigned char c, unsigned char control_type){
	//c -> numero hexadecimal para setar os pinos do LCD
	// control_type -> pode ser para configurações do LCD (0) ou envio de dados (1)
	
	if (control_type == CNFG){ 						//Configuração do LCD
		RS = 0; 														//Desliga RS
	}
	
	EN = 1;																//seta EN como 1
	P1 &= 0x0F; 													//0000 1111	
	P1 |= (c & 0x0F); 										//Envia o primeiro nibble
	EN = 0; 															//seta EN como 0
	
	//aguarda 40 us
	delay_40();
	
	EN = 1; 															//seta EN como 1
	P1 &= 0x0F; 													//1111 0000 
	P1 |= (c << 4); 											//Envia o segundo nibble
	EN = 0; 															//seta EN como 0
	
	delay_40();
	
	if (control_type == CNFG){						//Se realizou uma configuração
		RS = 1;															//volta a setar o RS para receber dados
	}
}

void delay_40(){
	unsigned char t = 70;
	
	while (t != 0)
		t--;
}

void LCD_init(){
	P1 = 0x20;
	RS = 0;
	E = 1;
	E = 0;
	
	delay_40();
	
	LCD_control(LCD_SET, CNFG); 					//function set
	LCD_control(LCD_DSP_CTR, CNFG); 			//display control
	LCD_control(LCD_ENT_MODE, CNFG); 			//entry mode
}