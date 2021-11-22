/*
- Tempo gasto (izi)
- Distância percorrida (izi)
- Velocidade media da viagem (izi) => V = (d/t)*3600 :)
 
 - Registro de ida e volta?
 - Na ida é possível adicionar marcadores utilizando o botão selec/config
 - Na volta sinalizar com LED amarelo qnd estiver 200 m próximo de um marcador
 - Indicar presença de marcador no LCD
 - Não permitir velocidade inst. maior que 80 km/h no trecho de 100 m antes e dps de um marcador (izi)
 
 */

#include "mbed.h"
#include "C12832.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);   //configuração do display

/*======================= Exibição no display LCD ==============*/
//botão para selecionar a função desejada 
InterruptIn btnFuncao(p10); //declarado como interrupção pra ser chamado sempre que pressionado
//botão para selecionar ou zerar valores
InterruptIn btnConfig(p11); //botao config


char exibicao = 0; //será incrementado para realizar o controle do que será exibido

/*====================================== Controle da opção B ====================================*/
Timer tempoIdaVolta;    //timer criado para marcar o tempo de viagem
Timer tempoVolta;
int marcadores[3] = {0, 0, 0};  //utilizado para marcar os pontos dos marcadores
char statusViagem = 0;  //utilizado para saber se está em ida ou volta e pra iniciar ou finalizar a ida ou volta
int posicaoFinal;


void BotaoFuncao(){ //função utilizada para indicar o que deve ser exibido no display (velocidade, quilometragem, umidade)
    exibicao++;

    /*
    exibicao = 0 -> quilometragem
    exibicao = 1 -> velocidade instantanea
    exibicao = 2 -> velocidade media
    exibicao = 3 -> temperatura 
    exibicao = 4 -> umidade
    exibicao = 5 -> inicia/finaliza ida/volta ao pressionar o botao config
    exibicao = 6 -> insere marcador com o botao config
    */


    if(exibicao == 5){  //inicia/finaliza ida/volta ao pressionar o botao config
        lcd.cls();
        lcd.printf("Alterar status viagem");
    }

    if(exibicao == 7){
        exibicao = 0;  //zera exibicao pra não passar das opções disponíveis
    }
   
}

int marcadores[3] = {0, 0, 0};

void BotaoSelec(){        //função utilizada para algumas configurações
    float velMedViagem;
    int distanciaIda;
    switch (exibicao){          //se tiver sendo exibindo a quilometragem
        case 0:
            quilometragem = 0;  //zera a quilometragem
            break;
        
        case 5:                 //se tiver sendo exibido a opçao de alterar status da viagem
            ControleViagem();
            break;
        
        case 6:                 //se tiver sendo exibida a opçao de marcador 
            SetaMarcadores();
            break;
    }
   
}

void ControleViagem(){
    static int distanciaIda = 0;
    int velMedViagem;

    statusViagem++;       //incrementa o status da viagem

    lcd.cls();
    lcd.locate(3,3);

    if(statusViagem == 0){
        lcd.printf("Iniciar ida");
    }

    if(statusViagem == 1){  //início da ida
        lcd.printf("Finalizar ida");
        posicaoInicial = quilometragem;    //pega a quilometragem atual, por ex. x = 1000
        tempoIdaVolta.start();              //inicia a contagem pra saber o tempo de percurso de ida e volta
    }

    if(statusViagem == 2){  //finaliza a ida e dá inicio à volta
        lcd.printf("Finalizar volta");
        tempoVolta.start(); //inicia o timer da viagem de volta
        posicaoFinal = quilometragem; //marca a posição final do trajeto, por ex. x = 2500
        distanciaIda = posicaoFinal - posicaoInicial;    //a distancia da ida vai ser a posiçao final - inicial, por ex. x = 1500
    }   

    if(statusViagem == 3){  //finaliza a volta
        lcd.printf("Volta finalizada!");
        tempoIdaVolta.stop();   //para o timer da viagem total
        tempoVolta.stop();      //para o timer da viagem de volta
        velMedViagem = ((distanciaIda*2)/tempoIdaVolta.read())*3600; //multiplica por 2 a distancia de ida para ter a distancia total e divide pelo tempo total do percurso
        lcd.cls();  //limpa o display
        lcd.locate(3,3);
        lcd.printf("Vmed: %f km/h", velMedViagem);  //exibe a velocidade media da viagem
        lcd.locate(3,13);   //coloca na segunda linha
        lcd.printf("D: %dm t: %dh",distanciaIda*2, tempoIdaVolta/3600); //exibe a distancia percorrida e o tempo gasto
    }

    if(statusViagem >=3 )
        statusViagem = 0;   //reseta o status para ida
}


