#include <pthread.h>

struct locks {
    pthread_mutex_t buy_lock;
    pthread_mutex_t sell_lock;
};