# Operating Systems - LAB 5
## Thread Synchronization

Note: The code is written in C and compiled using gcc compiler.
I have including the linking explicitly in the makefile. (-lpthread -lrt)

## Question 1
Problem: The task is to implement the classic synchronization - Dining philosopher problem.

To run the file:
1. To compile the file: 
```sh
# from the home directory
cd Q1

# compile  and run the file
make
./phil <N> 

# (where N is the number of philosophers)
```

2. To clean the files:
```sh
make clean
```
### Generic make command cleans the files before compiling the new files.

## Question 2
Problem: The task is to implement the classic synchronization - Multiple Producer-Consumer Problem

To run the file:
1. To compile the file: 
```sh
# from the home directory
cd Q2

# compile  and run the file
make
./multi_pc -p <p> -pc <pc> -c <c> -cc <cc>
```
2. To clean the files:
```sh
make clean
```
### Generic make command cleans the files before compiling the new files.
