#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>

long** matrix[5];

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

long main()
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

    struct rusage const_usage;
    getrusage(RUSAGE_SELF, &const_usage);
    long const_mem = const_usage.ru_maxrss;

    if(choice == 1)
    {
        for(long i=0; i<5; i++) matrix[i] = (long **)malloc(n * sizeof(long *));
        for(long mat_index = 0; mat_index < 5; mat_index++)
        {
            for (long j = 0; j < n; j++)
            {
                matrix[mat_index][j] = (long *)malloc(n * sizeof(long));
            }
        }
        for(long mat_index = 0; mat_index < 5; mat_index++)
        {
            for(long i = 0; i < n; i++)
            {
                for(long j = 0; j < n; j++)
                {
                    matrix[mat_index][i][j] = rand() % 100;
                }
            }
        }
        for(long i=0; i<10; i++)
        {
            struct rusage usage;
            getrusage(RUSAGE_SELF, &usage);
            long mem = usage.ru_maxrss;
            printf("memory usage: %ld + %ld, ", const_mem, mem - const_mem);
            for(int i=0; i<5; i++) matrix_transpose(matrix[i], n, n);
            printf("page faults: %lu\n", usage.ru_minflt);
        }
    }
    else if(choice == 2)
    {
        for (long i = 0; i < 10; i++)
        {
            long **matrix = (long **)malloc(n * sizeof(long *));
            for (long i = 0; i < n; i++)
            {
                matrix[i] = (long *)malloc(n * sizeof(long));
            }
            struct rusage usage;
            getrusage(RUSAGE_SELF, &usage);
            long mem = usage.ru_maxrss;
            printf("memory usage: %d + %d, ", const_mem, mem - const_mem);
            for(long i=0; i<n; i++)
            {
                for(long j=0; j<n; j++)
                {
                    matrix[i][j] = rand();
                }
            }
            matrix_transpose(matrix, n, n);
            printf("page faults: %lu\n", usage.ru_minflt);
        }
    }
    else
    {
        printf("Invalid Choice\n");
    }
}