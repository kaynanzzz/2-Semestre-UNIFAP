#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct No23 {
    int chave1, chave2;
    int nChaves;
    struct No23 *esq, *meio, *dir;
    struct No23 *pai;  // Adicionado ponteiro para o pai
} No23;

/* CRIAÇÃO */

No23* criarNo(int chave) {
    No23* no = (No23*) malloc(sizeof(No23));
    no->chave1 = chave;
    no->nChaves = 1;
    no->esq = no->meio = no->dir = no->pai = NULL;
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

/* FUNÇÃO PARA ENCONTRAR O NÓ ONDE A CHAVE ESTÁ */
No23* buscarNo(No23* raiz, int valor) {
    if (!raiz) return NULL;

    if (valor == raiz->chave1 || (raiz->nChaves == 2 && valor == raiz->chave2))
        return raiz;

    if (valor < raiz->chave1)
        return buscarNo(raiz->esq, valor);
    else if (raiz->nChaves == 1 || valor < raiz->chave2)
        return buscarNo(raiz->meio, valor);
    else
        return buscarNo(raiz->dir, valor);
}

/* FUNÇÕES AUXILIARES PARA REMOÇÃO */

int encontrarSucessor(No23* no, int valor) {
    // Encontra o sucessor em ordem de uma chave
    if (valor == no->chave1) {
        if (no->meio) {
            No23* atual = no->meio;
            while (atual->esq) atual = atual->esq;
            return atual->chave1;
        }
    } else if (no->nChaves == 2 && valor == no->chave2) {
        if (no->dir) {
            No23* atual = no->dir;
            while (atual->esq) atual = atual->esq;
            return atual->chave1;
        }
    }
    return -1;
}

void removerChave(No23* no, int valor) {
    // Remove uma chave de um nó folha
    if (no->chave1 == valor) {
        if (no->nChaves == 2) {
            no->chave1 = no->chave2;
        }
        no->nChaves--;
    } else if (no->nChaves == 2 && no->chave2 == valor) {
        no->nChaves--;
    }
}

No23* encontrarIrmao(No23* no) {
    // Encontra um irmão para empréstimo/fusão
    No23* pai = no->pai;
    if (!pai) return NULL;

    if (pai->esq == no) {
        // No é filho esquerdo
        if (pai->meio && pai->meio->nChaves == 2) return pai->meio;
        if (pai->nChaves == 2 && pai->dir && pai->dir->nChaves == 2) return pai->dir;
    } else if (pai->meio == no) {
        // No é filho do meio
        if (pai->esq && pai->esq->nChaves == 2) return pai->esq;
        if (pai->nChaves == 2 && pai->dir && pai->dir->nChaves == 2) return pai->dir;
    } else if (pai->nChaves == 2 && pai->dir == no) {
        // No é filho direito
        if (pai->meio && pai->meio->nChaves == 2) return pai->meio;
        if (pai->esq && pai->esq->nChaves == 2) return pai->esq;
    }
    return NULL;
}

void fundirComIrmao(No23* no, No23* irmao) {
    // Funde o nó com seu irmão (para nós internos)
    No23* pai = no->pai;
    
    if (pai->esq == no && pai->meio == irmao) {
        // Fundir esquerdo com meio
        no->chave2 = pai->chave1;
        no->nChaves = 2;
        no->meio = irmao->esq;
        no->dir = irmao->meio;
        
        // Ajustar o pai
        pai->esq = no;
        pai->meio = pai->dir;
        pai->nChaves--;
        
        free(irmao);
    } else if (pai->esq == irmao && pai->meio == no) {
        // Fundir meio com esquerdo
        irmao->chave2 = pai->chave1;
        irmao->nChaves = 2;
        irmao->meio = no->esq;
        irmao->dir = no->meio;
        
        // Ajustar o pai
        pai->esq = irmao;
        pai->meio = pai->dir;
        pai->nChaves--;
        
        free(no);
    }
    // Outros casos similares...
}

void emprestarDoIrmao(No23* no, No23* irmao, bool irmaoEsquerdo) {
    // Empresta uma chave de um irmão
    No23* pai = no->pai;
    
    if (irmaoEsquerdo) {
        // Irmão à esquerda
        if (no == pai->meio) {
            // No é filho do meio, irmão é esquerdo
            if (no->nChaves == 1) {
                no->chave2 = pai->chave1;
                pai->chave1 = irmao->chave2;
                no->dir = no->meio;
                no->meio = irmao->dir;
                irmao->nChaves = 1;
                no->nChaves = 2;
            }
        }
    } else {
        // Irmão à direita
        if (no == pai->meio && pai->dir == irmao) {
            if (no->nChaves == 1) {
                no->chave2 = pai->chave2;
                pai->chave2 = irmao->chave1;
                no->dir = irmao->esq;
                irmao->esq = irmao->meio;
                irmao->chave1 = irmao->chave2;
                irmao->nChaves = 1;
                no->nChaves = 2;
            }
        }
    }
}

/* FUNÇÃO PRINCIPAL DE REMOÇÃO */

No23* removerRec(No23* raiz, int valor) {
    No23* no = buscarNo(raiz, valor);
    if (!no) {
        printf("Valor nao encontrado!\n");
        return raiz;
    }

    // Caso 1: Nó folha
    if (!no->esq && !no->meio && !no->dir) {
        removerChave(no, valor);
        
        // Se o nó não está vazio, tudo ok
        if (no->nChaves > 0) return raiz;
        
        // Nó ficou vazio, precisa rebalancear
        No23* pai = no->pai;
        if (!pai) {
            // É a raiz
            free(no);
            return NULL;
        }
        
        // Encontrar irmão para empréstimo
        No23* irmao = encontrarIrmao(no);
        if (irmao) {
            emprestarDoIrmao(no, irmao, irmao->chave1 < no->chave1);
        } else {
            // Fundir com irmão
            No23* irmaoFundir = (pai->esq == no) ? pai->meio : 
                                (pai->meio == no && pai->nChaves == 2) ? pai->dir : pai->esq;
            if (irmaoFundir) {
                // Fundir os nós
                if (pai->esq == no || pai->esq == irmaoFundir) {
                    // Fundir esquerdo com meio
                    No23* esq = (pai->esq == no) ? no : irmaoFundir;
                    No23* meio = (pai->meio == no) ? no : irmaoFundir;
                    
                    esq->chave2 = pai->chave1;
                    esq->nChaves = 2;
                    esq->meio = meio->esq;
                    esq->dir = meio->meio;
                    
                    // Ajustar pai
                    pai->esq = esq;
                    pai->meio = pai->dir;
                    pai->nChaves--;
                    
                    free(meio);
                    free(no);
                    
                    if (pai->nChaves == 0) {
                        // Pai ficou vazio, promover filho
                        No23* novaRaiz = esq;
                        novaRaiz->pai = NULL;
                        free(pai);
                        return novaRaiz;
                    }
                }
            }
        }
    } else {
        // Caso 2: Nó interno - substituir pelo sucessor
        int sucessor = encontrarSucessor(no, valor);
        if (sucessor != -1) {
            // Substituir a chave pelo sucessor e remover o sucessor da folha
            if (no->chave1 == valor) {
                no->chave1 = sucessor;
            } else {
                no->chave2 = sucessor;
            }
            removerRec(raiz, sucessor);
        }
    }
    
    return raiz;
}

No23* remover(No23* raiz, int valor) {
    if (!raiz) {
        printf("Arvore vazia!\n");
        return NULL;
    }
    
    // Primeiro, verificar se o valor existe
    if (!buscar(raiz, valor)) {
        printf("Valor %d nao encontrado!\n", valor);
        return raiz;
    }
    
    raiz = removerRec(raiz, valor);
    
    // Garantir que a raiz não seja um nó vazio
    if (raiz && raiz->nChaves == 0) {
        No23* novaRaiz = raiz->esq;
        if (novaRaiz) novaRaiz->pai = NULL;
        free(raiz);
        return novaRaiz;
    }
    
    return raiz;
}

/* SPLIT (para inserção) - com ajuste do pai */

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
        if (novo->esq) novo->esq->pai = novo;
        if (novo->meio) novo->meio->pai = novo;
        
        no->meio = filho;
        if (no->meio) no->meio->pai = no;
        no->dir = NULL;
    }
    else if (no->nChaves == 1 || valor < no->chave2) {
        *promovida = valor;

        novo->chave1 = no->chave2;
        novo->nChaves = 1;

        novo->esq = filho;
        novo->meio = no->dir;
        if (novo->esq) novo->esq->pai = novo;
        if (novo->meio) novo->meio->pai = novo;

        no->nChaves = 1;
        no->dir = NULL;
    }
    else {
        *promovida = no->chave2;

        novo->chave1 = valor;
        novo->nChaves = 1;

        novo->esq = no->dir;
        novo->meio = filho;
        if (novo->esq) novo->esq->pai = novo;
        if (novo->meio) novo->meio->pai = novo;

        no->nChaves = 1;
        no->dir = NULL;
    }
    
    novo->pai = no->pai;
    return novo;
}

/* INSERÇÃO - modificada para manter o pai */

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
        if (raiz->dir) raiz->dir->pai = raiz;
        if (raiz->meio) raiz->meio->pai = raiz;
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
        if (novaRaiz->esq) novaRaiz->esq->pai = novaRaiz;
        if (novaRaiz->meio) novaRaiz->meio->pai = novaRaiz;
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
            printf("[%d] ", raiz->chave1);
        else
            printf("[%d|%d] ", raiz->chave1, raiz->chave2);
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
        printf("3 - Remover\n");
        printf("4 - Imprimir em Ordem\n");
        printf("5 - Imprimir por Nivel\n");
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
                printf("Valor a remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;

            case 4:
                printf("Em ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;

            case 5:
                imprimirPorNivel(raiz);
                break;
        }
    } while (opcao != 0);

    return 0;
}
