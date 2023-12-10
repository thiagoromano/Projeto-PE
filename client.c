#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigNumber.h"

int main() {
    bigNumber *primeiroNumero, *segundoNumero;

    while ((primeiroNumero = readNumber()) != NULL) {
        // Verifica se a leitura foi bem-sucedida
        if (primeiroNumero != NULL) {
            segundoNumero = readNumber();
            
            if (segundoNumero == NULL) {
                liberaMemoria(primeiroNumero);
                break;
            }
            if (primeiroNumero == NULL || segundoNumero == NULL) {
            break;
        }

            bigNumber* resultado = verificaOperacao(primeiroNumero, segundoNumero);

            printf("%s\n", resultado->numero);

            liberaMemoria(primeiroNumero);
            liberaMemoria(segundoNumero);
            liberaMemoria(resultado);

            // Consumir caractere de nova linha no buffer
            getchar();
        } else {
            // Se a leitura falhar, saia do loop
            break;
        }
    }
    return 0;
}