/*
Problemas atuais:
- Pressionar o btn 11 quando tá no status de finalizar a ida ele já finaliza a volta junto
- Exibição mssg de marcador quando exibicao == 6 e os marcadores não estao setados
- Primeiro marcador ta pegando em quilometragem = 0
*/

#include "mbed.h"
#include "C12832.h"
#include "Sht31.h"
/*================================================================== Definições ============================================================*/
/********* VELOCIDADE DO CARRO *************/
DigitalIn btnAcelerador(p13);   //define o botão conectado ao pino 13 (Acelerador) como entrada
DigitalIn btnFreio(p12);        //define o botão conectado ao pino 13 (Freio) como entrada
PwmOut ledInjecao(p23);         //define o pino 23 como saída PWM

Ticker timerPedais;             //interrupção que será chamada repetidamente pra ler os pedais
Ticker timer1s;           //interrupção que será chamada quando não tiver nenhum pedal sido pressionado

float velMedia = 0;

/********* PAINEL DO CARRO (BOTOES E DISPLAY LCD) *************/
C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);
int exibicao = 0;       //variavel para controlar a exibicao no LCD

InterruptIn btnFuncao(p10);
InterruptIn btnSelec(p11);


/********* TEMPERATURA E UMIDADE *************/
Sht31 sht31(I2C_SDA, I2C_SCL);
DigitalOut ledAlerta(p19);
PwmOut speaker(p21);

/********* OPCAO B *************/
Timer tempoIdaVolta;    //timer criado para marcar o tempo de viagem
Timer tempoVolta;
int marcadores[3] = {0, 0, 0};  //utilizado para marcar os pontos dos marcadores
char statusViagem = 0;  //utilizado para saber se está em ida ou volta e pra iniciar ou finalizar a ida ou volta
int posicaoFinal = 0, posicaoInicial = 0;
int quilometragem = 0;

/********* FUNCIONAMENTO GERAL DO SISTEMA *************/
Ticker timer_40ms;              //interrupt com timer de 40 ms
Ticker timer_10ms;


int timer40 = 0;        //variavel para controlar as funcoes q sao ativadas a cada 40 ms
int timer500 = 0;       //variavel para controlar funcoes q sao ativadas a cada 0,5s





/*================================================================== Funções ============================================================*/


/**************************************************** CONTROLE DO DISPLAY *********************************************************/
void ResetaCursor(){
    lcd.cls();
    lcd.locate(3,3);
}


/**************************************************** CONTROLE DE VELOCIDADE *********************************************************/

