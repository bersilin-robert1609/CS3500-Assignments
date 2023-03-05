# CPU Scheduler

## Input Format:

1. The first line if the quantum time for the CPU.
2. The second line is the number of processes.

Then in each line information about the processes is given in the following format:

3. Process ID, Arrival Time, Burst Time, Type, Priority
    1. Process ID and Type are strings,
    2. Arrival Time, Burst Time, Priority are integers,
    3. Type from the set {"sys", "ip", "iep", "bp", "sp"}.

## Execution Procedure:

The program can be executed in the following ways: 

1.  (MAIN) The program can be executed by the following instructions
```sh
# Compile the program
g++ main.cpp -o main -Wall -g

# Run the program
./main
```
2.  The program will take input from stdin and output to stdout.
    So redirection from file can be done as follows
```sh
./main < inputfile > outputfile
```
3.  If you want to take input from files in directory, you can use the following command

    (this only works if the input is in a .txt file in the directory <directory_name>, placed in the directory containing the cpp file)
```sh
bash run.sh

# Enter the directory name when prompted
<directory_name>
```

## Screenshots:

The first file "input.txt" is the sample input given in the problem statement
The second file "input2.txt" checks the idle case

These files are present in sample_input directory
Their outputs in the sample_output directory