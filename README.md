# Projeto-PE
<p>Código desenvolvido por Thiago Perellon Romano</p>
<p>RA: 11202130849</p>
    O bigNumber desenvolvido utiliza uma struct denominada bigNumber com 3 campos. O primeiro é um vetor de caracteres, feito para armazenar os dígitos do número digitado. O segundo armazena o tamanho do vetor, para possíveis operações que necessitem do tamanho, a fim de evitar o uso excessivo da função strlen(). O terceiro e último campo armazena, em um tipo inteiro, 1 caso o resultado seja negativo e 0 caso seja positivo. 
    A leitura de cada dígito é feita utilizando a função getchar, que capta todo caractere da entrada do usuário e salva no vetor de caracteres da estrutura bigNumber e, a cada iteração, aumenta a memória alocada para o vetor se necessário. Posteriormente, é chamada uma função que verifica se o primeiro caractere um o sinal de negativo, salva o resultado na variável que representa o valor negativo (isNegative), remove o sinal e move todo o bloco de memória para a esquerda, ocupando da posição 0 em diante do vetor. Após isso, o número é enviado para uma função que vai inverter a posição dos dígitos para facilitar operações posteriores envolvendo soma, subtração e multiplicação.
A interface pública foi simplificada em apenas 1 estrutura de dados e 3 funções principais:

typedef struct{ 
    char *numero; //Salva os digitos do número digitado
    size_t tamanho; //Armazena o tamanho do vetor de números
    int isNegative; //Salva 1 se for negativo e 0 se for positivo
} bigNumber; //Nome da estrutura

bigNumber* readNumber();
bigNumber* verificaOperacao(bigNumber* primeiroNumero, bigNumber* segundoNumero);
void liberaMemoria(bigNumber* numero);

