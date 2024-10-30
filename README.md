# CPU Scheduler Simulator

A comprehensive C++ program that simulates and analyzes CPU scheduling algorithms used in operating systems. This simulator helps in understanding how different scheduling algorithms perform by comparing their efficiency in process management, resource utilization, and timing metrics. It implements six major scheduling algorithms and provides visual representations along with detailed performance statistics.

## Supported Algorithms

1. First Come First Serve (FCFS)
2. Shortest Job First (SJF)
3. Shortest Remaining Time First (SRTF)
4. Priority Scheduling
5. Preemptive Priority Scheduling
6. Round Robin

## Features

- Visual timeline representation of process execution
- Detailed statistics for each scheduling algorithm:
  - Waiting time
  - Turnaround time
  - Response time
- Automatic comparison of algorithms to find the best performer

## How to Use

1. Compile the program using a C++ compiler:
   ```bash
   g++ main.cpp -o scheduler
   ```

2. Run the program:
   ```bash
   ./scheduler
   ```

3. Follow the prompts to enter:
   - Number of processes
   - For each process:
     - Process name
     - Arrival time
     - Service time
     - Priority
   - Time quantum for Round Robin algorithm

## Input Format

Enter process information in the following format:
```
ProcessName,ArrivalTime,ServiceTime,Priority
```

Example:
```
P1,0,5,2
P2,1,3,1
P3,2,4,3
```

## Output

The program will display:
1. Timeline visualization for each algorithm
2. Statistical metrics for each process
3. Average metrics for each algorithm
4. Best performing algorithm for each metric

## Example Output

```
Timeline:
  0  1  2  3  4  5
-------------------
P1 |* |* |* |. |. |
P2 |. |* |* |* |. |
P3 |. |. |. |* |* |
-------------------

Statistics:
Process  Arrival  Service  Finish  Turnaround  Waiting  Response
----------------------------------------------------------
P1          0        3        3          3        0         0
P2          1        3        6          5        2         2
P3          2        2        8          6        4         4
----------------------------------------------------------
Average                               4.67     2.00      2.00
```
