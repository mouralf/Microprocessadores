#include "mbed.h"

PwmOut ledInjecao(p23); //define o pino 23 como saída PWM


//função a ser chamada pra verificar se é necessário limitar a velocidade instantânea
void LimiteVelocidade(int distTrecho){  //recebe como parâmetro a distância do trecho
    if(distTrecho <= 100)               //se a distância for menor ou igual do que 100
        if(ledInjecao >= 0.8)           //se o dc estiver maior ou igual do que 0.8 (80 km/h)
            ledInjecao = 0.8;           //limita a ficar em 0.8 (80 km/h)
}

/*============================== Função principal ==============*/


int main() {
    
    wait_ms(osWaitForever);
}