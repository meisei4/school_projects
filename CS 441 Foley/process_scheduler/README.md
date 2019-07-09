# CS441/541 Project 2

## Author(s):

Ian Neville

## Date:

09/25/18


## Description:
Process scheduler that can use 4 different scheduling algorithms


## How to build the software
run makefile


## How to use the software

./scheduler -s # -q # filename
where -s is for the schedule algorithm (1=FCFS, 2=SJF, 3=priority, 4=RR)
and -q is for the quantum, only needed when -s 4 is in the arguments


## How the software was tested
run command "make check"
run tests 1-5 in tests folder with differing scheduling algorithms
