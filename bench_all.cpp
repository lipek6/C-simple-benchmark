#include <iostream>         // Padrão de usar em códigos de C++
#include <vector>           // Para poder utilizar vetores
#include <numeric>          // Facilitar somas dos vetores
#include <chrono>           // Monitorar o tempo real de execução
#include <ctime>            // Monitorar o tempo da CPU de execução
#include <thread>           // Configurar o uso de múltiplos núcleos
#include <cmath>            // Para calcularmos o desvio padrão



// Troque os valores dos macros à baixo para modificar algumas configurações dos testes
#define VARIABLE_TEST_1         10000000000 // Somar +1 10 Bilhões de vezes em um único thread
#define VARIABLE_TEST_2         10000000000   // Somar +1 625 Milhões de vezes em x threads (total de 1 Bilhão)
#define VARIABLE_TEST_3         250000000   // 250 Milhões de ints para gravar em disco (1 GB)
#define VARIABLE_TEST_4         250000000   // 250 Milhões de ints para ler do disco (1 GB)



// Variáveis globais para testes em multithread
volatile long long multithread_destiny = 0;




// Declaração das funções que usaremos, você verá essas funções abaixo de main()
void cpu_test1       (long double *real_time_spent, long double *cpu_time_spent);
void cpu_test2       (long double *real_time_spent, long double *cpu_time_spent, int thread_count);
void disk_write_test (long double *time_spent, long double *throughput);
void disk_read_test  (long double *time_spent, long double *throughput);
void thread_function (long long *destiny, long long operations);

// Declaração de funções auxiliares
long double deviation (const std::vector<long double>& vector, long double average, int n);
long double average(const std::vector<long double>& vector) ;


