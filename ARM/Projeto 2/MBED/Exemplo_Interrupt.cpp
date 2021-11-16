#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

Ticker t1; //timer com interrupção chamada repetidamente
Timeout t2; //interrupção chamada apenas 1x quando atingir determinado tempo
InterruptIn btn(BUTTON1); //associa o botão BUTTON1 com uma interrupção

void blink_led1() {
    printf("Ticker fired\n");
    led1 = !led1;
}

void toggle_led2() {
    printf("BUTTON1 fall invoked\n");
    led2 = !led2;   //inverte o estado do led
}

void turn_led3_on() {
    printf("Timeout fired\n");
    led3 = 1;
}

int main() {
    printf("Hello world!\n");
    printf("LED1 will blink every second, LED3 will toggle after 2.5 seconds, LED2 can be toggled through BUTTON1.\n");
    printf("-----------------------------------\n\n");

    t1.attach(callback(&blink_led1), 1.0f); //associa uma função a ser chamada pela interrupção t1, com o intervalo definido em segundos
    t2.attach(callback(&turn_led3_on), 2.5f); //associa uma função a ser chamada pela interrupção t1, com o intervalo definido em segundos
    btn.fall(callback(&toggle_led2));//associa uma função a ser chamada pela interrupção associada ao botão btn

    wait_ms(osWaitForever);
}
