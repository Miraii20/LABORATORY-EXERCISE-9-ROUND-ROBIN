#include <iostream>
#include <queue>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    char id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
};

void roundRobinScheduling(vector<Process>& processes, int timeQuantum) {
    queue<int> processQueue;
    int currentTime = 0;
    float totalWaitingTime = 0, totalTurnaroundTime = 0;
    int completed = 0;

    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int i = 0;
    while (i < processes.size() && processes[i].arrivalTime <= currentTime) {
        processQueue.push(i);
        i++;
    }

    while (completed < processes.size()) {
        if (!processQueue.empty()) {
            int index = processQueue.front();
            processQueue.pop();

            if (processes[index].remainingTime > timeQuantum) {
                currentTime += timeQuantum;
                processes[index].remainingTime -= timeQuantum;

                while (i < processes.size() && processes[i].arrivalTime <= currentTime) {
                    processQueue.push(i);
                    i++;
                }

                processQueue.push(index);
            } else {
                currentTime += processes[index].remainingTime;
                processes[index].remainingTime = 0;
                processes[index].completionTime = currentTime;
                processes[index].turnaroundTime = currentTime - processes[index].arrivalTime;
                processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
                totalWaitingTime += processes[index].waitingTime;
                totalTurnaroundTime += processes[index].turnaroundTime;

                completed++;

                while (i < processes.size() && processes[i].arrivalTime <= currentTime) {
                    processQueue.push(i);
                    i++;
                }
            }
        } else {
            if (i < processes.size()) {
                currentTime = processes[i].arrivalTime;
                processQueue.push(i);
                i++;
            }
        }
    }

    cout << "Process | Arrival Time | Burst Time | Completion Time | Waiting Time | Turnaround Time\n";
    cout << "-------------------------------------------------------------------------------------\n";
    for (auto& p : processes) {
        cout << setw(7) << p.id << " | "
             << setw(12) << p.arrivalTime << " | "
             << setw(10) << p.burstTime << " | "
             << setw(15) << p.completionTime << " | "
             << setw(12) << p.waitingTime << " | "
             << setw(15) << p.turnaroundTime << "\n";
    }

    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << totalWaitingTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << totalTurnaroundTime / processes.size() << endl;
}

int main() {
    int n, timeQuantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    cout << "Enter the arrival time and burst time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = 'A' + i;
        cout << "Process " << processes[i].id << " Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "Process " << processes[i].id << " Burst Time: ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }

    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    roundRobinScheduling(processes, timeQuantum);

    return 0;
}
