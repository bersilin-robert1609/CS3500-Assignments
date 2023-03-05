# Operating Systems - LAB 6
## Memory Management

Note: The code is written in C/C++ and compiled using gcc/g++ compiler.

## Question 1
Problem: The task is to implement a program that simulates the memory allocation of a computer system in 3 different ways: First Fit, Best Fit and Worst Fit.

To run the file:
1. To compile the file:
```sh
cd Q1
make
./mem_alloc
```

2. To clean the files:
```sh
make clean
```

3. To run the file manually:
```sh
cd Q1

# to compile and run the file
g++ memory_allocation.cpp -o <output_file_name>

./<output_file_name>
```

## Question 2
Problem: The task is to demonstrate the memory usage of a program in 2 different cases

To run the file:
1. To compile the file: 

```sh
cd Q2
make
./mem_usage
```

2. To clean the files:
```sh
make clean
```

3.  To run the file manually:
```sh
cd Q2

# to compile and run the file
gcc memory_usage.cpp -o <output_file_name>

./<output_file_name>
```

## Question 3
Problem: The task is to simulate the page replacement algorithms: FIFO, LRU and Optimal.

To run the file:
1. To compile the file: 
```sh
cd Q3
make
./page_replacement
```

2. To clean the files:
```sh
make clean
```

3. To run the file manually:
```sh
cd Q3

# to compile and run the file
g++ page_replacement.cpp -o <output_file_name>

./<output_file_name>
```

Generic make instruction cleans the object files and the executable files.