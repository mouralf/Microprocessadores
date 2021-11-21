#include "mbed.h"
#include "C12832.h"
#include "Sht31.h"
/*==================================== Definições ===============================*/
DigitalIn btnAcelerador(p13);   //define o botão conectado ao pino 13 (Acelerador) como entrada
DigitalIn btnFreio(p12);        //define o botão conectado ao pino 13 (Freio) como entrada
C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);
Sht31 sht31(I2C_SDA, I2C_SCL);
DigitalOut ledAlerta(p19);
InterruptIn botFunc(p10);
InterruptIn botSelec(p11);
PwmOut speaker(p21);
PwmOut ledInjecao(p23);         //define o pino 23 como saída PWM

Ticker timer_40ms;              //interrupt com timer de 40 ms
Ticker timer_500ms;             //interrupt com timer de 0,5seg
Ticker timerPedais;             //interrupção que será chamada repetidamente pra ler os pedais
Ticker pedaisLivres;           //interrupção que será chamada quando não tiver nenhum pedal sido pressionado
/*================================== Leitura dos pedais ===================================================================*/
int timer40 = 0;        //variavel para controlar as funcoes q sao ativadas a cada 40 ms
int timer500 = 0;       //variavel para controlar funcoes q sao ativadas a cada 0,5s
int exibicao = 0;       //variavel para controlar a funcao no LCD
int idaVolta = 0;       //variavel para controlar o estado da viagem
int marcador = 0;       //variavel para controlar se deve posicionar um marcador 

void muda_funcao(){
    exibicao++;             //incrementa mudando a funcao aparente no LCD
    if(exibicao >= 7)       //se chegar no limite reseta a exibicao
        exibicao = 0;
}

void buzzer(){                  //funcao do buzzer
    speaker.period(1.0/480);    //frequencia de 480Hz
    speaker = 0.5;              //volume em 0.5
}

void timer40ms(){
    timer40 = 1;        //a cada 40 ms permite a execucao de algumas funcoes
}

void timer500ms(){
    timer500 = 1;       //a cada 0,5 seg permite a execucao de algumas funcoes
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
    }
 
    LimiteDC(); //funcao para limitar o duty cycle

    //printf("\nDuty cycle injecao pressionado: %.2f \n", ledInjecao.read());
}

void VerificaPedais(){
    if(!(btnFreio.read()) && !(btnAcelerador.read())){   //se nenhum pedal estiver pressionado
        ledInjecao = ledInjecao - 0.1;  //decrementa o pwm
        //printf("\nDuty cycle injecao livre: %.2f \n", ledInjecao.read());
        
    }
    
    LimiteDC(); //funcao para limitar o duty cycle
}

float hodometro(float velocidade_media, float quilometragem, int exibicao){ 			// acumula distancia total e calcula distancia da velocidade media 
                                    // em cada período para adicionar na quilometragem
    float distancia;			// calcula distância percorrida através da velocidade média
    
    distancia = (velocidade_media*0.5)/3600; 		//converte velocidade media de Km/h para metros/segundo
    quilometragem = quilometragem + distancia;			// adiciona quilometragem total 

    if(exibicao == 0){                              //exibe a quilometragem no LCD se estiver no exibicaoice especifico
        lcd.cls();
        lcd.locate(3, 3);              //coloca o cursor na segunda linha
        lcd.printf("Quilometragem: %.2f km", quilometragem);   
    }
    return quilometragem;
} 

void ExibeVelocInst(int exibicao){      //função para printar a velocidade instantanea 
    if(exibicao == 1){                                //SE TIVER A FUNÇÃO DE PRINTAR FORA DESTA, APAGAR O IF 
        lcd.cls();
        lcd.locate(3, 3);              //coloca o cursor na segunda linha
        lcd.printf("Vel. Inst.: %.2f km/h", (ledInjecao.read())*100);
    }
}

