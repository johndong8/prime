#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>

#define BITMASK(b) (1 << ((b) % CHAR_BIT))
#define BITSLOT(b) ((b) / CHAR_BIT)
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1) / CHAR_BIT)

//#define MAX 4294967295
#define MAX 1000000

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
uint32_t bitarray[BITNSLOTS(MAX)];

void *find_prime(void *arg)
{
    int prime_count = 0;
    uint32_t i, j;
    int s;
    //memset(bitarray, 0, BITNSLOTS(MAX));
    memset(bitarray, 0, sizeof(bitarray));

    for (i = 2; i < MAX; i++){
        if (!BITTEST(bitarray, i)){
            s = pthread_mutex_lock(&mtx);
            if (s != 0)
                perror("pthread_mute_lock");
            prime_count++;
            s = pthread_mutex_unlock(&mtx);
            for (j = i + i; j < MAX; j += i)
                BITSET(bitarray, j);
        }

    }
    printf("%d \n", prime_count);
    return 0;

}


int main(int argc, char **argv)
{
    pthread_t *thread;
    pthread_attr_t attr;
    int num_threads;
    num_threads = atoi(argv[1]);
    int i;
    //int *prime_counter = 0;
    
    thread = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    pthread_attr_init(&attr);

    for (i = 0; i < num_threads; i++){

        if (pthread_create(&thread[i], &attr, find_prime, (void*)i) != 0){

            perror("thread creation failure");
            exit(-1);
        }
    }

    for (i = 1; i < num_threads; ++i) 
        pthread_join(thread[i], NULL);
    
    //find_prime(0);
    //printf("%d \n", prime_count);

    pthread_mutex_destroy(&mtx);
    pthread_exit(NULL);
    return 0;


}
