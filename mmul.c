
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "matrix.h"

#define NUM_THREADS 2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void check_pthread_return(int rv, const char *msg)
{
    if (rv != 0) {
        fprintf(stderr, "%s: %s\n", msg, strerror(rv));
        exit(EXIT_FAILURE);
    }
}

static void *thread_main(void *p_arg)
{
    thread_arg_t *arg = p_arg;

    TMatrix *m = arg->m;
    TMatrix *n = arg->n;
    TMatrix *t = arg->t;

    unsigned int mid = m->nrows / NUM_THREADS;

    unsigned int start = arg->id == 0 ? 0 : mid;
    unsigned int end = arg->id == 0 ? mid : m->nrows;

    for (unsigned int i = start; i < end; i++) {
        for (unsigned int j = 0; j < t->ncols; j++) {

            TElement sum = 0;

            for (unsigned int k = 0; k < m->ncols; k++)
                sum += m->data[i][k] * n->data[j][k];

            t->data[i][j] = sum;
        }
    }

    return NULL;
}

TMatrix *mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (m == NULL || n == NULL ||
        m->ncols != n->nrows)
        return NULL;

    TMatrix *t = newMatrix(m->nrows, n->ncols);

    if (t == NULL)
        return NULL;

    TMatrix *nt = transposeMatrix(n);

    if (nt == NULL) {
        freeMatrix(t);
        return NULL;
    }

    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = nt;
        args[i].t = t;

        check_pthread_return(
            pthread_create(&threads[i], NULL,
                           thread_main, &args[i]),
            "pthread_create");
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        check_pthread_return(
            pthread_join(threads[i], NULL),
            "pthread_join");
    }

    freeMatrix(nt);

    return t;
}
