#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <stdlib.h>

typedef struct{ 
    char *numero; //Salva os digitos do número digitado
    size_t tamanho; //Armazena o tamanho do vetor de números
    int isNegative; //Salva 1 se for negativo e 0 se for positivo
} bigNumber; //Nome da estrutura

bigNumber* readNumber();
bigNumber* verificaOperacao(bigNumber* primeiroNumero, bigNumber* segundoNumero);
void liberaMemoria(bigNumber* numero);

#endif
