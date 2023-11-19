#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 10 //Define um tamanho inicial para o vetor que armazenará os números

void verificaAlocacao(char *ptr) {
    if (ptr == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
} 

void verificaSinais(char **numero, int *negativo) {
    size_t comprimento = strlen(*numero);
    *negativo = ((*numero)[0] == '-');

    if (*negativo) {
        // Move todo o bloco de memória uma posição para a esquerda, removendo o sinal de negativo do número
        memmove(*numero, *numero + 1, comprimento);
        *numero = realloc(*numero, (comprimento) * sizeof(char));
    }
    printf("numero mudado verificaSinais: %s\n", *numero);

}

size_t* findBiggest(size_t *primeiroTamanho, size_t *segundoTamanho){
    if(*primeiroTamanho > *segundoTamanho)
        return primeiroTamanho;
    else
        return segundoTamanho;
}

void invertNumber(char *numero){
    char temp;
    size_t tamanho = strlen(numero);
    for (size_t i = 0; i < tamanho / 2; i++) {
        temp = numero[i];
        numero[i] = numero[tamanho - i - 1];
        numero[tamanho - i - 1] = temp;
    }
}


//Função que lê os números digitados pelo usuario e inverte
char* readNumber(size_t *tamanho, int *numeroNegativo){ 

    size_t capacidade = CAPACIDADE_INICIAL; 
    char *numero = malloc(capacidade * sizeof(char));     //Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    verificaAlocacao(numero);  
    *tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c

    while((c = getchar())!= '\n'){ //Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
        //Bloco condicional para aumentar, se necessário, a memória alocada no vetor
        if (*tamanho == capacidade){ //Se o tamanho do vetor for igual a capacidade
            capacidade *= 2; //Aumente a capacidade do vetor em 25%
            numero = realloc(numero, capacidade * sizeof(char)); //Comando para realocar a memória do vetor para a nova capacidade incrementada
            verificaAlocacao(numero);
        }
        numero[(*tamanho)++] = (char)c;
    }

    if (*tamanho > 0) {
        numero[(*tamanho)] = '\0';
        (*tamanho)--;
    }

    verificaSinais(&numero, numeroNegativo);

    *tamanho = strlen(numero);  
    numero = realloc(numero, (*tamanho + 1) * sizeof(char));  

    invertNumber(numero);

    return numero;

}

char* sum(char *primeiroNumero, size_t *primeiroTamanho, size_t *segundoTamanho, char *segundoNumero){
    // somar posição a posição, se passar de 9 adicionar lógica de carry para passar a casa da dezena para a proxima posição somando
    // pensar em como encerrar o processo quando o tamanho de um dos vetores chegar ao final e só repetir ate os numeros até o final dos carry

    //Tamanho máximo do resultado da soma será o tamanho do maior valor + 1 (para casos de carry em somas > 9)
    size_t tamanhoMaximo = (*findBiggest(primeiroTamanho, segundoTamanho)) + 1; 
    char *resultado = malloc((tamanhoMaximo+1)*sizeof(char));
    verificaAlocacao(resultado);
    
    int carry = 0;
    size_t i;

    for (i = 0; i < tamanhoMaximo; i++){
        int primeiroDigito = (i < *primeiroTamanho) ? primeiroNumero[i] - '0': 0;
        int segundoDigito = (i < *segundoTamanho) ? segundoNumero[i] - '0': 0;

        int soma = primeiroDigito + segundoDigito + carry;

        carry = soma/10;
        
        resultado[i] = (char)(soma % 10 + '0');
    }

    if(carry > 0){
        resultado[i] = (char)(carry+'0');
        i++;
    }

    resultado[i] = '\0';

    invertNumber(resultado);

    return resultado;
}

char* subtract(char *primeiroNumero, size_t *primeiroTamanho, size_t *segundoTamanho, char *segundoNumero){
   

}

int main(int argc, char const *argv[]){
    int primeiroNegativo, segundoNegativo;
    size_t primeiroTamanho, segundoTamanho;
    printf("Digite o primeiro numero: ");
    char *primeiroNumero = readNumber(&primeiroTamanho, &primeiroNegativo);
    printf("Digite o segundo numero: ");
    char *segundoNumero = readNumber(&segundoTamanho, &segundoNegativo);

    char *resultado = sum(primeiroNumero, &primeiroTamanho, &segundoTamanho, segundoNumero);

    size_t i = 0;
    if(resultado[i] == '0'){
        i = 1;
    }

    printf("Resultado da soma: %s", &resultado[i]);

    free(primeiroNumero);
    free(segundoNumero);

    return 0;
}

