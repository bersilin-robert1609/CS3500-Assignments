#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>

long** matrix[5];
struct rusage const_usage;
long const_mem;

void matrix_transpose(long** matrix, long m, long n)
{
    for (long i = 0; i < m; i++)
    {
        for (long j = i + 1; j < n; j++)
        {
            long temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void create_matrices(int n)
{
    for (int i = 0; i < 5; i++)
    {
        matrix[i] = (long**)malloc(n * sizeof(long*));
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = (long*)malloc(n * sizeof(long));
            for (int k = 0; k < n; k++)
            {
                matrix[i][j][k] = rand();
            }
        }
    }
}

void process(int n)
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long mem = usage.ru_maxrss;
    printf("memory_usage: %ld + %ld, ", const_mem, mem - const_mem);
    for(int i=0; i<5; i++) matrix_transpose(matrix[i], n, n);
    printf("page_faults: %lu\n", usage.ru_minflt);
}

int main()
{
    srand(0);
    long n;
    printf("Enter n value: ");
    scanf("%ld", &n);
    printf("Enter Choice\n");
    printf("Memory Allocated once - 1\n");
    printf("Memory Allocated each time - 2\n");
    long choice;
    scanf("%ld", &choice);

    getrusage(RUSAGE_SELF, &const_usage);
    const_mem = const_usage.ru_maxrss;

    if(choice == 1)
    {
        create_matrices(n);
        for(long i=0; i<10; i++) process(n);
    }
    else if(choice == 2)
    {
        for (long i = 0; i < 10; i++)
        {
            create_matrices(n);
            process(n);
        }
    }
    else
    {
        printf("Invalid Choice\n");
    }
    return 0;
}