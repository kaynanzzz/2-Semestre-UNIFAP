#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct NoAVL {
    int chave;
    int altura;
    struct NoAVL *esq, *dir;
} NoAVL;

/* ==================== FUNÇÕES AUXILIARES ==================== */

int altura(NoAVL *no) {
    if (no == NULL)
        return -1;
    return no->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int fatorBalanceamento(NoAVL *no) {
    if (no == NULL)
        return 0;
    return altura(no->esq) - altura(no->dir);
}

NoAVL* criarNo(int chave) {
    NoAVL* novo = (NoAVL*) malloc(sizeof(NoAVL));
    novo->chave = chave;
    novo->altura = 0;
    novo->esq = novo->dir = NULL;
    return novo;
}

/* ==================== ROTAÇÕES ==================== */

NoAVL* rotacaoDireita(NoAVL *y) {
    NoAVL *x = y->esq;
    NoAVL *T2 = x->dir;

    // Realiza rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza alturas
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

NoAVL* rotacaoEsquerda(NoAVL *x) {
    NoAVL *y = x->dir;
    NoAVL *T2 = y->esq;

    // Realiza rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza alturas
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

/* ==================== BALANCEAMENTO ==================== */

NoAVL* balancear(NoAVL *no) {
    int fb = fatorBalanceamento(no);

    // Caso Esquerda-Esquerda
    if (fb > 1 && fatorBalanceamento(no->esq) >= 0)
        return rotacaoDireita(no);

    // Caso Esquerda-Direita
    if (fb > 1 && fatorBalanceamento(no->esq) < 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    // Caso Direita-Direita
    if (fb < -1 && fatorBalanceamento(no->dir) <= 0)
        return rotacaoEsquerda(no);

    // Caso Direita-Esquerda
    if (fb < -1 && fatorBalanceamento(no->dir) > 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no;
}

/* ==================== INSERÇÃO ==================== */

NoAVL* inserir(NoAVL *raiz, int chave) {
    // 1. Inserção normal em árvore binária de busca
    if (raiz == NULL)
        return criarNo(chave);

    if (chave < raiz->chave)
        raiz->esq = inserir(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserir(raiz->dir, chave);
    else {
        printf("Chave %d ja existe na arvore!\n", chave);
        return raiz;
    }

    // 2. Atualiza altura do nó ancestral
    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;

    // 3. Balanceia o nó
    return balancear(raiz);
}

/* ==================== BUSCA ==================== */

bool buscar(NoAVL *raiz, int chave) {
    if (raiz == NULL)
        return false;

    if (chave == raiz->chave)
        return true;
    else if (chave < raiz->chave)
        return buscar(raiz->esq, chave);
    else
        return buscar(raiz->dir, chave);
}

NoAVL* buscarNo(NoAVL *raiz, int chave) {
    if (raiz == NULL || chave == raiz->chave)
        return raiz;

    if (chave < raiz->chave)
        return buscarNo(raiz->esq, chave);
    else
        return buscarNo(raiz->dir, chave);
}

/* ==================== FUNÇÕES AUXILIARES PARA REMOÇÃO ==================== */

NoAVL* minimo(NoAVL *no) {
    NoAVL *atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

NoAVL* maximo(NoAVL *no) {
    NoAVL *atual = no;
    while (atual->dir != NULL)
        atual = atual->dir;
    return atual;
}

/* ==================== REMOÇÃO ==================== */

NoAVL* remover(NoAVL *raiz, int chave) {
    // 1. Remoção normal em árvore binária de busca
    if (raiz == NULL) {
        printf("Chave %d nao encontrada!\n", chave);
        return NULL;
    }

    if (chave < raiz->chave)
        raiz->esq = remover(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = remover(raiz->dir, chave);
    else {
        // Chave encontrada - casos de remoção
        if (raiz->esq == NULL || raiz->dir == NULL) {
            // Caso 1: Nó com 0 ou 1 filho
            NoAVL *temp = raiz->esq ? raiz->esq : raiz->dir;

            if (temp == NULL) {
                // Sem filhos
                temp = raiz;
                raiz = NULL;
            } else {
                // 1 filho
                *raiz = *temp;
            }
            free(temp);
        } else {
            // Caso 2: Nó com 2 filhos
            NoAVL *temp = minimo(raiz->dir);
            raiz->chave = temp->chave;
            raiz->dir = remover(raiz->dir, temp->chave);
        }
    }

    // Se a árvore tinha apenas um nó
    if (raiz == NULL)
        return NULL;

    // 2. Atualiza altura do nó atual
    raiz->altura = max(altura(raiz->esq), altura(raiz->dir)) + 1;

    // 3. Balanceia o nó
    return balancear(raiz);
}

/* ==================== TRAVESSIAS ==================== */

void preOrdem(NoAVL *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->chave);
        preOrdem(raiz->esq);
        preOrdem(raiz->dir);
    }
}

void emOrdem(NoAVL *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        emOrdem(raiz->dir);
    }
}

void posOrdem(NoAVL *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esq);
        posOrdem(raiz->dir);
        printf("%d ", raiz->chave);
    }
}

/* ==================== IMPRESSÃO POR NÍVEL ==================== */

void imprimirNivel(NoAVL *raiz, int nivel) {
    if (raiz == NULL) {
        if (nivel == 0)
            printf("(null) ");
        return;
    }

    if (nivel == 0) {
        printf("%d ", raiz->chave);
    } else {
        imprimirNivel(raiz->esq, nivel - 1);
        imprimirNivel(raiz->dir, nivel - 1);
    }
}

int alturaArvore(NoAVL *raiz) {
    if (raiz == NULL)
        return -1;
    return max(alturaArvore(raiz->esq), alturaArvore(raiz->dir)) + 1;
}

void imprimirPorNivel(NoAVL *raiz) {
    int h = alturaArvore(raiz);
    for (int i = 0; i <= h; i++) {
        printf("Nivel %d: ", i);
        imprimirNivel(raiz, i);
        printf("\n");
    }
}

/* ==================== IMPRIMIR ÁRVORE COM DETALHES ==================== */

void imprimirArvore(NoAVL *raiz, int espaco, int nivel) {
    if (raiz == NULL)
        return;

    espaco += 5;

    imprimirArvore(raiz->dir, espaco, nivel + 1);

    printf("\n");
    for (int i = 5; i < espaco; i++)
        printf(" ");
    
    // Mostra a chave e a altura
    printf("%d (h=%d, fb=%d)\n", raiz->chave, raiz->altura, 
           fatorBalanceamento(raiz));

    imprimirArvore(raiz->esq, espaco, nivel + 1);
}

/* ==================== FUNÇÕES ESTATÍSTICAS ==================== */

int contarNos(NoAVL *raiz) {
    if (raiz == NULL)
        return 0;
    return 1 + contarNos(raiz->esq) + contarNos(raiz->dir);
}

int contarFolhas(NoAVL *raiz) {
    if (raiz == NULL)
        return 0;
    if (raiz->esq == NULL && raiz->dir == NULL)
        return 1;
    return contarFolhas(raiz->esq) + contarFolhas(raiz->dir);
}

int somatorio(NoAVL *raiz) {
    if (raiz == NULL)
        return 0;
    return raiz->chave + somatorio(raiz->esq) + somatorio(raiz->dir);
}

float calcularMedia(NoAVL *raiz) {
    int total = somatorio(raiz);
    int nos = contarNos(raiz);
    if (nos == 0)
        return 0;
    return (float)total / nos;
}

/* ==================== MENU PRINCIPAL ==================== */

int main() {
    NoAVL *raiz = NULL;
    int opcao, valor;

    do {
        printf("\n========== ARVORE AVL ==========\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Exibir em Pre-Ordem\n");
        printf("5 - Exibir em Ordem\n");
        printf("6 - Exibir em Pos-Ordem\n");
        printf("7 - Exibir por nivel\n");
        printf("8 - Exibir arvore completa\n");
        printf("9 - Estatisticas\n");
        printf("0 - Sair\n");
        printf("================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Valor a inserir: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                printf("Valor %d inserido!\n", valor);
                break;

            case 2:
                printf("Valor a buscar: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor))
                    printf("Valor %d encontrado!\n", valor);
                else
                    printf("Valor %d nao encontrado!\n", valor);
                break;

            case 3:
                printf("Valor a remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;

            case 4:
                printf("Pre-Ordem: ");
                preOrdem(raiz);
                printf("\n");
                break;

            case 5:
                printf("Em Ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;

            case 6:
                printf("Pos-Ordem: ");
                posOrdem(raiz);
                printf("\n");
                break;

            case 7:
                printf("\nArvore por nivel:\n");
                imprimirPorNivel(raiz);
                break;

            case 8:
                printf("\nArvore AVL (chave, altura e fator de balanceamento):\n");
                if (raiz == NULL)
                    printf("Arvore vazia!\n");
                else
                    imprimirArvore(raiz, 0, 0);
                break;

            case 9:
                printf("\n=== ESTATISTICAS ===\n");
                printf("Total de nos: %d\n", contarNos(raiz));
                printf("Total de folhas: %d\n", contarFolhas(raiz));
                printf("Altura da arvore: %d\n", alturaArvore(raiz));
                printf("Soma dos valores: %d\n", somatorio(raiz));
                printf("Media dos valores: %.2f\n", calcularMedia(raiz));
                printf("Raiz: ");
                if (raiz != NULL)
                    printf("%d\n", raiz->chave);
                else
                    printf("(vazia)\n");
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}