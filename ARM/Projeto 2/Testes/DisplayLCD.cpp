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

    if(exibicao == 4){
        exibicao = 0;  //zera exibicao pra não passar das opções disponíveis
    }
    
    /*
    exibicao = 0 -> velocidade instantanea
    exibicao = 1 -> velocidade media
    exibicao = 2 -> quilometragem
    exibicao = 3 -> temperatura e umidade
    */
    ExibeMssg();    //chama função que escreve no display

   
}


//função utilizada para selecionar ou zerar valores
void ConfiguraValores(){

}


void ExibeMssg(){
    lcd.cls();  //sempre que for mudar a mssg de exibição limpa a tela

    switch (exibicao)
    {
        case 0: //velocidade instantanea
            lcd.printf("Velocidade inst.");
            lcd.locate(1, 12); //coloca o cursor na segunda linha
            lcd.printf("%d ", velocInst);  //printa a velocidade instantanea
            lcd.printf(" km/h");
            break;

        case 1: //velocidade media
            lcd.printf("Velocidade media");
            lcd.locate(1, 2); //coloca o cursor na segunda linha
            lcd.printf("%d ", velocMedia);  //printa a velocidade instantanea
            lcd.printf("km/h");
            break;
        
        case 2: //quilometragem
            lcd.printf("Quilometragem");
            lcd.locate(1, 12); //coloca o cursor na segunda linha
            lcd.printf("%d ", quilometragem);  //printa a velocidade instantanea
            lcd.printf("m");

            break;

        case 3: //temperatura e umidade
            lcd.printf("Temp.: %d oC", temperatura);    //printa a temperatura
            lcd.locate(1, 12); //coloca o cursor na segunda linha
            lcd.printf("Umid.: %d %", umidade);  //printa a umidade
            break;
    }

}

/*============================== Função principal ==============*/


int main() {
     printf("Hello world!\n");
    
        btnFuncao.fall(&SelecionaFuncao);   //interrupção chamada sempre que o botão pra selecionar função for pressionado
    
    
    wait_ms(osWaitForever);
}