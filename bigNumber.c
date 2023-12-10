#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigNumber.h"
#define CAPACIDADE_INICIAL 50 //Define um tamanho inicial para o vetor que armazenará os números

//Verifica a alocação de memória, se der erro, encerra o programa
void verificaAlocacao(bigNumber* numero) {
    if (numero == NULL) {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
} 

// Função para liberar a memória do vetor de números e da estrutura
void liberaMemoria(bigNumber *numero){
    if (numero != NULL){
        if (numero->numero != NULL){
            free(numero->numero);
        }
        free(numero);
    }
}

// Verifica se um numero é negativo, se for, salva true em uma variavel do tipo inteiro, remove o sinal do vetor, e desloca tudo para a esquerda
void verificaSinais(bigNumber *numero){
    numero->isNegative = ((numero->numero)[0] == '-'); //O número será negativo caso o caractere na posição 0 seja -

    //Se o número for negativo:
    if (numero->isNegative){
        size_t novoTamanho = numero->tamanho - 1; // Desconsidera o sinal negativo
        memmove(numero->numero, (numero->numero) + 1, novoTamanho); // memmove move todo o bloco de dados uma posição para a esquerda
        numero->numero[novoTamanho] = '\0'; // Garantir que a string seja terminada corretamente
        numero->tamanho = novoTamanho; // Atualiza a variavel tamanho, desconsiderando o sinal de negativo
    }
}

// Encontra o maior numero (em digitos) para determinar o tamanho do vetor resultado na soma e na subtração
size_t findBiggest(bigNumber *primeiroNumero, bigNumber *segundoNumero){
    if (primeiroNumero->tamanho > segundoNumero->tamanho) // Se o primeiro tamanho for maior, retorna o tamanho do primeiroNumero
        return primeiroNumero->tamanho;
    else
        return segundoNumero->tamanho; // Se o segundo for maior, ou os tamanhos forem iguals, retorna o tamanho do segundo vetor
}


// Função que inverte o vetor de numeros
void invertNumber(bigNumber *numero){
    char temp; // Salva temporariamente os digitos do número
    size_t tamanho = strlen(numero->numero);

    for (size_t i = 0; i < tamanho / 2; i++){
        temp = numero->numero[i]; // Salva temporariamente os dígitos do número
        numero->numero[i] = numero->numero[tamanho - i - 1]; // Troca o elemento [i] do vetor pela posição correspondente inversa do indice atual
        numero->numero[tamanho - i - 1] = temp; // Manda para a posição [tamanho - i - 1] (inversa da posição [i]) o valor que antes estava na posição [i], salvo na variavel temp
    }
}

// Compara os números a fim de descobrir qual o maior em valor ABSOLUTO!
int compararNumeros(bigNumber *primeiroNumero, bigNumber *segundoNumero){
    invertNumber(primeiroNumero);
    invertNumber(segundoNumero);
    if (primeiroNumero->tamanho != segundoNumero->tamanho){ // Se os tamanhos forem diferentes
        return (primeiroNumero->tamanho < segundoNumero->tamanho) ? -1 : 1; // Operador ternário: retorna -1 se verdadeiro (segundoNumero > primeiroNumero) e 1 se falso
    }
    // Tamanhos iguais, comparar dígito a dígito
    for (size_t i = 0; i < primeiroNumero->tamanho; i++){
        if (primeiroNumero->numero[i] != segundoNumero->numero[i]){ // Compara dígito a dígito até encontrar um diferente e compara eles
            return (primeiroNumero->numero[i] < segundoNumero->numero[i]) ? -1 : 1; // Operador ternário: retorna -1 se verdadeiro (segundoNumero[i] > numero1[i]) e 1 se falso
        }
    }
    // Números são iguais
    return 0;
}

// Remove os zeros a esquerda (pós inversão) nas funções readNumber, sum e subtract
void removeZeros(bigNumber *numero, int direcao) {
    size_t tamanho = numero->tamanho;

    if (direcao == 0) {
        // A esquerda
        size_t zerosAEsquerda = strspn(numero->numero, "0"); // Conta quantos '0' tem no prefixo do número
        tamanho -= zerosAEsquerda; // Atualiza o tamanho de acordo com quantos zerosAEsquerda foram contados
        memmove(numero->numero, numero->numero + zerosAEsquerda, tamanho + 1); // Move todo o bloco de memória para a esquerda
    } else {
        // A direita
        while (tamanho > 0 && numero->numero[tamanho - 1] == '0') { // Loop que decresce o valor de tamanho a cada 0 encontrado no final da string
            tamanho--;
        }
        numero->numero[tamanho] = '\0'; // Atualiza o ultimo espaço da string para o '\0'
    }

    if (tamanho == 0) {
        // Realocar memória para conter APENAS um zero e o caractere de final de string
        numero->numero = realloc(numero->numero, 2 * sizeof(char));
        verificaAlocacao(numero);
        numero->numero[0] = '0';
        numero->numero[1] = '\0';
        tamanho = 1;
    }

    numero->tamanho = tamanho;
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
        if (feof(stdin)) { // Verifica se a entrada não chegou ao final do arquivo
            liberaMemoria(numero); // Libera a memória alocada ao numero
            return NULL; // Retorna NULL, encerrando o laço principal do código
        }
        if (numero->tamanho == capacidade - 1){ //Se o tamanho do vetor for igual a capacidade
            capacidade += capacidade/4 + 1;     //Aumente a capacidade do vetor em 25%
            numero->numero = realloc(numero->numero, capacidade * sizeof(char)); //Comando para realocar a memória do vetor para a nova capacidade incrementada
            verificaAlocacao(numero);   
        }
        numero->numero[(numero->tamanho)++] = (char)c; // Salva o correspondente char do inteiro digitado
    }
    
    // Salva o caractere nulo na ultima posição
    numero->numero[numero->tamanho] = '\0';
    
    verificaSinais(numero); // Verifica sinais a fim de remover o sinal de negativo, caso exista
    removeZeros(numero, 0); // Remove zeros (à esquerda)
    invertNumber(numero); // Inverte o número para facilitar nas operações futuras

    return numero;

}

