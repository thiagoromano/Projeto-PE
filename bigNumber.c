#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 10 //Define um tamanho inicial para o vetor que armazenará os números

typedef struct{
    char* numero;
    int tamanho;
    int isNegative;
} bigNumber;

//Verifica a alocação de memória, se der erro, encerra o programa
void verificaAlocacao(bigNumber* numero) {
    if (numero->numero == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
} 

void liberaMemoria(bigNumber* numero){
    free(numero->numero);
}

//Verifica se um numero é negativo, se for, salva true em uma variavel do tipo inteiro, remove o sinal do vetor, e desloca tudo para a esquerda
void verificaSinais(bigNumber* numero) {
    numero->tamanho = strlen(numero->numero);
    numero->isNegative = ((numero->numero)[0] == '-');

    if (numero->isNegative) {
        // Move todo o bloco de memória uma posição para a esquerda, removendo o sinal de negativo do número
        memmove(numero->numero, (numero->numero) + 1, numero->tamanho);
    }
}

//Encontra o maior numero (em digitos) para determinar o tamanho do vetor resultado na soma e na subtração
size_t findBiggest(bigNumber* primeiroNumero, bigNumber* segundoNumero) {
    if (primeiroNumero->tamanho > segundoNumero->tamanho)
        return primeiroNumero->tamanho;
    else
        return segundoNumero->tamanho;
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
    size_t tamanho = strlen(numero->numero);

    // Encontrar a quantidade de zeros à esquerda
    while (zerosAEsquerda < tamanho && numero->numero[zerosAEsquerda] == '0') {
        zerosAEsquerda++;
    }

    // Remover os zeros à esquerda
    if (zerosAEsquerda > 0) {
        memmove(numero->numero, numero->numero + zerosAEsquerda, tamanho - zerosAEsquerda + 1);
        numero->tamanho = tamanho - zerosAEsquerda;
    }
}

//Função que lê os números digitados pelo usuario
bigNumber* readNumber(){ 

    size_t capacidade = CAPACIDADE_INICIAL; 
    bigNumber* numero = malloc(sizeof(bigNumber));     //Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    verificaAlocacao(numero);  
    numero->numero = malloc(capacidade*sizeof(char));
    numero->tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c

    while((c = getchar())!= '\n'){ //Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
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

    verificaSinais(numero);

    numero->tamanho = strlen(numero->numero);  
    numero->numero = realloc(numero->numero, (numero->tamanho + 1) * sizeof(char));  

    invertNumber(numero);

    return numero;

}

//Função de soma
bigNumber* sum(bigNumber* primeiroNumero, bigNumber* segundoNumero){
    // somar posição a posição, se passar de 9 adicionar lógica de carry para passar a casa da dezena para a proxima posição somando
    // pensar em como encerrar o processo quando o tamanho de um dos vetores chegar ao final e só repetir ate os numeros até o final dos carry

    //Tamanho máximo do resultado da soma será o tamanho do maior valor + 1 (para casos de carry em somas > 9)
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero); 
    bigNumber* resultado = malloc((tamanhoMaximo+1)*sizeof(char));
    verificaAlocacao(resultado);
    
    int carry = 0;
    size_t i;

    for (i = 0; i < tamanhoMaximo; i++){
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

    invertNumber(resultado);

    return resultado;
}

//Função de subtração
bigNumber* subtract(bigNumber* primeiroNumero, bigNumber* segundoNumero) {
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero);
    bigNumber* resultado = malloc((tamanhoMaximo + 1) * sizeof(char));
    verificaAlocacao(resultado);

    int emprestimo = 0;
    size_t i;

    for (i = 0; i < tamanhoMaximo; i++) {
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

    invertNumber(resultado);
    removeZerosAEsquerda(resultado);


    return resultado;
}


bigNumber* verificaOperacao(bigNumber* primeiroNumero, bigNumber* segundoNumero, bigNumber* resultado){
    char operacao;
    printf("Digite a operacao a ser realizada (+, - ou *): ");
    scanf("%c", &operacao);

    switch(operacao){
         case ('+'):

            //codigo aqui (a+b); (-a+b); (-a+(-b) = -a - b)

            break;

        case('-'):

            //codigo aqui (-a-b); (-a-(-b) = - a + b);  (a-b); (a-(-b) = a + b)

            break;

        case('*'):

            //codigo aqui (a*b); (-a*b); (a*(-b)); ((-a)*(-b)) (esse é o mais facil)
            //logica de multiplocação vai ser muito dificil
            //falta terminar essa função, implementar a função de multiplicação e transformar em TAD

            break;
    }

}

//Função principal para testar tudo, DEVE SER APAGADA para alterar para TAD

int main(int argc, char const *argv[]) {
    bigNumber *primeiroNumero, *segundoNumero, *resultado;

    printf("Digite o primeiro numero: ");
    primeiroNumero = readNumber();

    printf("Digite o segundo numero: ");
    segundoNumero = readNumber();

    verificaOperacao(primeiroNumero, segundoNumero, resultado);

    if (resultado->tamanho == 0 || (resultado->tamanho == 1 && resultado->numero[0] == '0')) {
        printf("0\n");
    } else {
        printf(" %s\n", resultado->numero);
    }

    liberaMemoria(primeiroNumero);
    liberaMemoria(segundoNumero);
    liberaMemoria(resultado);

    return 0;
}

