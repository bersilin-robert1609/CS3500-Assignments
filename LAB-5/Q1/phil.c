#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int N; // number of philosophers
int* state; // state of each philosopher
int* phil; // philosopher number
sem_t mutex; // mutex lock
sem_t* sem_array; // semaphore array
int time_el = 0; // time elapsed
int* time_eat; // time each philosopher has eaten

#define THINKING 0
#define EATING 1
#define EATING_TIME 100
#define LEFT (phil_num + (N-1)) % N
#define RIGHT (phil_num + 1) % N
#define THINKING_TIME rand()%51 + 50
#define EAT_LIMIT 5

int check_if_eating_possible(int phil_num)
{
    if (state[phil_num] == THINKING && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[phil_num] = EATING;
        printf("<%d> Philosopher %d is Eating\n", time_el, phil_num + 1);
        time_el += EATING_TIME;
        sem_post(&sem_array[phil_num]);
    }
}

int eat_if_possible(int phil_num)
{
    int p1 = check_if_eating_possible(phil_num);
}

void *function(void *phil_no)
{
    while(1)
    {
        int phil_num = *(int *)phil_no;
        sem_wait(&mutex);
        time_el += THINKING_TIME;
        int result = eat_if_possible(phil_num);
        if(result == 0)
        {
            printf("<%d> Philosopher %d is Thinking\n", time_el, phil_num + 1);
        }
        sem_post(&mutex);
    }
}

int main(int argc, char* argv)
{
    srand(time(NULL));
    if(argc != 2)
    {
        printf("Usage: %s <number>\n", argv[0]);
        exit(1);
    }
    N = atoi(argv[1]);
    if(N < 2)
    {
        printf("Number of philosophers must be greater than 1\n");
        exit(1);
    }

    state = (int*)malloc(N*sizeof(int));
    phil = (int*)malloc(N*sizeof(int));
    time_eat = (int*)malloc(N*sizeof(int));
    for(int i = 0; i < N; i++) 
    {
        time_eat[i] = 0;
        phil[i] = i;
    }
    
    sem_array = (sem_t*)malloc(N*sizeof(sem_t));

    for(int i = 0; i < N; i++)
        sem_init(&sem_array[i], 0, 0);

    sem_init(&mutex, 0, 1);

    pthread_t* threads = (pthread_t*)malloc(N*sizeof(pthread_t));
    for(int i = 0; i < N; i++) 
    {
        pthread_create(&threads[i], NULL, function, &phil[i]);
        state[i] = THINKING;
        printf("<%d> Philosopher %d is thinking\n", time_el, phil[i]);
    }

    return 0;
}