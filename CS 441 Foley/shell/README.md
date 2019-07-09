# CS441/541 Project 3

## Author(s):
Wanqian Huang
Ian E. Neville

## Date:
October 14 2018

## Description:
a shell project that allows for batch and interactive modes

## How to build the software
run make mysh


## How to use the software
run ./mysh with or without arguments
with arguments (file paths) runs batch mode
without arguments runs interactive mode

## How the software was tested
the 5 test files:
test1.txt tests the use of exit in the middle of a batch file
test2.txt tests the use of fg and redirection functionality
test3.txt tests a basic wait proof
test4.txt tests a more complex wait with fg
test5.txt tests fg with arguments


## Known bugs and problem areas
hardcoded the job limit to be 1024, unable to correctly implement realloc

