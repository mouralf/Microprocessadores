/*
 * printa hora.c
 *
 * Created: 22/10/2021 12:24:07
 * Author : Tio will
 */ 

#include <avr/io.h>


void printa_hora(int segundos){
	
	hora = segundos/900;
	min = segundos/15 - 60*hora;
	
	if(min_atual != min)
	LCD_control(LCD_CLR, CNFG);
	
	enviaString("      ");
	
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


int main(void)
{
	segundos_totais = 0;
    /* Replace with your application code */
    while (1) 
    {
		printa_hora(segundos_totais);
    }
}

