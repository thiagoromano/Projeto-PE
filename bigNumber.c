#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 100 //Define um tamanho inicial para o vetor que armazenará os números

typedef struct{
    char* numero;
    size_t tamanho;
    int isNegative;
} bigNumber;

//Verifica a alocação de memória, se der erro, encerra o programa
void verificaAlocacao(bigNumber* numero) {
    if (numero == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
} 

void liberaMemoria(bigNumber* numero) {
    if (numero != NULL) {
        if (numero->numero != NULL) {
            free(numero->numero);
        }
        free(numero);
    }
}

//Verifica se um numero é negativo, se for, salva true em uma variavel do tipo inteiro, remove o sinal do vetor, e desloca tudo para a esquerda
void verificaSinais(bigNumber* numero) {
    numero->tamanho = strlen(numero->numero);
    numero->isNegative = ((numero->numero)[0] == '-');

    if (numero->isNegative) {
        size_t novoTamanho = numero->tamanho - 1; // Desconsiderar o sinal negativo
        memmove(numero->numero, (numero->numero) + 1, novoTamanho);
        numero->numero[novoTamanho] = '\0'; // Garantir que a string seja terminada corretamente
        numero->tamanho = novoTamanho;
    }
}


//Encontra o maior numero (em digitos) para determinar o tamanho do vetor resultado na soma e na subtração
size_t findBiggest(bigNumber* primeiroNumero, bigNumber* segundoNumero) {
    if (primeiroNumero->tamanho > segundoNumero->tamanho)
        return primeiroNumero->tamanho;
    else
        return segundoNumero->tamanho;
}

int compararNumeros(bigNumber *numero1, bigNumber *numero2) {
    if (numero1->tamanho < numero2->tamanho) {
        return -1;
    } else if (numero1->tamanho > numero2->tamanho) {
        return 1;
    }

    // Tamanhos iguais, comparar dígito a dígito
    for (size_t i = 0; i < numero1->tamanho; i++) {
        if (numero1->numero[i] < numero2->numero[i]) {
            return -1;
        } else if (numero1->numero[i] > numero2->numero[i]) {
            return 1;
        }
    }

    // Números são iguais
    return 0;
}

//Função que inverte o vetor de numeros
void invertNumber(bigNumber* numero) {
    char temp;
    size_t tamanho = strlen(numero->numero);

    for (size_t i = 0; i < tamanho / 2; i++) {
        temp = numero->numero[i];
        numero->numero[i] = numero->numero[tamanho - i - 1];
        numero->numero[tamanho - i - 1] = temp;
    }
}

void removeZerosAEsquerda(bigNumber *numero) {
    size_t zerosAEsquerda = 0;
    size_t tamanho = numero->tamanho;

    // Encontrar a quantidade de zeros à esquerda
    while (zerosAEsquerda < tamanho && numero->numero[zerosAEsquerda] == '0') {
        zerosAEsquerda++;
    }

    // Remover os zeros à esquerda
    if (zerosAEsquerda > 0) {
        memmove(numero->numero, numero->numero + zerosAEsquerda, tamanho - zerosAEsquerda + 1);
        numero->tamanho = tamanho - zerosAEsquerda;

        // Verificar se o vetor é todo composto por zeros
        if (numero->tamanho == 0) {
            // Realocar memória para conter um zero
            numero->numero = realloc(numero->numero, 2 * sizeof(char));
            verificaAlocacao(numero);
            numero->numero[0] = '0';
            numero->numero[1] = '\0';
            numero->tamanho = 1;
        }
    }
}

void removeZerosADireita(bigNumber* numero) {
    size_t tamanho = numero->tamanho;
    while (tamanho > 0 && numero->numero[tamanho - 1] == '0') {
        tamanho--;
    }

    if (tamanho > 0) {
        numero->numero[tamanho] = '\0';
        numero->tamanho = strlen(numero->numero);
    }
}

//Função que lê os números digitados pelo usuario
bigNumber* readNumber(){ 

    size_t capacidade = CAPACIDADE_INICIAL; 
    bigNumber* numero = malloc(sizeof(bigNumber));     //Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    numero->numero = malloc(capacidade*sizeof(char));
    verificaAlocacao(numero);
    numero->tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c

    while((c = getchar()) != '\n'){ //Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
        //Bloco condicional para aumentar, se necessário, a memória alocada no vetor
        if (numero->tamanho == capacidade){ //Se o tamanho do vetor for igual a capacidade
            capacidade += capacidade/4; //Aumente a capacidade do vetor em 25%
            numero->numero = realloc(numero->numero, capacidade * sizeof(char)); //Comando para realocar a memória do vetor para a nova capacidade incrementada
            verificaAlocacao(numero);   
        }
        numero->numero[(numero->tamanho)++] = (char)c;
    }
    

    if (numero->tamanho > 0) {
        numero->numero[(numero->tamanho)] = '\0';
        (numero->tamanho)--;
    }


    numero->tamanho = strlen(numero->numero);  
    // numero->numero = realloc(numero->numero, (numero->tamanho + 1) * sizeof(char));  
    verificaAlocacao(numero);
    
    verificaSinais(numero);
    removeZerosAEsquerda(numero);
    invertNumber(numero);

    return numero;

}

//Função de soma
bigNumber* sum(bigNumber* primeiroNumero, bigNumber* segundoNumero){
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero); 
    bigNumber* resultado = malloc(sizeof(bigNumber));
    resultado->numero = malloc((tamanhoMaximo+2)*sizeof(char));
    verificaAlocacao(resultado);
    
    
    size_t i = 0;
    int carry = 0;

    for (; i < tamanhoMaximo; i++){
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0': 0;
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0': 0;

        int soma = primeiroDigito + segundoDigito + carry;

        carry = soma/10;
        
        resultado->numero[i] = (char)(soma % 10 + '0');
    }

    if(carry > 0){
        resultado->numero[i] = (char)(carry+'0');
        i++;
    }

    resultado->numero[i] = '\0';
    resultado->tamanho = strlen(resultado->numero);

    invertNumber(resultado);
    removeZerosAEsquerda(resultado);

    return resultado;
}