float velocidadeMedia(int exibicao){ //função para calcular e printar a velocidade média
    static float velMedia;
    static float vel1, vel2 = 0, i = 0;

    vel1 = vel2;                //armazena a ultima velocidade instantanea
    vel2 = ledInjecao.read();   //armazena a velocidade instantanea atual
    i++;                        //incrementa o contador i, para alterar a velocidade media a cada 1 seg
    
    if(i == 2){
        velMedia = ((vel1+vel2)/2)*100;                   //calcula a velocidade média das ultimas 2 velocidades instantaneas calculadas
        i = 0;    
    }
    
    if(exibicao == 2){                  //funcao para printar a velocidade média, de acordo com a variavel exibicao
        lcd.cls();
        lcd.locate(3, 3);
        lcd.printf("Vel. Media: %.2f km/h", velMedia);
    }
    return velMedia;
}

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
            buzzer();                           //funcao para sinal sonoro quando em estado critico
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
            buzzer();                           //funcao para sinal sonoro quando em estado critico
            cont_crit++;                        //aumenta o contador de tempo em que a agua esta critico
        }
        else{                                   //mostra que a umidade esta baixa
            if(exibicao == 4){
                lcd.cls();
                lcd.locate(3, 3);
                lcd.printf("Umidade: %.2f %%", umidade);
            }
            lcd.locate(3, 13);
            lcd.printf("Umidade  baixa");
        }
    }
    else{                                       //caso a umidade esteja normal, apenas mostra a umidade no LCD
        if(exibicao == 4){
            lcd.cls();
            lcd.locate(3, 3);
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

void ida_volta(){
    idaVolta++;         //incrementa o valor da variavel que controla se o carro esta indo ou voltando do trajeto toda vez q o botao funcao for acionado
                        //idaVolta = 0: nao esta marcando um percurso
                        //idaVolta = 1: ida ; idaVolta = 2: volta
}

void ativar_marcador(){
    marcador = 1;
}

float tempoViagem(){    
    static float tempoTotal = 0;                       //funcao incrementara 1 seg no contador de tempo total
    tempoTotal++;                       //incrementa 1 seg

    if(idaVolta == 0)                   //só conta o tempo enquanto estiver no meio de uma viagem
        tempoTotal = 0;                 //se nao estiver em uma viagem, zera o tempo
    
    return tempoTotal;                  //retorna o tempo total 
}

float distPercorridaViagem(float velMediaViagem, float tempo){  //funcao para calcular a distancia da viagem
    float distancia_percurso;

    tempo = tempo/3600;                                 //transforma o tempo de segundos para horas

    distancia_percurso = velMediaViagem*tempo;          //calcula a distancia ate o momento

    return distancia_percurso;                          //retorna a distancia
}

float velMediaViagem(){  //função para calcular a velocidade media total 
    float velMediaAtual, velAtual;
    static float velTotal = 0, velTotalVolta = 0;
    static int cont_velMedia = 0, cont_velMediaVolta = 0;
    
    velAtual = (ledInjecao.read())*100;
    velTotal = velTotal + velAtual;             //incrementa a velocidade total do percurso
    cont_velMedia++;                            //incrementa o contador de quantas velocidades foram contadas.
    velMediaAtual = velTotal/cont_velMedia;     //calcula a media da velocidade total
    
    if(idaVolta == 2){
        velTotalVolta = velTotalVolta + velAtual;
        cont_velMediaVolta++;
        velMediaAtual = velTotalVolta/cont_velMediaVolta;
    }
    else{
        velMediaAtual = velTotal/cont_velMedia;     //calcula a media da velocidade total
    }

    return velMediaAtual;                               //retorna a velocidade media atual
}

void proximidade_marcador(float distanciaVolta, float marcador, float velMediaVolta, float tempoVolta){
    int proximidade;

    tempoVolta = tempoVolta/3600;       //tranforma o tempo em segundos para horas

    distanciaVolta = distanciaVolta - (velMediaVolta*tempoVolta)*1000;  //considera distancia de volta a distancia da ida
                                                                        //e decrementa dela o quanto percorreu na volta
    proximidade = marcador - distanciaVolta;            //verifica a distancia entre marcador e carro

    if(proximidade >= 200 || proximidade <= 200){       //verifica a distancia entre marcador e carro é por volta de 200 metros do marcados
        ledAlerta = 1;                          //aciona o led de alerta avisando que esta proximo do marcador
    }
    else{
        ledAlerta  = 0;
    }
}

void viagem(){
    static float marcadores[4] = {0,0,0,0};
    static float distanciaVolta = 0, tempoTotal = 0, tempoVolta = 0; 
    int i,k;
    
    tempoTotal = tempoViagem(); //armazena o tempo total da viagem

    if(marcador == 1){              //verifica se o botao para adicionar um marcador foi acionado
        k = 0;
        while(marcadores[k] == 0){     //verifica se tem espaço livre no vetor dos marcadores
            k++;
            if(k >= 4) {                //se chegou no limite de 4, vai redefinir o ultimo marcador posto
                marcadores[3] = 0;
                k = 3;
            }
        }
        marcadores[k] = distPercorridaViagem(velMediaViagem(), tempoTotal);    //Armazena a posicao do marcador no vetor
        marcador = 0;       //reseta a variavel que controla se um marcador deve ser posto
    }

    if(idaVolta != 0){              //se alguma viagem estiver ativa, ida ou volta
        if(idaVolta == 1){          //se estiver na ida
            lcd.cls();
            lcd.locate(3,3);          
            lcd.printf("Viagem: Ida");      //mostra no LCD que esta na ida
            distanciaVolta = distPercorridaViagem(velMediaViagem(),tempoTotal);
        }

        if(idaVolta == 2){                  //se estiver na volta
            tempoVolta = tempoViagem() - tempoTotal;    //armazena o tempo da volta da viagem

            for(i = 0; i<4; i++)    //verifica se algum marcador esta proximo
                proximidade_marcador(distanciaVolta, marcadores[i], velMediaViagem(), tempoVolta);                  
            
            lcd.cls();
            lcd.locate(3,3);          
            lcd.printf("Viagem: Volta");    //mostra no LCD que esta na volta
        }

        for(i = 0; i<4; i++)                
            if(marcadores[i] != 0){         //verifica se há algum marcador ativo na viagem (MAX 4)
                lcd.locate(3,13);          
                lcd.printf("Marcador ativo");
            }
        if(idaVolta == 3){
            exibicao = 0;   //caso acabe a volta da viagem volta para retorna para a exibicao 0 no LCD
            idaVolta = 0;   //reseta o valor de ida e volta
        }
    }
    else{                               //se nenhuma viagem estiver ativa
        lcd.cls();
        lcd.locate(3,3);          
        lcd.printf("Sem percurso");     //sinaliza que nao começou a viagem para rastreamento
    }
}









/*============================== Função principal ==============*/

int main() {
    float velMedia = 0, quilometragem = 0;
    int crit = 0;
    
    ledInjecao.period_ms(20);                   //define o período do PWM do led de injeção como 20 ms 
    timerPedais.attach(&LeituraPedais, 0.5f);   //chama a função que le o estado dos pedais a cada 0.5 s
    pedaisLivres.attach(&VerificaPedais, 1.0f); //chama a função que verifica se nenhum pedal foi pressionado a cada 1 s
    timer_500ms.attach(&timer500ms, 0.5f);      //chama a funcao para executar as funcoes a cada 500 ms
    timer_40ms.attach(&timer40ms, 0.04f);        //chama a funcao para executar as funcoes a cada 40 ms
    
    while(1){
        
        if(exibicao == 5){      //caso a exibicao seja = 6, ao clicar no botao funcao ativa ou termina a viagem
            botFunc.fall(callback(&ida_volta)); //FUNCOES DE INTERRUPÇÃO PARA OS BOTÕES
            botSelec.fall(callback(&ativar_marcador));
        }
        else{
            botFunc.fall(callback(&muda_funcao));   //ao clicar no botao funcao, alterna a funcao aparente no LCD
        }
        
        if(timer500 == 1){              //a cada 0,5 seg as funcoes são chamadas
            ExibeVelocInst(exibicao);   //funcao para a velocidade constante
            velMedia = velocidadeMedia(exibicao);  //funcao para a velocidade media
            quilometragem = hodometro(velMedia, quilometragem, exibicao); //funcao para a quilometragem
            if(exibicao == 5)
                viagem();
            timer500 = 0;               //reseta a variavel q permite a execução das funções acima
        }
        
        if(timer40 == 1){               //a cada 40 ms as funcoes sao chamadas
            crit = temp_umid(exibicao); //funcao para a temperatura e umidade
            timer40 = 0;                //reseta a variavel q permite a execução das funções acima
        }
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





