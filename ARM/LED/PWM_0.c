//programa pra gerar um PWM com duty cycle de 30% com frequencia de 400 Hz

/*
se pra freq de 4 Hz o CMOD era de 20833, então pra uma frequencia 100x maior o período vai ser 100x menor

Ou seja, CMOD = 208,33. Com isso, pode-se utilizar um prescaler menor, como o de 32.

TPM = (freq. CPU * 8) / 3, sendo fCPU = 8 MHz, então TPM = 8*8/3 = 21,33 MHz

O TPM é gerado pelo PWM, e não mais pela análise do flag de overflow pra inverter o estado do led. Então o período do TPM é
dado pelo MOD, assim, não precisa multiplicar por 125, mas sim por 0,25.

400 Hz = 2,5e-3 s = 2,5 ms

n_contagens = 21,33 MHz * 2,5 ms = 53325, assim, aplicando o prescaler de 32

n_contagens = (53325/32) = 1666 -> contagem pra gerar os 400 Hz (valor do MOD)

Para pegar os 30% disso,

n_contagens = 1666 * 0.3 = 500 -> valor do canal


Pelo manual da placa, o pino verde (19) pode ser saída TPM 2 no canal 1



*/
#include <MKL25Z4.h>



int main(void)
{
	SIM_SCGC5 |= 1 << 10; //Habilita PORTB
	PORTB_PCR19 |= 3 <<8; //modo TPM2 CH1 (alternativa 3), e não mais como saída GPIO digital
	
	GPIOB_PDDR |= 1 << 19; //define o pino 19 como saída
	
	
	SIM_SOPT2 = (1 << 24); //habilita o TPMSRC, responsável pelo controle geral dos TPMs
	
	SIM_SCGC6 = (1 << 26); //habilita o TPM2
	
	TPM0_SC |= (1 << 3) + 0x5; // (1 <<3) pra colocar 01 no CMOD (clk interno) e 0x5 pra habilitar o prescaler de 32 (PS = 101)
	
	TPM0_MOD = 1667;	//numero de contagens calculado inicialmente (para o valor de 400 Hz)
	
	// (2 << 4) + (2 << 2) configura os bits MSB e MSA com 11 + configura os bits ESLB e ELSA com 11 
	TPM2_C1SC = (2 << 4) + (2 << 2); //sobe o pino ao atingir C1V. Fica em 0 e quando atinge o C1V ele sobe. Coloca 1111
	
	TPM2_C1V = 1167;	//1667*0,7 (para o duty cycle de 30%). 70% em 0 e 30% em 1. Se fizesse o pino subir na linha acima, seria 1667 * 0.3
	
	while (1){
	};
}