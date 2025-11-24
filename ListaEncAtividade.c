#include <stdio.h>
#include <stdlib.h>

// Estrutura do Nó
typedef struct No {
    int valor;
    struct No *ant;
    struct No *prox;
} No;

// Inserir no Início
void inserirInicio(No **inicio, int valor) {
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->ant = NULL;
    novo->prox = *inicio;

    if (*inicio != NULL)
        (*inicio)->ant = novo;

    *inicio = novo;
}

// Inserir no Final
void inserirFinal(No **inicio, int valor) {
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = NULL;

    if (*inicio == NULL) {
        novo->ant = NULL;
        *inicio = novo;
        return;
    }

    No *aux = *inicio;
    while (aux->prox != NULL)
        aux = aux->prox;

    aux->prox = novo;
    novo->ant = aux;
}

// Inserir em Posição Específica
void inserirPosicao(No **inicio, int valor, int pos) {
    if (pos <= 1) {
        inserirInicio(inicio, valor);
        return;
    }

    No *aux = *inicio;
    int cont = 1;

    while (aux != NULL && cont < pos - 1) {
        aux = aux->prox;
        cont++;
    }

    if (aux == NULL) {
        inserirFinal(inicio, valor);
        return;
    }

    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = aux->prox;
    novo->ant = aux;

    if (aux->prox != NULL)
        aux->prox->ant = novo;

    aux->prox = novo;
}

// Remover por posição
void removerPosicao(No **inicio, int pos) {
    if (*inicio == NULL || pos < 1) {
        printf("Lista vazia ou posição inválida.\n");
        return;
    }

    No *aux = *inicio;

    if (pos == 1) {
        *inicio = aux->prox;
        if (*inicio != NULL)
            (*inicio)->ant = NULL;
        free(aux);
        return;
    }

    int cont = 1;
    while (aux != NULL && cont < pos) {
        aux = aux->prox;
        cont++;
    }

    if (aux == NULL) {
        printf("Posição inexistente.\n");
        return;
    }

    if (aux->ant != NULL)
        aux->ant->prox = aux->prox;
    if (aux->prox != NULL)
        aux->prox->ant = aux->ant;

    free(aux);
}

// Buscar valor
int buscar(No *inicio, int valor) {
    int pos = 1;
    while (inicio != NULL) {
        if (inicio->valor == valor)
            return pos;
        inicio = inicio->prox;
        pos++;
    }
    return -1;  // não encontrado
}

// Listar elementos
void listar(No *inicio) {
    if (inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    No *aux = inicio;
    printf("\n=== LISTA ===\n");
    while (aux != NULL) {
        printf("[ANT: %p] <- (%d) -> [PROX: %p]\n", (void*)aux->ant, aux->valor, (void*)aux->prox);
        aux = aux->prox;
    }
    printf("====================\n");
}

// Menu Principal
int main() {
    No *inicio = NULL;
    int op, valor, pos;

    do {
        printf("\n--- MENU ---\n");
        printf("1 - Inserir no início\n");
        printf("2 - Inserir no final\n");
        printf("3 - Inserir em uma posição\n");
        printf("4 - Remover posição\n");
        printf("5 - Buscar valor\n");
        printf("6 - Listar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
        case 1:
            printf("Valor: ");
            scanf("%d", &valor);
            inserirInicio(&inicio, valor);
            break;

        case 2:
            printf("Valor: ");
            scanf("%d", &valor);
            inserirFinal(&inicio, valor);
            break;

        case 3:
            printf("Valor: ");
            scanf("%d", &valor);
            printf("Posição: ");
            scanf("%d", &pos);
            inserirPosicao(&inicio, valor, pos);
            break;

        case 4:
            printf("Posição para remover: ");
            scanf("%d", &pos);
            removerPosicao(&inicio, pos);
            break;

        case 5:
            printf("Valor para buscar: ");
            scanf("%d", &valor);
            pos = buscar(inicio, valor);
            if (pos == -1)
                printf("Valor não encontrado.\n");
            else
                printf("Encontrado na posição %d\n", pos);
            break;

        case 6:
            listar(inicio);
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida.\n");
        }
    } while (op != 0);

    return 0;
}