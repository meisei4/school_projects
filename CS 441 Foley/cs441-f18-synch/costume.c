/*
 *
 *
 * CS 441/541: Pirates and Ninjas (Project 5)
 *
 */
#include "costume.h"
#include <string.h>

int main(int argc, char * argv[]) {
    int i = 0;
    initialize_vars();
    if (argc < 2) {
    fprintf(stderr, "Not enough Arguments");
        return 0;
    }
    if (argc > 5) {
        fprintf(stderr, "too many Arguments");
        return 0;
    }
    sleep_len = atoi(argv[1]);
    if (argc >= 3) {
        pirate_threads = atoi(argv[2]);
    }
    if (argc >= 4) {
        ninja_threads = atoi(argv[3]);
    }
    if (argc >= 5) {
        booth_cnt = atoi(argv[4]);
    }
    if(sleep_len <= 0 || pirate_threads <= 0 || ninja_threads <= 0 || booth_cnt <= 0){
        printf("One of the command arguments failed\n");
        return -1;
    }
    
    print_header();
    create_sem(&Nmutex, 1);
    create_sem(&Pmutex, 1);
    create_sem(&Costume_Nmutex, 1);
    create_sem(&Costume_Pmutex, 1);
    if(booth_cnt < ninja_threads){
        i = booth_cnt;
    }else{
        i = ninja_threads;
    }
    create_sem(&costume_shop_switch_ninja, i);
    create_sem(&costume_shop_switch_pirate, 0);
    sem_ninja = malloc(sizeof(semaphore_t)*ninja_threads);
    sem_pirate = malloc(sizeof(semaphore_t)*pirate_threads);
    for(i = 0;i<ninja_threads;i++){
        if( 0 != (sem = semaphore_create(&sem_ninja[i],0))){
            fprintf(stderr, "Error: semaphore_create() failed\n");
            return -1;
        }
    }
    for(i = 0;i<pirate_threads;i++){
        if( 0 != (sem = semaphore_create(&sem_pirate[i],0))){
            fprintf(stderr, "Error: semaphore_create() failed\n");
            return -1;
        }
    }
    //initialize semaphores
    srandom(time(NULL));
    pthread_t ninjas[ninja_threads];
    pthread_t pirates[pirate_threads];
    n_leave = malloc(sizeof(int)*ninja_threads);
    p_leave = malloc(sizeof(int)*pirate_threads);
    n_enter = malloc(sizeof(int)*ninja_threads);
    p_enter = malloc(sizeof(int)*pirate_threads);

    pthread_t costume_shop_n;
    costume_shop_c = pthread_create(&costume_shop_n,NULL,costume_shop_ninja,(void *)(intptr_t)i);
    if(costume_shop_c != 0){
        perror("pthread_create failed22");
        exit(-1);
    }

    pthread_t costume_shop_p;
    costume_shop_c = pthread_create(&costume_shop_p,NULL,costume_shop_pirate,(void *)(intptr_t)i);
    if(costume_shop_c != 0){
        perror("pthread_create failed22");
        exit(-1);
    }

    for(i = 0;i < ninja_threads;i++){
        ninja_c = pthread_create(&(ninjas[i]),NULL,ninja,(void *)(intptr_t)i);
        if(ninja_c != 0){
            perror("pthread_create failed23");
            exit(-1);
        }
    }

    for(i = 0;i < pirate_threads;i++){
        pirate_c = pthread_create(&(pirates[i]),NULL,pirate,(void *)(intptr_t)i);
        if(pirate_c != 0){
            perror("pthread_create failed23");
            exit(-1);
        }
    }
    sleep(sleep_len);
    print_footer();
    return 0;
}

int create_sem(semaphore_t * s, int init){
    int sem;
    if (0 != (sem = semaphore_create(s, init))) {
        fprintf(stderr, "Error: semaphore_create() failed\n");
        return -1;
    }
    return 1; 
}

void initialize_vars(){
    ninja_threads = 5;
    pirate_threads = 5;
    booth_cnt = 2;
    Ncount = 0;
    Pcount = 0; 
}

void print_header(){
    printf("Time To Live (seconds) : %d\n", sleep_len);
    printf("Number of Pirates : %d\n", pirate_threads);
    printf("Number of Ninjas : %d\n", ninja_threads);
    printf("---------------------------------\n");
    printf(" Thread Id |   Action\n");
    printf("------------------------------------------------\n");
}

void print_footer(){
    printf("---------+----------------------------------\n");
    int i;
    for (i = 0; i < pirate_threads; i++) {
        printf("Pirate %d : Entered    %d / Left     %d\n", i, p_enter[i], p_leave[i]);
    }
    for (i = 0; i < ninja_threads; i++) {
        printf("Ninja  %d : Entered    %d / Left     %d\n", i, n_enter[i], n_leave[i]);
    }
}

int wait(semaphore_t s){
    int ret = 0;
    if (0 != (ret = semaphore_wait(&s))) {
        fprintf(stderr, "Error: semaphore_wait() failed\n");
        pthread_exit(NULL);
        return -1;
    }   
    return 1;
}

int post(semaphore_t s){
    int ret = 0;
    if (0 != (ret = semaphore_post(&s))) {
        fprintf(stderr, "Error: semaphore_post() failed\n");
        pthread_exit(NULL);
        return -1;
    }
    return 1;
}