//Função de subtração
bigNumber* subtract(bigNumber* primeiroNumero, bigNumber* segundoNumero, bigNumber* resultado) {

    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero);
    resultado->numero = malloc((tamanhoMaximo + 2) * sizeof(char));
    verificaAlocacao(resultado);

    int emprestimo = 0;
    size_t i = 0;

    for (; i < tamanhoMaximo; i++) {
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0' : 0;
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0' : 0;

        int subtracao = primeiroDigito - segundoDigito - emprestimo;

        if (subtracao < 0) {
            subtracao += 10;
            emprestimo = 1;
        } else {
            emprestimo = 0;
        }

        resultado->numero[i] = (char)(subtracao + '0');
    }

    resultado->numero[i] = '\0';
    resultado->tamanho = strlen(resultado->numero);

    invertNumber(resultado);
    removeZerosAEsquerda(resultado);

    return resultado;
}

bigNumber* multiply(bigNumber* primeiroNumero, bigNumber* segundoNumero) {

    if (strcmp(primeiroNumero->numero, "0") == 0 || strcmp(segundoNumero->numero, "0") == 0) {
        bigNumber* resultadoZero = malloc(sizeof(bigNumber));
        resultadoZero->numero = malloc(2 * sizeof(char)); // Para armazenar '0' e '\0'
        strcpy(resultadoZero->numero, "0");
        resultadoZero->tamanho = 1;
        resultadoZero->isNegative = 0;
        return resultadoZero;
    }


    size_t tamanhoResultado = primeiroNumero->tamanho + segundoNumero->tamanho;
    bigNumber* resultado = malloc(sizeof(bigNumber));
    resultado->numero = malloc((tamanhoResultado + 2) * sizeof(char)); // +2 para o '\0' e possível carry
    verificaAlocacao(resultado);

    size_t i, j;

    // Inicializa o resultado com zeros
    for (i = 0; i <= tamanhoResultado + 1; i++) {
        resultado->numero[i] = '0';
    }

    for (i = 0; i < primeiroNumero->tamanho; i++) {
        // printf("i: %zu\n", i); 
        int carry = 0;
        for (j = 0; j < segundoNumero->tamanho; j++) {
            // printf("j: %zu\n", j);
            int produto = (primeiroNumero->numero[i] - '0') * (segundoNumero->numero[j] - '0') + carry + (resultado->numero[i + j] - '0');
            carry = produto / 10;
            resultado->numero[i + j] = (char)(produto % 10 + '0');
        }

        while (carry > 0) {
            int soma = carry + (resultado->numero[i + j] - '0');
            carry = soma / 10;
            resultado->numero[i + j] = (char)(soma % 10 + '0');
            j++;
        }
    }

    resultado->tamanho = tamanhoResultado;
    removeZerosADireita(resultado);
    invertNumber(resultado);

    resultado->isNegative = (primeiroNumero->isNegative != segundoNumero->isNegative);
    if (resultado->isNegative) {
        memmove(resultado->numero + 1, resultado->numero, resultado->tamanho + 1);
        resultado->numero[0] = '-';
        resultado->tamanho++;
    }

    return resultado;
}

