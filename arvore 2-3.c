#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct No23 {
    int chave1, chave2;
    int nChaves;
    struct No23 *esq, *meio, *dir;
} No23;

/* CRIAÇÃO */

No23* criarNo(int chave) {
    No23* no = (No23*) malloc(sizeof(No23));
    no->chave1 = chave;
    no->nChaves = 1;
    no->esq = no->meio = no->dir = NULL;
    return no;
}

/* BUSCA */

bool buscar(No23* raiz, int valor) {
    if (!raiz) return false;

    if (valor == raiz->chave1) return true;
    if (raiz->nChaves == 2 && valor == raiz->chave2) return true;

    if (valor < raiz->chave1)
        return buscar(raiz->esq, valor);
    else if (raiz->nChaves == 1 || valor < raiz->chave2)
        return buscar(raiz->meio, valor);
    else
        return buscar(raiz->dir, valor);
}

/* SPLIT */

No23* split(No23* no, int valor, No23* filho, int* promovida) {
    No23* novo = criarNo(0);

    if (valor < no->chave1) {
        *promovida = no->chave1;
        novo->chave1 = no->chave2;
        novo->nChaves = 1;

        no->chave1 = valor;
        no->nChaves = 1;

        novo->esq = no->meio;
        novo->meio = no->dir;
        no->meio = filho;
        no->dir = NULL;
    }
    else if (no->nChaves == 1 || valor < no->chave2) {
        *promovida = valor;

        novo->chave1 = no->chave2;
        novo->nChaves = 1;

        novo->esq = filho;
        novo->meio = no->dir;

        no->nChaves = 1;
        no->dir = NULL;
    }
    else {
        *promovida = no->chave2;

        novo->chave1 = valor;
        novo->nChaves = 1;

        novo->esq = no->dir;
        novo->meio = filho;

        no->nChaves = 1;
        no->dir = NULL;
    }

    return novo;
}

/* INSERÇÃO */

No23* inserirRec(No23* raiz, int valor, int* promovida, bool* cresceu) {
    if (!raiz) {
        *cresceu = true;
        *promovida = valor;
        return NULL;
    }

    No23* novoFilho = NULL;
    int promoFilho;
    bool filhoCresceu = false;

    if (!raiz->esq) {
        if (raiz->nChaves == 1) {
            if (valor < raiz->chave1) {
                raiz->chave2 = raiz->chave1;
                raiz->chave1 = valor;
            } else {
                raiz->chave2 = valor;
            }
            raiz->nChaves = 2;
            *cresceu = false;
            return raiz;
        }
        else {
            *cresceu = true;
            return split(raiz, valor, NULL, promovida);
        }
    }

    if (valor < raiz->chave1)
        novoFilho = inserirRec(raiz->esq, valor, &promoFilho, &filhoCresceu);
    else if (raiz->nChaves == 1 || valor < raiz->chave2)
        novoFilho = inserirRec(raiz->meio, valor, &promoFilho, &filhoCresceu);
    else
        novoFilho = inserirRec(raiz->dir, valor, &promoFilho, &filhoCresceu);

    if (!filhoCresceu) {
        *cresceu = false;
        return raiz;
    }

    if (raiz->nChaves == 1) {
        if (promoFilho < raiz->chave1) {
            raiz->chave2 = raiz->chave1;
            raiz->chave1 = promoFilho;
            raiz->dir = raiz->meio;
            raiz->meio = novoFilho;
        } else {
            raiz->chave2 = promoFilho;
            raiz->dir = novoFilho;
        }
        raiz->nChaves = 2;
        *cresceu = false;
        return raiz;
    }

    *cresceu = true;
    return split(raiz, promoFilho, novoFilho, promovida);
}

No23* inserir(No23* raiz, int valor) {
    int promovida;
    bool cresceu;
    No23* novo = inserirRec(raiz, valor, &promovida, &cresceu);

    if (cresceu) {
        No23* novaRaiz = criarNo(promovida);
        novaRaiz->esq = raiz;
        novaRaiz->meio = novo;
        return novaRaiz;
    }
    return raiz;
}

/* IMPRESSÃO */

void emOrdem(No23* raiz) {
    if (!raiz) return;

    emOrdem(raiz->esq);
    printf("%d ", raiz->chave1);
    emOrdem(raiz->meio);

    if (raiz->nChaves == 2) {
        printf("%d ", raiz->chave2);
        emOrdem(raiz->dir);
    }
}

void imprimirNivel(No23* raiz, int nivel) {
    if (!raiz) return;

    if (nivel == 0) {
        if (raiz->nChaves == 1)
            printf("[ %d ] ", raiz->chave1);
        else
            printf("[ %d , %d ] ", raiz->chave1, raiz->chave2);
    }
    else {
        imprimirNivel(raiz->esq, nivel - 1);
        imprimirNivel(raiz->meio, nivel - 1);
        if (raiz->nChaves == 2)
            imprimirNivel(raiz->dir, nivel - 1);
    }
}

int altura(No23* raiz) {
    if (!raiz) return -1;
    return 1 + altura(raiz->esq);
}

void imprimirPorNivel(No23* raiz) {
    int h = altura(raiz);
    for (int i = 0; i <= h; i++) {
        printf("Nivel %d: ", i);
        imprimirNivel(raiz, i);
        printf("\n");
    }
}

/* MENU */

int main() {
    No23* raiz = NULL;
    int opcao, valor;

    do {
        printf("\n=== ARVORE 2-3 ===\n");
        printf("1 - Inserir\n");
        printf("2 - Buscar\n");
        printf("3 - Imprimir em Ordem\n");
        printf("4 - Imprimir por Nivel\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;

            case 2:
                printf("Valor: ");
                scanf("%d", &valor);
                printf(buscar(raiz, valor) ? "Encontrado!\n" : "Nao encontrado!\n");
                break;

            case 3:
                emOrdem(raiz);
                printf("\n");
                break;

            case 4:
                imprimirPorNivel(raiz);
                break;
        }
    } while (opcao != 0);

    return 0;
}
