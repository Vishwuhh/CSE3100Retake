#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define NUM_THREADS 2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void *thread_main(void *p_arg)
{
    thread_arg_t *arg = p_arg;
    unsigned int middle = arg->m->nrows / NUM_THREADS;
    unsigned int start = arg->id == 0 ? 0 : middle;
    unsigned int end = arg->id == 0 ? middle : arg->m->nrows;
    unsigned int ncols = arg->m->ncols;

    for (unsigned int i = start; i < end; i++) {
        const TElement *a = arg->m->data[i];
        const TElement *b = arg->n->data[i];
        TElement *out = arg->t->data[i];
        for (unsigned int j = 0; j < ncols; j++)
            out[j] = a[j] + b[j];
    }
    return NULL;
}

TMatrix *addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (m == NULL || n == NULL || m->nrows != n->nrows || m->ncols != n->ncols)
        return NULL;
    TMatrix *t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return NULL;

    pthread_t worker;
    thread_arg_t args[NUM_THREADS] = {
        {0, m, n, t},
        {1, m, n, t}
    };

    int status = pthread_create(&worker, NULL, thread_main, &args[1]);
    if (status != 0) {
        fprintf(stderr, "pthread_create failed: %d\n", status);
        exit(EXIT_FAILURE);
    }

    thread_main(&args[0]);

    status = pthread_join(worker, NULL);
    if (status != 0) {
        fprintf(stderr, "pthread_join failed: %d\n", status);
        exit(EXIT_FAILURE);
    }
    return t;
}