// Se o resultado for negativo, adicionar o sinal ' - ' no inicio do vetor
void resultadoNegativo(bigNumber* resultado) { 
    char* temp = malloc(((resultado->tamanho) + 2) * sizeof(char)); // Adicionado +2 para acomodar o sinal negativo e o caractere nulo '\0'
    if (temp != NULL) {
        temp[0] = '-'; // Adiciona na posição 0 o sinal negativo
        strcpy(temp + 1, resultado->numero); // Copia o resultado->numero para o array temp
        free(resultado->numero); // Liberar a memória antiga
        resultado->numero = temp; // Atribui ao resultado->numero o valor de temp (sinal de negativo + valor do resultado)
        (resultado->tamanho)++;
    }
}

//Função de soma
bigNumber* sum(bigNumber* primeiroNumero, bigNumber* segundoNumero){
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero); // Atribui à variável tamanhoMaximo o maior tamanho (em dígitos) entre os dois números
    bigNumber* resultado = malloc(sizeof(bigNumber)); // Aloca memória do tipo bigNumber para o resultado
    resultado->numero = malloc((tamanhoMaximo+2) * sizeof(char)); // Aloca para o vetor de números o tamanho máximo + 2 (possivel carry e caracter nulo) * o tamanho de char
    verificaAlocacao(resultado); // Verifica alocação do resultado
    resultado->isNegative = 0; // Atribui 0 ao sinal do resultado (para facilitar futuras operações e evitar possiveis erros)
    
    size_t i = 0; // Contador do laço for
    int carry = 0; // Variável que armazenará os possíveis valores de carry, quando necessário

    for (; i < tamanhoMaximo; i++){
        // Operadores ternários: verifica se o contador i é menor que o tamanho do vetor (evitar acessar locais inválidos/indesejados)
        // Se verdadeiro: retorna o número da posição [i] - '0' (manipulação de caracteres) -> 0 em ASCII equivale à 48, quando fazemos por exemplo:
        // '5' - '0' seria igual a realizar: 53 - 48, que resultaria em um valor inteiro = 5
        // Se falso, retorna 0, e as demais operações são feitas somando os possiveis números restantes de um dos vetores + 0
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0': 0; 
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0': 0;

        int soma = primeiroDigito + segundoDigito + carry; // Soma os dois digitos + carry (se existir)

        carry = soma/10; // Carry vai ser o resultado da soma / 10 (isso levará somente o valor da dezena)
        
        resultado->numero[i] = (char)(soma % 10 + '0'); // Salva o valor do resto da divisão (ficando apenas com a casa da unidade) entre soma % 10 + '0' (novamente o '0' para manipulação de caracteres)
    }

    if(carry > 0){ // Se o carry for maior que 0 ao final do laço, adiciona ele ao vetor
        resultado->numero[i] = (char)(carry+'0');
        i++; // Incrementa o valor de i para armazenar o caractere nulo
    }

    resultado->numero[i] = '\0'; // Adiciona o caractere nulo ao final do vetor
    resultado->tamanho = strlen(resultado->numero); // Atualiza o tamanho do vetor

    invertNumber(resultado); // Inverte o número para ficar legivel ao usuário
    removeZeros(resultado, 0); // Remove possiveis zeros (à esquerda)

    return resultado;
}

