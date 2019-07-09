# CS441/541 Project 4

## Author(s):
Ian Edward Neville

## Date:

2018 October 30th 

## Description:

an implementation of a bounded buffer using semaphores and pthreads to produce and consume 
random numbers from the buffer

## How to build the software
run make


## How to use the software

./bounded-buffer [sleep time] [number of producer threads] [number of consumer threads] [optional bufsize]
where all the arguments are numbers


## How the software was tested
First tried basic cases with thread counts of around 5-10, with a time of 5 seconds, and tested on differing buffersizes. 
tried to increase thread amounts and found that -1s were being consumed. fixed that
tested the boundary case of many consumers and little producers
tested the boundary case of many producers and little consumers

## Known bugs and problem areas
when there is more than a 2:1 or 1:2 ratio between the producers and consumers it fails to finish
The difference of the producers - consumers must not exceed the minimum of the two.
for example 
./bounded-buffer 2 5 10 
works whereas 
./bounded-buffer 2 5 11 
or 
./bounded-buffer 2 4 10
do not finish.

creating threads that sum to 147 causes a segmentation fault
