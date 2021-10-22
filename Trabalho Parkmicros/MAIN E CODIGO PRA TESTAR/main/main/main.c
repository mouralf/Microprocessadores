/*
 * main.c
 *
 * Created: 22/10/2021 11:27:17
 * Author : pbatm
 */ 

#include <avr/io.h>
#include "biblioteca.h"



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

char placas_ativas[10][7] = {
	"-------",
	"-------",
	"-------",
	"-------",
	"-------",
	"-------",
	"-------",
	"-------",
	"-------",
	"-------"
};

void digita_placa(){
	LCD_control(0x80,CNFG);
	enviaString("Digitar Placa:");			//Aparece para digitar a placa na tela
	LCD_control(LCD_SEG_LINHA,CNFG);
}

int main(void)
{
	PartsConfig();
	int tempo = 9000,i,ind, especial = 0,aux, j, ind_cartao, saldo,custo, ind_placa;
	char estadoTeclado, placa_uso[7], tecla, valor[3];
	float val = 0;
	
    while (1) 
    {
		
		tempo = horarioAtual(tempo);
		
		estadoTeclado = TecladoMatricial();
		enviaChar(estadoTeclado);
		if(tempo > 7200 && tempo < 16200){		//verifica se está no horario de funcionamento, entre 8:00 e 18:00
			estadoTeclado = TecladoMatricial();
			
			while(estadoTeclado == ' '){ // '/' é o indicador que não tem nada pressionado no teclado ******************PERGUNTARLUIZ
				tempo = horarioAtual(tempo);
				printa_hora(tempo);
	
				
				estadoTeclado = TecladoMatricial();
			} //fim do while
			
			delay_ms(100);
			
 			LCD_control(LCD_CLR,CNFG);
 			
 			while(j < 7){
				if(j == 0){
					digita_placa();
				}
 				tecla = TecladoMatricial();
				placa_uso[j] = tecla;
 				if(tecla == '0' || tecla == '1' || tecla == '2' || tecla == '3' || tecla == '4' || tecla == '5' || tecla == '6' || tecla == '7' || tecla == '8' || tecla == '9'){
 					
 					j++;
 					if(j==7){
 						if(!(validaPlaca(placa_uso))){
 							LCD_control(LCD_CLR,CNFG);
							enviaString("Placa Invalida");
							delay_ms(500);
							delay_ms(500);
							delay_ms(500);
							delay_ms(500);
							LCD_control(LCD_CLR,CNFG);
							
 							j=0;
						}
					}
 				}
 			}
 			/* 			for(i=0; i<10; i++){
 				if(placas_ativas[i][1] == '-'){		//procura no array de strings se não atingiu o limite de cadastro
 					ind_placa = i;
 					i = 10;
 				}
 			}
 			
 			for(i=0; i<7; i++){
 				placas_ativas[ind_placa][i] = placa_uso[i];
 			}
 			
 			if(validaPlaca(placa_uso) == 2)
 				especial = 1;
 			
 			LCD_control(LCD_CLR,CNFG);
 			enviaString("Placa:")
 			LCD_control(LCD_SEG_LINHA,CNFG);
 			enviaString(placa_uso);
 				
			ind_cartao = verificaCartao(placa_uso);
 				
 			if(!(ind_cartao == 15)){
 				LCD_control(LCD_CLR,CNFG);
 				enviaString("Valor ate 4,50:");
 				LCD_control(LCD_SEG_LINHA,CNFG);
 					
 				while(j < 3){
 					tecla = TecladoMatricial();
 					if(tecla != ' '){
 						valor[j] = tecla;
 						j++;
 					}
 				}	
 				custo = ((valor/100)+48)*100+(((valor%100))+48);
 				saldo = verificaSaldo(ind_cartao, custo);
				if(saldo == 0){
 					LCD_control(LCD_CLR,CNFG);
 					enviaString("Saldo Invalido");
 				}					
 				else{
														//COMEÇA A CONTAR O TEMPO DE ACORDO COM O VALOR PAGO
 				}
 			}
 			else{
 				while(val <= 0){
					val = escreve_moedas();
 				}
 														//COMEÇA A CONTAR O TEMPO DE ACORDO COM O VALOR PAGO
 			}
 			
			
 			
 			
 			
 			
 			
 			
			
				
 				LCD_control(LCD_CLR,CNFG);
 				enviaString("Cartao:* Moeda:#");
 				while(estadoTeclado != '*' || estadoTeclado != '#'){
 					estadoTeclado = TecladoMatricial();
 				}
 				if(estadoTeclado == '*'){
 					if(verificaCartao())
 					
 					
 															//pagar com cartão
			}
 				if(estadoTeclado == '#'){
															//pagar moeda	
				} 
 			}*/
			 	
 		}
 		else{
			LCD_control(0x80, CNFG);
			enviaString("FORA DE OPERACAO");	//caso esteja fora do horario de funcionamento
 		}
    }
}

