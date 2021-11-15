//programa pra piscar o led verde com uma frequencia de 4 Hz

#include <MKL25Z4.h>

/*
4 Hz = 0,25 s = 250 ms

Como é pra ficar metade do tempo em alto e metade do tempo em baixo, a frequência vai ser 125 ms ou 0,125 s

TPM = (freq. CPU * 8) / 3, sendo fCPU = 8 MHz, então TPM = 8*8/3 = 21,33 MHz

Assim, temos que 

n_contagens = 0,125 * 21,33 = 2,67 M, portanto vai precisar de prescaler. Pegando o prescaler de 128, teremos

n_contagens = 2,67 M / 128 = 20833,333 = MOD que está dentro de um número de 16 bits, então esse prescaler é aceitável.

bit do TOF vai indicar o estouro do flag
*/



void atraso(){
	while ((TPM0_SC & (1 << 7)) == 0); //fica nesse loop enquanto o bit 7 (bit de overflow) for 0
	TPM0_SC |= 1 << 7; //quando o bit for pra 1, zera o flag de overflow 
}

int main(void)
{
	SIM_SCGC5 |= 1 << 10; //Habilita PORTB
	PORTB_PCR19 = 0x1 <<8; //define o pino 19 como digital (coloca 001 a partir do bit 8);
	
	GPIOB_PDDR |= 1 << 19; //define o pino 19 como saída
	
	
	SIM_SOPT2 = (1 << 24); //habilita o TPMSRC, responsável pelo controle geral dos TPMs
	SIM_SCGC6 = (1 << 24); //habilita o TPM0, ou seja, TPM específico
	
	TPM0_SC |= (1 << 3) + 0x7; // (1 <<3) pra colocar 01 no CMOD e 0x7 pra habilitar o prescaler de 128 (PS = 111)
	TPM0_MOD = 20833;	//numero de contagens calculado inicialmente
	
}