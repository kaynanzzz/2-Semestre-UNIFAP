#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "quicksort.h"
#include "heapsort.h"
#include "countingsort.h"
#include "radixsort.h"

// Função para ler os CEPs do arquivo
int* ler_ceps(const char* nome_arquivo, int* n) {
    FILE* arquivo = fopen(nome_arquivo, "r");

    if (arquivo == NULL) {
        printf("ERRO: Não foi possível abrir o arquivo %s\n", nome_arquivo);
        printf("Certifique-se que o arquivo está na mesma pasta do programa\n");
        return NULL;
    }

    // Primeiro, contar quantos CEPs existem no arquivo
    int cep;
    int count = 0;

    // CORREÇÃO: fscanf retorna 1 quando consegue ler um número
    // Enquanto conseguir ler (retornar 1), continua contando
    while (fscanf(arquivo, "%d", &cep) == 1) {
        count++;
    }

    printf("Total de CEPs encontrados: %d\n", count);

    // Se não encontrou nenhum CEP
    if (count == 0) {
        printf("ERRO: Arquivo vazio ou formato inválido\n");
        fclose(arquivo);
        return NULL;
    }

    // Voltar ao início do arquivo
    rewind(arquivo);

    // Alocar memória para o vetor
    int* ceps = (int*)malloc(count * sizeof(int));
    if (!ceps) {
        printf("ERRO: Memória insuficiente\n");
        fclose(arquivo);
        return NULL;
    }

    // Ler todos os CEPs
    for (int i = 0; i < count; i++) {
        fscanf(arquivo, "%d", &ceps[i]);
    }

    fclose(arquivo);
    
    // CORREÇÃO: Retornar o vetor alocado
    *n = count;
    return ceps;  // NÃO dar free aqui!
}

// Função para testar um algoritmo e salvar os resultados
void testar_algoritmo(const char* nome, void (*algoritmo)(int*, int), 
                      int* dados, int total, const char* arquivo_saida) {
    
    FILE* saida = fopen(arquivo_saida, "w");
    if (!saida) {
        printf("ERRO: Não foi possível criar %s\n", arquivo_saida);
        return;
    }
    
    fprintf(saida, "n,tempo_segundos\n");
    printf("\nTestando %s...\n", nome);
    
    // Testar com tamanhos crescentes (de 1000 em 1000)
    for (int tamanho = 1000; tamanho <= total; tamanho += 1000) {
        printf("  Processando %d elementos...\n", tamanho);
        
        // Criar uma cópia dos dados
        int* copia = (int*)malloc(tamanho * sizeof(int));
        if (!copia) {
            printf("    ERRO: Memória insuficiente para %d elementos\n", tamanho);
            continue;
        }
        
        for (int i = 0; i < tamanho; i++) {
            copia[i] = dados[i];
        }
        
        // Medir o tempo de execução
        clock_t inicio = clock();
        algoritmo(copia, tamanho);
        clock_t fim = clock();
        
        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        fprintf(saida, "%d,%.6f\n", tamanho, tempo);
        
        free(copia);
    }
    
    fclose(saida);
    printf("  Resultados salvos em %s\n", arquivo_saida);
}

int main() {
    printf("=== TESTE DE ALGORITMOS COM CEPs ===\n\n");
    
    // Verificar se os arquivos necessários existem
    FILE* test = fopen("ceps.txt", "r");
    if (test == NULL) {
        printf("ERRO: Arquivo 'ceps.txt' não encontrado!\n");
        printf("Certifique-se que o arquivo está na mesma pasta do programa.\n");
        printf("\nPressione ENTER para sair...");
        getchar();
        return 1;
    }
    fclose(test);
    
    // Ler os CEPs
    int total_ceps;
    int* ceps = ler_ceps("ceps.txt", &total_ceps);
    
    if (!ceps) {
        printf("\nPressione ENTER para sair...");
        getchar();
        return 1;
    }
    
    printf("\nIniciando testes...\n");
    
    // Testar cada algoritmo individualmente
    testar_algoritmo("QuickSort", quicksort_meio, ceps, total_ceps, "quick_ceps.csv");
    testar_algoritmo("HeapSort", heapsort, ceps, total_ceps, "heap_ceps.csv");
    testar_algoritmo("CountingSort", countingsort, ceps, total_ceps, "counting_ceps.csv");
    testar_algoritmo("RadixSort", radixsort, ceps, total_ceps, "radix_ceps.csv");
    
    // Teste comparativo (todos juntos)
    printf("\n=== TESTE COMPARATIVO ===\n");
    FILE* comparativo = fopen("comparativo_ceps.csv", "w");
    if (!comparativo) {
        printf("ERRO: Não foi possível criar arquivo comparativo\n");
        free(ceps);
        return 1;
    }
    
    fprintf(comparativo, "n,quick,heap,counting,radix\n");
    
    int max_comparativo = total_ceps;
    if (max_comparativo > 20000) {  // Reduzido para 20000 para não demorar muito
        max_comparativo = 20000;
        printf("Limitando teste comparativo a 20000 elementos\n");
    }
    
    for (int tamanho = 1000; tamanho <= max_comparativo; tamanho += 1000) {
        printf("\nComparando com %d elementos:\n", tamanho);
        
        double tempos[4];
        
        // QuickSort
        int* copia = (int*)malloc(tamanho * sizeof(int));
        if (!copia) {
            printf("    ERRO: Memória insuficiente\n");
            continue;
        }
        for (int i = 0; i < tamanho; i++) copia[i] = ceps[i];
        clock_t inicio = clock();
        quicksort_meio(copia, tamanho);
        tempos[0] = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        free(copia);
        printf("  QuickSort: %.4f s\n", tempos[0]);
        
        // HeapSort
        copia = (int*)malloc(tamanho * sizeof(int));
        if (!copia) continue;
        for (int i = 0; i < tamanho; i++) copia[i] = ceps[i];
        inicio = clock();
        heapsort(copia, tamanho);
        tempos[1] = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        free(copia);
        printf("  HeapSort: %.4f s\n", tempos[1]);
        
        // CountingSort
        copia = (int*)malloc(tamanho * sizeof(int));
        if (!copia) continue;
        for (int i = 0; i < tamanho; i++) copia[i] = ceps[i];
        inicio = clock();
        countingsort(copia, tamanho);
        tempos[2] = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        free(copia);
        printf("  CountingSort: %.4f s\n", tempos[2]);
        
        // RadixSort
        copia = (int*)malloc(tamanho * sizeof(int));
        if (!copia) continue;
        for (int i = 0; i < tamanho; i++) copia[i] = ceps[i];
        inicio = clock();
        radixsort(copia, tamanho);
        tempos[3] = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        free(copia);
        printf("  RadixSort: %.4f s\n", tempos[3]);
        
        fprintf(comparativo, "%d,%.6f,%.6f,%.6f,%.6f\n", 
                tamanho, tempos[0], tempos[1], tempos[2], tempos[3]);
    }
    
    fclose(comparativo);
    free(ceps);
    
    printf("\n=== TESTES CONCLUÍDOS ===\n");
    printf("\nArquivos gerados na pasta atual:\n");
    printf("- quick_ceps.csv\n");
    printf("- heap_ceps.csv\n");
    printf("- counting_ceps.csv\n");
    printf("- radix_ceps.csv\n");
    printf("- comparativo_ceps.csv\n");
    
    printf("\nPressione ENTER para sair...");
    getchar();
    
    return 0;
}