Operating Systems - LAB 5
Thread Synchronization

Note: The code is written in C and compiled using gcc compiler.
I have including the linking explicitly in the makefile. (-lpthread -lrt)

| ----------------------------------------------------------------------------------------------- |
Q 1
Problem: The task is to implement the classic synchronization - Dining philosopher problem.

To run the file:
1. To compile the file: 
    cd Q1
    make
    ./phil <N> 
(where N is the number of philosophers)

2. To clean the files:
    make clean

Generic make command cleans the files before compiling the new files.
| ----------------------------------------------------------------------------------------------- |

Q 2
Problem: The task is to implement the classic synchronization - Multiple Producer-Consumer Problem

To run the file:
1. To compile the file: 
    cd Q2
    make
    ./multi_pc -p <p> -pc <pc> -c <c> -cc <cc>

2. To clean the files:
    make clean

Generic make command cleans the files before compiling the new files.
| ----------------------------------------------------------------------------------------------- |
