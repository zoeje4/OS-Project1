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
    int startTime;
    int completionTime;
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
        p.waitingTime = p.turnaroundTime = p.startTime = p.completionTime = 0;
        processes.push_back(p);
    }
    file.close();
    return processes;
}

// Function to calculate waiting, turnaround, start, and completion times
void calculateTimes(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.startTime = currentTime;
        p.completionTime = currentTime + p.burstTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        p.turnaroundTime = p.completionTime - p.arrivalTime;
        currentTime += p.burstTime;
    }
}

// Function to display a Gantt chart
void displayGanttChart(const vector<Process>& processes) {
    cout << "\nGantt Chart:\n";

    // Top border
    cout << " ";
    for (const auto& p : processes) {
        for (int i = 0; i < p.burstTime; i++) cout << "--";
        cout << " ";
    }
    cout << "\n|";

    // Process execution sequence
    for (const auto& p : processes) {
        for (int i = 0; i < p.burstTime - 1; i++) cout << " ";
        cout << "P" << p.pid;
        for (int i = 0; i < p.burstTime - 1; i++) cout << " ";
        cout << "|";
    }
    cout << "\n ";

    // Bottom border
    for (const auto& p : processes) {
        for (int i = 0; i < p.burstTime; i++) cout << "--";
        cout << " ";
    }
    cout << "\n";

    // Timeline
    cout << "0";
    for (const auto& p : processes) {
        for (int i = 0; i < p.burstTime; i++) cout << "  ";
        cout << p.completionTime;
    }
    cout << "\n";
}

// Function to calculate and display average WT and TAT
void displayAverages(const vector<Process>& processes) {
    double totalWT = 0, totalTAT = 0;
    int n = processes.size();

    cout << "\nWaiting Time (WT) and Turnaround Time (TAT) for each process:\n";
    cout << "PID  WT  TAT\n";
    for (const auto& p : processes) {
        cout << setw(3) << p.pid << setw(4) << p.waitingTime << setw(5) << p.turnaroundTime << endl;
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
    }

    cout << "\nAverage Waiting Time (AWT): " << fixed << setprecision(2) << (totalWT / n) << endl;
    cout << "Average Turnaround Time (ATAT): " << fixed << setprecision(2) << (totalTAT / n) << endl;
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

    displayGanttChart(processes);
    displayAverages(processes);
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

    displayGanttChart(processes);
    displayAverages(processes);
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
