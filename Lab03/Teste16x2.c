//Lab 3 de Microprocessadores
//Turma D
//Luiz Felipe de Moura 00305017
//Pedro Barros Baitelli 00304123

#include <stdio.h>
#include <reg51.h>

sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS = P1^3;
sbit E = P1^2;

sbit clear = P2^4;
sbit ret = P2^5;				  
sbit left = P2^6;
sbit right = P2^7;

void returnHome(void);
void entryModeSet(bit id, bit s);
void displayOnOffControl(bit display, bit cursor, bit blinking);
void cursorOrDisplayShift(bit sc, bit rl);
void functionSet(void);
void setDdRamAddress(char address);

void sendChar(char c);
bit getBit(char c, char bitNumber);
void delay(void);

//AS QUATRO MELHORES NOTAS SÃO SOMADAS PARA DETERMINAR A PONTUAÇÃO
	char xdata LISTA[] = {
	'N','Y','A','H',' ', 0, 7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0,  //7,90; 9,11; 0,0; 0,0; 0, 0; 0,0
	'K','E','L','V','I','N',' ',0, 8, 98, 8, 84, 8, 99, 0, 0, 0, 0, 7,58, 9,34, //8,98; 8,84; 8,99; 0,0; 0,0; 7,58; 9,34
	};
void main()
{
	unsigned char indiceLista = 0; //variável para controlar a posição na lista
  unsigned int notasTemporarias[7]; //variavel para auxiliar no armazenamento das notas
  char nomeCompetidor[8][8]; //matriz para armazenar o índice e o nome do competidor
  unsigned int notasFinais[8]; //matriz para armazenar as notas dos competidores
	unsigned char  numCompetidor, letrasNome, indiceNota,indiceNotaLista, k, j, aux; //variáveis para auxiliar a armazenar notas e nome
	int notaOuro, notaPrata; //variáveis para armazenar as melhores notas do podio
  unsigned char indiceOuro , indicePrata; //variáveis para armazenar os índices do pódio
	
	
	for (numCompetidor = 0; numCompetidor<8; numCompetidor++){ //for para percorrer a lista
		letrasNome = 0; //para armazenar as letras do nome, será resetado a cada vez que for ler os dados de um competidor
		
		/*le o nome do competidor até chegar no 0*/
		do{
			
			nomeCompetidor[numCompetidor][letrasNome] = LISTA[indiceLista]; //armazena as letras do nome na matriz adequada
    	letrasNome++; //incrementa as letras do nome
      indiceLista++; //incrementa o indice da lista
		}while(LISTA[indiceLista-1] != ' ');
		
		while(LISTA[indiceLista] != 0){
			indiceLista++;
		}
		indiceLista++;  //incrementa o indice da lista

		indiceNota = 0; //para ser incrementado e pegar as notas

		for (indiceNotaLista = 0; indiceNotaLista<7; indiceNotaLista++){ //percorre a lista para pegar as notas
			notasTemporarias[indiceNota] = LISTA[indiceLista]*100 + LISTA[indiceLista+1]; //pega as notas temporarias e armazena
			indiceNota++;
			indiceLista+=2;
			
		} //fecha o for que le as notas
        

		/*ordena as 7 notas em ordem decrescente*/
		for (k = 1; k < 8; k++) {
        
        for (j = 0; j < 7; j++) {

            if (notasTemporarias[j] > notasTemporarias[j + 1]) {
                aux = notasTemporarias[j];
                notasTemporarias[j] = notasTemporarias[j + 1];
                notasTemporarias[j + 1] = aux;
            }
        }
    }
		
		
		
		//pega só as 4 maiores notas de cada competidor
		notasFinais[numCompetidor] = (notasTemporarias[6]+notasTemporarias[5]+notasTemporarias[4]+notasTemporarias[3]);
	
	}//fecha o for que percorre LISTA[]
	
	/* pega as maiores notas para definir o podio */
	for(numCompetidor = 0; numCompetidor < 8; numCompetidor++){
            if(notasFinais[numCompetidor]>notaOuro){ //se a nota lida for maior que a nota antiga do Ouro
							//redefine o prata para os valores antigos do ouro
							setDdRamAddress(0x40); // set address to start of second line
							letrasNome = 0;
							//pega o nome de cada competidor do podio
							while (nomeCompetidor[j][letrasNome] != ' '){
								sendChar(nomeCompetidor[j][letrasNome]); //armazena em RESULT o nome do competidor (ordem de podio)								
								//aux++;
								letrasNome++;
						}
							sendChar((notasFinais[j]/1000)+48); //armazena em RESULT o dígito da dezena
							sendChar(((notasFinais[j]/100)%10)+48); //armazena em RESULT o dígito da unidade
							sendChar(',');
							sendChar(((notasFinais[j]-(notasFinais[j]/100)*100)/10)+48); ////armazena em RESULT o dígito da dezena após a vírgula
							sendChar (((notasFinais[j]-(notasFinais[j]/100)*100)%10)+48); //armazena em RESULT o dígito da unidade após a virgula
						
							returnHome();
						
							letrasNome = 0;
						
						//pega o nome do ouro
							while (nomeCompetidor[j][letrasNome] != ' '){
								sendChar(nomeCompetidor[j][letrasNome]); //armazena em RESULT o nome do competidor (ordem de podio)								
								//aux++;
								letrasNome++;
						}
							sendChar((notasFinais[j]/1000)+48); //armazena em RESULT o dígito da dezena
							sendChar(((notasFinais[j]/100)%10)+48); //armazena em RESULT o dígito da unidade
							sendChar(',');
							sendChar(((notasFinais[j]-(notasFinais[j]/100)*100)/10)+48); ////armazena em RESULT o dígito da dezena após a vírgula
							sendChar (((notasFinais[j]-(notasFinais[j]/100)*100)%10)+48); //armazena em RESULT o dígito da unidade após a virgula
					
						
            } else if (notasFinais[numCompetidor]>notaPrata){ //caso a nota seja maior que a nota antiga do Prata
							setDdRamAddress(0x40); // set address to start of second line
							while (nomeCompetidor[j][letrasNome] != ' '){
								sendChar(nomeCompetidor[j][letrasNome]); //armazena em RESULT o nome do competidor (ordem de podio)								
								//aux++;
								letrasNome++;
						}
							sendChar((notasFinais[j]/1000)+48); //armazena em RESULT o dígito da dezena
							sendChar(((notasFinais[j]/100)%10)+48); //armazena em RESULT o dígito da unidade
							sendChar(',');
							sendChar(((notasFinais[j]-(notasFinais[j]/100)*100)/10)+48); ////armazena em RESULT o dígito da dezena após a vírgula
							sendChar (((notasFinais[j]-(notasFinais[j]/100)*100)%10)+48); //armazena em RESULT o dígito da unidade após a virgula
						
            } 
						
    }
		
		//inicialização do display
		functionSet();
		entryModeSet(1, 0); // increment and no shift
		displayOnOffControl(1, 1, 1); // display on, cursor on and blinking on
		
		//aux = 0;
		for (k = 0; k<2; k++){ //pega as duas melhores notas para o podio que será exibido no display
			if( k == 0){ //pega o índice do ouro
				j = indiceOuro;
			}else if( k == 1){ //pega o índice do prata
				j = indicePrata;
			} 
			
			letrasNome = 0;
			//pega o nome de cada competidor do podio
			while (nomeCompetidor[j][letrasNome] != ' '){
				sendChar(nomeCompetidor[j][letrasNome]); //armazena em RESULT o nome do competidor (ordem de podio)
				//aux++;
				letrasNome++;
		}
			sendChar((notasFinais[j]/1000)+48); //armazena em RESULT o dígito da dezena
			sendChar(((notasFinais[j]/100)%10)+48); //armazena em RESULT o dígito da unidade
			sendChar(',');
			sendChar(((notasFinais[j]-(notasFinais[j]/100)*100)/10)+48); ////armazena em RESULT o dígito da dezena após a vírgula
			sendChar (((notasFinais[j]-(notasFinais[j]/100)*100)%10)+48); //armazena em RESULT o dígito da unidade após a virgula
			//aux+=6;	  
		
			//pula pra segunda linha
			setDdRamAddress(0x40); // set address to start of second line
		}
		
		
while (1) {
		if (ret == 0) {
			returnHome();
		}
		else {	
			if (left == 0 && right == 1) {
				cursorOrDisplayShift(1, 0); // shift display left
			}
			else if (left == 1 && right == 0) {
				cursorOrDisplayShift(1, 1); // shift display right
			}	
		}	
	}
}

