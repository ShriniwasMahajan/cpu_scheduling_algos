#include <bits/stdc++.h>

using namespace std;

string algos[] = {"FCFS", "SJF", "SRTF", "Priority", "Preemptive Priority", "Round Robin"};

struct Process
{
    string name;
    int arrivalTime;
    int serviceTime;
    int priority;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    bool hasStarted;
};

class Scheduler
{
private:
    vector<Process> processes;
    vector<vector<char>> timeline;
    int lastInstant;

    int calculateLastInstant()
    {
        int lastInstant = 0;
        int totalServiceTime = 0;
        int latestArrival = 0;

        for (const auto &p : processes)
        {
            totalServiceTime += p.serviceTime;
            latestArrival = max(latestArrival, p.arrivalTime);
            lastInstant = max(lastInstant, p.arrivalTime + p.serviceTime);
        }

        lastInstant = max(lastInstant, latestArrival + totalServiceTime);

        return lastInstant;
    }

    void clearTimeline()
    {
        for (int i = 0; i < lastInstant; i++)
            for (int j = 0; j < processes.size(); j++)
                timeline[i][j] = ' ';
    }

    void fillWaitTime()
    {
        for (int i = 0; i < processes.size(); i++)
            for (int k = processes[i].arrivalTime; k < processes[i].finishTime; k++)
                if (timeline[k][i] != '*')
                    timeline[k][i] = '.';
    }

    void updateProcessStats(Process &p, int currentTime)
    {
        if (!p.hasStarted)
        {
            p.responseTime = currentTime - p.arrivalTime;
            p.hasStarted = true;
        }
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.serviceTime;
    }

public:
    Scheduler(const vector<tuple<string, int, int, int>> &inputProcesses)
    {
        for (const auto &p : inputProcesses)
            processes.push_back({get<0>(p), get<1>(p), get<2>(p), get<3>(p), 0, 0, 0, 0, false});
        lastInstant = calculateLastInstant();
        timeline.resize(lastInstant, vector<char>(processes.size(), ' '));
    }

    void fcfs()
    {
        clearTimeline();
        int currentTime = 0;
        for (auto &p : processes)
        {
            currentTime = max(currentTime, p.arrivalTime);
            updateProcessStats(p, currentTime);
            for (int i = currentTime; i < currentTime + p.serviceTime; i++)
                timeline[i][&p - &processes[0]] = '*';
            currentTime += p.serviceTime;
            p.finishTime = currentTime;
            updateProcessStats(p, currentTime);
        }
        fillWaitTime();
    }

    void sjf()
    {
        clearTimeline();
        int currentTime = 0;
        int completedProcesses = 0;
        vector<bool> completed(processes.size(), false);

        while (completedProcesses < processes.size())
        {
            int shortestJob = -1;
            int shortestTime = INT_MAX;

            for (int i = 0; i < processes.size(); i++)
                if (!completed[i] && processes[i].arrivalTime <= currentTime && processes[i].serviceTime < shortestTime)
                {
                    shortestJob = i;
                    shortestTime = processes[i].serviceTime;
                }

            if (shortestJob == -1)
            {
                currentTime++;
                continue;
            }

            updateProcessStats(processes[shortestJob], currentTime);
            for (int i = currentTime; i < currentTime + processes[shortestJob].serviceTime; i++)
                timeline[i][shortestJob] = '*';

            currentTime += processes[shortestJob].serviceTime;
            processes[shortestJob].finishTime = currentTime;
            updateProcessStats(processes[shortestJob], currentTime);
            completed[shortestJob] = true;
            completedProcesses++;
        }
        fillWaitTime();
    }

    void srtf()
    {
        clearTimeline();
        int currentTime = 0;
        int completedProcesses = 0;
        vector<int> remainingTime(processes.size());

        for (int i = 0; i < processes.size(); i++)
            remainingTime[i] = processes[i].serviceTime;

        while (completedProcesses < processes.size())
        {
            int shortestJob = -1;
            int shortestTime = INT_MAX;

            for (int i = 0; i < processes.size(); i++)
                if (remainingTime[i] > 0 && processes[i].arrivalTime <= currentTime && remainingTime[i] < shortestTime)
                {
                    shortestJob = i;
                    shortestTime = remainingTime[i];
                }

            if (shortestJob == -1)
            {
                currentTime++;
                continue;
            }

            updateProcessStats(processes[shortestJob], currentTime);
            timeline[currentTime][shortestJob] = '*';
            remainingTime[shortestJob]--;

            if (remainingTime[shortestJob] == 0)
            {
                completedProcesses++;
                processes[shortestJob].finishTime = currentTime + 1;
                updateProcessStats(processes[shortestJob], currentTime + 1);
            }

            currentTime++;
        }
        fillWaitTime();
    }

