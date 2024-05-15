#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define NUM_POINTS 1000000

long long points_inside_circle = 0;
sem_t sem;

void *generate_points(void *param) {
    int i;
    double x, y;
    long long local_count = 0;
    unsigned int seed = time(NULL) ^ pthread_self();

    for (i = 0; i < NUM_POINTS / NUM_THREADS; i++) {
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1) {
            local_count++;
        }
    }

    sem_wait(&sem);
    points_inside_circle += local_count;
    sem_post(&sem);

    return NULL;
}

int cpi() {
    pthread_t threads[NUM_THREADS];

    sem_init(&sem, 0, 1); // 세마포어 초기화, 세마포어 값은 1

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, generate_points, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double pi_estimate = 4.0 * points_inside_circle / NUM_POINTS;
    printf("Estimated value of Pi: %f\n", pi_estimate);

    sem_destroy(&sem);

    return 0;
}
