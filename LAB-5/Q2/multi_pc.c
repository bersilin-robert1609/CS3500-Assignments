#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// Global variables
// Number of producers and consumers
// Number of items to be produced and consumed
int p, pc, c, cc;

// Semaphore variables
sem_t mutex, empty, full;
// Buffer
int* stack;
// Top of the stack
int current_size = 0;

void get_arguments(int argc, char** argv)
{ // Get the arguments from the command line

    if(argc != 9)
    {
        printf("Usage: ./multi_pc -p <p> -c <c> -pc <pc> -cc <cc>");
        exit(1);
    }
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-p") == 0) p = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-pc") == 0) pc = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-c") == 0) c = atoi(argv[i+1]);
        else if(strcmp(argv[i], "-cc") == 0) cc = atoi(argv[i+1]);
    }
    return;
}

void* producer(void* arg)
{ // Producer thread

    int id = *((int*)arg);
    int count = pc;
    while(count > 0)
    {
        // wait for mutexes
        sem_wait(&empty);
        sem_wait(&mutex);
        // produce an item
        int item_code = rand() % 1000;
        stack[current_size++] = item_code;
        printf("Producer %d produced %d\n", id+1, item_code);
        count--;
        // signal mutexes
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}

void* consumer(void* arg)
{ // Consumer thread

    int id = *((int*)arg);
    int count = cc;
    while(count > 0)
    {
        // wait for mutexes
        sem_wait(&full);
        sem_wait(&mutex);
        // consume an item
        int item_code = stack[--current_size];
        printf("Consumer %d consumed %d\n", id+1, item_code);
        count--;
        // signal mutexes
        sem_post(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    // Get the arguments from the command line
    get_arguments(argc, argv);

    // Initialize the semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, p*pc);
    sem_init(&full, 0, 0);

    // Initialize the buffer
    stack = (int*)malloc(sizeof(int) * p*pc);

    // Initialize the producer and consumer threads
    pthread_t* producers = (pthread_t*)malloc(sizeof(pthread_t) * p);
    pthread_t* consumers = (pthread_t*)malloc(sizeof(pthread_t) * c);

    // Create the producer ids
    int* producer_ids = (int*)malloc(sizeof(int) * p);
    int* consumer_ids = (int*)malloc(sizeof(int) * c);
    for(int i = 0; i < p; i++) producer_ids[i] = i;
    for(int i = 0; i < c; i++) consumer_ids[i] = i;

    // Create the producer and consumer threads
    for(int i = 0; i < c; i++) pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    for(int i = 0; i < p; i++) pthread_create(&producers[i], NULL, producer, &producer_ids[i]);

    // Join the producer and consumer threads
    for(int i = 0; i < p; i++) pthread_join(producers[i], NULL);
    for(int i = 0; i < c; i++) pthread_join(consumers[i], NULL);
    return 0;
}