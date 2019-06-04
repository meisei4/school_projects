# CS 224 Assignment 8


## JSMN Utility API

This assignment will build on the JSMN library by providing a convenient API for dereferencing json data in as close to the style of javascript as possible with C. 

The repository contains 

* jsmnutil.h - defines the access API
* jsmnutil.c - implementation of API
* jsonfac.c - test program for jsmnutil.h
* json-data-faculty - test data
* jsondump.c - a utility for looking at JSMN parse data
* makefile - targets for building the JSMN library, jsmnutil and the test programs

jsmnutil.c is implemented except for the derefBy() and nextKeyForObject() functions. 

The test program can be built with 
```
make jsonfac
./jsonfac <json-data-faculty
```

On the first use of this it will clone and build the JSMN library.

## What To Do

- Fork this repository in GitLab
- Clone your fork to your development machine.
- Implement the assignment. 
- "make clean" and then "git add/commit/push" back to GitLab.
- Add me as a reporter member of your repository.
- Create an issue with me as the assignee. Use the issue title "Assign5 Ready for Grading"."
- In the issue make a note of your difficulty factor estimate for this assignment relative to the first assignment. 

