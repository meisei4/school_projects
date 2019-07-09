/*
 * 
 *
 * CS 441/541: Bounded Buffer (Project 4)
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "lib/semaphore_support.h"

/*****************************
 * Defines
 *****************************/
#define USEC 1000000
#define RND_MX 10
/*****************************
 * Structures
 *****************************/


/*****************************
 * Global Variables
 *****************************/
int * buffer;
int productions, consumptions;
int indx, i, bufsize, prod_threads, cons_threads;
int sleep_len;
time_t start;
time_t endwait;

int c, p;
pthread_t *prods; 
pthread_t *cons;
semaphore_t *mutex;
semaphore_t *mootex;
semaphore_t *fill_cnt;
semaphore_t *empty_cnt;

/*****************************
 * Function Declarations
 *****************************/
void initialize();
void join_threads();
int remove_item();
int insert_item();
void print_header();
void print_info();
void print_buffer();
void print_footer();
void *consume();
void *produce();
