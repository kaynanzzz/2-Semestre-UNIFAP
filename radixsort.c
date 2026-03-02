#include "radixsort.h"
#include <stdlib.h>
#include <string.h>

// CountingSort adaptado para ordenar por um dígito específico
void counting_sort_por_digito(int v[], int n, int exp) {
    // exp é a potência de 10 (1, 10, 100, 1000, ...)
    
    int* output = (int*)malloc(n * sizeof(int));
    int count[10] = {0};  // 10 dígitos (0 a 9)
    
    if (!output) return;
    
    // PASSO 1: Contar ocorrências de cada dígito
    for (int i = 0; i < n; i++) {
        int digito = (v[i] / exp) % 10;
        count[digito]++;
    }
    
    // PASSO 2: Acumular contagens
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    // PASSO 3: Construir array ordenado (estável)
    for (int i = n - 1; i >= 0; i--) {
        int digito = (v[i] / exp) % 10;
        output[count[digito] - 1] = v[i];
        count[digito]--;
    }
    
    // PASSO 4: Copiar de volta
    memcpy(v, output, n * sizeof(int));
    
    free(output);
}

void radixsort(int v[], int n) {
    if (n <= 0) return;
    
    // PASSO 1: Encontrar o valor máximo
    int max = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    
    // PASSO 2: Ordenar por cada dígito
    // exp é 1, 10, 100, 1000... até max/exp > 0
    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort_por_digito(v, n, exp);
    }
}