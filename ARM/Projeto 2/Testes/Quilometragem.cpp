int quilometragem = 0; // variável global que acumula quilometragem total

void hodometro(int velocidade_media){ 			// acumula distancia total e calcula distancia da velocidade media 
                                    // em cada período para adicionar na quilometragem
    int distancia;			// calcula distância percorrida através da velocidade média

    distancia = (velocidade_media*1000)/3600; 		//converte velocidade media de Km/h para metros/segundo

    quilometragem = quilometragem + distancia;			// adiciona quilometragem total 

}


int main(){

        while(1){
		hodometro(velocidade_media);
		lcd.cls();
		lcd.locate(3,3);          
		lcd.printf("valor do hodometro: %d", quilometragem);
        }
return 0;
}