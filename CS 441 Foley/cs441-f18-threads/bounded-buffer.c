/*
 * 
 *
 * CS 441/541: Bounded Buffer (Project 4)
 *
 */
#include "bounded-buffer.h"

int main(int argc, char * argv[]) {
    int rc;
    if (argc < 3) {
        return 0;
    }
    sleep_len = atoi(argv[1]);
    prod_threads = atoi(argv[2]);
    cons_threads = atoi(argv[3]);
    if (argc == 5) {
        bufsize = atoi(argv[4]);
    } else {
        bufsize = 10;
    }
    if(!sleep_len || !prod_threads || !cons_threads || !bufsize){
        printf("One of the command arguments failed\n"); 
        return -1;
    }
    //initialize global variables and semaphores
    initialize();
    //fill the buffer with -1s
    buffer = malloc(sizeof(int) * bufsize);
    for (i = 0; i < bufsize; i++) {
        buffer[i] = -1;
    }
    print_header();
    print_buffer();
    srandom(time(NULL));
    prods = malloc(sizeof(pthread_t) * prod_threads);
    cons = malloc(sizeof(pthread_t) * cons_threads);
    start = time(NULL);
    endwait = start + sleep_len;
    for (i = 0; i < prod_threads; i++) {
        prods[i] = malloc(sizeof(pthread_t));
        rc = pthread_create(prods[i], NULL, produce, (void *)(intptr_t)i);
        if(rc != 0){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }
    for (i = 0; i < cons_threads; i++) {
        cons[i] = malloc(sizeof(pthread_t));
        rc = pthread_create(cons[i], NULL, consume, (void *)(intptr_t)i);
        if(rc != 0){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
        }
    }
    sleep(sleep_len);
    join_threads();
    print_footer();
    pthread_exit(NULL);
    return 0;
}

void *produce(void *threadid){
    int item, rc;
    while (start < endwait) {
        usleep(rand() % USEC);
        item = rand() % 10;
        semaphore_wait(empty_cnt); //block while the empty_cnt is 0
        semaphore_wait(mutex);     //wait to access and change buffer data
        rc = insert_item(item);
        semaphore_wait(mootex);    //wait to access the print section
        if (rc == 0) {
            print_info(1, item, threadid);
            print_buffer();
        }
        semaphore_post(mootex);    //signal access to print section
        semaphore_post(mutex);     //signal mutex because buffer has been changed.
        semaphore_post(fill_cnt);  //signal fill_cnt and increment it
        start = time(NULL);
    }
    //open up all the locks to prevent deadlock on consumer if finishes first
    semaphore_post(mootex);
    semaphore_post(mutex);
    semaphore_post(fill_cnt);
    pthread_exit(NULL);
}

void *consume(void *threadid){
    int item, rc;
    while (start < endwait) {
        usleep(rand() % USEC);
        semaphore_wait(fill_cnt);  //block while the fill_cnt is 0
        semaphore_wait(mutex);     //wait to access and change buffer data
        item = buffer[indx % bufsize];
        rc = remove_item(&item);
        semaphore_wait(mootex);    //wait to access print section
        if (rc == 0) {
            print_info(0, item, threadid);
            print_buffer();
        }
        semaphore_post(mootex);
        semaphore_post(mutex);     //signal done with buffer edit
        semaphore_post(empty_cnt); //signal empty_cnt and decrement it.
        start = time(NULL);
    }
    //open locks to prevent deadlock on producer if finishes first
    semaphore_post(mootex);
    semaphore_post(mutex);
    semaphore_post(empty_cnt);
    pthread_exit(NULL);
}

int remove_item(int *item){
    if (indx < 0 || buffer[c % bufsize] == -1) {
        return -1;
    }
    consumptions++;
    *item = buffer[c % bufsize];
    buffer[c % bufsize] = -1;
    c++;
    return 0;
}

int insert_item(int item){
    if (buffer[p % bufsize] != -1) {
        return -1; 
    }
    productions++;
    buffer[p % bufsize] = item;
    indx++;
    p++;
    return 0;
}

//join all the threads 
void join_threads(){
    int i;
    for (i = 0; i < prod_threads; i++) {
        pthread_join(*prods[i], NULL);
    }
    for (i = 0; i < cons_threads; i++) {
        pthread_join(*cons[i], NULL);
    }
}

//intialize variables
void initialize(){
    mutex = malloc(sizeof(semaphore_t));
    mootex = malloc(sizeof(semaphore_t));
    fill_cnt = malloc(sizeof(semaphore_t));
    empty_cnt = malloc(sizeof(semaphore_t));
    semaphore_create(mutex, 1);
    semaphore_create(mootex, 1);
    semaphore_create(fill_cnt, 0);
    semaphore_create(empty_cnt, bufsize);
    indx = 0;
    consumptions = 0;
    productions = 0;
}

//OUTPUT FUNCTIONS
void print_header(){
    fprintf(stdout, "Buffer Size               :   %d\n", bufsize);
    fprintf(stdout, "Time to Live (seconds)    :   %d\n", sleep_len);
    fprintf(stdout, "Number of Producer threads:   %d\n", prod_threads);
    fprintf(stdout, "Number of Consumer threads:   %d\n", cons_threads);
    fprintf(stdout, "----------------------------------\n");
    fprintf(stdout, "Initial Buffer:                 ");
}

//if c_p is 1 then producer else consumer
void print_info(int c_p, int item, int tid){
    char * role_str;
    int role_int;
    role_str = c_p ? "Producer" : "Consumer";
    role_int = c_p ? productions : consumptions; 
    fprintf(stdout, "%s %2d: Total %3d, Value %2d", role_str, tid, role_int, item);
}

//print out the buffer array
void print_buffer(){
    int j;
    fprintf(stdout, "   [ ");
    for (j = 0; j < bufsize; j++) {
        fprintf(stdout, "%5d", buffer[j]);
        if (j == p % bufsize) {
            fprintf(stdout, "^");
        }
        if (j == c % bufsize) {
            fprintf(stdout, "v");
        }
    }
    fprintf(stdout, "]\n");
}

void print_footer(){
    fprintf(stdout, "-------------+--------\n");
    fprintf(stdout, "Produced     |%5d\n", productions);
    fprintf(stdout, "Consumed     |%5d\n", consumptions);
    fprintf(stdout, "-------------+--------\n");
}
