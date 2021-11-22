#include "mbed.h"
#include "C12832.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);   //configuração do display


/*======================= Exibição no display LCD ==============*/
//botão para selecionar a função desejada 
InterruptIn btnFuncao(p10); //declarado como interrupção pra ser chamado sempre que pressionado
//botão para selecionar ou zerar valores
InterruptIn btnSelec(p11);
//função utilizada para indicar o que deve ser exibido no display (velocidade, quilometragem, umidade)

//variáveis de controle
char exibicao; //será incrementado para realizar o controle do que será exibido
int statusViagem = 0;


//variáveis só pra demonstração
int quilometragem = 400;
int posicao = 800;
float ledInjecao = 0.3;
int velMedia = 70;



void BtnSelecPressed(){        //função utilizada para algumas configurações
    lcd.cls();          //limpa o display
    lcd.locate(3,3);    //coloca na posição inicial
    switch (exibicao){          //se tiver sendo exibindo a quilometragem
        case 0: //quilometragem
            quilometragem = 0;  //zera a quilometragem
            lcd.printf("Quilometragem: %d", quilometragem);
            break;
        
        case 1: //vel. instantanea
            lcd.printf("Vinst: %.2f km/h", ledInjecao*100);
            break;
        
        case 2: //vel. media
            lcd.printf("Vmed: %d km/h", velMedia);
            break;

        case 3: //temperatura
            lcd.printf("Temperatura: 30 C");
            break;
        
        case 4: //umidade
            lcd.printf("Umidade: 10");
            break;
        
        case 5: //status da viagem
            statusViagem++; // 1 ->inicio da ida; 2 = fim da ida e inicio da volta; 3 = fim da volta
            
            lcd.printf("Status: %d", statusViagem);
            
            if(statusViagem >= 3){
                statusViagem = 0;
            }
            break;
        
        case 6: //marcadores
            lcd.printf("Marcado em %d", posicao);
            break;
            
        default:
            lcd.printf("Exibicao: %d", exibicao);
    }
   
}

void BtnFncPressed(){ //função utilizada para indicar o que deve ser exibido no display 
    lcd.cls();          //limpa o display
    lcd.locate(3,3);    //coloca na posição inicial
    
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

    switch(exibicao){
        case 0: //quilometragem
            lcd.printf("Quilometragem");
            break;

        case 1: //velocidade inst.
            lcd.printf("Velocidade inst.");
            break;

        case 2: //vel. media
            lcd.printf("Velocidade media");
            break;

        case 3: //temperatura
            lcd.printf("Temperatura");
            break;

        case 4: //umidade
             lcd.printf("Umidade");
            break;

        case 5: //status da viagem
            lcd.printf("Status viagem");
            break;

        case 6: //marcadores
            lcd.printf("Marcadores");
            break;
        
        default:
            exibicao = 0;

    }
   
}

/*============================== Função principal ==============*/


int main() {
    exibicao = 0;
    printf("Hello world!\n");
    
    lcd.printf("Quilometragem");
    
        btnFuncao.fall(&BtnSelecPressed);   //interrupção chamada sempre que o botão pra selecionar função for pressionado
        btnSelec.fall(&BtnFncPressed);  //interrupção chamada para zerar a quilometragem
    
    wait_ms(osWaitForever);
}