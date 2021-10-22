// Biblioteca poggers



/************************************ LCD ************************************/ 
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

// ------------------------------------- Protótipos pro LCD --------------------------
void LCD_control(unsigned char c, unsigned char control_type);
void LCD_init();
void LCDConfig();
void enviaString(char* str);
void enviaChar(char c);
void enviaInt(int c);
void delay_lcd();


/************************************ TECLADO ************************************/
#include <math.h>
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


// -------------------------------- Protótipos pro teclado --------------------------
void Keyboard_config();
void Keyboard_validation();
char TecladoMatricial();
char TecladoTelefonico();

/************************************ LED ************************************/

#define led PB2
// -------------------------------- Protótipos pro LED --------------------------
void LedConfig();
void acendeLed();
void apagaLed();
void piscaLed();


/************************************ HORÁRIO ************************************/
void Timer1Config();
void atraso();
int horarioAtual(int segundos);
void printa_hora(int segundos);


/************************************ MOEDAS/BOTÕES ************************************/
#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7

void Buttons_config();
float Buttons_validation();
void escreve_moedas();

/************************************ Delay ************************************/
void delay_ms(float tempo_ms);


/************************************ CARTÃO ************************************/
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

void verificaCartao(char* placa);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ------------------------------------- Funções pro LCD --------------------------
void delay_lcd(){
	char t;
	t = 70;
	while (t != 0)
	t--;
}

