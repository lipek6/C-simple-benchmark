#include <iostream>         // Padrão de usar em códigos de C++
#include <vector>           // Para poder utilizar vetores
#include <chrono>           // Monitorar o tempo real de execução
#include <ctime>            // Monitorar o tempo da CPU de execução
#include <thread>           // Configurar o uso de múltiplos núcleos





// Troque os valores dos macros à baixo para modificar algumas configurações dos testes
#define VARIABLE_TEST_1         10000000000 // Somar +1 10 Bilhões de vezes
#define VARIABLE_TEST_2         10000000000 // Somar +1 10 Bilhões de vezes
#define VARIABLE_TEST_3         250000000   // 250 Milhões de ints para gravar em disco (1 GB)
#define VARIABLE_TEST_4         250000000   // 250 Milhões de ints para ler do disco (1 GB)




// Declaração das funções que usaremos, você verá essas funções abaixo de main()
void cpu_test1       (long double *real_time_spent, long double *cpu_time_spent);
void disk_write_test (long double *time_spent, long double *throughput);
void disk_read_test (long double *time_spent, long double *throughput);




int main (void)
{
    int tests = 1;        // Quantidade de testes que serão realizados (input sobrescreverá o 1)
    int thread_count = 1; // Quantidade de threads que serão gerados (input sobrescreverá o 1)



    printf("Iniciando processos de benchmark da sua máquina\n");
    printf("Determine quantas vezes você gostaria de rodar os testes: ");
    scanf("%i", &tests);



    // Declaração de vetores para registrar run time. Serão enviadas para cada função_teste via endereço
    // RELACIONADO À CPU
    std::vector<long double> real_time_singcore(tests);
    std::vector<long double> cpu_time_singcore(tests);
    
    std::vector<long double> real_time_multcore(tests);
    std::vector<long double> cpu_time_multcore(tests);


    // RELACIONADO AO DISCO
    std::vector<long double> time_write(tests);
    std::vector<long double> rate_write(tests);

    std::vector<long double> time_read (tests);
    std::vector<long double> rate_read (tests);


    // Início dos testes
    printf("Testando o CPU:\n");

        printf("1. Testando apenas 1 núcleo\n");
        for(int i = 0; i < tests; i++)
        {
            cpu_test1(&real_time_singcore[i], &cpu_time_singcore[i]);
            printf("Tempo de execução real    %i: %Lf segundos\n", (i+1), real_time_singcore[i]);
            printf("Tempo de execução da CPU  %i: %Lf segundos\n", (i+1),  cpu_time_singcore[i]);
        }


        printf("2. Testando múltiplos núcleos\n");
        for(int i = 0; i < tests; i++)
        {
            cpu_test2(&real_time_multcore[i], &cpu_time_multcore[i]);
            printf("Tempo de execução real    %i: %Lf segundos\n", (i+1), real_time_multcore[i]);
            printf("Tempo de execução da CPU  %i: %Lf segundos\n", (i+1),  cpu_time_multcore[i]);
        }

        printf("3. Testando escrita e leitura em disco\n");
        for(int i = 0; i < tests; i++)
        {
            disk_write_test(&time_write[i], &rate_write[i]);
            printf("Tempo de escrita %i: %Lf segundos | Taxa: %.2Lf MB/s\n", (i+1), time_write[i], rate_write[i]);
           
            disk_read_test(&time_read[i], &rate_read[i]);
            printf("Tempo de leitura %i: %Lf segundos | Taxa: %.2Lf MB/s\n", (i+1), time_read[i], rate_read[i]);
        }
}





// Funções para os testes. Chamadas em main(), declaradas acima de main().
void cpu_test1 (long double *real_time_spent, long double *cpu_time_spent)
{
    using namespace std::chrono;

    high_resolution_clock::time_point real_time_initial = high_resolution_clock::now();
    clock_t cpu_time_initial = clock();
        volatile long long i = 0; 
        while(i < VARIABLE_TEST_1)
        {
            i++;
        }
    clock_t cpu_time_ending = clock();
    high_resolution_clock::time_point real_time_ending = high_resolution_clock::now();


    duration <long double> calc_real_time = duration_cast<duration<long double>>(real_time_ending - real_time_initial);
    long double calc_cpu_time  = static_cast<double>(cpu_time_ending  - cpu_time_initial) / CLOCKS_PER_SEC;

    *real_time_spent = calc_real_time.count();
    *cpu_time_spent  = calc_cpu_time;
}

void disk_write_test (long double *time_spent, long double *throughput)
{
    using namespace std::chrono;

    high_resolution_clock::time_point time_initial = high_resolution_clock::now();
        int *vetor;
        FILE *arquivo;

        // aloca memória
        vetor = (int*) malloc(VARIABLE_TEST_3 * sizeof(int));
        if (vetor == NULL) {
            printf("Erro: memória insuficiente!\n");
            exit(1);
        }

        // preenche o vetor
        for (int i = 0; i < VARIABLE_TEST_3; i++) {
            vetor[i] = i % 256;
        }

        arquivo = fopen("teste.bin", "wb");
        if (arquivo == NULL) {
            printf("Erro ao abrir arquivo para escrita!\n");
            free(vetor);
            exit(1);
        }

        fwrite(vetor, sizeof(int), VARIABLE_TEST_3, arquivo);
    high_resolution_clock::time_point time_ending = high_resolution_clock::now();


    duration <long double> calc_time = duration_cast<duration<long double>>(time_ending - time_initial);

    *time_spent = calc_time.count();
    *throughput = (VARIABLE_TEST_3 * sizeof(int)) / (1024.0 * 1024.0) / (*time_spent);

    fclose(arquivo);
    free(vetor);
}

void disk_read_test (long double *time_spent, long double *throughput)
{
    using namespace std::chrono;

    high_resolution_clock::time_point time_initial = high_resolution_clock::now();
        int *vetor;
        FILE *arquivo;

        vetor = (int*) malloc(VARIABLE_TEST_4 * sizeof(int));
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

        fread(vetor, sizeof(int), VARIABLE_TEST_4, arquivo);
    high_resolution_clock::time_point time_ending = high_resolution_clock::now();

    duration <long double> calc_time = duration_cast<duration<long double>>(time_ending - time_initial);


    *time_spent = calc_time.count();
    *throughput = (VARIABLE_TEST_4 * sizeof(int)) / (1024.0 * 1024.0) / (*time_spent);

    fclose(arquivo);
    free(vetor);
}

void cpu_test2 (long double *real_time_spent, long double *cpu_time_spent)
{
    using namespace std::chrono;

    high_resolution_clock::time_point real_time_initial = high_resolution_clock::now();
    clock_t cpu_time_initial = clock();
        volatile long long i = 0; 
        while(i < VARIABLE_TEST_1)
        {
            i++;
        }
    clock_t cpu_time_ending = clock();
    high_resolution_clock::time_point real_time_ending = high_resolution_clock::now();


    duration <long double> calc_real_time = duration_cast<duration<long double>>(real_time_ending - real_time_initial);
    long double calc_cpu_time  = static_cast<double>(cpu_time_ending  - cpu_time_initial) / CLOCKS_PER_SEC;

    *real_time_spent = calc_real_time.count();
    *cpu_time_spent  = calc_cpu_time;
}
