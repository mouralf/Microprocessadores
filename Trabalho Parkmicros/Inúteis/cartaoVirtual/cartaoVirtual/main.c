/*
 * cartaoVirtual.c
 *
 * Created: 21/10/2021 00:54:23
 * Author : pbatm
 */ 

#include <avr/io.h>

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

#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7


//definições para as linhas
#define LINHA1 PD0	//D0
#define LINHA2 PD1	//D1
#define LINHA3 PD2	//D2
#define LINHA4 PD3	//D3

//definições para as colunas
#define COLUNA1 PC1	//A1
#define COLUNA2 PC2	//A2
#define COLUNA3 PC3	//A3

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
		delay_ms(50);								//delay para corrigir o debouncaa
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

int escreve_moedas(){
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
			break;
		}
						
		if(val >= 4.50){
			contagem = 0;									// reinicia contagem
			LCD_control(LCD_CLR, CNFG);						// informa que o usuário não poderá inserir mais moedas, pois chegou ao limite
			enviaString("   ");
			enviaString("4,50");
			LCD_control(LCD_SEG_LINHA, CNFG);
			enviaString("   ");
			enviaString("VALOR MAXIMO!");
			break;
		}
		delay_ms(100);
	}
	return val*100;
}







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
	
	if(c >= 10){
		LCD_control(c/10+48,1);
		LCD_control(c%10+48,1);
	}else
		LCD_control(c+48, 1); //soma 48 para ser o numero em char da tabela ascii
}








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

/*
int cartaoVirtual(char* placa){				//FUNÇÃO NÃO FINALIZADA
	int ind, custo;
	
	ind = verificaCartao(placa);
	
	if(ind == 15){				//se a placa nao tiver um cartao registrado, retorna 0
		return 0
	}
	
	custo = selecionaCusto();
	
	if(!verificaSaldo(ind, custo)){
										//exibir "SEM CREDITO"
	}
	else{
										//Pagamento feito :)
	}
}*/

void recargaCartao(){
	int valor;
	
	valor = escreve_moedas();
	
	
	
}

int main(void)
{
    int i;
	
	LCDConfig();
	delay_ms(100);
	LCD_init();
	delay_ms(100);
	
    while (1) 
    {
		/*LCD_control(LCD_CLR, CNFG);   TESTAR verificaCartao, mas ta funcionando
		i = verificaCartao("IOP1931");
		if(i<15)
			enviaInt(i);
		if(i == 15)
			enviaString("SEM CADASTRO");
		delay_ms(500);*/
		
		/*LCD_control(LCD_CLR,CNFG);
		int ind = 3;
		
		if(verificaSaldo(ind, 350)){					
			enviaInt(creditos_cartao[ind][0]);
			enviaChar(',');
			enviaInt(creditos_cartao[ind][1]);
			delay_ms(50);
		}else{
			//enviaString("Saldo Insuficiente");
			enviaInt(creditos_cartao[ind][0]);
			enviaChar(',');
			enviaInt(creditos_cartao[ind][1]);
			delay_ms(50);
		}
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);*/
		LCD_control(LCD_CLR,CNFG);
		int val = escreve_moedas();
		enviaInt(val);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
		delay_ms(500);
    }
}