bigNumber* verificaOperacao(bigNumber* primeiroNumero, bigNumber* segundoNumero){
    bigNumber* resultado;
    resultado = malloc(sizeof(bigNumber));
    char operacao;
    bigNumber *temp;
    printf("um\n");
    temp = malloc(sizeof(bigNumber));
    printf("dois\n");
    scanf(" %c", &operacao);

    int compara = compararNumeros(primeiroNumero, segundoNumero);
    printf("tres\n");
    switch(operacao){
        case ('+'):
            printf("quatro\n");
            resultado = sum(primeiroNumero, segundoNumero);
            printf("quatro.2\n");
            break;
        case('-'):
            printf("cinco\n");
            if(compara == 0){
                resultado->tamanho = 1;
                resultado->numero = malloc(2 * sizeof(char));
                strcpy(resultado->numero, "0");
                return resultado;
            } else if (compara < 0){
                temp = primeiroNumero;
                primeiroNumero= segundoNumero;
                segundoNumero = temp;
                resultado->isNegative = 1;
            }
            printf("seis\n");
            subtract(primeiroNumero, segundoNumero, resultado);
            if(resultado->isNegative){
                printf("sete\n");
                memmove(resultado->numero + 1, resultado->numero, resultado->tamanho + 1);
                printf("oito\n");
                resultado->numero[0] = '-';
                resultado->tamanho++;
            }
            break;
        case('*'):
            resultado = multiply(primeiroNumero, segundoNumero);
            break;
        default:
            printf("Operacao invalida: %c\n", operacao);
            resultado = NULL;
    }
    if (temp != NULL) {
        liberaMemoria(temp);
    }
    return resultado;
}


//Função principal para testar tudo, DEVE SER APAGADA para alterar para TAD

int main() {
    bigNumber *primeiroNumero, *segundoNumero, *resultado;

    while ((primeiroNumero = readNumber()) != NULL) {
        // Verifica se a leitura foi bem-sucedida
        if (primeiroNumero != NULL) {
            segundoNumero = readNumber();
            
            if (segundoNumero == NULL) {
                break;
            }

            resultado = verificaOperacao(primeiroNumero, segundoNumero);
            removeZerosAEsquerda(resultado);

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

    liberaMemoria(primeiroNumero);
    liberaMemoria(segundoNumero);
    liberaMemoria(resultado);

    return 0;
}