#include "heapsort.h"

// Função para manter a propriedade do heap
void heapify(int v[], int n, int i) {
    int maior = i;           // Inicializa maior como raiz
    int esquerda = 2 * i + 1; // Filho esquerdo
    int direita = 2 * i + 2;   // Filho direito
    
    // Se filho esquerdo é maior que a raiz
    if (esquerda < n && v[esquerda] > v[maior]) {
        maior = esquerda;
    }
    
    // Se filho direito é maior que o maior atual
    if (direita < n && v[direita] > v[maior]) {
        maior = direita;
    }
    
    // Se maior não é a raiz
    if (maior != i) {
        // Troca a raiz com o maior
        int temp = v[i];
        v[i] = v[maior];
        v[maior] = temp;
        
        // Recursivamente aplica heapify na subárvore afetada
        heapify(v, n, maior);
    }
}

// Função principal do HeapSort
void heapsort(int v[], int n) {
    // PASSO 1: Construir o heap (reorganizar o array)
    // Começa do último nó não-folha e vai até a raiz
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(v, n, i);
    }
    
    // PASSO 2: Extrair elementos do heap um por um
    for (int i = n - 1; i > 0; i--) {
        // Move a raiz atual para o final
        int temp = v[0];
        v[0] = v[i];
        v[i] = temp;
        
        // Chama heapify na raiz reduzida
        heapify(v, i, 0);
    }
}