#include "mbed.h"
#include "C12832.h"
#include "Sht31.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);
Sht31 sht31(I2C_SDA, I2C_SCL);
DigitalOut led(LED1);

int cont_temp = 0;          //variavel global para controlar se a temperatura esta a 0,2 seg > 36 graus

float temperatura(int ind){                     //função para verificar e printar a temperatura
    float temp = sht31.readTemperature();       //lê a temperatura atual do motor
    
    if(temp >= 36)                              //caso a temperatura esteja acima de 36 graus aumenta o contador
        cont_temp++;
    else                                        //zera o contador caso a temperatura abaixe de 36 graus
        cont_temp = 0;
    
    if(cont_temp >= 5){                         //se o contador chegar a 5, ou seja, o motor está a 0,2 seg quente
        if(temp >= 45){                         //se a temperatura for maior que 45 graus, mostra temperatura critica
            lcd.cls();
            lcd.locate(3, 3);
            lcd.printf("Motor Critico");
            cont_crit++;                        //aumenta o contador de tempo em que o motor esta critico
        }
        else{                                   //mostra que o motor está quente
            lcd.cls();
            lcd.locate(3, 3);
            lcd.printf("Motor Aquecido");
            cont_crit = 0;                      //zera a contagem critica
        }
    }
    else{                                       //caso não esteja muito quente, apenas mostra a temperatura no LCD

        if(exibicao == 3){  
            lcd.cls();
            lcd.locate(3, 3);
            lcd.printf("Temperature: %.2f C", temp);
        }
    }

    if(cont_crit >= 3000)                        //caso o motor esteja em temperatura critica por mais de 2 min
        temp = 100;                              //assume um valor absurdo para temp para ser identificada na main depois

    return temp;                                 //retorna a temperatura atual
}

int velocidadeInsta(int velInsta){      //função para printar a velocidade instantanea 
    if(exibicao == 1){                                //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.locate(3, 3);
        lcd.printf("Velocidade: %dkm/h ", velInsta);
    }
    return velInsta;
}

int velocidadeMedia(int vel1, int vel2){ //função para calcular e printar a velocidade média
    int velMedia;

    velMedia = (vel1 + vel2)/2;                   //calcula a velocidade média das ultimas 2 velocidades instantaneas calculadas

    if(exibicao == 2){                                  //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.locate(3, 3);
        lcd.printf("Velocidade Media: %dkm/h ", velMedia);
    }
    return velMedia;
}

int main() {
    float temp = 0;
    int velInsta = 0, velAntiga = 0, velMedia = 0;

    lcd.cls();

    while (1) {
        velAntiga = velInsta;                       //atualiza a velocidade antiga



        
        velInsta = velocidadeInsta(velInsta);          
        velMedia = velocidadeMedia(velAntiga, velInsta);

        wait_ms(1500);
    }
}