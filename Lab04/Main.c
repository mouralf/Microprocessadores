#include <reg51.h>
#include "LCDeca.h"

void main(){
	
	LCD_init();
	enviaString("Luiz");
	LCD_control(LCD_SEG_LINHA, CNFG);
	enviaString("Pedro");
	while(1){
		
	}
}