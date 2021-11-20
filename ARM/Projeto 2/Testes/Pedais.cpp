#include "mbed.h"

/*==================================== Definições ===============================*/
DigitalIn btnAcelerador(p13);   //define o botão conectado ao pino 13 (Acelerador) como entrada
DigitalIn btnFreio(p12);        //define o botão conectado ao pino 13 (Freio) como entrada

PwmOut ledInjecao(p23);         //define o pino 23 como saída PWM

Ticker timerPedais;             //interrupção que será chamada repetidamente pra ler os pedais
Ticker pedaisLivres;           //interrupção que será chamada quando não tiver nenhum pedal sido pressionado
/*================================== Leitura dos pedais ===================================================================*/

void LeituraPedais(){                   //função que será chamada a cada 0.5 s pra verificar o estado dos pedais
    if (btnFreio.read()){               //verifica se o freio foi pressionado
        ledInjecao = ledInjecao - 0.5;  //decrementa o duty cycle em 50%
    }

    else if (btnAcelerador.read()){     //se não estiver pressionado, verifica se o acelerador foi pressionado
        ledInjecao = ledInjecao + 0.1;  //incrementa o duty cycle
    }
    
    else{                               //se nenhum tiver sido pressionado
        ledInjecao = ledInjecao - 0.1;
        //espera 1s
        pedaisLivres.attach(callback(&LeituraPedais), 0.5f);                     //como a funçao LeituraPedais é chamada a cada 0.5 s, então se não tiver nada pressionado chama novamente a função depois de 0.5s
    }
    
    
    printf("\nDuty cycle injecao pressionado: %.2f \n", ledInjecao.read());
    
}

void LimiteDC(){ //função que limita o duty cycle a ficar entre 0 e 1
    //impede que o duty cycle fique negativo
    if(ledInjecao <= 0){
        ledInjecao = 0;
    }

    //impede que o duty cycle fique acima de 100%
    if(ledInjecao >= 1){
        ledInjecao = 1;
    }

}

void VerificaPedais(){
    if((btnFreio.read() == 0) && (btnAcelerador.read() == 0))   //se nenhum pedal estiver pressionado
        ledInjecao = ledInjecao - 0.1;  //decrementa o pwm
    
    printf("\nDuty cycle injecao livre: %.2f \n", ledInjecao.read());

}

/*============================== Função principal ==============*/


int main() {
    ledInjecao.period_ms(20);                   //define o período do PWM do led de injeção como 20 ms 
    
    timerPedais.attach(&LeituraPedais, 0.5f);   //chama a função que le o estado dos pedais a cada 0.5s
    pedaisLivres.attach(&VerificaPedais, 1.0f);
    wait_ms(osWaitForever);
}