//Função de subtração
bigNumber* subtract(bigNumber* primeiroNumero, bigNumber* segundoNumero) {
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero); // Atribui à variável tamanhoMaximo o maior tamanho (em dígitos) entre os dois números
    bigNumber *resultado = malloc(sizeof(bigNumber)); // Aloca memória do tipo bigNumber para o resultado
    resultado->numero = malloc((tamanhoMaximo + 2) * sizeof(char)); // Aloca para o vetor de números o tamanho máximo + 2 (possivel carry e caracter nulo) * o tamanho de char
    verificaAlocacao(resultado); // Verifica alocação do resultado
    resultado->isNegative = 0; // Atribui 0 ao sinal do resultado (para facilitar futuras operações e evitar possiveis erros)
    
    size_t i = 0; // Contador do laço for
    int emprestimo = 0; // Variável que armazenará possivel empréstimo para a subtração

    for (; i < tamanhoMaximo; i++) {
        // Operadores ternários: verifica se o contador i é menor que o tamanho do vetor (evitar acessar locais inválidos/indesejados)
        // Se verdadeiro: retorna o número da posição [i] - '0' (manipulação de caracteres) -> 0 em ASCII equivale à 48, quando fazemos por exemplo:
        // '5' - '0' seria igual a realizar: 53 - 48, que resultaria em um valor inteiro = 5
        // Se falso, retorna 0, e as demais operações são feitas somando os possiveis números restantes de um dos vetores + 0
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0' : 0;
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0' : 0;

        int subtracao = primeiroDigito - segundoDigito - emprestimo; // subtrai os dois digitos e depois subtrai o valor de empréstimo

        if (subtracao < 0) { // Verifica se é necessário um empréstimo para realizar a subtração
            subtracao += 10; // Soma 10 para compensar a subtração negativa
            emprestimo = 1; // Define empréstimo como 1 para a próxima iteração
        } else { // Caso contrário, define empréstimo como 0
            emprestimo = 0;
        }

        resultado->numero[i] = (char)(subtracao + '0'); // Salva o valor do resultado da subtração + '0' (novamente o '0' para manipulação de caracteres)
    }

    resultado->numero[i] = '\0'; // Adiciona o caractere nulo ao final do vetor
    resultado->tamanho = strlen(resultado->numero); // Atualiza o tamanho do vetor

    invertNumber(resultado); // Inverte o número para ficar legivel ao usuário
    removeZeros(resultado, 0); // Remove possiveis zeros (à esquerda)

    return resultado;
}

