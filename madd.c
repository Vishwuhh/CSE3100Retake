#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;


static void * thread_main(void * p_arg)
{
    thread_arg_t *arg = p_arg;
    unsigned int middle = arg->m->nrows / NUM_THREADS;
    unsigned int start = (arg->id == 0) ? 0 : middle;
    unsigned int end = (arg->id == 0) ? middle : arg->m->nrows;

    for (unsigned int i = start; i < end; i++) {
        for (unsigned int j = 0; j < arg->m->ncols; j++) {
            arg->t->data[i][j] = arg->m->data[i][j] + arg->n->data[i][j];
        }
    }

    return NULL;
}

TMatrix * addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (m == NULL || n == NULL
        || m->nrows != n->nrows || m->ncols != n->ncols)
        return NULL;

    TMatrix *t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return NULL;

    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;

        int status = pthread_create(&threads[i], NULL, thread_main, &args[i]);
        if (status != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        int status = pthread_join(threads[i], NULL);
        if (status != 0) {
            fprintf(stderr, "pthread_join failed: %d\n", status);
            exit(EXIT_FAILURE);
        }
    }

    return t;
}