/*
 * Ninja thread.
 */
void *ninja(void *ptr){
    int tid = 0;
    int rnd_time = 0;
    int i = 0;
    int in;
    while(1){
        tid = (intptr_t)ptr;
        //WAIT NMUTEX
        wait(Nmutex);
        Ncount++;
        append_ninja(tid);
        line_count_ninja++;
        printf("Ninja    %d | Waiting\n",tid);
        //post NMUTEX
        post(Nmutex); 
        //WAIT SEM_NINJA[tid]
        wait(sem_ninja[tid]); 
        n_enter[tid]++;
        booth_ninja++;
        line_count_ninja--;
        printf("Ninja    %d | Costume preparation booth %d\n",tid,booth_ninja);
        rnd_time = random()%50001;
        usleep(rnd_time);
        //WAIT NMUTEX
        wait(Nmutex); 
        booth_ninja--;
        printf("Ninja    %d | Leaving\n",tid);
        n_leave[tid]++;
        rnd_time = random()%1000001;
        usleep(rnd_time);
        Ncount--;
        if(booth_ninja == 0 && pirate_threads > 0){
            // costume_shop_p start.
            if(queue_count_pirate < booth_cnt){
                in = queue_count_pirate;
            }else{
                in = booth_cnt;
            }
            for(i = 0;i<in;i++){
                semaphore_post(&costume_shop_switch_pirate);
            }
        }
        //POST NMUTEX
        post(Nmutex); 
    }
    pthread_exit(NULL);
}

void *pirate(void *ptr){
    int tid = 0;
    int rnd_time = 0;
    int i = 0;
    int in = 0;
    while(1){
        tid = (intptr_t)ptr;
        //WAIT PMUTEX
        wait(Pmutex);
        Pcount++;
        append_pirate(tid);
        line_count_pirate++;
        //POST PMUTEX
        printf("Pirate   %d | Waiting\n",tid);
        post(Pmutex); 
        //WAIT SEM_PIRATE[TID]
        wait(sem_pirate[tid]);
        p_enter[tid]++;
        booth_pirate++;
        line_count_pirate--;
        printf("Pirate   %d | Costume preparation booth %d\n",tid,booth_pirate);
        rnd_time = random()%50001;
        usleep(rnd_time);
        //WAIT PMUTEX
        wait(Pmutex); 
        booth_pirate--;
        printf("Pirate   %d | Leaving\n",tid);
        p_leave[tid]++;
        rnd_time = random()%1000001;
        usleep(rnd_time);
        Pcount--;
        if(booth_pirate == 0 && ninja_threads > 0){
            if(queue_count_ninja < booth_cnt){
                in = queue_count_ninja;
            }else{
                in = booth_cnt;
            }
            for(i = 0;i<in;i++){
                //after pirates leave allow as many as there are booths ninjas in
                //if less ninjas than booths let them all in
                semaphore_post(&costume_shop_switch_ninja);
            }
        }
        //POST PMUTEX
        post(Pmutex); 
    }
    pthread_exit(NULL);
}

//DECIDES WHO IS NEXT TO ENTER
void *costume_shop_ninja(void *ptr){
    while(1){
        if(queue_count_ninja > 0 ){
            if(pirate_threads > 0){
                semaphore_wait(&costume_shop_switch_ninja);
            }
            //WAIT Costume_Nmutex
            wait(Costume_Nmutex); 
            int sem = pop_ninja();
            //POST SEM_NINJA[SEM]
            post(sem_ninja[sem]); 
            //POST COSTUME_NMUTEX
            post(Costume_Nmutex); 
        }
    }
}

void *costume_shop_pirate(void *ptr){
    while(1){
        if(queue_count_pirate > 0){
            if(ninja_threads > 0){
                semaphore_wait(&costume_shop_switch_pirate);
            }
            //WAIT COSTUME PMUTEX
            wait(Costume_Pmutex);
            int sem = pop_pirate();
            //POST SEMPIRATESEM
            post(sem_pirate[sem]); 
            //POST COMSTUME PMUTEX
            post(Costume_Pmutex); 
        }
    }
}

//QUEUE FUNCTIONS
int is_full_ninja(){
    return queue_count_ninja == ninja_threads;
}

void append_ninja(int element){
    queue_ninja = realloc(queue_ninja,(queue_count_ninja+1) * sizeof(int));
    queue_ninja[++end_ninja] = element;
    queue_count_ninja++;
}

int pop_ninja(){
    int data;
    int i;
    data = queue_ninja[0];
    for(i = 0;i<queue_count_ninja-1;i++){
        queue_ninja[i] = queue_ninja[i+1];
    }
    end_ninja--;
    queue_count_ninja--;
    return data;
}

int is_full_pirate(){
    return queue_count_pirate == pirate_threads;
}

void append_pirate(int element){
    queue_pirate = realloc(queue_pirate, (queue_count_pirate+1) * sizeof(int));
    queue_pirate[++end_pirate] = element;
    queue_count_pirate++;
}

int pop_pirate(){
    int i;
    int data;
    data = queue_pirate[0];
    for(i = 0;i<queue_count_pirate-1;i++){
        queue_pirate[i] = queue_pirate[i+1];
    }
    end_pirate--;
    queue_count_pirate--;
    return data;
}