// Função de multiplicação
bigNumber* multiply(bigNumber* primeiroNumero, bigNumber* segundoNumero) {

    size_t tamanhoMaximo = primeiroNumero->tamanho + segundoNumero->tamanho; // Tamanho máximo do vetor resultado será a soma do tamanho dos dois números
    bigNumber* resultado = malloc(sizeof(bigNumber)); // Aloca memória do tipo bigNumber para o resultado
    resultado->numero = malloc((tamanhoMaximo + 2) * sizeof(char)); // Aloca para o vetor de números o tamanho máximo + 2 (possivel carry e caracter nulo) * o tamanho de char
    verificaAlocacao(resultado); // Verifica alocação do resultado
    resultado->isNegative = 0; // Atribui 0 ao sinal do resultado (para facilitar futuras operações e evitar possiveis erros)

    size_t i, j;

    // Inicializa o resultado com zeros, não funcionou usar direto do MALLOC e também não funcionou utilizar a função CALLOC, que inicia todas as posições em 0
    for (i = 0; i <= tamanhoMaximo + 1; i++) {
        resultado->numero[i] = '0';
    }
    
    // Laço percorrido como uma matriz, para cada digito do primeiro numero, é percorrido completamente o segundo numero realizando as operações
    for (i = 0; i < primeiroNumero->tamanho; i++) {
        int carry = 0;
        for (j = 0; j < segundoNumero->tamanho; j++) {
            int primeiroDigito = (primeiroNumero->numero[i] - '0'); // Converte o char do primeiro digito da posição [i] em inteiro
            int segundoDigito = (segundoNumero->numero[j] - '0'); // Converte o char do segundo digito da posição [j] em inteiro
            int resultadoDigito = (resultado->numero[i + j] - '0'); // Converte o char do digito do resultado na posição [i+j] em inteiro

            int produto = primeiroDigito * segundoDigito + carry + resultadoDigito; // Soma os digitos com o carry
            carry = produto / 10; // Carry vai ser o resultado da soma / 10 (isso levará somente o valor da dezena)
            resultado->numero[i + j] = (char)(produto % 10 + '0'); // Salva o valor do resto da divisão (ficando apenas com a casa da unidade)(novamente o '0' para manipulação de caracteres)
        }

        while (carry > 0) { // Enquanto o carry for maior que 0 ao final do laço, adiciona ele ao vetor
            int soma = carry + (resultado->numero[i + j] - '0');
            carry = soma / 10;
            resultado->numero[i + j] = (char)(soma % 10 + '0');
            j++;
        }
    }

    resultado->tamanho = tamanhoMaximo; // Atualiza o tamanho do resultado
    removeZeros(resultado, 1); // Remove os zeros A DIREITA ANTES DA INVERSÃO, POR ALGUM NAO FUNCIONOU REMOVER OS ZEROS A ESQUERDA APÓS A INVERSAO
    invertNumber(resultado); // Inverte o número

    return resultado;
}