void hodometro(){ 			// acumula distancia total e calcula distancia da velocidade media 
                                    // em cada período para adicionar na quilometragem
    float distancia;			// calcula distância percorrida através da velocidade média
    
    distancia = (velMedia*1000)/3600; 		//converte velocidade media de Km/h para metros/segundo
    quilometragem = quilometragem + distancia;			// adiciona quilometragem total 
    
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

void LeituraPedais(){                   //função que será chamada a cada 0.5 s pra verificar o estado dos pedais
    if (btnFreio.read()){               //verifica se o freio foi pressionado
        ledInjecao = ledInjecao - 0.5;  //decrementa o duty cycle em 50%
    }

    else if (btnAcelerador.read()){     //se não estiver pressionado, verifica se o acelerador foi pressionado
        ledInjecao = ledInjecao + 0.1;  //incrementa o duty cycle
        hodometro();    //atualiza a quilometragem
    }
 
    LimiteDC(); //funcao para limitar o duty cycle

    //printf("\nDuty cycle injecao pressionado: %.2f \n", ledInjecao.read());
}


/**************************************************** EXIBICAO DE VELOCIDADE *********************************************************/


void CalcVelMedia(){ //função para calcular e printar a velocidade média
    static float velAtual = 0, velAnterior;

    velAnterior = velAtual;                //armazena a ultima velocidade instantanea
    velAtual = ledInjecao.read();   //armazena a velocidade instantanea atual
   
   velMedia = ((velAnterior+velAtual)/2)*100;
}

/**************************************************** TEMPERATURA E UMIDADE *********************************************************/
void buzzer(){                  //funcao do buzzer
    speaker.period(1.0/480);    //frequencia de 480Hz
    speaker = 0.5;              //volume em 0.5
}


int temp_umid(){                    //função para verificar e printar a temperatura
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
                ResetaCursor();
                lcd.printf("Temperatura: %.2f C", temp);
            }
            lcd.locate(3, 13);
            lcd.printf("Motor Critico");
            buzzer();                           //funcao para sinal sonoro quando em estado critico
            cont_crit++;                        //aumenta o contador de tempo em que o motor esta critico
        }
        else{                                   //mostra que o motor está quente
            if(exibicao == 3) {  
                ResetaCursor();
                lcd.printf("Temperatura: %.2f C", temp);
            }
            lcd.locate(3, 13);
            lcd.printf("Motor Aquecido");
        }
    }
    else{                                       //caso não esteja muito quente, apenas mostra a temperatura no LCD
        if(exibicao == 3) {  
            ResetaCursor();
            lcd.printf("Temperatura: %.2f C", temp);
        }
    }

    if(cont_umid >= 5){                         //se o contador chegar a 5, ou seja, a agua está a 0,2 seg baixa
        ledAlerta = 1;
        if(umidade <= 17){                      //se a umidade estiver abaixo de 17%, mostra umidade critica
            if(exibicao == 4){
                ResetaCursor();
                lcd.printf("Umidade: %.2f %%", umidade);
            }
            lcd.locate(3, 13);                  
            lcd.printf("Umidade critica"); 
            buzzer();                           //funcao para sinal sonoro quando em estado critico
            cont_crit++;                        //aumenta o contador de tempo em que a agua esta critico
        }
        else{                                   //mostra que a umidade esta baixa
            if(exibicao == 4){
                ResetaCursor();
                lcd.printf("Umidade: %.2f %%", umidade);
            }
            lcd.locate(3, 13);
            lcd.printf("Umidade  baixa");
        }
    }
    else{                                       //caso a umidade esteja normal, apenas mostra a umidade no LCD
        if(exibicao == 4){
            ResetaCursor();
            lcd.printf("Umidade: %.2f %%", umidade);
        }
        
    }

    if(cont_temp < 5 && cont_umid < 5)          //caso esteja tudo normal, o led é desligado
        ledAlerta = 0;

    if(temp < 45 && umidade > 17)                  //zera o contador critico caso nenhum parametro esteja critico
        cont_crit = 0;

    if(cont_crit >= 300)                       //caso a temperatura ou umidade estejam criticas por mais de 2 min
        crit = 1;                               //atualiza a variavel q sera retornada para fazer o carro parar

    return crit;                                //retorna se a situação está critica e o carro ira parar
}



/**************************************************** OPÇÃO B *********************************************************/


void ControleViagem(){      //função chamada quando se pressiona o botão de selec na exibicao == 5 ou pra atualizar o display a cada 500 ms
    static int distanciaIda = 0;
    int velMedViagem;
    static int i = 0;
    
    

    if(statusViagem == 0){
        ResetaCursor();
        lcd.printf("Iniciar ida");
        posicaoInicial = quilometragem;    //pega a quilometragem atual, por ex. x = 1000
    }
    
    if(statusViagem == 1){  //início da ida
        ResetaCursor();
        lcd.printf("Finalizar ida");  
                
    }

    if(statusViagem == 2){  //finaliza a ida e dá inicio à volta
        ResetaCursor();
        lcd.printf("Finalizar volta");

        posicaoFinal = quilometragem; //marca a posição final do trajeto, por ex. x = 2500
        distanciaIda = posicaoFinal - posicaoInicial;    //a distancia da ida vai ser a posiçao final - inicial, por ex. x = 1500

        
        if(quilometragem == (posicaoInicial+ distanciaIda*2)){  //se tiver dado o percurso de volta completo
            statusViagem = 3;
        }
    }   

    //tem que colocar isso aqui no display certinho <<<<
    if(statusViagem == 3){  //finaliza a volta
        ResetaCursor();
        lcd.printf("Volta finalizada!");
        tempoIdaVolta.stop();      //para o timer da viagem de ida e volta
        velMedViagem = ((distanciaIda*2)/tempoIdaVolta.read())*3600; //multiplica por 2 a distancia de ida para ter a distancia total e divide pelo tempo total do percurso
        ResetaCursor();
        lcd.printf("Vmed: %f km/h", velMedViagem);  //exibe a velocidade media da viagem
        lcd.locate(3,13);   //coloca na segunda linha
        lcd.printf("D: %dm t: %dh",distanciaIda*2, tempoIdaVolta/3600); //exibe a distancia percorrida e o tempo gasto
    }

    if(statusViagem >=3 )
        statusViagem = 0;   //reseta o status para ida
}


