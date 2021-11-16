#include "mbed.h"

/*======================= Pedais de freio e acelerador ==============*/
DigitalIn btnAcelerador(p13);   //define o botão conectado ao pino 13 (Acelerador) como entrada
DigitalIn btnFreio(p12);    //define o botão conectado ao pino 13 (Freio) como entrada

PwmOut ledInjecao(p23); //define o pino 23 como saída PWM

float dcInjecao = 0;    //variável pra fazer o controle do duty cycle da injeção

Ticker timerPedais; //interrupção que sreá chamada repetidamente pra ler os pedais

//função que será chamada a cada 0.5 s pra verificar o estado dos pedais
void LeituraPedais(){
    //verifica se o freio foi pressionado
    if (btnFreio.read()){
        dcInjecao = dcInjecao - 0.5; //decrementa o duty cycle em 50%
    }
    //se não estiver pressionado, verifica se o acelerador foi pressionado
    else if (btnAcelerador.read()){
        dcInjecao = dcInjecao + 0.1; //incrementa o duty cycle
    }
    //se nenhum tiver sido pressionado
    else{
        dcInjecao = dcInjecao - 0.1;
        wait(1); //espera 1s
    }
    printf("\nDuty cycle injecao: %f \n", dcInjecao);

    //impede que o duty cycle fique negativo
    if(dcInjecao <= 0){
        dcInjecao = 0;
    }

    //impede que o duty cycle fique acima de 100%
    if(dcInjecao >= 1){
        dcInjecao = 1;
    }
    
}

/*============================== Função principal ==============*/


int main() {
    ledInjecao.period_ms(20);    //define o período do PWM do led de injeção como 20 ms 
    
    timerPedais.attach(&LeituraPedais, 0.5f); //chama a função que le o estado dos pedais a cada 0.5s
    
    wait_ms(osWaitForever);
}