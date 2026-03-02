#include "countingsort.h"
#include <stdlib.h>
#include <string.h>

void countingsort(int v[], int n) {
    if (n <= 0) return;  // Vetor vazio
    
    // PASSO 1: Encontrar o valor máximo no vetor
    int max = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    
    // PASSO 2: Criar array de contagem (tamanho = max+1)
    // calloc já inicializa tudo com zero
    int* count = (int*)calloc(max + 1, sizeof(int));
    int* output = (int*)malloc(n * sizeof(int));
    
    // Verificar se alocou memória corretamente
    if (!count || !output) {
        free(count);
        free(output);
        return;
    }
    
    // PASSO 3: Contar quantas vezes cada valor aparece
    for (int i = 0; i < n; i++) {
        count[v[i]]++;
    }
    
    // PASSO 4: Acumular as contagens
    // count[i] agora contém a posição do último elemento i
    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }
    
    // PASSO 5: Construir o array ordenado
    // Percorre de trás para frente para manter a estabilidade
    for (int i = n - 1; i >= 0; i--) {
        output[count[v[i]] - 1] = v[i];
        count[v[i]]--;
    }
    
    // PASSO 6: Copiar de volta para o vetor original
    memcpy(v, output, n * sizeof(int));
    
    // Liberar memória
    free(count);
    free(output);
}