void SetaMarcadores(){
    int i;
    if(statusViagem == 3){
        for(i = 0; i < 3; i++){
            marcadores[i] = 0;  //zera os marcadores
        }
    }

    if(statusViagem == 1){  //se estiver na viagem de ida
         for (i = 0; i<3; i++){
                    if(marcadores[i] == 0) {//se o marcador atual estiver em 0
                        marcadores[i] = quilometragem - posicaoInicial;    //marca a distancia percorrida no momento, por ex. 2200
                        lcd.locate(3,13);//bota na segunda linha
                        lcd.printf("Marcado em %d m", marcadores[i]);
                        break;  //sai do for pra não preencher todos os marcadores 
                    }
         }
    }   
}


void ControleMarcadores(){  //função que realiza calculos referentes aos marcadores e exibiçoes pertinentes. Chamada a cada 500 ms
    int posicaoVolta;
    static int proximoMarcador, marcadorAnterior;
    static int distMarcadorAnterior, distProxMarcador;
    int i;
    
     ResetaCursor();

    if(statusViagem == 0){
        lcd.printf("Marcadores");

    }

    if(statusViagem == 1){  //se estiver na ida
        lcd.printf("Pressione para marcar");
    }

     if(statusViagem == 2){  //se estiver no trajeto de volta
        posicaoVolta = posicaoFinal - (quilometragem - posicaoFinal); //por exemplo, 2400 se quilometragem = 2600 e posicao final = 2500

        for(i = 3; i > 0; i--){                 //itera sobre os últimos marcadores pra definir o mais próximo
            if(posicaoVolta > marcadores[i]){       //verifica se a posiçao atual do carro é maior do que a posiçao do marcador
                marcadorAnterior = proximoMarcador; //pega a posiçao do ultimo marcador, por ex. 2200
                proximoMarcador = marcadores[i];    //se for, o marcador mais próximo corresponde àquele índice, por ex. 1600.
                ResetaCursor();
                lcd.printf("Proximo marcador: %d", proximoMarcador);
                break;                              //sai do laço
            }
        }
        
        distProxMarcador = posicaoVolta - proximoMarcador;  //a distancia do marcador vai ser a posição atual - a posição do proximo marcador
        distMarcadorAnterior = posicaoVolta - marcadorAnterior;  //a distancia do marcador anterior pega a posiçao atual - a posiçao do marcador anterior

        if(distProxMarcador <= 200){ //se a distancia do marcador estiver a menos que 200 m
            ledAlerta = 1;  //acende o LED amarelo pra indicar que está próximo
            ResetaCursor();
            lcd.printf("Marcador a %dm", distProxMarcador); //exibe no display a distancia do marcador
        }

        if(distMarcadorAnterior <= 200){
            ledAlerta = 1;
            ResetaCursor();
            lcd.printf("Marcador a %dm", distMarcadorAnterior); //exibe no display a distancia do marcador
        }

        if(distProxMarcador <= 100 || distMarcadorAnterior <= 100){   //se a distancia do marcador for menor que 100 m
            if(ledInjecao >= 0.8)       //se a velocidade passar de 80 km/h
                ledInjecao = 0.8;   //limita a velocidade a ficar em 80 km/h
        }


    }
}





/**************************************************** FUNCIONALIDADES GERAIS *********************************************************/
void AtualizaDisplay(){
    
     switch(exibicao){
        case 0: //quilometragem
            ResetaCursor();
            lcd.printf("Quilometragem: %d", quilometragem);
            break;

        case 1: //velocidade inst.
            ResetaCursor();
            lcd.printf("Vinst: %.2f km/h", (ledInjecao.read())*100);
            break;

        case 2: //vel. media
            ResetaCursor();
            lcd.printf("Vmed: %.2f", velMedia);
            break;

        case 3: //temperatura
            temp_umid();
            break;

        case 4: //umidade
            temp_umid();
            break;

        case 5: //status da viagem
            ResetaCursor();
            //lcd.printf("Status viagem: %d", statusViagem);
            ControleViagem();
            break;

        case 6: //marcadores
            //lcd.locate(3,3);
            //lcd.printf("Marcadores");
            //ControleMarcadores();
            break;
        
        default:
            exibicao = 0;
    }
}