// Função que verifica a operação solicitada pelo usuário e manipula de acordo com os valores de entrada
// Lembrando que: TODAS OPERAÇÕES SÃO REALIZADAS COM OS VALORES ABSOLUTOS (MÓDULO) DOS NÚMEROS
bigNumber* verificaOperacao(bigNumber* primeiroNumero, bigNumber* segundoNumero) {
    char operacao; // Variável da operação solicitada
    scanf("%c", &operacao); // Entrada da operação
    int compara = compararNumeros(primeiroNumero, segundoNumero); // Compara os dois números para descobrir qual dos dois tem valor ABSOLUTO MAIOR
    invertNumber(primeiroNumero);
    invertNumber(segundoNumero);
    bigNumber *resultado = NULL; // Cria o vetor resultado
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // SE A OPERAÇÃO FOR SOMA
    if (operacao == '+') {
// -------------------------------------------------------------------------------------------------------------- //
        // PRIMEIRO NUMERO NEGATIVO -> -A + B
        if (primeiroNumero->isNegative && !segundoNumero->isNegative) {
            // A > B -> -( A - B )
            if (compara >= 0) {
                resultado = subtract(primeiroNumero, segundoNumero); // Troca a operação por subtração
                resultado->isNegative = 1; // Atribui o resultado como negativo
            } else { // B > A -> (B - A)
                resultado = subtract(segundoNumero, primeiroNumero); // Troca a posição dos números e realiza subtração
            }
        }
// -------------------------------------------------------------------------------------------------------------- //
        // SEGUNDO NUMERO NEGATIVO -> A + (-B)
        else if (!primeiroNumero->isNegative && segundoNumero->isNegative) {
            // A > B -> A - B
            if (compara >= 0) {
                resultado = subtract(primeiroNumero, segundoNumero); // Troca a operação por subtração
            } else { // B > A -> -(B - A)
                resultado = subtract(segundoNumero, primeiroNumero); // Troca a posição dos números e realiza subtração
                resultado->isNegative = 1; // Atribui o resultado como negativo
            }
         }
// -------------------------------------------------------------------------------------------------------------- //
        // AMBOS OS NÚMEROS NEGATIVOS -> -A + (-B) = -A - B = -(A + B)
        else if (primeiroNumero->isNegative && segundoNumero->isNegative) {
            resultado = sum(primeiroNumero, segundoNumero); // Soma os dois números
            resultado->isNegative = 1; // Atribui o resultado como negativo
        }
// -------------------------------------------------------------------------------------------------------------- //
        // AMBOS OS NÚMEROS POSITIVOS
        else {
            resultado = sum(primeiroNumero, segundoNumero);
        }
// -------------------------------------------------------------------------------------------------------------- //
    }
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // SE A OPERAÇÃO FOR SUBTRAÇÃO
    else if (operacao == '-') {
        // Bloco condicional necessária para a subtração, inicializando todos os campos do resultado (não funcionava sem inicializar, valgrind acusava erro)
        if (resultado != NULL) {
            resultado->numero = NULL;
            resultado->tamanho = 0;
        }
// -------------------------------------------------------------------------------------------------------------- //
        // PRIMEIRO NUMERO NEGATIVO -> (-A) - B = - (A + B)
        if (primeiroNumero->isNegative && !segundoNumero->isNegative) {
            resultado = sum(primeiroNumero, segundoNumero); // Troca a operação por soma
            resultado->isNegative = 1; // Atribui o resultado como negativo
        }
// -------------------------------------------------------------------------------------------------------------- //
        // SEGUNDO NUMERO NEGATIVO -> A - (-B) = A + B
        else if (!primeiroNumero->isNegative && segundoNumero->isNegative) {
            resultado = sum(primeiroNumero, segundoNumero); // Troca a operação por soma
        }
// -------------------------------------------------------------------------------------------------------------- //
        // AMBOS OS NÚMEROS NEGATIVOS -> -A - (-B) = -A + B
        else if (primeiroNumero->isNegative && segundoNumero->isNegative) {
            // A > B -> - (A - B)
            if (compara >= 0) {
                resultado = subtract(primeiroNumero, segundoNumero); // Subtrai os dois números
                resultado->isNegative = 1; // Atribui o resultado como negativo
            } 
            // B > A -> B - A
            else {
                resultado = subtract(segundoNumero, primeiroNumero); // Troca as posições na subtração
            }
        } 
// -------------------------------------------------------------------------------------------------------------- //
        // AMBOS OS NÚMEROS POSITIVOS -> A - B
        else {
            // A > B -> A - B
            if (compara >= 0) {
                resultado = subtract(primeiroNumero, segundoNumero); // Subtrai os dois números
            } 
            // B > A -> - (B - A)
            else {
                resultado = subtract(segundoNumero, primeiroNumero); // Troca as posições na subtração
                resultado->isNegative = 1; // Atribui o resultado como negativo
            }
        }
// -------------------------------------------------------------------------------------------------------------- //
    }
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // SE A OPERAÇÃO FOR MULTIPLICAÇÃO
    else if (operacao == '*') {
// -------------------------------------------------------------------------------------------------------------- //
        // SE UM DOS NÚMEROS FOR 0
        // Faz a comparação para ver se um dos números é zero, se for, retornará 0 que posteriormente é comparado à 0, retornando verdadeiro (1)
        if (strcmp(primeiroNumero->numero, "0") == 0 || strcmp(segundoNumero->numero, "0") == 0) { 
            resultado = malloc(sizeof(bigNumber)); // Aloca memória para o resultado
            resultado->numero = malloc(2 * sizeof(char)); // Aloca apenas 2 espaços, para armazenar '0' e '\0'
            resultado->numero[0] = '0'; // Atribui a posição 0 o valor '0'
            resultado->numero[1] = '\0'; // Atribui a posição 1 o caractere nulo
            resultado->tamanho = 1;
            resultado->isNegative = 0;
            return resultado;
        }
// -------------------------------------------------------------------------------------------------------------- //
        // QUANDO NENHUM DOS NUMEROS É ZERO
        resultado = multiply(primeiroNumero, segundoNumero);
        if ((primeiroNumero->isNegative && !segundoNumero->isNegative) || (!primeiroNumero->isNegative && segundoNumero->isNegative)) {
            resultado->isNegative = 1;
        }
// -------------------------------------------------------------------------------------------------------------- //
    }
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= //
    // Se o resutlado for diferente de NULL, e o resultado for NEGATIVO
    if (resultado != NULL) {
        if (resultado->isNegative) {
            resultadoNegativo(resultado); // Chama a função resultadoNegativo para adicionar o sinal de negativo na frente do resultado
        }
    }
    return resultado;
}

