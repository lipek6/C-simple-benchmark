#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//quantidade de inteiros que queremos gravar no arquivo
#define TAMANHO 10000000  // 10 milhões de inteiros (~40 MB se int = 4 bytes)

// Declaração das funções
void disk_write_test(double *time_spent, double *throughput);
void disk_read_test(double *time_spent, double *throughput);

int main(void) {
    double tempo_escrita = 0, tempo_leitura = 0;
    double taxa_escrita = 0, taxa_leitura = 0;

    printf("Benchmark de Disco:\n");

    printf("1. Testando escrita em arquivo\n");
    disk_write_test(&tempo_escrita, &taxa_escrita);
    printf("Tempo de escrita: %f segundos | Taxa: %.2f MB/s\n", tempo_escrita, taxa_escrita);

    printf("2. Testando leitura em arquivo\n");
    disk_read_test(&tempo_leitura, &taxa_leitura);
    printf("Tempo de leitura: %f segundos | Taxa: %.2f MB/s\n", tempo_leitura, taxa_leitura);

    return 0;
}

// Função que mede tempo de escrita
void disk_write_test(double *time_spent, double *throughput) {
    int *vetor;
    FILE *arquivo;
    clock_t inicio, fim;

    // aloca memória
    vetor = (int*) malloc(TAMANHO * sizeof(int));
    if (vetor == NULL) {
        printf("Erro: memória insuficiente!\n");
        exit(1);
    }

    // preenche o vetor
    for (int i = 0; i < TAMANHO; i++) {
        vetor[i] = i % 256;
    }

    arquivo = fopen("teste.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita!\n");
        free(vetor);
        exit(1);
    }

    inicio = clock();
    fwrite(vetor, sizeof(int), TAMANHO, arquivo);
    fim = clock();

    *time_spent = (double)(fim - inicio) / CLOCKS_PER_SEC;
    *throughput = (TAMANHO * sizeof(int)) / (1024.0 * 1024.0) / (*time_spent);

    fclose(arquivo);
    free(vetor);
}

// Função que mede tempo de leitura
void disk_read_test(double *time_spent, double *throughput) {
    int *vetor;
    FILE *arquivo;
    clock_t inicio, fim;

    vetor = (int*) malloc(TAMANHO * sizeof(int));
    if (vetor == NULL) {
        printf("Erro: memória insuficiente!\n");
        exit(1);
    }

    arquivo = fopen("teste.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para leitura!\n");
        free(vetor);
        exit(1);
    }

    inicio = clock();
    fread(vetor, sizeof(int), TAMANHO, arquivo);
    fim = clock();

    *time_spent = (double)(fim - inicio) / CLOCKS_PER_SEC;
    *throughput = (TAMANHO * sizeof(int)) / (1024.0 * 1024.0) / (*time_spent);

    fclose(arquivo);
    free(vetor);
}