    void priority()
    {
        clearTimeline();
        int currentTime = 0;
        int completedProcesses = 0;
        vector<bool> completed(processes.size(), false);

        while (completedProcesses < processes.size())
        {
            int highestPriorityJob = -1;
            int highestPriority = INT_MIN;

            for (int i = 0; i < processes.size(); i++)
                if (!completed[i] && processes[i].arrivalTime <= currentTime && processes[i].priority > highestPriority)
                {
                    highestPriorityJob = i;
                    highestPriority = processes[i].priority;
                }

            if (highestPriorityJob == -1)
            {
                currentTime++;
                continue;
            }

            updateProcessStats(processes[highestPriorityJob], currentTime);
            for (int i = currentTime; i < currentTime + processes[highestPriorityJob].serviceTime; i++)
                timeline[i][highestPriorityJob] = '*';

            currentTime += processes[highestPriorityJob].serviceTime;
            processes[highestPriorityJob].finishTime = currentTime;
            updateProcessStats(processes[highestPriorityJob], currentTime);
            completed[highestPriorityJob] = true;
            completedProcesses++;
        }
        fillWaitTime();
    }

    void preemptivePriority()
    {
        clearTimeline();
        int currentTime = 0;
        int completedProcesses = 0;
        vector<int> remainingTime(processes.size());

        for (int i = 0; i < processes.size(); i++)
            remainingTime[i] = processes[i].serviceTime;

        while (completedProcesses < processes.size())
        {
            int highestPriorityJob = -1;
            int highestPriority = INT_MIN;

            for (int i = 0; i < processes.size(); i++)
                if (remainingTime[i] > 0 && processes[i].arrivalTime <= currentTime && processes[i].priority > highestPriority)
                {
                    highestPriorityJob = i;
                    highestPriority = processes[i].priority;
                }

            if (highestPriorityJob == -1)
            {
                currentTime++;
                continue;
            }

            updateProcessStats(processes[highestPriorityJob], currentTime);
            timeline[currentTime][highestPriorityJob] = '*';
            remainingTime[highestPriorityJob]--;

            if (remainingTime[highestPriorityJob] == 0)
            {
                completedProcesses++;
                processes[highestPriorityJob].finishTime = currentTime + 1;
                updateProcessStats(processes[highestPriorityJob], currentTime + 1);
            }

            currentTime++;
        }
        fillWaitTime();
    }

    void roundRobin(int quantum)
    {
        clearTimeline();
        queue<int> readyQueue;
        vector<int> remainingTime(processes.size());
        for (int i = 0; i < processes.size(); i++)
            remainingTime[i] = processes[i].serviceTime;

        int currentTime = 0;
        int completedProcesses = 0;

        while (completedProcesses < processes.size())
        {
            for (int i = 0; i < processes.size(); i++)
                if (processes[i].arrivalTime == currentTime)
                    readyQueue.push(i);

            if (!readyQueue.empty())
            {
                int i = readyQueue.front();
                readyQueue.pop();

                updateProcessStats(processes[i], currentTime);
                for (int j = 0; j < quantum && remainingTime[i] > 0; j++)
                {
                    timeline[currentTime][i] = '*';
                    remainingTime[i]--;
                    currentTime++;

                    for (int k = 0; k < processes.size(); k++)
                        if (processes[k].arrivalTime == currentTime)
                            readyQueue.push(k);
                }

                if (remainingTime[i] > 0)
                    readyQueue.push(i);
                else
                {
                    processes[i].finishTime = currentTime;
                    updateProcessStats(processes[i], currentTime);
                    completedProcesses++;
                }
            }
            else
                currentTime++;
        }
        fillWaitTime();
    }

    void printTimeline()
    {
        cout << "Timeline:\n";
        for (int i = 0; i <= lastInstant; i++)
            cout << setw(3) << i;
        cout << "\n"
             << string(lastInstant * 3 + 3, '-') << "\n";
        for (int i = 0; i < processes.size(); i++)
        {
            cout << processes[i].name << " |";
            for (int j = 0; j < lastInstant; j++)
                cout << setw(2) << timeline[j][i] << "|";
            cout << "\n";
        }
        cout << string(lastInstant * 3 + 3, '-') << "\n";
    }

