#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ESTRUTURA DA SALA (ÁRVORE BINÁRIA DA MANSÃO) */

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ESTRUTURA DA BST DE PISTAS  */

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/*  ESTRUTURA DA TABELA HASH (PISTA -> SUSPEITO)  */

#define TAM_HASH 10

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

HashNode *tabelaHash[TAM_HASH];

/* FUNÇÃO: criarSala
   Cria dinamicamente um cômodo da mansão  */

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (!nova) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

/* FUNÇÃO: inserirPista
   Insere pista na árvore BST de forma ordenada  */

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (!raiz) {
        PistaNode *novo = malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/* FUNÇÃO HASH  */

int funcaoHash(const char *pista) {
    int soma = 0;
    for (int i = 0; pista[i] != '\0'; i++)
        soma += pista[i];
    return soma % TAM_HASH;
}

/* FUNÇÃO: inserirNaHash
   Associa pista a suspeito na tabela hash  */

void inserirNaHash(const char *pista, const char *suspeito) {
    int idx = funcaoHash(pista);

    HashNode *novo = malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

/* FUNÇÃO: encontrarSuspeito
   Retorna o suspeito associado a uma pista  */

char* encontrarSuspeito(const char *pista) {
    int idx = funcaoHash(pista);
    HashNode *atual = tabelaHash[idx];

    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

/* FUNÇÃO: explorarSalas
   Navega pela mansão e coleta pistas */

void explorarSalas(Sala *atual, PistaNode **pistas) {
    char opcao;

    while (atual) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf(" Pista encontrada: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
            atual->pista[0] = '\0';
        }

        printf("\nOpções:\n");
        if (atual->esquerda) printf("e - Esquerda\n");
        if (atual->direita) printf("d - Direita\n");
        printf("s - Sair\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita)
            atual = atual->direita;
        else if (opcao == 's')
            return;
        else
            printf("Opção inválida!\n");
    }
}

/* FUNÇÃO: exibirPistas
   Exibe pistas coletadas em ordem alfabética  */

void exibirPistas(PistaNode *raiz) {
    if (raiz) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* FUNÇÃO AUXILIAR: conta pistas de um suspeito  */

int contarPistasSuspeito(PistaNode *raiz, const char *suspeito) {
    if (!raiz) return 0;

    int total = contarPistasSuspeito(raiz->esquerda, suspeito);

    char *s = encontrarSuspeito(raiz->pista);
    if (s && strcmp(s, suspeito) == 0)
        total++;

    return total + contarPistasSuspeito(raiz->direita, suspeito);
}

/* FUNÇÃO: verificarSuspeitoFinal
   Realiza o julgamento final  */

void verificarSuspeitoFinal(PistaNode *pistas) {
    char acusado[50];

    printf("\nQuem você acusa como culpado? ");
    scanf(" %[^\n]", acusado);

    int qtd = contarPistasSuspeito(pistas, acusado);

    if (qtd >= 2)
        printf("\n Acusação correta! %s é o culpado.\n", acusado);
    else
        printf("\n Acusação rejeitada. Provas insuficientes.\n");
}

/* FUNÇÃO PRINCIPAL  */

int main() {
    /* Inicializa hash */
    for (int i = 0; i < TAM_HASH; i++)
        tabelaHash[i] = NULL;

    /* Mapa fixo da mansão */
    Sala *hall = criarSala("Hall", "Pegadas sujas no chão");
    Sala *sala = criarSala("Sala de Estar", "Relógio quebrado às 22h");
    Sala *biblioteca = criarSala("Biblioteca", NULL);
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas suspeitas");
    Sala *jardim = criarSala("Jardim", "Terra revirada recentemente");
    Sala *escritorio = criarSala("Escritório", "Carta rasgada na gaveta");

    hall->esquerda = sala;
    hall->direita = biblioteca;
    sala->esquerda = cozinha;
    sala->direita = jardim;
    biblioteca->esquerda = escritorio;

    /* Associa pistas a suspeitos */

    inserirNaHash("Pegadas sujas no chão", "Sr. Black");
    inserirNaHash("Relógio quebrado às 22h", "Sr. Black");
    inserirNaHash("Faca com manchas suspeitas", "Sra. White");
    inserirNaHash("Terra revirada recentemente", "Sr. Green");
    inserirNaHash("Carta rasgada na gaveta", "Sr. Black");

    PistaNode *pistasColetadas = NULL;

    printf(" Bem-vindo ao Detective Quest!\n");
    explorarSalas(hall, &pistasColetadas);

    printf("\n Pistas coletadas:\n");
    if (!pistasColetadas)
        printf("Nenhuma pista encontrada.\n");
    else
        exibirPistas(pistasColetadas);

    verificarSuspeitoFinal(pistasColetadas);

    return 0;
}