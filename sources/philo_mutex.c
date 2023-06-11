#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 5

typedef struct
{
    int id;
    pthread_mutex_t fork_mutex;
    pthread_cond_t forks_condition;
    int forks_available;

} Philosopher;

Philosopher philosophers[NUM_PHILOSOPHERS];

void think(int philosopher_id)
{
    printf("Philosopher %d is thinking.\n", philosopher_id);
    sleep(rand() % 2);
}

void eat(int philosopher_id)
{
    printf("Philosopher %d is eating.\n", philosopher_id);
    sleep(rand() % 2);
}

void take_forks(int philosopher_id) {
    int left_philosopher_id = philosopher_id;
    int right_philosopher_id = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    if (philosopher_id % 2 == 0) {
        left_philosopher_id = (philosopher_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
        right_philosopher_id = philosopher_id;
    }

    pthread_mutex_lock(&philosophers[left_philosopher_id].fork_mutex);
    while (philosophers[left_philosopher_id].forks_available == 0) {
        pthread_cond_wait(&philosophers[left_philosopher_id].forks_condition, &philosophers[left_philosopher_id].fork_mutex);
    }

    pthread_mutex_lock(&philosophers[right_philosopher_id].fork_mutex);
    while (philosophers[right_philosopher_id].forks_available == 0) {
        pthread_cond_wait(&philosophers[right_philosopher_id].forks_condition, &philosophers[right_philosopher_id].fork_mutex);
    }

    philosophers[left_philosopher_id].forks_available = 0;
    philosophers[right_philosopher_id].forks_available = 0;
    printf("Philosopher %d takes the forks.\n", philosopher_id);
}

void put_forks(int philosopher_id) {
    int left_philosopher_id = philosopher_id;
    int right_philosopher_id = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    if (philosopher_id % 2 == 0) {
        left_philosopher_id = (philosopher_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
        right_philosopher_id = philosopher_id;
    }

    philosophers[left_philosopher_id].forks_available = 1;
    philosophers[right_philosopher_id].forks_available = 1;
    printf("Philosopher %d puts the forks back.\n", philosopher_id);

    pthread_mutex_unlock(&philosophers[left_philosopher_id].fork_mutex);
    pthread_mutex_unlock(&philosophers[right_philosopher_id].fork_mutex);

    pthread_cond_signal(&philosophers[left_philosopher_id].forks_condition);
    pthread_cond_signal(&philosophers[right_philosopher_id].forks_condition);

    sleep(rand() % 2);
}

void *philosopher_process(void *arg)
{
    int philosopher_id = *(int *)arg;

    while (1)
    {
        think(philosopher_id);

        take_forks(philosopher_id);

        eat(philosopher_id);

        put_forks(philosopher_id);
    }

    return NULL;
}

int main()
{
    srand(time(NULL));

    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosopher_ids[i] = i;
        pthread_mutex_init(&philosophers[i].fork_mutex, NULL);
        pthread_cond_init(&philosophers[i].forks_condition, NULL);
        philosophers[i].forks_available = 1;
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        if (pthread_create(&threads[i], NULL, philosopher_process, &philosopher_ids[i]) != 0)
        {
            printf("Error creating thread for philosopher %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        pthread_mutex_destroy(&philosophers[i].fork_mutex);
        pthread_cond_destroy(&philosophers[i].forks_condition);
    }

    return 0;
}