    tuple<float, float, float> printStatistics()
    {
        size_t maxNameLength = 0;
        for (const auto &p : processes)
            maxNameLength = max(maxNameLength, p.name.length());
        maxNameLength = max(maxNameLength, size_t(7));

        cout << "\nStatistics:\n";
        cout << left << setw(maxNameLength + 2) << "Process"
             << right << setw(10) << "Arrival" << setw(10) << "Service"
             << setw(10) << "Finish" << setw(12) << "Turnaround" << setw(12) << "Waiting"
             << setw(12) << "Response" << "\n";
        cout << string(maxNameLength + 66, '-') << "\n";

        float avgTurnaround = 0, avgWaiting = 0, avgResponse = 0;
        for (const auto &p : processes)
        {
            cout << left << setw(maxNameLength + 2) << p.name
                 << right << setw(10) << p.arrivalTime << setw(10) << p.serviceTime
                 << setw(10) << p.finishTime << setw(12) << p.turnaroundTime
                 << setw(12) << p.waitingTime << setw(12) << p.responseTime << "\n";
            avgTurnaround += p.turnaroundTime;
            avgWaiting += p.waitingTime;
            avgResponse += p.responseTime;
        }
        avgTurnaround /= processes.size();
        avgWaiting /= processes.size();
        avgResponse /= processes.size();

        cout << string(maxNameLength + 66, '-') << "\n";
        cout << left << setw(maxNameLength + 2) << "Average"
             << right << setw(10) << "" << setw(10) << ""
             << setw(10) << "" << setw(12) << fixed << setprecision(2) << avgTurnaround
             << setw(12) << avgWaiting << setw(12) << avgResponse << "\n";

        return make_tuple(avgTurnaround, avgWaiting, avgResponse);
    }
};

int main()
{
    int processCount;
    vector<tuple<string, int, int, int>> inputProcesses;

    cout << "Enter the number of processes:\n";
    cin >> processCount;

    cout << "For each process enter the arrival time, service time and priority (comma separated):\n";

    for (int i = 0; i < processCount; i++)
    {
        string processInfo;
        cin >> processInfo;
        size_t comma1 = processInfo.find(',');
        size_t comma2 = processInfo.find(',', comma1 + 1);
        size_t comma3 = processInfo.find(',', comma2 + 1);
        string name = processInfo.substr(0, comma1);
        int arrival = stoi(processInfo.substr(comma1 + 1, comma2 - comma1 - 1));
        int service = stoi(processInfo.substr(comma2 + 1, comma3 - comma2 - 1));
        int priority = stoi(processInfo.substr(comma3 + 1));
        inputProcesses.push_back({name, arrival, service, priority});
    }

    int quantum;
    cout << "Enter time quantum for Round Robin: ";
    cin >> quantum;

    double avgWT[6], avgTAT[6], avgRT[6];

    Scheduler scheduler(inputProcesses);

    for (int i = 0; i < 6; i++)
    {
        switch (i)
        {
        case 0:
            cout << "\nAlgorithm: FCFS\n";
            scheduler.fcfs();
            break;
        case 1:
            cout << "\nAlgorithm: SJF\n";
            scheduler.sjf();
            break;
        case 2:
            cout << "\nAlgorithm: SRTF\n";
            scheduler.srtf();
            break;
        case 3:
            cout << "\nAlgorithm: Priority\n";
            scheduler.priority();
            break;
        case 4:
            cout << "\nAlgorithm: Preemptive Priority\n";
            scheduler.preemptivePriority();
            break;
        case 5:
            cout << "\nAlgorithm: Round Robin - Quantum: " << quantum << "\n";
            scheduler.roundRobin(quantum);
            break;
        }
        scheduler.printTimeline();
        tie(avgWT[i], avgTAT[i], avgRT[i]) = scheduler.printStatistics();
    }

    int bestWT = 0, bestTAT = 0, bestRT = 0;
    for (int i = 1; i < 6; i++)
    {
        if (avgWT[i] < avgWT[bestWT])
            bestWT = i;
        if (avgTAT[i] < avgTAT[bestTAT])
            bestTAT = i;
        if (avgRT[i] < avgRT[bestRT])
            bestRT = i;
    }

    cout << "\nBest Algorithm for Average Waiting Time: " << algos[bestWT] << " (Avg WT: " << avgWT[bestWT] << ")\n";
    cout << "Best Algorithm for Average Turnaround Time: " << algos[bestTAT] << " (Avg TAT: " << avgTAT[bestTAT] << ")\n";
    cout << "Best Algorithm for Average Response Time: " << algos[bestRT] << " (Avg RT: " << avgRT[bestRT] << ")\n";

    return 0;
}