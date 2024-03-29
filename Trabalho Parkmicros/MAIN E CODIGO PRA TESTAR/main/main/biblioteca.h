#include <avr/io.h>
#include <math.h>

// Biblioteca poggers
/******************************************** SE��O 1 - DEFINI��ES E PROT�TIPOS *************************************/

/************************************ Delay ************************************/
void delay_ms(float tempo_ms);


/************************************ LCD ************************************/
//define os pinos de enable e registrer select
#define RS PB0
#define EN PB1

//variaveis para controle do LCD (valor pro RS)
#define CNFG 0													//para configurar o display
#define DADO 1													//para envio de dados

//defini��es para inicializar o LCD :)
#define LCD_4BIT_MODE 0x02							// Modo 4-bits
#define LCD_SET 0x28										//dois nibbles: 0010 1000 -> function set
#define LCD_DSP_CTR 0x0C 								//dois nibbles: 0000 1100 -> display control
#define LCD_ENT_MODE 0x06 							//dois nibbles: 0000 0110 -> entry mode
#define LCD_CLR 0x01										//dois nibbles: 0000 0001 -> display clear



//configura��es visuais do LCD
#define LCD_SEG_LINHA 0xC0							//1100 0000 -> envia o cursor pra segunda linha
#define LCD_HOME 	0x02							//0000 0010 -> volta o cursor para o inicio
#define LCD_SHIFT 	0x1D							//0001 1101 -> incrementa o display pra direita
#define LCD_NO_SHIFT	0x11							//0001 0001 -> decrementa o cursor pra esquerda

// ------------------------------------- Prot�tipos pro LCD --------------------------
void LCD_control(unsigned char c, unsigned char control_type);
void LCD_init();
void LCDConfig();
void enviaString(char* str);
void enviaChar(char c);
void enviaCharEsq(char c);
void enviaInt(int c);


/************************************ TECLADO ************************************/
#include <math.h>
// -------------------------------- Defini��es pro teclado --------------------------
//defini��es para as linhas
#define LINHA1 PD0	//D0
#define LINHA2 PD1	//D1
#define LINHA3 PD2	//D2
#define LINHA4 PD3	//D3

//defini��es para as colunas
#define COLUNA1 PC1	//A1
#define COLUNA2 PC2	//A2
#define COLUNA3 PC3	//A3

#define INTERVALOMAX 1000 //m�ximo intervalo de tempo em ms entre cliques no bot�o pra  percorrer o alfabeto


// -------------------------------- Prot�tipos pro teclado --------------------------
void Keyboard_config();
void Keyboard_validation();
char TecladoMatricial();

/************************************ LED ************************************/

#define led PB2
// -------------------------------- Prot�tipos pro LED --------------------------
void LedConfig();
void acendeLed();
void apagaLed();
void piscaLed();


/************************************ HOR�RIO ************************************/
void Timer1Config();
void atraso();
int horarioAtual(int segundos);
void printa_hora(int segundos);


/************************************ MOEDAS/BOT�ES ************************************/
#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7

void Buttons_config();
float Buttons_validation();
void escreve_moedas();


