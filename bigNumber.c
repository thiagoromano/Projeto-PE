#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 10 //Define um tamanho inicial para o vetor que armazenará os números

void verificarAlocacao(char *ptr) {
    if (ptr == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
}   

//Função que lê os números digitados pelo usuario e inverte
char* readNumber(size_t *tamanho){ 

    size_t capacidade = CAPACIDADE_INICIAL; 
    char *numero = malloc(capacidade * sizeof(char));     //Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    verificarAlocacao(numero);  
    *tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c
    while((c = getchar())!= '\n'){ //Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
        //Bloco condicional para aumentar, se necessário, a memória alocada no vetor
        if (*tamanho == capacidade){ //Se o tamanho do vetor for igual a capacidade
            capacidade += capacidade/4; //Aumente a capacidade do vetor em 25%
            numero = realloc(numero, capacidade * sizeof(char)); //Comando para realocar a memória do vetor para a nova capacidade incrementada
            verificarAlocacao(numero);
        }
        numero[(*tamanho)++] = (char)c;
    }

    realloc(numero, (*tamanho + 1) * sizeof(char)); // Como durante o bloco condicional a memória alocada vai ser superior ao necessário, aqui ela será realocada para exatamente o tamanho do vetor
    numero[*tamanho] = '\0'; //Após a leitura dos dados adiciona o caracter de fim de vetor na ultima posição

    //Criar vetor separado para inverter
    char *numeroInvertido = malloc((*tamanho + 1) * sizeof(char));
    verificarAlocacao(numeroInvertido);
    for (size_t i = 0; i < *tamanho; i++) {
        numeroInvertido[i] = numero[*tamanho - i - 1];  
    }
    numeroInvertido[*tamanho] = '\0';

    free(numero);

    return numeroInvertido;

}

char* sumNumber(size_t *tamanho, char *primeiroNumero, char *segundoNumero){
    // somar posição a posição, se passar de 9 adicionar lógica de carry para passar a casa da dezena para a proxima posição somando
    // pensar em como encerrar o processo quando o tamanho de um dos vetores chegar ao final e só repetir ate os numeros até o final dos carry
}


int main(int argc, char const *argv[]){
    size_t primeiroTamanho, segundoTamanho;
    printf("Digite o primeiro numero: ");
    char *primeiroNumero = readNumber(&primeiroTamanho);
    printf("Digite o segundo numero: ");
    char *segundoNumero = readNumber(&segundoTamanho);
    printf("Primeiro numero digitado invertido: %s\n", primeiroNumero);
    printf("Segundo numero digitado invertido: %s\n", segundoNumero);


    free(primeiroNumero);
    free(segundoNumero);

    return 0;
}


