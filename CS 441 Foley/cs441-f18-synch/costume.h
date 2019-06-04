/*
 *
 *
 * CS 441/541: Pirates and Ninjas (Project 5)
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
 * Global Variables
 *****************************/
int sleep_len;
int booth_cnt;
int ninja_threads;
int pirate_threads;
semaphore_t Nmutex, Pmutex, Costume_Nmutex, Costume_Pmutex, costume_shop_switch_pirate, costume_shop_switch_ninja;
int * n_leave;
int * p_leave;
int * n_enter;
int * p_enter;
int NLeave, PLeave, NEnter, PEnter = 0;
int booth_pirate, booth_ninja = 0;
int queue_count, line_count_ninja, line_count_pirate = 0;
int *queue_ninja;
int *queue_pirate;
//list of semaphores for each individual ninja and pirate
semaphore_t *sem_ninja;
semaphore_t *sem_pirate;
int queue_count_ninja, queue_count_pirate = 0;
int front_ninja = 0;
int end_ninja = -1;
int front_pirate = 0;
int end_pirate = -1;
int ninja_c, pirate_c, costume_shop_c = 0;
int sem = 0;
int Ncount;
int Pcount;

/*****************************
 * Function Declarations
 *****************************/
//CLEANING
void print_footer();
void print_header();
void initialize_vars();
int create_sem(semaphore_t * s, int init);

//NINJAS AND PIRATES
void *ninja(void *ptr);
void *pirate(void *ptr);
//COSTUMERS
void *costume_shop_ninja(void *ptr);
void *costume_shop_pirate(void *ptr);

int wait(semaphore_t s);
int post(semaphore_t s);

int is_full_ninja();
int is_full_pirate();

void append_ninja(int element);
void append_pirate(int element);

int pop_ninja();
int pop_pirate();

void print_ninja();
void print_pirate();
