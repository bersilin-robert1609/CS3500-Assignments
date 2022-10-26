# LAB - 7  - Disk Scheduling Algorithms Simulation

## Aim

 - To simulate the disk scheduling algorithms and compare the performance of each algorithm.
 - To compare the performance for different initial positions of the disk head.

## Theory

 - Disk scheduling algorithms are used to determine the order in which the disk requests are to be processed. The disk scheduling algorithms are classified into two categories:
   - Non-preemptive algorithms
   - Preemptive algorithms

 - FCFS - The first come first serve algorithm is a non-preemptive algorithm. It is the simplest disk scheduling algorithm. It simply services the requests in the order they arrive in the disk queue.

 - SSTF - The shortest seek time first algorithm is a non-preemptive algorithm. It is similar to the FCFS algorithm, except that it always services the request that is closest to the current head position.

 - SCAN - The SCAN algorithm is a preemptive algorithm. It moves the disk head in one direction until it reaches the end of the disk, then reverses its direction and scans the disk in the other direction.

 - C-SCAN - The C-SCAN algorithm is a preemptive algorithm. It is similar to the SCAN algorithm, except that it does not reverse its direction. Instead, it goes to the other end of the disk and continues in the same direction.

 - LOOK - The LOOK algorithm is a preemptive algorithm. It is similar to the SCAN algorithm, except that it does not go till the end of the disk. It changes direction at the last request that it services in the current direction.

 - C-LOOK - The C-LOOK algorithm is a preemptive algorithm. It is similar to the LOOK algorithm, except that it does not reverse its direction. Instead, it goes to the other end of the disk and continues in the same direction.

## Algorithms Used

1. FCFS - First Come First Serve
2. SSTF - Shortest Seek Time First
3. SCAN - Elevator Algorithm
4. C-SCAN - Circular SCAN
5. LOOK - LOOK Algorithm (Similar to SCAN but does not go till last cylinder)
6. C-LOOK - Circular LOOK (Similar to CSCAN)

## Average Head Movement

The average head movement for different algorithms and different head positions is given below:

|  **A/H**   |  **500**  |  **1000**  |  **1500**  |  **2000**  |  **2500**  |  **3000**  |  **3500**  |  **4000**  |  **4500**  |
|    :-:     |    :-:    |     :-:    |     :-:    |    :-:     |    :-:     |    :-:     |     :-:    |    :-:     |    :-:     |
|  **FCFS**  |  1637.26  |   1636.76  |   1636.26  |   1635.76  |   1636.22  |   1636.72  |   1637.22  |   1637.72  |   1638.22  |
|  **SSTF**  |   5.483   |    5.989   |    6.485   |    7.984   |    7.483   |    7.983   |    6.484   |   8.983    |    9.489   |
|  **SCAN**  |   5.495   |    8.992   |    8.492   |    7.992   |    7.495   |    7.995   |    6.492   |   5.992    |    5.492   |
| **C-SCAN** |   9.992   |    9.989   |    9.989   |    9.985   |    9.991   |    9.99    |    9.993   |   9.994    |    9.996   |
|  **LOOK**  |   5.483   |    8.984   |    8.484   |    7.984   |    7.483   |    7.983   |    6.484   |   5.984    |    5.484   |
| **C-LOOK** |   9.972   |    9.969   |    9.969   |    9.965   |    9.971   |    9.97    |    9.973   |   9.974    |    9.976   |

## Conclusion

 - The average head movement for FCFS is the highest among all the algorithms, which is expected as it does not take into account the previous head position when a request is made.

 - If the disk movement from end to start or start to end in case of Circular Algorithms are not considered, then the average head movement for C-SCAN and C-LOOK is the lowest among all the algorithms. Even among them the C-LOOK is the best as it does not have to go to the end of the disk to start again.
 
 - The average head movement for SCAN and LOOK is the same as they are similar algorithms. But the SCAN algorithm has to go to the end of the disk to start again, which increases the average head movement.

 - The average head movement may vary in different runs of the program as the requests are generated randomly and the direction is chosen based on the closest request in case of SCAN, LOOK, C-SCAN and C-LOOK.

## Request Sequence

The request sequence is generated randomly using code similar to the following:

```c
#include <iostream>
#include <time.h>
using namespace std;

int main()
{
    srand(time(0));
    int n = 1000;
    cout << n << endl;
    for (int i = 0; i < n; i++)
    {
        cout << rand() % 5000 << " ";
    }
    cout << endl;
    return 0;
}
```