#include "header.h"

static int stop = 0;
static pthread_mutex_t stop_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

static int enqueue(struct LogMsg *m) 
{
    pthread_mutex_lock(&queue_mutex);
    size_t next = (head + 1) % QCAP;
    if (next == tail) {
        pthread_mutex_unlock(&queue_mutex);
        return -1;
    }
    queue[head] = m;
    head = next;
    pthread_mutex_unlock(&queue_mutex);
    return 0;
}


static struct LogMsg* dequeue(void)
 {
    pthread_mutex_lock(&queue_mutex);
    if (tail == head) {
        pthread_mutex_unlock(&queue_mutex);
        return NULL; 
    }
    struct LogMsg *m = queue[tail];
    tail = (tail + 1) % QCAP;
    pthread_mutex_unlock(&queue_mutex);
    return m;
}

static void *producer_thread(void *arg) 
{
    (void)arg;

    const char *samples[] = {
        "system started",
        "user login ok",
        "temperature warning: 83C",
        "restarting worker 7",
        "network: rx=12345 tx=67890",
        "db: slow query detected",
        "cron: ran backup job",
        "audit: permission denied",
        "daemon: rotating logs",
        "metrics: p99=12.3ms"
    };
    const size_t NS = sizeof(samples)/sizeof(samples[0]);

    for (int i = 0; i < 5000; i++) {
        struct LogMsg *m = (struct LogMsg*)malloc(sizeof *m);
        if (!m) continue;

        const char *src = samples[(unsigned)i % NS];

        size_t want = strlen(src);
        m->len = want;
        // strncpy(m->buf, src, want); THIS IS UNSAFE
        strncpy(m->buf, src, sizeof(m->buf)-1); 
        m->buf[sizeof(m->buf)-1] = '\0';
        
        if (enqueue(m) != 0)
            free(m);
        if ((i % 100) == 0) {
            usleep(1000);
        }
    }
    pthread_mutex_lock(&stop_mutex);
    stop = 1;
    pthread_mutex_unlock(&stop_mutex);
    return NULL;
}

static void *consumer_thread(void *arg) {
    (void)arg;
    int local_stop;

    while (1) {
        pthread_mutex_lock(&stop_mutex);
        local_stop = stop;
        pthread_mutex_unlock(&stop_mutex);


        if (local_stop && (tail == head)) {
            break;
        }

        struct LogMsg *m = dequeue();
        if (!m)
            usleep(100);
        else 
        {
            fprintf(logfp ? logfp : stdout, "LOG: %s\n", m->buf);
            free(m);
        }
    }
    return NULL;
}

int main(void) {
    logfp = fopen("racey.log", "w");
    setvbuf(logfp ? logfp : stdout, NULL, _IOLBF, 0);

    pthread_t prod, cons;

    if (pthread_create(&prod, NULL, producer_thread, NULL) != 0) {
        perror("pthread_create prod");
        return 1;
    }
    if (pthread_create(&cons, NULL, consumer_thread, NULL) != 0) {
        perror("pthread_create cons");
        return 1;
    }

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    if (logfp) 
        fclose(logfp);
    return 0;
}

