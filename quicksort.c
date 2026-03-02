#include "quicksort.h"
#include <stdlib.h>

// Função auxiliar para o QuickSort recursivo (pivô do meio)
void quick_recursivo_meio(int v[], int esq, int dir) {
    if (esq < dir) {  // Se há elementos para ordenar
        int i = esq;
        int j = dir;
        
        // Pega o elemento do meio como pivô
        int pivo = v[(esq + dir) / 2];
        
        // Enquanto os índices não se cruzarem
        while (i <= j) {
            // Encontra elemento maior que o pivô à esquerda
            while (v[i] < pivo) {
                i++;
            }
            // Encontra elemento menor que o pivô à direita
            while (v[j] > pivo) {
                j--;
            }
            
            // Se encontrou elementos para trocar
            if (i <= j) {
                // Troca os elementos de posição
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
                i++;
                j--;
            }
        }
        
        // Chamadas recursivas para as duas partições
        if (esq < j) {
            quick_recursivo_meio(v, esq, j);
        }
        if (i < dir) {
            quick_recursivo_meio(v, i, dir);
        }
    }
}

// Função principal do QuickSort com pivô do meio
void quicksort_meio(int v[], int n) {
    quick_recursivo_meio(v, 0, n - 1);
}

// Função auxiliar para o QuickSort recursivo (pivô aleatório)
void quick_recursivo_aleatorio(int v[], int esq, int dir) {
    if (esq < dir) {
        int i = esq;
        int j = dir;
        
        // Escolhe um índice aleatório para o pivô
        int indice_pivo = esq + rand() % (dir - esq + 1);
        int pivo = v[indice_pivo];
        
        while (i <= j) {
            while (v[i] < pivo) {
                i++;
            }
            while (v[j] > pivo) {
                j--;
            }
            
            if (i <= j) {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
                i++;
                j--;
            }
        }
        
        if (esq < j) {
            quick_recursivo_aleatorio(v, esq, j);
        }
        if (i < dir) {
            quick_recursivo_aleatorio(v, i, dir);
        }
    }
}

// Função principal do QuickSort com pivô aleatório
void quicksort_aleatorio(int v[], int n) {
    quick_recursivo_aleatorio(v, 0, n - 1);
}