#include <stdio.h>

int main()
{

    char LISTA[] = {7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0};
    int notasCompetidores[8] = {0};
    float notasParticipantes[8];

    //pega as notas do competidor
    for (int i = 0; i<14 ; i++)
    {
        if(i%2 != 0) //ta na unidade decimal
        {
            notasCompetidores[(i-1)/2] = (LISTA[i-1])*100+LISTA[i];
        }
    }

    printf("\nAgora vamo ver se deu certo essa merda!\n");

    //exibe as 7 notas do infeliz
    for (int j = 0; j<7; j++)
    {
        printf("Nota: %d\n", notasCompetidores[j]);
    }

    //ordena em ordem decrescente
    for (int i = 1; i < 8; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (notasCompetidores[i] > notasCompetidores[j])
            {
                int temp = notasCompetidores[i];
                notasCompetidores[i] = notasCompetidores[j];
                notasCompetidores[j] = temp;
            }
        }
    }

    for (int j = 0; j<7; j++)
    {
        printf("\nNota ordenada[%d]: %d\n", j, notasCompetidores[j]);
    }

    for (int indiceSkatista = 0; indiceSkatista<8; indiceSkatista++ )
    {
        //colocar isso aqui dentro dum for pra armazenar pra cada participante
        int notaFinal = 0;
        //soma as 4 melhores notas
        for (int indiceNota = 0; indiceNota<4; indiceNota++)
        {
            printf("\nNota[%d]: %d", indiceNota, notasCompetidores[indiceNota]);
            notaFinal+=notasCompetidores[indiceNota];

        }
        //atribui a nota de cada participante ao array
        notasParticipantes[indiceSkatista] = (notaFinal)/100;
        printf("\nNota final [%d]: %f", indiceSkatista, notasParticipantes[indiceSkatista]);
    }


    return 0;
}


