#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THINKING 1
#define EATING 0
#define EATING_TIME 100
#define LEFT (phil_num + N - 1) % N
#define RIGHT (phil_num + 1) % N
#define THINKING_TIME rand()%51 + 50
#define EAT_LIMIT 5

int N; // number of philosophers
int* state; // state of each philosopher
int* phil; // philosopher number
sem_t mutex; // mutex lock

void check_if_eating_possible(int phil_num)
{
    if(state[phil_num] == THINKING && state[LEFT] != EATING && state[RIGHT] != EATING) 
        state[phil_num] = EATING;
}

void manage_args(int argc, char** argv)
{
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
}

void *function(void *phil_no)
{ // philosopher function

    int phil_num = *((int *)phil_no);   // philosopher number
    int count = EAT_LIMIT;              // number of times to eat
    int time_el = 0;                    // time elapsed
    int first_think = 1;                // flag to check if first time thinking

    while(count > 0)
    { // while not done eating

        while(state[phil_num] != EATING)
        { // while not eating
            int think_time = THINKING_TIME; // time to think
            if(first_think)
            {
                printf("<%d> Philosopher %d in THINKING state\n", time_el, phil_num);
                first_think = 0;
            }

            usleep(think_time*1000); // think
            time_el += think_time; // update time elapsed

            // check if neighbours are eating
            sem_wait(&mutex);
            check_if_eating_possible(phil_num);
            sem_post(&mutex);
        }
        // eating time

        printf("<%d> Philosopher %d in EATING state - %d\n", time_el, phil_num, 6-count);

        usleep(EATING_TIME*1000); // eat for 100 ms
        time_el += EATING_TIME; // update time elapsed

        count--; // update count
        state[phil_num] = THINKING; // update state
        first_think = 1; // set flag to 1
    }
    return NULL;
}

void init_join_threads(pthread_t* threads)
{
    for(int i = 0; i < N; i++) 
    {
        state[i] = THINKING;
        pthread_create(&threads[i], NULL, function, &phil[i]); // create threads
    }
    for(int i = 0; i < N; i++)
        pthread_join(threads[i], NULL); // join threads
}

int main(int argc, char** argv)
{ // main function

    srand(time(NULL));
    manage_args(argc, argv); // manage arguments
    
    state = (int*)malloc(N*sizeof(int));
    phil = (int*)malloc(N*sizeof(int));
    for(int i = 0; i < N; i++) phil[i] = i;

    sem_init(&mutex, 0, 1); // initialize mutex lock

    pthread_t* threads = (pthread_t*)malloc(N*sizeof(pthread_t)); // array of threads
    init_join_threads(threads); // initialize threads

    return 0;
}