Operating Systems - LAB 7
Disk Scheduler

Note: The code is written in C++ and compiled using g++ compiler.
| ----------------------------------------------------------------------------------------------- |
Problem: Write a program to simulate the disk scheduling algorithms FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK.

To run the file:
1. To compile the file: 
    make
    ./main -h <head_position> -a <algorithm>

    The arguments are: 

    -h <head_position> : The head position of the disk
    The limits are : 0 - 4999

    -a <algorithm> : The algorithm (an integer from 1-6) to be used for scheduling. The options are:
        1 - FCFS
        2 - SSTF
        3 - SCAN
        4 - CSCAN
        5 - LOOK
        6 - CLOOK

    Note : 1. The default values for head_position is 2000 and algorithm is 1 (FCFS)
           2. These arguments are optional. If not provided, the default values will be used.

2. To clean the files:
    make clean

3. To run the file manually:

    g++ disk-scheduler.cpp -o main
    ./main -h <head_position> -a <algorithm>

I have attached the sample_outputs.txt file for reference.
This was generated using the shell script "create_table.sh" which is in the helper folder.
To run the script:

    cp ./helper/create_table.sh .
    bash create_table.sh

This recreates the sample_outputs.txt file 
| ----------------------------------------------------------------------------------------------- |