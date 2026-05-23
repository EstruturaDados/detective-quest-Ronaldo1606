#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura da Árvore Binária  */

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* Cria uma nova sala (nó) */

Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;

    return novaSala;
}

/* Exploração interativa da mansão */

void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        /* Verifica se é um nó-folha */
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos a seguir. Exploração encerrada.\n");
            return;
        }

        printf("Escolha o caminho:\n");
        if (atual->esquerda != NULL)
            printf("e - Ir para a esquerda\n");
        if (atual->direita != NULL)
            printf("d - Ir para a direita\n");

        printf("s - Sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (opcao == 's') {
            printf("Você saiu da exploração.\n");
            return;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/* Função principal  */

int main() {
    /* Criação automática do mapa da mansão */
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *escritorio = criarSala("Escritório");

    /* Montagem da árvore binária */
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;

    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;

    biblioteca->esquerda = escritorio;

    /* Início da exploração */
    printf("Bem-vindo ao jogo Detective Quest!\n");
    explorarSalas(hall);

    return 0;
}