/************************************ CART�O ************************************/
char placas_cartao[10][7] = {
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

int creditos_cartao[10][2] = {
	{30, 50}, 	// R$30,5
	{8,  50},		// R$8,5
	{20,  0},		// R$20,0
	{30,  0},		// R$30,0
	{20,  0},		// R$20,0
	{10, 50},		// R$10,5
	{0,	0},
	{0, 0},
	{0, 0},
	{0, 0},
};

int verificaCartao(char* placa);
int verificaSaldo(int ind, int valor);
int cadastraCartao(char* placa);


/************************************ PLACA ************************************/
char const placas_idosos_PNE[3][7] = {
	"IDO1020",
	"IDS0089",
	"PNE0102"
};

int comparaString(char* str1, char* str2);
int validaPlaca(char* placa);

/************************ CONFIGURA��ES GERAIS ***************************/
void PartsConfig();

// ******************************************************* SE��O 2 - FUN��ES ********************************************************************************
// ------------------------------------- Fun��o do Delay --------------------------
void delay_ms(float tempo_ms){
	/*Fun��o que recebe o tempo em ms como par�metro e proporciona um delay correspondente ao tempo solicitado*/
	TCCR0A = 0x2;		//modo CTC para o timer 0
	TCCR0B = 0x5;		//PRESCALER de 1024
	TCNT0 = 0;			//Zera timer

	//Sendo a frequ�ncia do timer de 16 MHz e um prescaler de 1024 temos: 16 MHz/1024 = 15625
	//Desse modo, o tempo do timer � dado por 1/15625 = 64 us
	//Assim, o n�mero de contagens � dado por tempo_ms/64 * 1000
	unsigned char n_contagem = round(tempo_ms/64*1000);		//n�mero de contagens arredondado
	OCR0A = n_contagem;										//Valor de compara��o 3 contagens (1,66 ms)

	TIFR0 = (1 << 1);										//limpa flag de comparacao A
	while((TIFR0 & (1 << 1)) == 0);
}


// ------------------------------------- Fun��es pro LCD --------------------------
void LCD_control(unsigned char c, unsigned char control_type){
	/*Fun��o para controle e envio de dados do/pro LCD  */
	//c -> numero hexadecimal para setar os pinos do LCD
	// control_type -> pode ser para configura��es do LCD (0) ou envio de dados (1)

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
	//configura��o inicial pro LCD
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

// ------------------------------------- Fun��es pro teclado --------------------------
void Keyboard_config(){
	//configura as linhas como sa�da
	DDRD |= (1 << DDD0); //set PD0 as OUTPUT
	DDRD |= (1 << DDD1); //set PD1 as OUTPUT
	DDRD |= (1 << DDD2); //set PD2 as OUTPUT
	DDRD |= (1 << DDD3); //set PD3 as OUTPUT

	//configura as colunas como entrada
	DDRC &= ~(1 << DDC1); //set PC1 as INPUT
	DDRC &= ~(1 << DDC2); //set PC2 as INPUT
	DDRC &= ~(1 << DDC3); //set PC3 as INPUT

	//configura as resist�ncias de  pull_up das colunas
	PORTC |= (1 << COLUNA1); //set PC1 as INPUT_PULLUP
	PORTC |= (1 << COLUNA2); //set PC2 as INPUT_PULLUP
	PORTC |= (1 << COLUNA3); //set PC3 as INPUT_PULLUP
}



char teclaAnterior = '/';
int nContagens = 0;

char TecladoMatricial(){
	//LCD_control(LCD_SEG_LINHA, 0); //NECESS�RIO CHAMAR ESSA FUN��O ANTES DE PEGAR ALGO DO TECLADO
	
	//fun��o para realizar a multiplexa��o pra identificar a tecla pressionada
	char teclasMatricial [4][3] =	{	//[linhas][colunas]
		{'1','2', '3'},
		{'4','5', '6'},
		{'7','8', '9'},
		{'*','0', '#'},
	};
	
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
	
	
	
	
	int linhaM = 0; //para percorrer as matrizes
	char caracter;
	
	char tecla_pressionada = ' ';

	
	//in�cio do algoritmo para varrer o teclado
	for (int linha = 0; linha<4; linha++){				//percorre todas as linhas
		PORTD &= ~(1 << linha);							//coloca o pino referente � linha em LOW
		for (int outras = 0; outras < 4; outras++){		//percorre novamente todas as linhas
			if(outras!= linha){							//verifica se outras � diferente de linha, se for
				PORTD |= (1 << outras);					//coloca as outras portas em HIGH
			}
		}
		//delay_ms(10);
		for (int colunas = 1; colunas <=3; colunas++){			//percorre todas as colunas
			if(!((PINC & (1 << colunas)) >> colunas)){			//se a coluna em quest�o for LOW, ent�o significa que foi pressionada
				tecla_pressionada =  (teclasMatricial[linha][colunas-1]); //armazena a tecla pressionada
				while(!((PINC & (1 << colunas)) >> colunas));	//debounce simples
				//delay_ms(5);
				break;
			}
		}
	}
	
	delay_ms(10);
	
	char teclaAtual = tecla_pressionada;
	
	if(teclaAtual != ' '){
		//enviaChar('d');
		if (teclaAtual == '1' || teclaAtual == '0' || teclaAtual == '*' || teclaAtual == '#')
		{
			caracter = teclaAtual;
			enviaChar(caracter);
		}//fecha o if tecla atual � 1, 0, #, *
		
		else{ //se n�o for 1, 0, # ou *
			
			if(teclaAtual == teclaAnterior){ //se a tecla pressionada for igual � pressionada anteriormente
				nContagens ++;	//incrementa o n�mero de contagens
				switch (teclaAtual){	//altera a linha da matriz conforme o n�mero
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
				}	//fecha o if que verifica se a tecla � 7 ou 9
				
				else{ //se a tecla for 2,3,4,5,6, ou 8 percorre a matriz de 3 letras
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
				enviaChar(caracter);
			} //else teclaAtual == teclaAnterior
			
		} //else tecla atual � 1, 0, #, *
		
	} //teclaAtual != ' '
	
	//teclaAnterior = caracter;
	
	return caracter;
}

// ------------------------------------- Fun��es pro Led --------------------------
void LedConfig(){
	DDRB |= 0x04;
}

void acendeLed(){
	PORTB |= (1<<led);
}

void apagaLed(){
	PORTB &= ~(1<<led);
}

void piscaLed(){
	acendeLed();
	delay_ms(25);
	apagaLed();
	delay_ms(25);
}


// ------------------------------------- Fun��es pro Hor�rio --------------------------
void Timer1Config(){
	TCCR1A = 0;
	TCCR1B = 0x5;
	TCNT1 = 3036;		 //ajustado com o erro do loop
}

void atraso(){
	TCNT1 = 3036;				    //Para 0,25 Hz
									// assim: 65536 - 62500 = 3036
	while ((TIFR1 & (1<<0)) == 0);  // Aguarda flag de estouro
	TIFR1 = (1<<0);				    //Limpa flag de estouro
}

void printa_hora(int segundos){

	int hora = segundos/900;			//transforma os segundos em horas
	int min = segundos/15 - 60*hora;	//transforma os segundos em minutos


	LCD_control(0x80, CNFG);		//posiciona o cursor do LCD no inicio da linha

	enviaString("      ");			//a partir daqui ser� o envio da hora e minuto para o LCD

	if(hora > 9){
		enviaInt(hora/10);
		enviaInt(hora%10);

	}else{enviaString("0");enviaInt(hora);}


	enviaString(":");

	if(min > 9){
		enviaInt(min/10);
		enviaInt(min%10);

	}else{enviaString("0");enviaInt(min);}
}

int horarioAtual(int segundos){ //Fun��o para saber o hor�rio

	atraso();
	segundos++;						//incrementa a vari�vel de segundos, sendo cada segundo equivalente a 4 segundos reais
	if(segundos >= 21600){			//Esse valor � igual ao numero de segundos no dia, e quando chegar ir� resetar o valor.
		segundos = 0;
	}
	return segundos;				//retorna a variavel segundos para que se tenha controle do tempo
}



// ------------------------------------- Fun��es pros Bot�es --------------------------
void Buttons_config(){
	DDRB &= ~(1 << DDB3);	//set D11 as INPUT
	DDRB &= ~(1 << DDB4);	//set D12 as INPUT
}

float Buttons_validation(){
	/*Valida se algum bot�o foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	int contagem = 0;

	while(count != BOUNCE && contagem != 15){		//enquanto count n�o atingir 7 e espera sem toque no bot�o passar de determinado tempo com contagem
		delay_ms(50);								//delay para corrigir o debouncaa
		contagem++;									// vari�vel para determinar tempo de espera dentro desse while
		if(PINB >> BOTAO1 == 1){					//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o bot�o D11/PB3
			botaoAtual = (PINB >> BOTAO1);			//le o estado atual do bot�o D11
			if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
				count++;}else{count = 0;}						//incrementa o contador			//sen�o, zera o contador

				if (count == BOUNCE){						//se o contador atingir o valor de 7
					valor = valor + 0.50;					//seta o valor a ser retornado para 0.50
					contagem = 0;							//reinicia contagem
					}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
				}

				if(PINB >> BOTAO2 == 1){					//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o bot�o D12/PB4
					botaoAtual = (PINB >> BOTAO2);			//le o estado atual do bot�o D12
					if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
						count++;							//incrementa o contador de bounce
						}else{count = 0;}					//sen�o, zera o contador de bounce

						if (count == BOUNCE){				//se o contador atingir o valor de 7
							valor = valor + 1;				//seta o valor a ser retornado para 1
							contagem = 0;					//reinicia contagem  do while
							}else{botaoAntes = botaoAtual;}	//se nao tiver atingido 7, volta a contar
						}
					}

					return valor;									// retorna o valor inserido pelas moedas

				}

void escreve_moedas(){
	//fun��o para escrever no lcd quando o usu�rio terminar de inserir o dinheiro

	float val = 0;													// valor recebido para cada moeda inserida, iniciado com valor zero para ser somada
	int contagem = 0;												// contagem para informar ap�s um tempo sem apertar um bot�o o dinheiro total
	//inserido

	while(1){

		contagem++;												 // inicia contagem

		val = val + Buttons_validation();						// verifica moeda inserida na fun��o e soma ao valor anterior do total de dinheiro j� inserido

		if(PINB >> BOTAO2 == 1 || PINB >> BOTAO1 == 1){			// se bot�o apertado, reinicia contagem
			contagem = 0;
		}

		if(contagem == 5){										//ap�s certo tempo sem tocar em um bot�o, informa o dinheiro total colocado
			contagem = 0;									// reinicia contagem
			LCD_control(LCD_CLR, CNFG);
			enviaString("   ");
			enviaInt(((int)val));							// fun��es para printar o valor em reais e centavos no lcd
			enviaString(",");
			enviaInt(((int)(val*10)%2)*5);
			enviaInt(0);
			val = 0;
			break;
		}

		if(val > 4.50){
			contagem = 0;									// reinicia contagem
			LCD_control(LCD_CLR, CNFG);						// informa que o usu�rio n�o poder� inserir mais moedas, pois chegou ao limite
			enviaString("   ");
			enviaString("4,50");
			LCD_control(LCD_SEG_LINHA, CNFG);
			enviaString("   ");
			enviaString("VALOR MAXIMO!");
			val = 0;
			break;
		}
	}
}



// ------------------------------------- Fun��es do Cart�o --------------------------
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
	return 15;									//caso n�o tenha cartao registrado retorna 15
}

int verificaSaldo(int ind, int valor){			//recebe o valor a ser pago e o indice do cartao
	int aux = 0;

	aux = ((creditos_cartao[ind][0])*100) + creditos_cartao[ind][1]; //armazena em aux o valor do cartao , se for 3,50 reais, armazena 350

	if(aux >= valor){							//verifica se o valor do cartao � suficiente
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
		if(placas_cartao[j][1] == '-'){		//procura no array de strings se n�o atingiu o limite de cadastro
			ind = j;
			j = 10;
		}
	}

	if(ind != 15){							//caso nao tenha atinjido o limite, registra a placa no pr�ximo espa�o para placa no array
		for(j=0; j<7; j++){
			placas_cartao[ind][j] = placa[j];
		}
	}
	return ind;							//retorna o indice para poder ser incrementado o valor inserido depois,
	//se for igual a 15, atinjiu o limite e nao pode ser cadastrado
}


// ------------------------------------- Fun��es pra placa --------------------------
int comparaString(char* str1, char* str2){ //compara duas strings de 7 algarismos para verificar se sao iguais
	int i;

	for(i=0; i<7; i++){
		if(str1[i] != str2[i])				//caso sejam diferentes, retorna 0
		return 0;
	}
	return 1;								//caso sejam iguais, retorna 1
}


int validaPlaca(char* str){  //retorna 0 se a placa � inv�lida, e 1 se a placa � v�lida
	int j, k, i = 0;
	char aux[7];


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


	for(j=0; j<3; j++){
		for(k=0; k<7; k++){
			aux[k] = placas_idosos_PNE[j][k];		//copia a placa do idoso para uma string auxiliar
		}
		if(comparaString(str, aux))				//compara as duas placas
		return 2;							//retorna 2 se for caso especial, ex. idoso
	}
	return 1;									//retorna 1 se a placa for v�lida
}


// ------------------------------- Fun��es de inicializa��o ou configura��es gerais
void PartsConfig(){
	//fun��o respons�vel por configurar os perif�ricos e garantir a inicializa��o correta e de uma forma r�pida
	LCD_init();
	Keyboard_config();
	Buttons_config();
	Timer1Config();
	LedConfig();
}