void returnHome(void) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB5 = 1;
	E = 1;
	E = 0;
	delay();
}	

void entryModeSet(bit id, bit s) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB6 = 1;
	DB5 = id;
	DB4 = s;
	E = 1;
	E = 0;
	delay();
}

void displayOnOffControl(bit display, bit cursor, bit blinking) {
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 0;
	E = 1;
	E = 0;
	DB7 = 1;
	DB6 = display;
	DB5 = cursor;
	DB4 = blinking;
	E = 1;
	E = 0;
	delay();
}

void cursorOrDisplayShift(bit sc, bit rl) {
	RS = 0;
	DB7 = 0;
	DB6 = 0;
	DB5 = 0;
	DB4 = 1;
	E = 1;
	E = 0;
	DB7 = sc;
	DB6 = rl;
	E = 1;
	E = 0;
	delay();
}

void functionSet(void) {
	// The high nibble for the function set is actually sent twice. Why? See 4-bit operation
	// on pages 39 and 42 of HD44780.pdf.
	DB7 = 0;
	DB6 = 0;
	DB5 = 1;
	DB4 = 0;
	RS = 0;
	E = 1;
	E = 0;
	delay();
	E = 1;
	E = 0;
	DB7 = 1;
	E = 1;
	E = 0;
	delay();
}

void setDdRamAddress(char address) {
	RS = 0;
	DB7 = 1;
	DB6 = getBit(address, 6);
	DB5 = getBit(address, 5);
	DB4 = getBit(address, 4);
	E = 1;
	E = 0;
	DB7 = getBit(address, 3);
	DB6 = getBit(address, 2);
	DB5 = getBit(address, 1);
	DB4 = getBit(address, 0);
	E = 1;
	E = 0;
	delay();
}

void sendChar(char c) {
	DB7 = getBit(c, 7);
	DB6 = getBit(c, 6);
	DB5 = getBit(c, 5);
	DB4 = getBit(c, 4);
	RS = 1;
	E = 1;
	E = 0;
	DB7 = getBit(c, 3);
	DB6 = getBit(c, 2);
	DB5 = getBit(c, 1);
	DB4 = getBit(c, 0);
	E = 1;
	E = 0;
	delay();
}

bit getBit(char c, char bitNumber) {
	return (c >> bitNumber) & 1;
}

void delay(void) {
	char c;
	for (c = 0; c < 50; c++);
}