#include <stdio.h>
#include <pthread.h>
#include <math.h>

#define PROJECT_NAME "DemoApp"

int counter = 0;
pthread_mutex_t t_mutex;

static void *worker(void *arg) {
    (void)arg;
    double v = sin(0.5);

    pthread_mutex_lock(&t_mutex);
    counter += (int)(v * 1000.0);
    pthread_mutex_unlock(&t_mutex);
    
    return NULL;
}

int main(void) {
    printf("Project: %s\n", PROJECT_NAME);
    pthread_t th[2];
    pthread_mutex_init(&t_mutex, NULL);
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&th[i], NULL, worker, NULL) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(th[i], NULL);
    }

    printf("counter=%d\n", counter);
    pthread_mutex_destroy(&t_mutex);
    return 0;
}
