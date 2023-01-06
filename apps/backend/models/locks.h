#include <pthread.h>

struct locks {
    pthread_mutex_t promotions_lock;
};