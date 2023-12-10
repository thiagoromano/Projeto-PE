#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define CAPACIDADE_INICIAL 10 // Define um tamanho inicial para o vetor que armazenará os números

// Estrutura do bigNumber utilizada no programa
typedef struct{ 
    char *numero; //Salva os digitos do número digitado
    size_t tamanho; //Armazena o tamanho do vetor de números
    int isNegative; //Salva 1 se for negativo e 0 se for positivo
} bigNumber; //Nome da estrutura

// Verifica a alocação de memória, se der erro, encerra o programa
void verificaAlocacao(bigNumber *numero){
    if (numero == NULL){
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

// Compara os números a fim de descobrir qual o maior em valor ABSOLUTO!
int compararNumeros(bigNumber *numero1, bigNumber *numero2){
    if (numero1->tamanho != numero2->tamanho){ // Se os tamanhos forem diferentes
        return (numero1->tamanho < numero2->tamanho) ? -1 : 1; // Operador ternário: retorna -1 se verdadeiro (numero2 > numero1) e 1 se falso
    }
    // Tamanhos iguais, comparar dígito a dígito
    for (size_t i = 0; i < numero1->tamanho; i++){
        if (numero1->numero[i] != numero2->numero[i]){ // Compara dígito a dígito até encontrar um diferente e compara eles
            return (numero1->numero[i] < numero2->numero[i]) ? -1 : 1; // Operador ternário: retorna -1 se verdadeiro (numero2[i] > numero1[i]) e 1 se falso
        }
    }
    // Números são iguais
    return 0;
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

// Função que lê os números digitados pelo usuario
bigNumber *readNumber(){

    size_t capacidade = CAPACIDADE_INICIAL;
    bigNumber *numero = malloc(sizeof(bigNumber)); // Aloca para o vetor número espaços de memória do tipo char para a quantidade definida pela variavel capacidade
    numero->numero = malloc(capacidade * sizeof(char));
    verificaAlocacao(numero);
    numero->tamanho = 0; // define o valor de tamanho para 0;

    int c; // cria um inteiro c

    while ((c = getchar()) != '\n'){ // Lê os dados do usuário utilizando getchar() para pegar digito a digito enquanto a entrada for diferente de '\n'
        if (feof(stdin) || ferror(stdin)){
            liberaMemoria(numero);
            return NULL;
        }
        // Bloco condicional para aumentar, se necessário, a memória alocada no vetor
        if (numero->tamanho == capacidade){                                      // Se o tamanho do vetor for igual a capacidade
            capacidade += capacidade / 4;                                        // Aumente a capacidade do vetor em 25%
            numero->numero = realloc(numero->numero, capacidade * sizeof(char)); // Comando para realocar a memória do vetor para a nova capacidade incrementada
            verificaAlocacao(numero);
        }
        numero->numero[(numero->tamanho)++] = (char)c;
    }

    if (numero->tamanho > 0){
        numero->numero[(numero->tamanho)] = '\0';
        (numero->tamanho)--;
    }

    numero->tamanho = strlen(numero->numero);
    verificaAlocacao(numero);

    verificaSinais(numero);
    removeZeros(numero, 0);
    invertNumber(numero);

    return numero;
}

void resultadoNegativo(bigNumber *resultado)
{
    size_t tamanho = strlen(resultado->numero);
    char *temp = malloc((tamanho + 2) * sizeof(char)); // Adicionado +2 para acomodar o sinal negativo e o caractere nulo '\0'
    if (temp != NULL){
        temp[0] = '-';
        strcpy(temp + 1, resultado->numero);
        free(resultado->numero); // Liberar a memória antiga
        resultado->numero = temp;
        resultado->tamanho = tamanho + 1;
    }
}

// Função de soma
bigNumber *sum(bigNumber *primeiroNumero, bigNumber *segundoNumero)
{
    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero);
    bigNumber *resultado = malloc(sizeof(bigNumber));
    resultado->numero = malloc((tamanhoMaximo + 2) * sizeof(char));
    verificaAlocacao(resultado);
    resultado->isNegative = 0;

    size_t i = 0;
    int carry = 0;

    for (; i < tamanhoMaximo; i++)
    {
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0' : 0;
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0' : 0;

        int soma = primeiroDigito + segundoDigito + carry;

        carry = soma / 10;

        resultado->numero[i] = (char)(soma % 10 + '0');
    }

    if (carry > 0)
    {
        resultado->numero[i] = (char)(carry + '0');
        i++;
    }

    resultado->numero[i] = '\0';
    resultado->tamanho = strlen(resultado->numero);

    invertNumber(resultado);
    removeZeros(resultado, 0);

    return resultado;
}

// Função de subtração
bigNumber *subtract(bigNumber *primeiroNumero, bigNumber *segundoNumero)
{

    size_t tamanhoMaximo = findBiggest(primeiroNumero, segundoNumero);
    bigNumber *resultado = malloc(sizeof(bigNumber));
    resultado->numero = malloc((tamanhoMaximo + 2) * sizeof(char));
    verificaAlocacao(resultado);
    resultado->isNegative = 0;

    int emprestimo = 0;
    size_t i = 0;

    for (; i < tamanhoMaximo; i++)
    {
        int primeiroDigito = (i < primeiroNumero->tamanho) ? primeiroNumero->numero[i] - '0' : 0;
        int segundoDigito = (i < segundoNumero->tamanho) ? segundoNumero->numero[i] - '0' : 0;

        int subtracao = primeiroDigito - segundoDigito - emprestimo;

        if (subtracao < 0)
        {
            subtracao += 10;
            emprestimo = 1;
        }
        else
        {
            emprestimo = 0;
        }

        resultado->numero[i] = (char)(subtracao + '0');
    }

    resultado->numero[i] = '\0';
    resultado->tamanho = strlen(resultado->numero);

    invertNumber(resultado);
    removeZeros(resultado, 0);

    return resultado;
}

bigNumber *multiply(bigNumber *primeiroNumero, bigNumber *segundoNumero)
{

    size_t tamanhoResultado = primeiroNumero->tamanho + segundoNumero->tamanho;
    bigNumber *resultado = malloc(sizeof(bigNumber));
    resultado->numero = malloc((tamanhoResultado + 2) * sizeof(char)); // +2 para o '\0' e possível carry
    verificaAlocacao(resultado);
    resultado->isNegative = 0;

    size_t i, j;

    // Inicializa o resultado com zeros NÃO FUNCIONOU COM CALLOC()
    for (i = 0; i <= tamanhoResultado + 1; i++)
    {
        resultado->numero[i] = '0';
    }

    for (i = 0; i < primeiroNumero->tamanho; i++)
    {
        int carry = 0;
        for (j = 0; j < segundoNumero->tamanho; j++)
        {
            int produto = (primeiroNumero->numero[i] - '0') * (segundoNumero->numero[j] - '0') + carry + (resultado->numero[i + j] - '0');
            carry = produto / 10;
            resultado->numero[i + j] = (char)(produto % 10 + '0');
        }

        while (carry > 0)
        {
            int soma = carry + (resultado->numero[i + j] - '0');
            carry = soma / 10;
            resultado->numero[i + j] = (char)(soma % 10 + '0');
            j++;
        }
    }

    resultado->tamanho = tamanhoResultado;
    removeZeros(resultado, 1); // removeZerosAEsquerda não funcionou na multiplicação, então removeZerosADireita remove os zeros antes da chamada da função invertNumber
    invertNumber(resultado);

    return resultado;
}

bigNumber *verificaOperacao(bigNumber *primeiroNumero, bigNumber *segundoNumero)
{
    char operacao;
    scanf("%c", &operacao);
    int compara = compararNumeros(primeiroNumero, segundoNumero);
    bigNumber *resultado = NULL;
    if (operacao == '+')
    {
        // PRIMEIRO NUMERO NEGATIVO
        if (primeiroNumero->isNegative && !segundoNumero->isNegative)
        {
            // A > B
            if (compara >= 0)
            {
                resultado = subtract(primeiroNumero, segundoNumero);
                resultado->isNegative = 1;
            }
            // B > A
            else
            {
                resultado = subtract(segundoNumero, primeiroNumero);
            }
            // SEGUNDO NUMERO NEGATIVO
        }
        else if (!primeiroNumero->isNegative && segundoNumero->isNegative)
        {
            // A > B
            if (compara >= 0)
            {
                resultado = subtract(primeiroNumero, segundoNumero);
            }
            // B > A
            else
            {
                resultado = subtract(segundoNumero, primeiroNumero);
                resultado->isNegative = 1;
            }
            // AMBOS OS NÚMEROS NEGATIVOS
        }
        else if (primeiroNumero->isNegative && segundoNumero->isNegative)
        {
            resultado = sum(primeiroNumero, segundoNumero);
            resultado->isNegative = 1;
            // AMBOS OS NÚMEROS POSITIVOS
        }
        else
        {
            resultado = sum(primeiroNumero, segundoNumero);
        }
    }
    else if (operacao == '-')
    {
        // Esse condicional foi necessário no condicional da subtração pois valgrind dizia estar tentando acessar variavel não inicializada
        if (resultado != NULL)
        {
            resultado->numero = NULL;
            resultado->tamanho = 0;
        }
        // PRIMEIRO NEGATIVO
        if (primeiroNumero->isNegative && !segundoNumero->isNegative)
        {
            resultado = sum(primeiroNumero, segundoNumero);
            resultado->isNegative = 1;
            // SEGUNDO NUMERO NEGATIVO
        }
        else if (!primeiroNumero->isNegative && segundoNumero->isNegative)
        {
            resultado = sum(primeiroNumero, segundoNumero);
            // AMBOS OS NÚMEROS NEGATIVOS
        }
        else if (primeiroNumero->isNegative && segundoNumero->isNegative)
        {
            // A > B
            if (compara >= 0)
            {
                resultado = subtract(primeiroNumero, segundoNumero);
                resultado->isNegative = 1;
            }
            // B > A
            else
            {
                resultado = subtract(segundoNumero, primeiroNumero);
            }
            // AMBOS OS NÚMEROS POSITIVOS
        }
        else
        {
            // A > B
            if (compara >= 0)
            {
                resultado = subtract(primeiroNumero, segundoNumero);
            }
            // B > A
            else
            {
                resultado = subtract(segundoNumero, primeiroNumero);
                resultado->isNegative = 1;
            }
        }
    }
    else
    {
        if (strcmp(primeiroNumero->numero, "0") == 0 || strcmp(segundoNumero->numero, "0") == 0)
        {
            liberaMemoria(resultado);
            resultado = malloc(sizeof(bigNumber));
            if (!resultado)
            {
                return NULL;
            }
            resultado->numero = malloc(2 * sizeof(char)); // Para armazenar '0' e '\0'
            if (!resultado->numero)
            {
                liberaMemoria(resultado);
                return NULL;
            }
            strcpy(resultado->numero, "0");
            resultado->tamanho = 1;
            resultado->isNegative = 0;
            return resultado;
        }
        resultado = multiply(primeiroNumero, segundoNumero);
        if ((primeiroNumero->isNegative && !segundoNumero->isNegative) || (!primeiroNumero->isNegative && segundoNumero->isNegative))
        {
            resultado->isNegative = 1;
        }
    }
    if (resultado != NULL)
    {
        if (resultado->isNegative)
        {
            resultadoNegativo(resultado);
        }
    }

    return resultado;
}

// Função principal para testar tudo, DEVE SER APAGADA para alterar para TAD

int main()
{
    bigNumber *primeiroNumero, *segundoNumero;

    while ((primeiroNumero = readNumber()) != NULL)
    {
        // Verifica se a leitura foi bem-sucedida
        if (primeiroNumero != NULL)
        {
            segundoNumero = readNumber();

            if (segundoNumero == NULL)
            {
                liberaMemoria(primeiroNumero);
                break;
            }
            if (primeiroNumero == NULL || segundoNumero == NULL)
            {
                break;
            }

            bigNumber *resultado = verificaOperacao(primeiroNumero, segundoNumero);

            printf("%s\n", resultado->numero);

            liberaMemoria(primeiroNumero);
            liberaMemoria(segundoNumero);
            liberaMemoria(resultado);

            // Consumir caractere de nova linha no buffer
            getchar();
        }
        else
        {
            // Se a leitura falhar, saia do loop
            break;
        }
    }
    return 0;
}