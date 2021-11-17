#include "mbed.h"
#include "C12832.h"

C12832 lcd(SPI_MOSI, SPI_SCK, SPI_MISO, p8, p11);   //configuração do display

/*======================= Exibição no display LCD ==============*/
//botão para selecionar a função desejada 
InterruptIn btnFuncao(p10); //declarado como interrupção pra ser chamado sempre que pressionado
//botão para selecionar ou zerar valores
DigitalIn btnConfig(p11);
//função utilizada para indicar o que deve ser exibido no display (velocidade, quilometragem, umidade)

char exibicao = 0; //será incrementado para realizar o controle do que será exibido

void SelecionaFuncao(){
    exibicao++;

    /*
    exibicao = 0 -> quilometragem
    exibicao = 1 -> velocidade instantanea
    exibicao = 2 -> velocidade media
    exibicao = 3 -> temperatura 
    exibicao = 4 -> umidade
    */

    if(exibicao == 5){
        exibicao = 0;  //zera exibicao pra não passar das opções disponíveis
    }
   
}


//função utilizada para selecionar ou zerar valores
void ConfiguraValores(){

}


/*============================== Função principal ==============*/


int main() {
     printf("Hello world!\n");
    
        btnFuncao.fall(&SelecionaFuncao);   //interrupção chamada sempre que o botão pra selecionar função for pressionado
    
    
    wait_ms(osWaitForever);
}