
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void die(char *s)
{
    if (errno)
        perror(s);
    else
        fprintf(stderr, "Error: %s\n", s);
    exit(EXIT_FAILURE);
}

void check_pthread_return(int rv, char *msg)
{
    if (rv == 0)
        return;
    errno = rv;
    die(msg);
}

#define DEFAULT_NUM_JOBS 20
#define DEFAULT_NUM_PRINTERS 2

#define print_job(x) usleep((x)*100 + 50)

typedef struct {
    unsigned char *jobs;
    int njobs_fetched;
    int njobs_max;
    pthread_mutex_t mutex;
} job_queue_t;

typedef struct {
    int id;
    job_queue_t *jq;
    int njobs;
    pthread_t thread_id;
} printer_t;

int q_init(job_queue_t *q, int max_jobs)
{
    q->njobs_fetched = 0;
    q->njobs_max = max_jobs;
    q->jobs = malloc(max_jobs);

    if (q->jobs == NULL)
        die("malloc()");

    for (int i = 0; i < max_jobs; i++)
        q->jobs[i] = rand() % 100 + 1;

    return 0;
}

int q_num_jobs(job_queue_t *q)
{
    return q->njobs_max - q->njobs_fetched;
}

int q_fetch_job(job_queue_t *q, int id)
{
    int r = q_num_jobs(q);

    if (r <= 0) {
        fprintf(stderr, "Error: printer %2d tries to get a job "
                "after the max number of jobs have been fetched.", id);
        exit(EXIT_FAILURE);
    }

    printf("Printer  %2d fetched   job %2d\n",
           id, q->njobs_fetched);

    return q->jobs[q->njobs_fetched++];
}

void q_destroy(job_queue_t *q)
{
    if (q->jobs) {
        free(q->jobs);
        q->jobs = NULL;
    }
}

void print_printer_summary(printer_t *pprinter, int nprinters)
{
    int total = 0;

    for (int i = 0; i < nprinters; i++, pprinter++) {
        printf("Printer  %2d completed %d jobs.\n",
               pprinter->id, pprinter->njobs);

        total += pprinter->njobs;
    }

    printf("Total completed is %d.\n", total);
}

void printer_single(printer_t *pprinter)
{
    int done = 0;
    job_queue_t *jq = pprinter->jq;

    pprinter->njobs = 0;

    while (!done) {
        int r = q_num_jobs(jq);

        if (r <= 0) {
            done = 1;
            continue;
        }

        int job = q_fetch_job(jq, pprinter->id);

        print_job(job);

        pprinter->njobs++;
    }
}

void *printer_main(void *arg)
{
    printer_t *pprinter = arg;
    job_queue_t *jq = pprinter->jq;

    while (1) {
        check_pthread_return(
            pthread_mutex_lock(&jq->mutex),
            "pthread_mutex_lock");

        if (q_num_jobs(jq) <= 0) {
            check_pthread_return(
                pthread_mutex_unlock(&jq->mutex),
                "pthread_mutex_unlock");

            break;
        }

        int job = q_fetch_job(jq, pprinter->id);

        check_pthread_return(
            pthread_mutex_unlock(&jq->mutex),
            "pthread_mutex_unlock");

        print_job(job);

        pprinter->njobs++;
    }

    return arg;
}

int main(int argc, char *argv[])
{
    int num_printers = DEFAULT_NUM_PRINTERS;
    int num_jobs = DEFAULT_NUM_JOBS;
    int demo = 0;

    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            num_printers = atoi(argv[++i]);

            if (num_printers <= 0)
                die("number of printers must be positive.");

        } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
            num_jobs = atoi(argv[++i]);

            if (num_jobs <= 0)
                die("number of jobs must be positive.");

        } else if (strcmp(argv[i], "-d") == 0) {
            demo = 1;

        } else {
            fprintf(stderr, "Usage: %s [-p N] [-j N]\n", argv[0]);
            fprintf(stderr, "-p <n> : number of printers.\n");
            fprintf(stderr, "-j <n> : total number of jobs.\n");
            fprintf(stderr, "-d     : demo mode.\n");

            return 1;
        }
    }

    printf("num_printers=%d\n", num_printers);
    printf("num_jobs=%d\n", num_jobs);

    unsigned int seed = ((unsigned int)num_jobs << 24) ^
                        ((unsigned int)num_printers << 8);

    srand(seed);

    job_queue_t job_queue;

    q_init(&job_queue, num_jobs);

    printer_t printers[num_printers];

    if (demo) {
        printf("Demo: there is only one printer.\n");

        printers[0].id = 0;
        printers[0].jq = &job_queue;

        printer_single(printers);

        q_destroy(&job_queue);

        print_printer_summary(printers, 1);

        exit(0);
    }

    check_pthread_return(
        pthread_mutex_init(&job_queue.mutex, NULL),
        "pthread_mutex_init");

    for (i = 0; i < num_printers; i++) {
        printers[i].id = i;
        printers[i].jq = &job_queue;
        printers[i].njobs = 0;

        check_pthread_return(
            pthread_create(
                &printers[i].thread_id,
                NULL,
                printer_main,
                &printers[i]),
            "pthread_create");
    }

    for (i = 0; i < num_printers; i++) {
        check_pthread_return(
            pthread_join(printers[i].thread_id, NULL),
            "pthread_join");
    }

    check_pthread_return(
        pthread_mutex_destroy(&job_queue.mutex),
        "pthread_mutex_destroy");

    q_destroy(&job_queue);

    print_printer_summary(printers, num_printers);

    return 0;
}
