#define BOTAO1 PB3 //D11
#define BOTAO2 PB4 //D12

#define BOUNCE 7

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


void Buttons_validation(){
 int val = 0;
  if(val == HIGH)
    Serial.println(val);
  else
    Serial.println("low");
      
     /*              
  if (PINB & (1 << PB3)) {         // check if the input is HIGH (button released)
    Serial.println("Pressionado D11");
    delay_1ms();
  }
  else if (PINB & (1 << PB4)) {         // check if the input is HIGH (button released)
    Serial.println("Pressionado D12");
    delay_1ms();
  }
  */
  
}
/*
int Buttons_debounce(int botaoPressionado){
	//avalia o estado da tecla para verificar se parou de oscilar o sinal
	unsigned char count = 0; //utilizado para verificar se estabilizou (deve ser = 7)
	unsigned char tecla_antes = 0;
	unsigned char tecla_atual;
	
  	
  switch(botaoPressionado){ //verifica qual o botão foi pressionado
    case 3: //se for o D11/PB3
    while(count != BOUNCE){
      delay_1ms();
      tecla_atual = (PINB >> botaoPressionado & PB3 >> botaoPressionado);
      
      if (tecla_atual == tecla_antes)
        count++;
      else
        count = 0;
      
      if (count == BOUNCE)
        break;
      else
        tecla_antes = tecla_atual;
    }
    break;
  }
  	
	//inicia o algoritmo para verificação do debounce
	while (count != BOUNCE){
		delay_1ms(); 													//espera 1 ms para fazer a leitura
		tecla_atual = teclaPressionada;									//captura o estado atual da tecla
		
		if (tecla_atual == tecla_antes)				//verifica se é igual ao estado anterior
		count++;														//incrementa o contador se for
		else
		count = 0;													//zera o contador se não for
		
		if (count == BOUNCE)									//se chegar no valor de debounce
		break;															//sai do if para finalizar o while
		else
		tecla_antes = tecla_atual;					//se nao atingiu o BOUNCE, pega o estado atual da tecla
	}
	return tecla_atual;
}

*/

void setup(){
  	Serial.begin(9600);
 	Buttons_config();
  
}

void loop(){
 
 // Buttons_validation();
  //int val = ((PINB & (1 << BOTAO1)) >> 3); //D12
  //int val = ((PINB & (1 << BOTAO2)) >> 4); //D12
  //int val = (PINB >> PB4); //D12
  //int val = (PINB >> PB3); //D12
  unsigned char botaoAntes = 0, botaoAtual;
  unsigned char count = 0;
  
  while(count != BOUNCE){
    delay(20);
    if(PINB >> PB3 == 1){
      botaoAtual = (PINB >> PB3);
	  if (botaoAtual == botaoAntes)
        count++;
      else
        count =0;
      
      if (count == BOUNCE){
        Serial.println("D11 pressionado");
        break;
      }
      else
        botaoAntes = botaoAtual;
    }
  }
  
  /*if(PINB >> PB3 == 1){
    botaoPressionado = 3; //PB3
  }
    Serial.println("D11 pressed");
  /*
  else if (PINB >> PB4 == 1){
    botaoPressionado = 4; //PB4
    Serial.println("D12 pressed");
  }
  */
   


  
 

}