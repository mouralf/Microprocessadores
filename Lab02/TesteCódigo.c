#include <stdio.h>

int main()
{

    char LISTA[] = {8, 02, 6, 77, 9, 3, 0, 0, 9, 35, 9, 50, 9,30};
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





