#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 10 //Define um tamanho inicial para o vetor que armazenará os números

void verifyAlloc(char *ptr) {
    if (ptr == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
} 

size_t* findBiggest(size_t *primeiroTamanho, size_t *segundoTamanho){
    if(*primeiroTamanho > *segundoTamanho)
        return primeiroTamanho;
    else
        return segundoTamanho;
}

void invertNumber(char *numero, size_t tamanho){
    char temp;
    for (size_t i = 0; i < tamanho / 2; i++) {
        temp = numero[i];
        numero[i] = numero[tamanho - i - 1];
        numero[tamanho - i - 1] = temp;
    }
}


//Função que lê os números digitados pelo usuario e inverte
char* readNumber(size_t *tamanho){ 

    size_t capacidade = CAPACIDADE_INICIAL; 
    char *numero = malloc(capacidade * sizeof(char));     //Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    verifyAlloc(numero);  
    *tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c
    while((c = getchar())!= '\n'){ //Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
        //Bloco condicional para aumentar, se necessário, a memória alocada no vetor
        if (*tamanho == capacidade){ //Se o tamanho do vetor for igual a capacidade
            capacidade += capacidade/4; //Aumente a capacidade do vetor em 25%
            numero = realloc(numero, capacidade * sizeof(char)); //Comando para realocar a memória do vetor para a nova capacidade incrementada
            verifyAlloc(numero);
        }
        numero[(*tamanho)++] = (char)c;
    }

    realloc(numero, (*tamanho + 1) * sizeof(char)); // Como durante o bloco condicional a memória alocada vai ser superior ao necessário, aqui ela será realocada para exatamente o tamanho do vetor
    numero[*tamanho] = '\0'; //Após a leitura dos dados adiciona o caracter de fim de vetor na ultima posição

    //Criar vetor separado para inverter
    invertNumber(numero, *tamanho);

    return numero;

}

char* sum(char *primeiroNumero, size_t *primeiroTamanho, size_t *segundoTamanho, char *segundoNumero){
    // somar posição a posição, se passar de 9 adicionar lógica de carry para passar a casa da dezena para a proxima posição somando
    // pensar em como encerrar o processo quando o tamanho de um dos vetores chegar ao final e só repetir ate os numeros até o final dos carry

    //Tamanho máximo do resultado da soma será o tamanho do maior valor + 1 (para casos de carry em somas > 9)
    size_t tamanhoMaximo = (*findBiggest(primeiroTamanho, segundoTamanho)) + 1; 
    char *resultado = malloc((tamanhoMaximo+1)*sizeof(char));
    verifyAlloc(resultado);
    
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

    invertNumber(resultado, i);

    return resultado;
    

}

char* subtract(char *primeiroNumero, size_t *primeiroTamanho, size_t *segundoTamanho, char *segundoNumero){
   

}

int main(int argc, char const *argv[]){
    size_t primeiroTamanho, segundoTamanho;
    printf("Digite o primeiro numero: ");
    char *primeiroNumero = readNumber(&primeiroTamanho);
    printf("Digite o segundo numero: ");
    char *segundoNumero = readNumber(&segundoTamanho);

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

