/*
 * LED.c
 *
 * Created: 15/11/2021 11:18:49
 * Author : luiz-
 */ 

#include <avr/io.h>


int main(void)
{
	SIM_SCGC5 |= 1 << 10; //Habilita a porta B
	PORTB_PCR19 = 0x1 <<8; //define o pino 19 como digital (coloca 001 a partir do bit 8);
	
	GPIOB_PDDR |= 1 << 19; //define o pino 19 como sa�da
    /* Replace with your application code */
    while (1) 
    {
		/*m�todo 1*/
		//registrador comum de escrita (PDOR)
		GPIOB_PDOR |= 1 << 19;		//coloca o pino 19 em HIGH
		//atraso();
		GPIOB_PDOR &= ~ (1<<19);	//coloca o pino 19 em LOW
		//atraso();
		
		/*m�todo 2*/
		//registrador PSOR - mant�m os pinos atuais e seta somente o desejado
		GPIOB_PSOR = 1 << 19;
		//atraso();
		//registrador PCOR - mant�m os pinos atuais e limpa apenas o desejado
		GPIOB_PCOR = 1 << 19;
		//atraso();
		
		
		/*m�todo 3*/
		//registrador PTOR - mant�m os pinos atuais e inverte apenas o desejado
		GPIOB_PTOR = 1 << 19;
		//atraso();
		
    }
}