void BtnConfig(){        //função utilizada para algumas configurações
    ResetaCursor();

    switch (exibicao){          //se tiver sendo exibindo a quilometragem
        case 0: //quilometragem
            quilometragem = 0;  //zera a quilometragem
            lcd.printf("Quilometragem: %d", quilometragem);
            break;
        
        case 1: //vel. instantanea
            lcd.printf("Vinst: %.2f km/h", (ledInjecao.read())*100);
            break;
        
        case 2: //vel. media
            lcd.printf("Vmed: %.2f", velMedia);
            break;

        case 3: //temperatura
            temp_umid();
            break;
        
        case 4: //umidade
            temp_umid();
            break;
        
        case 5: //status da viagem
            statusViagem++;

            if(statusViagem == 1){                  //se tiver inicio a ida
                tempoIdaVolta.start();              //inicia a contagem pra saber o tempo de percurso de ida e volta
            }

            if(statusViagem == 3){  //volta foi finalizada
                tempoIdaVolta.stop();   //para o timer da viagem total1
            }

            //ControleViagem();
        
        
            if(statusViagem > 3){
                statusViagem = 0;
            }
            break;
        
        case 6: //marcadores
            //SetaMarcadores();
            lcd.printf("Marcador em: %d", quilometragem);
            break;
            
        default:
            lcd.printf("Exibicao: %d", exibicao);
    }
   
}


void SelecionaExibicao(){ //função utilizada para indicar o que deve ser exibido no display 
    ResetaCursor();
    
    exibicao++;         //incrementa a variavel de controle
    
    if(exibicao >= 7){
        exibicao = 0;  //zera exibicao pra não passar das opções disponíveis
    }

    /*
    exibicao = 0 -> quilometragem
    exibicao = 1 -> velocidade instantanea
    exibicao = 2 -> velocidade media
    exibicao = 3 -> temperatura 
    exibicao = 4 -> umidade
    exibicao = 5 -> inicia/finaliza ida/volta ao pressionar o botao config
    exibicao = 6 -> insere marcador com o botao config
    */

    ResetaCursor();
    if(exibicao == 6){
        lcd.printf("Marcadores");
    }
    //AtualizaDisplay();
   
}

void Funcoes1s(){   //chama as funções que devem ser chamadas a cada 1s
     if(!(btnFreio.read()) && !(btnAcelerador.read())){   //se nenhum pedal estiver pressionado
        ledInjecao = ledInjecao - 0.1;  //decrementa o pwm
        //printf("\nDuty cycle injecao livre: %.2f \n", ledInjecao.read());
    }
    LimiteDC(); //funcao para limitar o duty cycle

    CalcVelMedia();

    hodometro();
    
}

/*========================================================== FUNÇÃO PRINCIPAL =============================================================*/

int main() {
    int crit = 0;
    ledAlerta = 0;

    ledInjecao.period_ms(20);                   //define o período do PWM do led de injeção como 20 ms 
    timerPedais.attach(&LeituraPedais, 0.5f);   //chama a função que le o estado dos pedais a cada 0.5 s
    timer1s.attach(&Funcoes1s, 1.0f); //chama a função que verifica se nenhum pedal foi pressionado a cada 1 s
   
    timer_40ms.attach(&hodometro, 0.04f);        //chama a funcao para executar as funcoes a cada 40 ms
    timer_10ms.attach(&AtualizaDisplay, 0.01f);       //atualiza o display a cada 10 ms
   
    
    
    btnFuncao.fall(&SelecionaExibicao);   //chama a função de mudar o que vai ser exibido no display ao pressionar o botão
    btnSelec.fall(&BtnConfig);   //chama a função de selecionar alguma configuraçaõ ao pressionar o botão

    while(1){
        

        if(crit == 1)
            while(1){
                wait_ms(100);
                lcd.cls();
                lcd.locate(3,3);
                lcd.printf("Carro parando");
                lcd.locate(3,13);
                lcd.printf("Velocidade: %.2f km/h", (ledInjecao.read())*100);
            }
        
        wait_ms(100);
    }
}





