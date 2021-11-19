#include "mbed.h"
#include "C12832.h"
#include "Sht31.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);
Sht31 sht31(I2C_SDA, I2C_SCL);
DigitalOut ledAlerta(p19);

int temp_umid(int exibicao){                    //função para verificar e printar a temperatura
    float temp = sht31.readTemperature();       //lê a temperatura atual do motor
    float umidade = sht31.readHumidity();       //lê a umidade atual do motor 
    int crit = 0;                               //variavel para controlar se o carro deverá parar ou nao por causas criticas
    
    static int cont_temp = 0;                   //variaveis estaticas para manter a contagem 
    static int cont_umid = 0;                   //das variaveis na funcao
    static int cont_crit = 0;
    
    if(temp >= 36)                              //caso a temperatura esteja acima de 36 graus aumenta o contador
        cont_temp++;
    else                                        //zera o contador caso a temperatura abaixe de 36 graus
        cont_temp = 0;

    if(umidade < 30)                            //caso a umidade esteja abaixo de 30%, aumenta o contador
        cont_umid++;
    else                                        //zera o contador caso a umidade supere 30%
        cont_umid = 0;    
    
    if(cont_temp >= 5){                         //se o contador chegar a 5, ou seja, o motor está a 0,2 seg quente
        ledAlerta = 1;
        if(temp >= 45){                         //se a temperatura for maior que 45 graus, mostra temperatura critica
            if(exibicao == 3) {  
                lcd.cls();
                lcd.locate(3, 3);
                lcd.printf("Temperatura: %.2f C", temp);
            }
            lcd.locate(3, 13);
            lcd.printf("Motor Critico");
            cont_crit++;                        //aumenta o contador de tempo em que o motor esta critico
        }
        else{                                   //mostra que o motor está quente
            if(exibicao == 3) {  
                lcd.cls();
                lcd.locate(3, 3);
                lcd.printf("Temperatura: %.2f C", temp);
            }
            lcd.locate(3, 13);
            lcd.printf("Motor Aquecido");
        }
    }
    else{                                       //caso não esteja muito quente, apenas mostra a temperatura no LCD
        if(exibicao == 3) {  
            lcd.cls();
            lcd.locate(3, 3);
            lcd.printf("Temperatura: %.2f C", temp);
        }
    }

    if(cont_umid >= 5){                         //se o contador chegar a 5, ou seja, a agua está a 0,2 seg baixa
        ledAlerta = 1;
        if(umidade <= 17){                      //se a umidade estiver abaixo de 17%, mostra umidade critica
            if(exibicao == 4){
                lcd.cls();
                lcd.locate(3, 3);
                lcd.printf("Umidade: %.2f %%", umidade);
            }
            lcd.locate(3, 13);                  
            lcd.printf("Umidade critica"); 
            cont_crit++;                        //aumenta o contador de tempo em que a agua esta critico
        }
        else{                                   //mostra que a umidade esta baixa
            if(exibicao == 4){
                lcd.cls();
                lcd.locate(3, 3);
                lcd.printf("Umidade: %.2f %%", umidade);
            }
            lcd.locate(3, 13);
            lcd.printf("Umidade baixa");
        }
    }
    else{                                       //caso a umidade esteja normal, apenas mostra a umidade no LCD
        if(exibicao == 4){
            lcd.locate(3, 3);
            lcd.printf("Umidade: %.2f %%", umidade);
        }
        
    }

    if(cont_temp < 5 && cont_umid < 5)          //caso esteja tudo normal, o led é desligado
        ledAlerta = 0;

    if(temp < 45 && umidade > 17)                  //zera o contador critico caso nenhum parametro esteja critico
        cont_crit = 0;

    if(cont_crit >= 3000)                       //caso a temperatura ou umidade estejam criticas por mais de 2 min
        crit = 1;                               //atualiza a variavel q sera retornada para fazer o carro parar

    return crit;                                //retorna se a situação está critica e o carro ira parar
}

void ExibeVelocInst(int velInsta){      //função para printar a velocidade instantanea 
    if(exibicao == 1){                                //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.printf("Velocidade:");
        lcd.locate(3, 13);              //coloca o cursor na segunda linha
        lcd.printf("%d km/h ", velInsta);
        printf("Velocidade: %d km/h ", velInsta);
    }
}

int velocidadeMedia(int vel1, int vel2, int exibicao){ //função para calcular e printar a velocidade média
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

int velMediaViagem(int velAtual, int velTotal, int exibicao){  //função para calcular a velocidade media total 
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

float hodometro(int velocidade_media, float quilometragem, int exibicao){ 			// acumula distancia total e calcula distancia da velocidade media 
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

float distPercorridaViagem(int velMediaViagem, float tempo){  //funcao para calcular a distancia da viagem
    float distancia_percurso;

    tempo = tempo/3600;                                 //transforma o tempo de segundos para horas

    distancia_percurso = velMediaViagem*tempo;          //calcula a distancia ate o momento

    return distancia_percurso;                          //retorna a distancia
}

float tempoViagem(float tempoTotal){    
    tempoTotal++;                       //funcao incrementara 1 seg no contador de tempo total

    return tempoTotal;                  //retorna o tempo total 
}

void ida_volta(){
    idaVolta++;         //incrementa o valor da variavel que controla se o carro esta indo ou voltando do trajeto toda vez q o botao funcao for acionado
                        //idaVolta = 0: nao esta marcando um percurso
                        //idaVolta = 1: ida ; idaVolta = 2: volta
}

void marcador(){
    marcador = 1;
}

int main() {