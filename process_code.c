#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Process structure
struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
};

// Function to read processes from a file
vector<Process> readProcesses(const string& filename) {
    vector<Process> processes;
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file!" << endl;
        return processes;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Process p;
        ss >> p.pid >> p.arrivalTime >> p.burstTime >> p.priority;
        processes.push_back(p);
    }
    file.close();
    return processes;
}

// Function to calculate waiting & turnaround times
void calculateTimes(vector<Process>& processes) {
    int completionTime = 0;
    for (auto& p : processes) {
        if (completionTime < p.arrivalTime) {
            completionTime = p.arrivalTime;
        }
        p.waitingTime = completionTime - p.arrivalTime;
        p.turnaroundTime = p.waitingTime + p.burstTime;
        completionTime += p.burstTime;
    }
}

// First-Come, First-Served (FCFS) Scheduling
void fcfsScheduling(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime < b.arrivalTime;
    });

    calculateTimes(processes);

    cout << "\nFCFS Scheduling:\n";
    cout << "PID  Arrival  Burst  Waiting  Turnaround\n";
    for (const auto& p : processes) {
        cout << setw(3) << p.pid << setw(8) << p.arrivalTime << setw(7)
             << p.burstTime << setw(9) << p.waitingTime << setw(11) << p.turnaroundTime << endl;
    }
}

// Shortest Job First (SJF) Scheduling
void sjfScheduling(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.burstTime < b.burstTime;
    });

    calculateTimes(processes);

    cout << "\nSJF Scheduling:\n";
    cout << "PID  Arrival  Burst  Waiting  Turnaround\n";
    for (const auto& p : processes) {
        cout << setw(3) << p.pid << setw(8) << p.arrivalTime << setw(7)
             << p.burstTime << setw(9) << p.waitingTime << setw(11) << p.turnaroundTime << endl;
    }
}

// Main function
int main() {
    string filename = "processes.txt";
    vector<Process> processes = readProcesses(filename);

    if (processes.empty()) {
        cout << "No processes found.\n";
        return 1;
    }

    fcfsScheduling(processes);
    sjfScheduling(processes);

    return 0;
}