int main (void)
{
    int tests   = 1;                                    // Quantidade de testes que serão realizados (input sobrescreverá o 1)
    int threads = std::thread::hardware_concurrency();  // Conta a quantidade de threads da máquina

    

    printf("Iniciando processos de benchmark da sua máquina\n");
    printf("Determine quantas vezes você gostaria de rodar os testes: \n");
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
        printf("\n");

        printf("2. Testando múltiplos núcleos (%i núcleos lógicos da sua máquina)\n", threads);
        for(int i = 0; i < tests; i++)
        {
            cpu_test2(&real_time_multcore[i], &cpu_time_multcore[i], threads);
            printf("Tempo de execução real    %i: %Lf segundos\n", (i+1), real_time_multcore[i]);
            printf("Tempo de execução da CPU  %i: %Lf segundos\n", (i+1),  cpu_time_multcore[i]);
        }
        printf("\n");

        printf("3. Testando escrita e leitura em disco\n");
        for(int i = 0; i < tests; i++)
        {
            disk_write_test(&time_write[i], &rate_write[i]);
            printf("Tempo de escrita %i: %Lf segundos | Taxa: %.2Lf MB/s\n", (i+1), time_write[i], rate_write[i]);
           
            disk_read_test(&time_read[i], &rate_read[i]);
            printf("Tempo de leitura %i: %Lf segundos | Taxa: %.2Lf MB/s\n", (i+1), time_read[i], rate_read[i]);
        }
        printf("\n");


        printf("\n");
        printf("------------------------------Testes concluídos------------------------------\n");
        printf("\n");
        printf("\n");


        printf("Resultados da CPU utilizando apenas 1 núcleo:\n");
        printf("----------------------------------------------------\n");
        for (int i = 0; i < tests; i++)
        {
            printf("|Teste %i: Tempo real %Lfs| Tempo CPU %Lfs|\n", i, real_time_singcore[i], cpu_time_singcore[i]);
        }
        printf("----------------------------------------------------\n");
        printf("\n");

        printf("Resultados da CPU utilizando todos os %i threads:\n", threads);
        printf("----------------------------------------------------\n");
        for (int i = 0; i < tests; i++)
        {
            printf("|Teste %i: Tempo real %Lfs| Tempo CPU %Lfs|\n", i, real_time_multcore[i], cpu_time_multcore[i]);
        }
        printf("----------------------------------------------------\n");
        printf("\n");

        printf("Resultados da escrita em disco:\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < tests; i++)
        {
            printf("|Tempo : %Lfs| Taxa %LfMB/s|\n", time_write[i], rate_write[i]);
        }
        printf("----------------------------------------\n");
        printf("\n");

        printf("Resultados da leitura em disco:\n");
        printf("---------------------------------------\n");
        for (int i = 0; i < tests; i++)
        {
            printf("|Tempo : %Lfs| Taxa %LfMB/s|\n", time_read[i], rate_read[i]);
        }
        printf("---------------------------------------\n");
        printf("\n");


        long double single_real_time_average = average(real_time_singcore);
        long double single_cpu_time_average  = average(cpu_time_singcore);

        long double multi_real_time_average  = average(real_time_multcore);
        long double multi_cpu_time_average   = average(cpu_time_multcore);

        long double disk_write_time_average  = average(time_write);
        long double disk_write_rate_average  = average(rate_write);

        long double disk_read_time_average   = average(time_read);
        long double disk_read_rate_average   = average(rate_read);      
        
        
        printf("\n");
        printf("---------------------------Médias obtidas---------------------------\n");

        printf("| CPU 1 núcleo    | Real: %-10.4Lf s | CPU        %-10.4Lf s   |\n", single_real_time_average, single_cpu_time_average);
        printf("| CPU multithread | Real: %-10.4Lf s | CPU        %-10.4Lf s   |\n", multi_real_time_average,  multi_cpu_time_average);
        printf("| DISCO escrita   | Tempo: %-9.4Lf s | Velocidade %-9.2Lf MB/s |\n", disk_write_time_average,  disk_write_rate_average);
        printf("| DISCO leitura   | Tempo: %-9.4Lf s | Velocidade %-9.2Lf MB/s |\n", disk_read_time_average,   disk_read_rate_average);

        printf("--------------------------------------------------------------------\n");

        
        long double d_single_real = deviation(real_time_singcore, single_real_time_average, tests); 
        long double d_single_cpu  = deviation(cpu_time_singcore, single_cpu_time_average, tests); 

        long double d_multi_real = deviation(real_time_multcore, multi_real_time_average, tests); 
        long double d_multi_cpu  = deviation(cpu_time_multcore, multi_cpu_time_average, tests); 

        long double d_write_time = deviation(time_write, disk_write_time_average, tests); 
        long double d_write_rate = deviation(rate_write, disk_write_rate_average, tests); 

        long double d_read_time = deviation(time_read, disk_read_time_average, tests); 
        long double d_read_rate = deviation(rate_read, disk_read_rate_average, tests); 


        printf("\n");        
        printf("--------------------------Desvios padrões---------------------------\n");

        printf("| CPU 1 núcleo    | Real: %-10.4Lf s | CPU        %-10.4Lf s   |\n", d_single_real, d_single_cpu);
        printf("| CPU multithread | Real: %-10.4Lf s | CPU        %-10.4Lf s   |\n", d_multi_real,  d_multi_cpu);
        printf("| DISCO escrita   | Tempo: %-9.4Lf s | Velocidade %-9.2Lf MB/s |\n", d_write_time,  d_write_rate);
        printf("| DISCO leitura   | Tempo: %-9.4Lf s | Velocidade %-9.2Lf MB/s |\n", d_read_time,   d_read_rate);
        printf("--------------------------------------------------------------------\n");

        remove("teste.bin");
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

void cpu_test2 (long double *real_time_spent, long double *cpu_time_spent, int thread_count)
{
    using namespace std::chrono;

    high_resolution_clock::time_point real_time_initial = high_resolution_clock::now();
    clock_t cpu_time_initial = clock();
        
        std::vector<std::thread> threads;                               // Vetor de threads com a quantidade de threads da máquina
        std::vector<long long> results(thread_count);                   // Vetor para armazeanar os resultados de cada uma das threads
        long long total_sum = 0;                                        // Soma do resultado de cada uma das threads
        long long work_per_thread = VARIABLE_TEST_2 / thread_count;     // Tamanho do trabalho que cada thread deve executar
        

        // Inicialização de threads para executarem a função thread_function
        for (int i = 0; i < thread_count; ++i)
        {
            threads.emplace_back(thread_function, &results[i], work_per_thread);
        }

        
        // Esperamos pela conclusão de todos os threads para reuni-los
        for (auto &thread : threads)
        {
            thread.join();
        }

        //Soma do resultado de cada thread
        for(long long val : results) 
        {
            total_sum += val;
        }
    
    clock_t cpu_time_ending = clock();
    high_resolution_clock::time_point real_time_ending = high_resolution_clock::now();


    duration <long double> calc_real_time = duration_cast<duration<long double>>(real_time_ending - real_time_initial);
    long double calc_cpu_time  = static_cast<double>(cpu_time_ending  - cpu_time_initial) / CLOCKS_PER_SEC;

    *real_time_spent = calc_real_time.count();
    *cpu_time_spent  = calc_cpu_time;
}

void thread_function (long long *destiny, long long operations)
{
    long long local_counter = 0;
    for(long long i = 0; i < operations; i++) 
    {
        local_counter++;
    }
    *destiny = local_counter;
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

// Funções auxiliares para cácluos matemáticos repetitivos
long double deviation (const std::vector<long double>& vector, long double average, int n)
{
    if (n < 2) 
    {
        return 0.0;
    }

    long double sums = 0.0;

    for (const auto& value : vector)
    {
        long double difference = value - average;
        sums += difference * difference;
    }
    long double result = std::sqrt(sums / n);
    return result;
}

long double average(const std::vector<long double>& vector) 
{
    if (vector.empty())
    {
        return 0.0L;
    }

    long double sum = std::accumulate(vector.begin(), vector.end(), 0.0L);
    
    return sum / static_cast<long double>(vector.size());
}