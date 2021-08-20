//#include <stdio.h>

char xdata LISTA[] = {
'N','Y','A','H',' ','H','U','S','T','O','N',0, 7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0,  //7,90; 9,11; 0,0; 0,0; 0, 0; 0,0
'J','A','G','G','E','R',' ','E','A','T','O','N',0, 8, 20, 9, 5, 0, 0, 8, 70,9,40, 0, 0, 0, 0, //8,20; 9,05; 0,0; 8,70; 9,40; 0,0; 0,0
'G','U','S','T','A','V','O',' ','R','I','B','E','I','R','O',0, 7, 23, 5, 82, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,  //7,23; 5,82; 0,0; 0,0; 0,0; 0,0; 2,0
'K','E','L','V','I','N',' ','H','O','E','F','L','E','R',0, 8, 98, 8, 84, 8, 99, 0, 0, 0, 0, 7,58, 9,34, //8,98; 8,84; 8,99; 0,0; 0,0; 7,58; 9,34
'V','I','C','E','N','T',' ','M','I','L','O','U',0, 7, 87, 5, 54, 9, 23, 0, 0, 8, 34, 0,0, 8,70, //7,87; 5,54; 9,23; 0,0; 8,34; 0,0; 8,70
'A','N','G','E','L','O',' ','N','A','R','V','A','E','Z',0, 7, 1, 6, 9, 9, 0, 0, 0, 0, 0, 8,65, 8,21, //7,01; 6,89; 9,00; 0,0; 0,0; 8,65; 8,21
'A','U','R','E','L','I','A','N',' ','G','R','A','U','D',0, 4, 21, 7, 20, 8, 68, 0, 0, 9, 0, 0, 0, 0, 0, //4,21; 7,20; 8,68; 0,0; 9,0; 0,0;
'Y','U','T','O',' ','H','O','R','I','G','A','M','E',0, 8, 02, 6, 77, 9, 3, 0, 0, 9, 35, 9, 50, 9,30 //8,02; 6,77; 9,03; 0,0; 9,35; 9,50; 9,30
};


char xdata RESULT[100] ;

//AS QUATRO MELHORES NOTAS SÃO SOMADAS PARA DETERMINAR A PONTUAÇÃO

void main()
{
	int indiceLista = 0; //variável para controlar a posição na lista
  float xdata notasTemporarias[7]; //variavel para auxiliar no armazenamento das notas
  unsigned char xdata nomeCompetidor[8][15]; //matriz para armazenar o índice e o nome do competidor
  int xdata notasFinais[8]; //matriz para armazenar as notas dos competidores
	unsigned char  numCompetidor, letrasNome, indiceNota,indiceNotaLista, k, j; //variáveis para auxiliar a armazenar notas e nome
	unsigned short int  aux; //variável auxiliar
	int notaOuro , notaPrata; //variáveis para armazenar as melhores notas do podio
  unsigned short int indiceOuro , indicePrata; //variáveis para armazenar os índices do pódio
	

	
	for (numCompetidor = 0; numCompetidor<8; numCompetidor++){ //for para percorrer a lista
		letrasNome = 0; //para armazenar as letras do nome, será resetado a cada vez que for ler os dados de um competidor
		
		/*le o nome do competidor até chegar no espaço em branco*/
		//talvez mudar para do while para pegar o espaço em branco e reaproveitar no final.
		while(LISTA[indiceLista] != ' '){
                nomeCompetidor[numCompetidor][letrasNome] = LISTA[indiceLista]; //armazena as letras do nome na matriz adequada
                letrasNome++; //incrementa as letras do nome
                indiceLista++; //incrementa o indice da lista
		} 
		
		/*Incrementa o indice da lista até chegar no 0, indicando que chegou nas notas*/
		do{
			indiceLista++;  //incrementa o indice da lista
		} while(LISTA[indiceLista] != 0);
		

		indiceNota = 0; //para ser incrementado e pegar as notas

		//opção 1: vai ter que no final separar novamente a unidade e a parte decimal
		for (indiceNotaLista = 0; indiceNotaLista<7; indiceNotaLista++){ //percorre a lista para pegar as notas
			notasTemporarias[indiceNota] = LISTA[indiceLista]*100 + LISTA[indiceLista+1]; //pega as notas temporarias e armazena
			indiceNota++;
			indiceLista+=2;
			
		} //fecha o for que le as notas
		
		//opção 2: vai armazenar a unidade e a parte decimal em variáveis diferentes
		/*
		for (indiceNotaLista = 0; indiceNotaLista<7; indiceNotaLista++){ //percorre a lista para pegar as notas
			notasTemporarias[indiceNota] = LISTA[indiceLista]; //pega as notas (unidade) temporarias e armazena
			virgulasTemporarias[indiceNota] = LISTA[indiceLista+1]; //pega as notas (decimais) e armazena
			indiceNota++;
			indiceLista+=2;
			
		} //fecha o for que le as notas
    */  
		
		/*algoritmo para ordenar as 7 notas em ordem decrescente*/
		//mudar para um que gaste menos memoria
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
		//caso seja usada a opçao 1 :)
		notasFinais[numCompetidor] = (notasTemporarias[6]+notasTemporarias[5]+notasTemporarias[4]+notasTemporarias[3]);
		
	
	
	}//fecha o for que percorre LISTA[]
	
	/* pega as maiores notas para definir o podio */
	for(numCompetidor = 0; numCompetidor < 8; numCompetidor++){
            if(notasFinais[numCompetidor]>notaOuro){ //se a nota lida for maior que a nota antiga do Ouro
							//redefine o prata para os valores antigos do ouro
							notaPrata = notaOuro;
							indicePrata= indiceOuro;
							//redefine os novos valores para o ouro
							notaOuro = notasFinais[numCompetidor];
              indiceOuro = numCompetidor;
								
            } else if (notasFinais[numCompetidor]>notaPrata){ //caso a nota seja maior que a nota antiga do Prata
							//redefine os novos valores para o prata
							notaPrata = notasFinais[numCompetidor];
              indicePrata = numCompetidor;

            } 
    }
		
		aux = 0;
		for (k = 0; k<2; k++){ //percorre as melhores notas para pegar o ouro e prata
			
			if( k == 0){ //pega o índice do ouro
				j = indiceOuro;
			}else if( k == 1){ //pega o índice do prata
				j = indicePrata;
			} 
			//pega o nome de cada competidor do podio
			for(letrasNome = 0; letrasNome<15; letrasNome++){
				RESULT[aux] = nomeCompetidor[j][letrasNome]; //armazena em RESULT o nome do competidor (ordem de podio)
				aux++;
		}
			
			//pega a nota de cada competidor do podio
			//caso seja utilizada a primeira opção para armazenar as notas
			RESULT[aux] = notasFinais[j]/100; //armazena em RESULT a nota centesimal do competidor
			RESULT[aux+1] = notasFinais[j]-(notasFinais[j]/100)*100; //armazena a parte das dezenas + unidades na memória
		
			aux+=2;
		}
while(1) ; // Final do programa 
}