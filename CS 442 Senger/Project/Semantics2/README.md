# CS 442 - Compiler Construction
## Semantics Part 2

This repository contains files to be addded to your main project repository to include the semantic processing capabilities to compile programs of the language Y. 

## The Assignment

- Step 5, 5 pts, Implement increment and decrement either as operators or builtin functions.
- Step 6, 10 pts, Implement boolean operators.
- Step 7, 10 pts, Implement a FOR loop.
- Step 8, 10 pts, Implement a LOOP construct and BREAK statment.
- Step 9, 10 pts, Implement constants and constant expression evaluation.
- Step 10, 15 pts, Implement a SWITCH construct.
- Step 11, 15 pts, Implement an array data type.
- Step 12, 15 pts, Implement function calls for parameterless functions.
- Step 13, 15 pts, Implement function parameters.
- Step 14, 20 pts, Implement local variables in functions.

The repository contains a selection of test programs that implement various combinations of these features. You can make your own test programs but they should actually do something. 

## Extra Steps for Grading

To indicate which of these features you have implemented please add the following to your make file

    sem2test:	 feature1 feature2 etc
    
    feature1:    Y
                 echo "Some message identifying the feature"
                 rm -f f1.asm
                 ./Y f1
                 cat f1.asm
                 spim -noexception -file f1.asm < f1.in
                 
    feature2:    etc
    
Remember that the action commands must be preceeded by one or more tab characters (not spaces). To test your submission I will execute "make sem2test" and this will then run the tests for each of your features. The "f1.in" file is only required if the test program takes input from the terminal. 

## What To Do

- Do not fork this repository on Gitlab, instead
- On your development machine, in a location different than your assignment repository, clone this project. Your assignment repository will continue to accumulate all files for the project. 
- Copies the files from this repository to your assignment repository.
- Discard the clone of this repository.
- Implement the required capabilities. 
- When complete, "make clean" to remove object files and executables, use git to add and commit your changes and push the project back up to your repository. Rememger to create an issue in Gitlab with me as the assignee indicating that your submission is ready for grading. 
