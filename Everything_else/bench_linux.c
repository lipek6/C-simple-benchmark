#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


// Troque os valores dos macros à baixo para modificar algumas configurações dos testes
#define MAX_AMOUNT_OF_TESTS 10      // Quantidade máxima de testes
#define VARIABLE_TEST_1 10000000000 // Somar +1 10 Bilhões de vezes
#define VARIABLE_TEST_2 10000000000 // Somar +1 10 Bilhões de vezes
#define MAX_AMOUNT_OF_THREADS 16    // Quantidade máxima de threads

// Declaração das funções que usaremos, você verá essas funções abaixo de main()
void cpu_test1 (double *time_spent);
void cpu_test2 (double *time_spent, int thread_count);
void *thread_function(void *arg);


// Bloco principal de código
int main (void)
{

    int tests = 1;        // Quantidade de testes que serão realizados (input sobrescreverá o 1)
    int thread_count = 1; // Quantidade de threads que serão gerados (input sobrescreverá o 1)

    printf("Iniciando processos de benchmark da sua máquina\n");
    printf("Determine quantas vezes você gostaria de rodar os testes: ");
    scanf("%i", &tests);

    while(!(tests <= MAX_AMOUNT_OF_TESTS))
    {
        printf("Máximo de %i testes, digite novamente: ", (int) MAX_AMOUNT_OF_TESTS);
        scanf("%i", &tests);
        if (tests <= MAX_AMOUNT_OF_TESTS)
        {
            break;
        }
    }
    
    // Declaração de variáveis que serão enviadas para cada função_teste via endereço
    double time_1[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double time_2[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};



    // Início dos testes
    printf("Estressando o CPU:\n");


    printf("1. Estressando apenas 1 núcleo\n");
    for(int i = 0; i < tests; i++)
    {
        cpu_test1(&time_1[i]);
        printf("Tempo de execução %i: %f segundos\n", (i+1), time_1[i]);
    }

    printf("2. Estressando múltiplos núcleos\n");
    printf("Digite quantos núcleos lógicos (threads) sua máquina têm: ");
    scanf("%i", &thread_count);
    for(int i = 0; i < tests; i++)
    {
        cpu_test2(&time_2[i], thread_count);
        printf("Tempo de execução %i: %f segundos\n", (i+1), time_2[i]);
    }
}



// Funções contendo testes de benchmark
void cpu_test1 (double *time_spent)
{
    //1. Teste de contínuos incrementos de 1
    volatile long long i = 0; // É volátil
    
    clock_t begin, end;
    
    begin = clock();
        while(i < VARIABLE_TEST_1)
        {
            i++;
        }
    end = clock();
    
    *time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    // *time_spent, editamos diretamente o conteúdo de time_1 em main(), pois estamos usando ponteiros para a variável e não cópias da mesma
}

void cpu_test2 (double *time_spent, int thread_count)
{
    pthread_t thread[MAX_AMOUNT_OF_THREADS];

    clock_gettime(CLOCK_MONOTONIC, &start_time);
    clock_t begin, end;
    begin = clock();

        for(int i = 0; i < thread_count; i++)
        {
            // pthread_create já chama a função da thread como argumento e começa à operar
            if(pthread_create(&thread[i], NULL, thread_function, NULL) != 0)
            {
                perror("Erro ao criar thread");
                printf("Deu merda aqui em '\n");
                break;
            }
        }
        
        for (int i = 0; i < thread_count; i++)
        {
            pthread_join(thread[i], NULL);
        }

    end = clock();

    *time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
}

void *thread_function(void *arg)
{
    volatile long long i = 0;
    
    while(i < VARIABLE_TEST_2)
    {
        i++;
    }
    pthread_exit(NULL);
}