#include "mbed.h"
#include "C12832.h"
#include "Sht31.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);
Sht31 sht31(I2C_SDA, I2C_SCL);
DigitalOut ledAlerta(p19);

int cont_temp = 0;          //variavel global para controlar se a temperatura esta a 0,2 seg > 36 graus
int cont_umid = 0;          //variavel global para controlar se a umidade esta a 0,2 seg < 30% 
int cont_crit = 0;          //variavel global para controlar se está em condição critica a 2 min 

int temp_umid(int exibicao){                  //função para verificar e printar a temperatura
    float temp = sht31.readTemperature();       //lê a temperatura atual do motor
    float umidade = sht31.readHumidity();       //lê a umidade atual do motor 
    int crit = 0;                               //variavel para controlar se o carro deverá parar ou nao por causas criticas

    if(temp >= 36)                              //caso a temperatura esteja acima de 36 graus aumenta o contador
        cont_temp++;
    else                                        //zera o contador caso a temperatura abaixe de 36 graus
        cont_temp = 0;

    if(umidade < 30)                              //caso a umidade esteja abaixo de 30%, aumenta o contador
        cont_umid++;
    else                                        //zera o contador caso a umidade supere 30%
        cont_umid = 0;    
    
    if(cont_temp >= 5){                         //se o contador chegar a 5, ou seja, o motor está a 0,2 seg quente
        ledAlerta = 1;
        if(temp >= 45){                         //se a temperatura for maior que 45 graus, mostra temperatura critica
            lcd.locate(3, 13);
            lcd.printf("Motor Critico");
            cont_crit++;                        //aumenta o contador de tempo em que o motor esta critico
            printf("\nMotor critico");
        }
        else{                                   //mostra que o motor está quente
            lcd.locate(3, 13);
            lcd.printf("Motor Aquecido");
            printf("\nMotor aquecido");
        }
    }
    else{                                       //caso não esteja muito quente, apenas mostra a temperatura no LCD
        if(exibicao == 3) {  
            lcd.cls();
            lcd.locate(3, 3);
            lcd.printf("Temperatura: %.2f C", temp);
            printf("\nTemperatura: %.2f", temp);
        }
    }

    if(cont_umid >= 5){                         //se o contador chegar a 5, ou seja, a agua está a 0,2 seg baixa
        ledAlerta = 1;
        if(umidade <= 17){                      //se a umidade estiver abaixo de 17%, mostra umidade critica
            lcd.locate(3, 13);
            lcd.printf("Umidade critica");
            printf("\nUmidade critica");
            cont_crit++;                        //aumenta o contador de tempo em que a agua esta critico
        }
        else{                                   //mostra que a umidade esta baixa
            lcd.locate(3, 13);
            lcd.printf("Umidade baixa");
            printf("\nUmidade baixa");
        }
    }
    else{                                       //caso a umidade esteja normal, apenas mostra a umidade no LCD
        if(exibicao == 3 && temp < 36){
            lcd.locate(3, 13);
            lcd.printf("Umidade: %.2f %", umidade);
            printf("\nUmidade: %.2f", umidade);
        }
    }

    if(cont_temp < 5 && cont_umid < 5)          //caso esteja tudo normal, o led é desligado
        ledAlerta = 0;

    if(temp < 45 && umid > 17)                  //zera o contador critico caso nenhum parametro esteja critico
        cont_crit = 0;

    if(cont_crit >= 3000)                       //caso a temperatura ou umidade estejam criticas por mais de 2 min
        crit = 1;                               //atualiza a variavel q sera retornada para fazer o carro parar

    return crit;                                //retorna se a situação está critica e o carro ira parar
}

void ExibeVelocInst(int velInsta){      //função para printar a velocidade instantanea 
    if(exibicao == 1){                                //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.printf("Velocidade:");
        lcd.locate(3, 12);              //coloca o cursor na segunda linha
        lcd.printf("%d km/h ", velInsta);
        printf("Velocidade: %d km/h ", velInsta);
    }
}

int velocidadeMedia(int vel1, int vel2){ //função para calcular e printar a velocidade média
    int velMedia;

    velMedia = (vel1 + vel2)/2;                   //calcula a velocidade média das ultimas 2 velocidades instantaneas calculadas

    if(exibicao == 2){                                  //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.locate(3, 3);
        lcd.printf("Velocidade Media:");
        lcd.locate(3, 12);              //coloca o cursor na segunda linha
        lcd.printf("%d km/h ", velMedia);
        printf("Velocidade: %d km/h ", velMedia);
    }
    return velMedia;
}

int velMediaViagem(int velAtual, int velTotal){  //função para calcular a velocidade media total 
    int velMediaAtual;

    velTotal = velTotal + velAtual;             //incrementa a velocidade total do percurso
    cont_velMedia++;                            //incrementa o contador de quantas velocidades foram contadas.
    velMediaAtual = velTotal/cont_velMedia;     //calcula a media da velocidade total

    if(exibicao == 5){                          //printa a velocidade media total da viagem até o momento 
        lcd.cls();                                 //TALVEZ TIRAR ISSO DEPOIS
        lcd.printf("Vel. Med. Tot. :");
        lcd.locate(3, 12);              //coloca o cursor na segunda linha
        lcd.printf("%d km/h ", velMediaAtual);
        printf("Velocidade Media Total: %d km/h ", velMediaAtual);
    }
        
    return velTotal;                               //retorna a velocidade total para manter o controle da variavel na main
}

float hodometro(int velocidade_media, float quilometragem){ 			// acumula distancia total e calcula distancia da velocidade media 
                                    // em cada período para adicionar na quilometragem
    int distancia;			// calcula distância percorrida através da velocidade média
    
    distancia = (velocidade_media*1000)/3600; 		//converte velocidade media de Km/h para metros/segundo
    quilometragem = quilometragem + distancia;			// adiciona quilometragem total 

    if(exibicao == 0){                              //exibe a quilometragem no LCD se estiver no exibicaoice especifico
        lcd.cls();
        lcd.printf("Quilometragem:");
        lcd.locate(3, 12);              //coloca o cursor na segunda linha
        lcd.printf("%d km/h ", quilometragem);   
		printf("Quilometragem: %d km", quilometragem);
    }

    return quilometragem;
} 

float distPercorridaViagem(int velMediaViagem, float tempo, int exibicao){  //funcao para calcular a distancia da viagem
    float distancia_percurso;

    tempo = tempo/3600;                                 //transforma o tempo de segundos para horas

    distancia_percurso = velMediaViagem*tempo;          //calcula a distancia ate o momento

    return distancia_percurso;                          //retorna a distancia
}

float tempoViagem(float tempoTotal){    
    tempoTotal++;                       //funcao incrementara 1 seg no contador de tempo total

    return tempoTotal;                  //retorna o tempo total 
}

int main() {
    float temp = 0;
    int velInsta = 0, velAntiga = 0, velMedia = 0;
    int exibicao = 1;

    lcd.cls();

    while (1) {
        
        velAntiga = velInsta;                       //atualiza a velocidade antiga


        velInsta = ledInjecao*100;
        velMedia = velocidadeMedia(velAntiga, velInsta);

        wait_ms(1500);
    }
}