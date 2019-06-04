/*
 * 
 *
 * CS 441/541: CPU Scheduler (Project 1)
 */
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int q, s, pcount = 0;

struct Process {
    int pid;
    int burst;
    int priority;
    int wait;
    int turnaround;
    int timeleft;
    int cycle;
    int arrivaltime;
};

char *
preparationInfo(int argc, char **args){
    char *fn;
    int i;
    for(i = 1; i < argc; i++){
        if(strcmp(args[i], "-s") == 0){
            //might break if number ascii is not after -s
            s = atoi(args[++i]);
            switch(s){
                case 1 : 
                    printf("Scheduler    :  1 FCFS\n");
                    break;
                case 2 : 
                    printf("Scheduler    :  2 SJF\n");
                    break;
                case 3 : 
                    printf("Scheduler    :  3 Priority\n");
                    break;
                case 4 :
                    printf("Scheduler    :  4 RR\n");
                    break;
                default : 
                    printf("broken\n");
            }
        }else if(strcmp(args[i], "-q") == 0){
            q = atoi(args[++i]);
        }else{
            fn = strdup(args[i]);
        }     
    }
    printf("Quantum      :  %i\n", q);
    printf("Sch. File    :  %s\n", fn); 
    return fn;
}

struct Process *
readInfo(FILE *file){
    fscanf(file, "%i", &pcount);
    struct Process * processes = malloc(sizeof(struct Process) * pcount);
    int i;
    for(i = 0; i < pcount; i++){
        processes[i].pid = 0;
        processes[i].burst = 0;
        processes[i].priority = 0;
        processes[i].wait = 0;
        processes[i].turnaround = 0;
        processes[i].timeleft = 0;
    }
    printf("Arrival Order:  ");
    for(i = 0; i < pcount; i++){      
        fscanf(file, "%i %i %i", &processes[i].pid, &processes[i].burst, &processes[i].priority);
        if(i == pcount-1){
            printf("%i\n", processes[i].pid);
        }else{
            printf("%i, ", processes[i].pid);
        }
        processes[i].timeleft = processes[i].burst;
        processes[i].cycle = 0;
        processes[i].arrivaltime = i;
    }
    printf("Process Information:\n");
    for(i = 0; i < pcount; i++){
        printf("%d%8d%8d\n", processes[i].pid, processes[i].burst, processes[i].priority);
    }
    return processes;
}

int
burstcmpfunc(const void * proc1, const void * proc2){
    int p1 = ((struct Process *)proc1)->burst;
    int p2 = ((struct Process *)proc2)->burst;
    return p1 - p2;
}

int 
pricmpfunc(const void * proc1, const void * proc2){
    int p1 = ((struct Process *)proc1)->priority;
    int p2 = ((struct Process *)proc2)->priority;
    return p1 - p2;
}

void
sortProcesses(struct Process * processes){
    switch(s){
        case 2: qsort(processes, pcount, sizeof(struct Process), burstcmpfunc);               
                break;
        case 3: qsort(processes, pcount, sizeof(struct Process), pricmpfunc);
                break;
    }
}

int 
arrivalcmpfunc(const void * proc1, const void * proc2){
    int p1 = ((struct Process *)proc1)->arrivaltime;
    int p2 = ((struct Process *)proc2)->arrivaltime;
    return p1 - p2;
}


int 
runtime(struct Process * processes){
    int alltime = 0;
    int i;
    for(i = 0; i < pcount; i++){
        alltime += processes[i].burst; 
    }
    return alltime;
}

void
runRR(struct Process * processes){
    int i, j;
    int alltime = 0;
    int endtime = runtime(processes);
    while(alltime < endtime){
        for(i = 0; i < pcount; i++){
            if(processes[i].timeleft){
                processes[i].wait = alltime - (processes[i].cycle * q);
                processes[i].turnaround = alltime;
                for(j = 0; j < q && processes[i].timeleft; j++){
                    processes[i].timeleft--;
                    processes[i].turnaround++;
                    alltime++;
                }
                processes[i].cycle++;
            }
        }
    }
}

void 
runProcess(struct Process * processes){
    int alltime = 0;
    int i;
    for(i = 0; i < pcount; i++){
        processes[i].wait = alltime;
        processes[i].turnaround = alltime;
        processes[i].timeleft = processes[i].burst;
        while(processes[i].timeleft){
            processes[i].timeleft--;
            processes[i].turnaround++;
            alltime++;
        }
    }
}

double *
avgs(struct Process * processes){
    double * avgs = malloc(sizeof(double) * 2);
    double w = 0;
    double t = 0;
    int i;
    for(i = 0; i < pcount; i++){
        w += processes[i].wait;
        t += processes[i].turnaround;
    }
    double avgW = w/pcount;
    double avgT = t/pcount;
    avgs[0] = avgW;
    avgs[1] = avgT;
    return avgs;
}

int 
main(int argc, char **argv) {
    char *fn = preparationInfo(argc, argv);
    printf("-------------------------------\n");
    FILE *file = fopen(fn, "r");
    struct Process *processes = readInfo(file);
    printf("-------------------------------\n");
    printf("Running...\n");
    if(s == 4){
        runRR(processes);
    }else{
        sortProcesses(processes);
        runProcess(processes);
    }
    qsort(processes, pcount, sizeof(struct Process), arrivalcmpfunc);
    printf("##################################################\n");
    printf("#  #    CPU     Pri       W       T\n");
    int i;
    for(i = 0; i < pcount; i++){
        printf("# %2d%7d%8d%8d%8d\n", processes[i].pid, processes[i].burst, processes[i].priority, processes[i].wait, processes[i].turnaround);
    }
    printf("##################################################\n");
    double * avg = avgs(processes);
    printf("# Avg. Waiting Time   :  %.2f\n", avg[0]);
    printf("# Avg. Turnaround Time:  %.2f\n", avg[1]);
    printf("##################################################\n");
    return 0;
}