void SetaMarcadores(){
    if(statusViagem == 1){  //se estiver na viagem de ida
         for (int i = 0; i<3; i++){
                    if(marcadores[i] == 0) //se o marcador atual estiver em 0
                        marcadores[i] = quilometragem - posicaoInicial;    //marca a distancia percorrida no momento, por ex. 2200
                        lcd.printf("Marcado em %d m", marcadores[i]);
                        break;  //sai do for pra não preencher todos os marcadores 
                    }
    }   
}

void ControleMarcadores(){  //função que realiza calculos referentes aos marcadores e exibiçoes pertinentes
    int posicaoVolta;
    static int proximoMarcador, marcadorAnterior;
    static int distMarcadorAnterior, distProxMarcador;

    if(statusViagem == 0){
        lcd.printf("Marcadores");

    }
    if(statusViagem == 1){  //se estiver na viagem de ida
        lcd.cls();
        lcd.locate(3,3);
        lcd.printf("Pressione para marcar");
    }

     if(statusViagem == 2){  //se estiver no trajeto de volta
        posicaoVolta = posicaoFinal - (quilometragem - posicaoFinal); //por exemplo, 2400 se quilometragem = 2600 e posicao final = 2500

        for(int i = 3; i = 0; i--){                 //itera sobre os últimos marcadores pra definir o mais próximo
            if(posicaoVolta > marcadores[i]){       //verifica se a posiçao atual do carro é maior do que a posiçao do marcador
                marcadorAnterior = proximoMarcador; //pega a posiçao do ultimo marcador, por ex. 2200
                proximoMarcador = marcadores[i];    //se for, o marcador mais próximo corresponde àquele índice, por ex. 1600.
                break;                              //sai do laço
            }
        }
        
        distProxMarcador = posicaoVolta - proximoMarcador;  //a distancia do marcador vai ser a posição atual - a posição do proximo marcador
        distMarcadorAnterior = posicaoVolta - marcadorAnterior;  //a distancia do marcador anterior pega a posiçao atual - a posiçao do marcador anterior

        if(distProxMarcador <= 200){ //se a distancia do marcador estiver a menos que 200 m
            ledAlerta = 1;  //acende o LED amarelo pra indicar que está próximo
            lcd.cls();  //limpa o LCD
            lcd.locate(3,3);
            lcd.printf("Marcador a %dm", distProxMarcador); //exibe no display a distancia do marcador
        }

        if(distMarcadorAnterior <= 200){
            ledAlerta = 1;
            lcd.cls();  //limpa o LCD
            lcd.locate(3,3);
            lcd.printf("Marcador a %dm", distMarcadorAnterior); //exibe no display a distancia do marcador
        }

        if(distProxMarcador <= 100 || distMarcadorAnterior <= 100){   //se a distancia do marcador for menor que 100 m
            if(ledInjecao >= 0.8)       //se a velocidade passar de 80 km/h
                ledInjecao = 0.8;   //limita a velocidade a ficar em 80 km/h
        }
    }
}



int main() {
    float velMedia = 0, quilometragem = 0;
    int crit = 0;
    int ledAlerta = 0;
    
    ledInjecao.period_ms(20);                   //define o período do PWM do led de injeção como 20 ms 
    timerPedais.attach(&LeituraPedais, 0.5f);   //chama a função que le o estado dos pedais a cada 0.5 s
    pedaisLivres.attach(&VerificaPedais, 1.0f); //chama a função que verifica se nenhum pedal foi pressionado a cada 1 s
    timer_500ms.attach(&timer500ms, 0.5f);      //chama a funcao para executar as funcoes a cada 500 ms
    timer_40ms.attach(&timer40ms, 0.04f);        //chama a funcao para executar as funcoes a cada 40 ms
    
    btnFuncao.fall(&BtnFncPressed);   //chama a função de mudar o que vai ser exibido no display ao pressionar o botão
    btnSelec.fall(&BtnSelecPressed);   //chama a função de selecionar alguma configuraçaõ ao pressionar o botão

    //chama a cada 500 ms a função de ControleMarcadores if(exibicao == 6)

    while(1){
      
        wait_ms(100);
    }
}