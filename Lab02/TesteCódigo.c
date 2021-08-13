#include <stdio.h>

int main()
{

    char LISTA[] = {'N','Y','A','H',' ','H','U','S','T','O','N',0, 7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0};
    int notaIndividual[8] = {0};
    float notasParticipantes[8];
    int i = 0, k = 0;
    char nomes[8][15];

    //percorre a LISTA[]
    for(int numCompetidores = 0; numCompetidores<8; numCompetidores++){
            //pega o nome do skatista
            while(LISTA[i] != 0){
                    nomes[k][i] = LISTA[i];
                    i++;
            }
            //apenas exibe o nome do cara. Apagar depois
            int o = 0;
            while(LISTA[o] != 0){
                printf("%c", nomes[k][o]);
                o++;
            } //apagar até aqui



    }



    //pega as notas do competidor
    for (int todasNotas = 0; todasNotas<14 ; todasNotas++)
    {
        if(todasNotas%2 != 0) //ta na unidade decimal
        {
            notaIndividual[(todasNotas-1)/2] = (LISTA[todasNotas-1])*100+LISTA[todasNotas];
        }
    }

    //exibe as 7 notas do infeliz. Apagar depois
    for (int q = 0; q<7; q++)
    {
        printf("Nota: %d\t", notaIndividual[q]);
    } //apagar até aqui

    //ordena em ordem decrescente
    for (int m = 1; m < 8; m++)
    {
        for (int n = 0; n < m; n++)
        {
            if (notaIndividual[m] > notaIndividual[n])
            {
                int temp = notaIndividual[m];
                notaIndividual[m] = notaIndividual[n];
                notaIndividual[n] = temp;
            }
        }
    }

    for (int p = 0; p<7; p++)
    {
        printf("\nNota ordenada[%d]: %d\n", p, notaIndividual[p]);
    }

    for (int indiceSkatista = 0; indiceSkatista<8; indiceSkatista++ )
    {
        printf("\nCompetidor num %d:\n", indiceSkatista);
        //colocar isso aqui dentro dum for pra armazenar pra cada participante
        float notaFinal = 0;
        //soma as 4 melhores notas
        for (int indiceNota = 0; indiceNota<4; indiceNota++)
        {
            printf("Nota[%d]: %d \t", indiceNota, notaIndividual[indiceNota]);
            notaFinal+=notaIndividual[indiceNota];
            //printf("\nNota final parcial: %f\t", notaFinal);

        }
        //atribui a nota de cada participante ao array
        notasParticipantes[indiceSkatista] = (notaFinal)/100;
        //float teste = notaFinal/100;
        //printf("\n\t\t TESTE: %f\n", teste);
        printf("\nNota final [%d]: %f\n", indiceSkatista, notasParticipantes[indiceSkatista]);
    }


    return 0;
}


void calculaNotas(float vetorNotas[]){


}


