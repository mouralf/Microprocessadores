#include <avr/io.h>
#include <math.h>

void delay_ms(float tempo_ms){
	/*Função que recebe o tempo em ms como parâmetro e proporciona um delay correspondente ao tempo solicitado*/
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer
	
	//Sendo a frequência do timer de 16 MHz e um prescaler de 1024 temos: 16 MHz/1024 = 15625
	//Desse modo, o tempo do timer é dado por 1/15625 = 64 us
	//Assim, o número de contagens é dado por tempo_ms/64 * 1000
	unsigned char n_contagem = round(tempo_ms/64*1000);		//número de contagens arredondado
	OCR0A = n_contagem;										//Valor de comparação 3 contagens (1,66 ms)
	
	TIFR0 = (1 << 1);										//limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


//-------------------------------------------- Definições do LCD --------------------------------------------
//define os pinos de enable e registrer select
#define RS PB0
#define EN PB1

//variaveis para controle do LCD (valor pro RS)
#define CNFG 0										//para configurar o display
#define DADO 1										//para envio de dados

//definições para inicializar o LCD :)
#define LCD_4BIT_MODE 0x02							// Modo 4-bits
#define LCD_SET 0x28								//dois nibbles: 0010 1000 -> function set
#define LCD_DSP_CTR 0x0C 							//dois nibbles: 0000 1100 -> display control
#define LCD_ENT_MODE 0x06 							//dois nibbles: 0000 0110 -> entry mode
#define LCD_CLR 0x01								//dois nibbles: 0000 0001 -> display clear



//configurações visuais do LCD
#define LCD_SEG_LINHA	0xC0							//1100 0000 -> envia o cursor pra segunda linha
#define LCD_HOME 		0x02							//0000 0010 -> volta o cursor para o inicio
#define LCD_SHIFT 		0x1D							//0001 1101 -> incrementa o display pra direita
#define LCD_NO_SHIFT	0x11							//0001 0001 -> decrementa o cursor pra esquerda

// ------------------------------------- Funções pro LCD --------------------------
void LCD_control(unsigned char c, unsigned char control_type);
void LCD_init();
void enviaString(char* str);



void LCD_control(unsigned char c, unsigned char control_type){
	/*Função para controle e envio de dados do/pro LCD  */
	//c -> numero hexadecimal para setar os pinos do LCD
	// control_type -> pode ser para configurações do LCD (0) ou envio de dados (1)
	
	if(control_type == 1){
		PORTB |= (1<<RS);
	}
	else{
		if(control_type == 0){
			PORTB &= ~(1<<RS);
		}
	}

	PORTD &= 0x0F; 						//0000 1111
	PORTD |= (c & 0xF0);				//Envia o primeiro nibble

	PORTB |= (1<<EN);					//seta EN como 1
	PORTB &= ~(1<<EN);					//seta EN como 0

	delay_ms(10);

	c = c<<4;							//desloca 4 bits para a esquerda
	PORTD &= 0x0F; 						//1111 0000
	PORTD |= (c & 0xF0); 				//Envia o segundo nibble

	PORTB |= (1<<EN);					//seta EN como 1
	PORTB &= ~(1<<EN);	 				//seta EN como 0

	delay_ms(10);
}



void LCD_init(){
	//configuração inicial pro LCD
	DDRB = 0x03;
	DDRD = 0xF0;
	delay_ms(10);
	
	DDRB |= 0x03;								// Define os pinos PB0 e PB1 como output
	DDRD |= 0xF0;								// Define os pinos PD4, PD5, PD6 e PD7 como output
	delay_ms(20);
	LCD_control(LCD_HOME, CNFG);				//TESTANDO
	LCD_control(LCD_SET, CNFG); 				//function set
	LCD_control(LCD_DSP_CTR, CNFG); 			//display control
	LCD_control(LCD_ENT_MODE, CNFG); 			//entry mode
	LCD_control(LCD_CLR, CNFG);					//Limpa a tela
	LCD_control(0x80, CNFG);					// Sets cursor to (0,0). SE DER CERTO TESTAR SEM DEP
	
	delay_ms(10);
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

void enviaCharEsq(char c){
	//envia o char com o deslocamento pra esquerda
	LCD_control(LCD_NO_SHIFT, 0);	//configura o display pra deslocar o cursor pra esquerda
	LCD_control(c, 1);    //envia o char c

}
void enviaInt(int c){
	LCD_control(c+48, 1); //soma 48 para ser o numero em char da tabela ascii
}



// -------------------------------- Definições pro teclado --------------------------
//definições para as linhas
#define LINHA1 PD0	//D0
#define LINHA2 PD1	//D1
#define LINHA3 PD2	//D2
#define LINHA4 PD3	//D3

//definições para as colunas
#define COLUNA1 PC1	//A1
#define COLUNA2 PC2	//A2
#define COLUNA3 PC3	//A3

#define INTERVALOMAX 1000 //máximo intervalo de tempo em ms entre cliques no botão pra  percorrer o alfabeto
void Keyboard_config(){
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
	PORTC |= (1 << COLUNA1); //set PC1 as INPUT_PULLUP
	PORTC |= (1 << COLUNA2); //set PC2 as INPUT_PULLUP
	PORTC |= (1 << COLUNA3); //set PC3 as INPUT_PULLUP
}






char TecladoMatricial(){
	//função para realizar a multiplexação pra identificar a tecla pressionada
	char teclasMatricial [4][3] =	{	//[linhas][colunas]
		{'1','2', '3'},
		{'4','5', '6'},
		{'7','8', '9'},
		{'*','0', '#'},
	};
	char tecla_pressionada = ' ';
	
	//início do algoritmo para varrer o teclado
	for (int linha = 0; linha<4; linha++){				//percorre todas as linhas
		PORTD &= ~(1 << linha);							//coloca o pino referente à linha em LOW
		for (int outras = 0; outras < 4; outras++){		//percorre novamente todas as linhas
			if(outras!= linha){							//verifica se outras é diferente de linha, se for
				PORTD |= (1 << outras);					//coloca as outras portas em HIGH
			}
		}
		//delay_ms(10);
		for (int colunas = 1; colunas <=3; colunas++){			//percorre todas as colunas
			if(!((PINC & (1 << colunas)) >> colunas)){			//se a coluna em questão for LOW, então significa que foi pressionada
				tecla_pressionada =  (teclasMatricial[linha][colunas-1]); //armazena a tecla pressionada
				while(!((PINC & (1 << colunas)) >> colunas));	//debounce simples
				//delay_ms(5);
				break;
			}
		}
	}
	delay_ms(10);

	return tecla_pressionada;
}


int main(void)
{
	LCD_init();
	//delay_ms(10);
	
	char teclasTelefone_3L [8][4] = { //[linhas][colunas]
		{'2', 'A', 'B', 'C'},	//linha 0
		{'3', 'D', 'E', 'F'},	//linha 1
		{'4', 'G', 'H', 'I'},	//linha 2
		{'5', 'J', 'K', 'L'},	//linha 3
		{'6', 'M', 'N', 'O'},	//linha 4
		{'8', 'T', 'U', 'V'},	//linha 5
	};	//fim de teclasTelefone_3L
	
	
	char teclasTelefone_4L [2][5] = { //[linhas][colunas]
		{'7', 'P', 'Q', 'R', 'S'}, //linha 0
		{'9', 'W', 'X', 'Y', 'Z'}, //linha 1
	}; //fim de teclasTelefone_4L
	
	char nContagens = 0;
	char linhaM;
	char caracter;
	
	Keyboard_config();
	
	/*------------------ FUNÇÃO ---------------------*/
	LCD_control(LCD_SEG_LINHA, 0);
	char teclaAnterior = '/';
	/* Replace with your application code */
	while (1)
	{
		
		//
		char teclaAtual = TecladoMatricial();
		
		if(teclaAtual != ' '){
			if (teclaAtual == '1' || teclaAtual == '0' || teclaAtual == '*' || teclaAtual == '#')
			{
				enviaChar(teclaAtual);
			}//fecha o if tecla atual é 1, 0, #, *
			
			else{ //se não for 1, 0, # ou *
				
				if(teclaAtual == teclaAnterior){ //se a tecla pressionada for igual à pressionada anteriormente
					nContagens ++;	//incrementa o número de contagens
					switch (teclaAtual){	//altera a linha da matriz conforme o número
						case '2':
						linhaM = 0;
						break;
						case '3':
						linhaM = 1;
						break;
						case '4':
						linhaM = 2;
						break;
						case '5':
						linhaM = 3;
						break;
						case '6':
						linhaM = 4;
						break;
						case '7':
						linhaM = 0;
						break;
						case '8':
						linhaM = 5;
						break;
						case '9':			//se a tecla pressionada for 9, vai pra segunda linha da matriz
						linhaM = 1;
						break;
					}
					
					if (teclaAtual == '7' || teclaAtual == '9')	//se a tecla for 7 ou 9, percorre a matriz de 4 letras
					{
						if (nContagens == 5){	//se atingir o limite do tamanho da matriz zera o contador
							nContagens = 0;
						}
						caracter = teclasTelefone_4L[linhaM][nContagens];
					}	//fecha o if que verifica se a tecla é 7 ou 9
					
					else{ //se a tecla for 2,3,4,5,6, ou 8
						if (nContagens == 4){	//se atingir o limite do tamanho da matriz zera o contador
							nContagens = 0;
						}
						caracter = teclasTelefone_3L[linhaM][nContagens];
					}
					
					enviaCharEsq(caracter);
					
				} //fecha o if teclaAtual == teclaAnterior
				else{
					nContagens = 0;
					teclaAnterior = teclaAtual;
					enviaChar(teclaAtual);
					
				} //else teclaAtual == teclaAnterior
				
			} //else tecla atual é 1, 0, #, *
			
		} //teclaAtual != ' '
		
	} //while
	
}//main
