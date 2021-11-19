#include "mbed.h"

Ticker nomeTimer;    //interrupção que será chamada repetidamente pra ler os pedais

int main(){
    timerPedais.attach(&nomeFuncao, 0.5f);   //chama a função nomeFuncao que le o estado dos pedais a cada 0.5s
}
    