void LCDConfig(){
	DDRB = 0x03;
	DDRD = 0xF0;
	delay_ms(100);
	
	LCD_init();
	delay_ms(100);
	
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

// ------------------------------------- Funções pro teclado --------------------------
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

void Keyboard_validation(){
	unsigned char count = 0;
	unsigned char teclaAntes = 0, teclaAtual;
	
	for (int LINHA = 0; LINHA<4; LINHA++){				//percorre todas as linhas
		PORTD &= ~(1 << LINHA);							//coloca o pino referente à LINHA em LOW
		for (int OUTRAS = 0; OUTRAS<4; OUTRAS++){		//percorre novamente todas as linhas
			if(OUTRAS!= LINHA){							//verifica se OUTRAS é diferente de LINHA, se for
				PORTD |= (1<<OUTRAS);					//coloca as outras portas em HIGH
			}
		}
		delay_ms(10);
		for (int COLUNAS = 1; COLUNAS <=3; COLUNAS++){	//percorre todas as colunas
			if(!((PINC & (1 << COLUNAS)) >> COLUNAS)){	//se a coluna em questão for LOW, então significa que foi pressionada
				
				//LCD_control(LCD_CLR, CNFG);
				
				enviaInt(LINHA);
				enviaInt(COLUNAS);
				while(!((PINC & (1 << COLUNAS)) >> COLUNAS));	//bounce simples
				delay_ms(10);
				break;
				
			}
		}
		
	}
	delay_lcd();
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
	delay_lcd();
	return tecla_pressionada;
}

char TecladoTelefonico(){
	int intervaloCliques = 0;
	
	char teclaAtual = TecladoMatricial();
	char teclaAnterior = teclaAtual;
	char nApertos = 0;
	char caracterPressionado;	//utilizado para armazenar e retornar o caractere correspondente ao que foi pressionado
	char linhaM = 0, colunaM = 0; //utilizados para percorrer as matrizes das teclas
	
	//matriz com as teclas de telefone com apenas 3 letras por número
	char teclasTelefone_3L [8][4] = { //[linhas][colunas]
		{'2', 'A', 'B', 'C'},	//linha 0
		{'3', 'D', 'E', 'F'},	//linha 1
		{'4', 'G', 'H', 'I'},	//linha 2
		{'5', 'J', 'K', 'L'},	//linha 3
		{'6', 'M', 'N', 'O'},	//linha 4
		{'8', 'A', 'B', 'C'},	//linha 5
	};	//fim de teclasTelefone_3L
	
	//matriz com as teclas de telefone com 4 letras por número
	char teclasTelefone_4L [2][5] = { //[linhas][colunas]
		{'7', 'P', 'Q', 'R', 'S'}, //linha 0
		{'9', 'W', 'X', 'Y', 'Z'}, //linha 1
	}; //fim de teclasTelefone_4L
	
	
	//se a tecla apertada não tiver letras, retorna direto o caracter correspondente
	if (teclaAtual == '1' || teclaAtual == '0' || teclaAtual == '*' || teclaAtual == '#')
	{
		caracterPressionado = teclaAtual;
	} //fecha o if que verifica se a tecla pressionada foi 1, 0, * ou #
	
	else //se a tecla pressionada não for 1, 0, * ou #
	{
		if (teclaAtual == '7' || teclaAtual == '9')		//se a tecla em questão tiver 4 letras percorre a matriz teclasTelefone_4L
		{
			if (teclaAtual == 9)						//se a tecla pressionada for a 9, altera a linha para 1. Se não for, já está setado em 0
			{
				linhaM = 1;	//correspondente à segunda linha da matriz teclasTelefone_4L
			}//fecha o if que define a linha da matriz caso a tecla seja 9
			
			
			//verifica se a tecla pressionada novamente é igual à tecla pressionada anteriormente
			if (teclaAtual == teclaAnterior)
			{
				//chama o timer que verifica se o intervalo de tempo foi excedido
				while (intervaloCliques < INTERVALOMAX)
				{
					nApertos++;
					if (nApertos == 4)	//zera o nApertos caso ele atinja o valor de 4, pois passa do limite da matriz
					{
						nApertos == 0;
					}
					
					caracterPressionado = teclasTelefone_4L[linhaM][nApertos];
					break;
				} //fecha o while do intervalo de cliques
			} //fecha o if que verifica se a tecla atual é igual à anterior
			
			
		}	//fecha o if que verifica se a tecla é 7 ou 9
	}

// ------------------------------------- Funções pro Led --------------------------
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


// ------------------------------------- Funções pro Horário --------------------------
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
	
	enviaString("      ");			//a partir daqui será o envio da hora e minuto para o LCD
	
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

int horarioAtual(int segundos){ //Função para saber o horário
	
	atraso();
	segundos++;						//incrementa a variável de segundos, sendo cada segundo equivalente a 4 segundos reais
	
	if(segundos >= 21600){			//Esse valor é igual ao numero de segundos no dia, e quando chegar irá resetar o valor.
		segundos = 0;
	}		
		return segundos;				//retorna a variavel segundos para que se tenha controle do tempo
}



// ------------------------------------- Funções pros Botões --------------------------
void Buttons_config(){
	DDRB &= ~(1 << DDB3);	//set D11 as INPUT
	DDRB &= ~(1 << DDB4);	//set D12 as INPUT
}

float Buttons_validation(){
	/*Valida se algum botão foi pressionado e, se sim, faz o debounce e retorna o valor correspondnete (0,50 ou 1,00 real)*/
	unsigned char botaoAntes = 0, botaoAtual;	//utilizados para controle do debounce
	unsigned char count = 0;					//utilizados para controle do debounce
	float valor = 0;
	int contagem = 0;
	
	while(count != BOUNCE && contagem != 15){		//enquanto count não atingir 7 e espera sem toque no botão passar de determinado tempo com contagem
		_delay_ms(50);								//delay para corrigir o debouncaa
		contagem++;									// variável para determinar tempo de espera dentro desse while
		if(PINB >> BOTAO1 == 1){					//se a leitura do registrador B deslocada em 3 bits for 1 foi pressionado o botão D11/PB3
			botaoAtual = (PINB >> BOTAO1);			//le o estado atual do botão D11
			if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
				count++;}else{count = 0;}						//incrementa o contador			//senão, zera o contador
				
				if (count == BOUNCE){						//se o contador atingir o valor de 7
					valor = valor + 0.50;					//seta o valor a ser retornado para 0.50
					contagem = 0;							//reinicia contagem
					}else{botaoAntes = botaoAtual;}			//se nao tiver atingido 7, volta a contar
				}
				
				if(PINB >> BOTAO2 == 1){					//se a leitura do registrador B deslocada em 4 bits for 1 foi pressionado o botão D12/PB4
					botaoAtual = (PINB >> BOTAO2);			//le o estado atual do botão D12
					if (botaoAtual == botaoAntes){			//se for igual ao estado anterior
						count++;							//incrementa o contador de bounce
						}else{count = 0;}					//senão, zera o contador de bounce
						
						if (count == BOUNCE){				//se o contador atingir o valor de 7
							valor = valor + 1;				//seta o valor a ser retornado para 1
							contagem = 0;					//reinicia contagem  do while
							}else{botaoAntes = botaoAtual;}	//se nao tiver atingido 7, volta a contar
						}
					}
					
					return valor;									// retorna o valor inserido pelas moedas
					
				}

void escreve_moedas(){
	//função para escrever no lcd quando o usuário terminar de inserir o dinheiro
	
	float val = 0;													// valor recebido para cada moeda inserida, iniciado com valor zero para ser somada
	int contagem = 0;												// contagem para informar após um tempo sem apertar um botão o dinheiro total
	//inserido
	
	while(1){
		
		contagem++;												 // inicia contagem
		
		val = val + Buttons_validation();						// verifica moeda inserida na função e soma ao valor anterior do total de dinheiro já inserido
		
		if(PINB >> BOTAO2 == 1 || PINB >> BOTAO1 == 1){			// se botão apertado, reinicia contagem
			contagem = 0;
		}
		
		if(contagem == 5){										//após certo tempo sem tocar em um botão, informa o dinheiro total colocado
			contagem = 0;									// reinicia contagem
			LCD_control(LCD_CLR, CNFG);
			enviaString("   ");
			enviaInt(((int)val));							// funções para printar o valor em reais e centavos no lcd
			enviaString(",");
			enviaInt(((int)(val*10)%2)*5);
			enviaInt(0);
			val = 0;
			break;
		}
		
		if(val > 4.50){
			contagem = 0;									// reinicia contagem
			LCD_control(LCD_CLR, CNFG);						// informa que o usuário não poderá inserir mais moedas, pois chegou ao limite
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

// ------------------------------------- Função do Delay --------------------------
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



// ------------------------------------- Funções do Cartão --------------------------
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


// ------------------------------------- Funções pra placa --------------------------
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