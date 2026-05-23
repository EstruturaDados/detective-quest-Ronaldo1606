#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura da Sala (Árvore Binária) */

typedef struct Sala {
    char nome[50];
    char pista[100];           // pista opcional
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Estrutura da Árvore BST de Pistas */

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/*Cria uma sala com ou sem pista */

Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (nova == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        nova->pista[0] = '\0';

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

/* Insere pista na BST */

PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
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

/* Exploração da mansão com coleta de pistas  */

void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf(" Pista encontrada: %s\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
            atual->pista[0] = '\0'; // evita coletar novamente
        }

        printf("Opções:\n");
        if (atual->esquerda) printf("e - Ir para esquerda\n");
        if (atual->direita) printf("d - Ir para direita\n");
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

/* Exibe pistas em ordem alfabética  */

void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* Função principal  */

int main() {
    /* Mapa fixo da mansão */
    Sala *hall = criarSala("Hall de Entrada", "Pegadas sujas no chão");
    Sala *salaEstar = criarSala("Sala de Estar", "Relógio quebrado às 22h");
    Sala *biblioteca = criarSala("Biblioteca", NULL);
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas suspeitas");
    Sala *jardim = criarSala("Jardim", "Terra revirada recentemente");
    Sala *escritorio = criarSala("Escritório", "Carta rasgada na gaveta");

    /* Montagem da árvore */

    hall->esquerda = salaEstar;
    hall->direita = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    biblioteca->esquerda = escritorio;

    /* Árvore BST de pistas */

    PistaNode *pistasColetadas = NULL;

    printf(" Bem-vindo ao Detective Quest!\n");
    explorarSalasComPistas(hall, &pistasColetadas);

    printf("\n Pistas coletadas (ordem alfabética):\n");
    if (pistasColetadas == NULL)
        printf("Nenhuma pista encontrada.\n");
    else
        exibirPistas(pistasColetadas);

    return 0;
}