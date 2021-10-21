/*
 * GccApplication2.c
 *
 * Created: 19/10/2021 19:36:28
 * Author : Tio will
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7

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
	for (c = 0; c<300; c++);
}

void Buttons_config(){
	DDRB &= ~(1 << DDB3);	//set D11 as INPUT
	DDRB &= ~(1 << DDB4);	//set D12 as INPUT
}

void delay_1ms(){
	TCCR0A = 0x2;        //modo CTC para o timer 0
	TCCR0B = 0x4;		//clock/256
	TCNT0 = 0;            //Zera timer
	OCR0A = 63;        //Valor de comparação 63 contagens (1 ms)

	TIFR0 = (1 << 1); //limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}

float Buttons_validation(){
	/*Valida se algum botão foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	int contagem = 0;
	
	while(count != BOUNCE && contagem != 10){						//enquanto count não atingir 7
		_delay_ms(100);
		contagem++;						//delay para corrigir o debouncaa
		if(PINB >> BOTAO1 == 1){					//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o botão D11/PB3
			botaoAtual = (PINB >> BOTAO1);			//le o estado atual do botão D11
			if (botaoAtual == botaoAntes){		//se for igual ao estado anterior
			count++;}else{count = 0;}						//incrementa o contador						//senão, zera o contador
			
			if (count == BOUNCE){				//se o contador atingir o valor de 7
				valor = valor + 0.50;					//seta o valor a ser retornado para 0.50
				enviaString("B");
				contagem = 0;
			}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
		}
		
		if(PINB >> BOTAO2 == 1){					//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o botão D12/PB4
			botaoAtual = (PINB >> BOTAO2);			//le o estado atual do botão D12
			if (botaoAtual == botaoAntes){		//se for igual ao estado anterior
			count++;							//incrementa o contador
			}else{count = 0;}							//senão, zera o contador
			
			if (count == BOUNCE){				//se o contador atingir o valor de 7
				valor = valor + 1;					//seta o valor a ser retornado para 1
				enviaString("B");
				contagem = 0;
			}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
		}
	}
	
	return valor;
	
}

int main(void){
	
	float val = 0;
	int contagem = 0;
	
	Buttons_config();
	
	DDRB = 0x03;
	DDRD = 0xF0;
	delay_lcd();
	
	LCD_init();
	delay_lcd();
	
		
		
	
    while(1){ 
		
		contagem++;
		
		delay_lcd();
		enviaString(".");
		val = val + Buttons_validation();
		enviaString(",");
		
		if(PINB >> BOTAO2 == 1 || PINB >> BOTAO1 == 1){
			contagem = 0;
		}
		
		if(contagem == 10){
		contagem = 0;
		LCD_control(LCD_CLR, CNFG);
		enviaString("   ");
		enviaInt(((int)val));
		enviaString(",");
		enviaInt(((int)(val*10)%2)*5);
		enviaInt(0);
		val = 0;
		}
		
		if(val > 4.50){
		contagem = 0;
		LCD_control(LCD_CLR, CNFG);
		enviaString("   ");
		enviaString("4,50");
		LCD_control(LCD_SEG_LINHA, CNFG);
		enviaString("   ");
		enviaString("VALOR MAXIMO!");
		val = 0;
		}
		
	}
		
		
	return 0;
}
