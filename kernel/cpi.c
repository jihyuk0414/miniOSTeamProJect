#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define NUM_POINTS 1000000

long long points_inside_circle = 0;
pthread_mutex_t lock;

void *generate_points(void *param){
    int i;
    double x, y;
    long long local_count = 0;
    unsigned int seed = time(NuLL) ^ pthread_self();

    for(i = 0; i < NUM_POINTS / NUM_THREADS; i++) {
        x = (double)rand_r(&seed) / RAND_MAX;
        y = (double)rand_r(&seed) / RAND_MAX;
        if (x*x + y*y <= 1){
            local_count++;
        }
    }
    pthread_mutex_lock(&lock);
    points_inside_circle += local_count;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int cpi() {
    pthread_t threads[NUM_THREADS];
    int i;

    pthread_mutex_init(&lock, NULL);

    for(i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, generate_points, NULL);
    }
    for(i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    double pi_estimate = 4.0 * points_inside_circle / NUM_POINTS;
    printf("Estimated value of n: %f\n", pi_estimate);

    pthread_mutex_destroy(&lock);

    return 0;
}