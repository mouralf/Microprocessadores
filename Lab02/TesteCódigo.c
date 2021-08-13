#include <stdio.h>

int main()
{

    char LISTA[] = {'N','Y','A','H',' ','H','U','S','T','O','N',0, 7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0};
    int notasCompetidores[8] = {0};
    float notasParticipantes[8];
    int i = 0, k = 0;
    char nomes[8][15];

    while(LISTA[i] != 0){
            nomes[k][i] = LISTA[i];
            i++;
    }

    int o = 0;
     while(LISTA[o] != 0){
        printf("%c", nomes[k][o]);
        o++;
    }

    //pega as notas do competidor
    for (int l = 0; l<14 ; l++)
    {
        if(l%2 != 0) //ta na unidade decimal
        {
            notasCompetidores[(l-1)/2] = (LISTA[l-1])*100+LISTA[l];
        }
    }

    printf("\nAgora vamo ver se deu certo essa merda!\n");

    //exibe as 7 notas do infeliz
    for (int q = 0; q<7; q++)
    {
        printf("Nota: %d\t", notasCompetidores[q]);
    }

    //ordena em ordem decrescente
    for (int m = 1; m < 8; m++)
    {
        for (int n = 0; n < m; n++)
        {
            if (notasCompetidores[m] > notasCompetidores[n])
            {
                int temp = notasCompetidores[m];
                notasCompetidores[m] = notasCompetidores[n];
                notasCompetidores[n] = temp;
            }
        }
    }

    for (int p = 0; p<7; p++)
    {
        printf("\nNota ordenada[%d]: %d\n", p, notasCompetidores[p]);
    }

    for (int indiceSkatista = 0; indiceSkatista<8; indiceSkatista++ )
    {
        printf("\nCompetidor num %d:\n", indiceSkatista);
        //colocar isso aqui dentro dum for pra armazenar pra cada participante
        float notaFinal = 0;
        //soma as 4 melhores notas
        for (int indiceNota = 0; indiceNota<4; indiceNota++)
        {
            printf("Nota[%d]: %d \t", indiceNota, notasCompetidores[indiceNota]);
            notaFinal+=notasCompetidores[indiceNota];
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


