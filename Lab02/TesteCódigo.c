#include <stdio.h>

int main()
{

    char LISTA[] = {7, 90, 9, 11, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0};
    int notasCentesimal[8] = {0};
    int notasOrdenadas[7];
    int nota1 = 0, nota2 = 0, nota3 = 0, nota4 = 0;
    int notasParticipantes[8];

    //pega as notas do competidor
    for (int i = 0; i<14 ; i++)
    {
        if(i%2 != 0){//ta na unidade decimal
                notasCentesimal[(i-1)/2] = (LISTA[i-1])*100+LISTA[i];
        }

    }

    printf("\nAgora vamo ver se deu certo essa merda!\n");

    //exibe as 7 notas do infeliz
    for (int j = 0; j<7; j++){
        printf("Nota: %d\n", notasCentesimal[j]);
    }

     for (int i = 1; i < 8; i++) {
        for (int j = 0; j < i; j++) {
            if (notasCentesimal[i] > notasCentesimal[j]) {
                int temp = notasCentesimal[i];
                notasCentesimal[i] = notasCentesimal[j];
                notasCentesimal[j] = temp;
            }
        }
    }

      for (int j = 0; j<7; j++){
        printf("\nNota ordenada[%d]: %d\n", j, notasCentesimal[j]);
    }


    return 0;
}


