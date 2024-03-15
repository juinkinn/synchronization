#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
using namespace std;


int hackers = 0;
int employees = 0;
mutex mtx;

bool HackerArrives() {
    mtx.lock();
    hackers++;
    if (hackers % 4 == 0 && employees % 4 != 3) {
        cout << "Hacker: Enough hackers to cross the river." << endl;
        mtx.unlock();
        return true;
    }
    else {
        cout << "Hacker: Waiting for more hackers or proper mix." << endl;
        mtx.unlock();
        return false;
    }
}

bool EmployeeArrives() {
    mtx.lock();
    employees++;
    if (employees % 4 == 0 && hackers % 4 != 3) {
        cout << "Employee: Enough employees to cross the river." << endl;
        mtx.unlock();
        return true;
    }
    else {
        cout << "Employee: Waiting for more employees or proper mix." << endl;
        mtx.unlock();
        return false;
    }
}

void Hacker() {
    while (true) {
        // Simulate running to the river
        // Sleep for a random duration
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Hacker arrived at the river." << endl;

        // Check if conditions allow the hacker to board the boat
        if (HackerArrives()) {
            // If conditions allow, hacker crosses the river
            cout << "Hacker crossing the river..." << endl;
            // Simulate crossing the river
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

void Employee() {
    while (true) {
        // Simulate running to the river
        // Sleep for a random duration
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Employee arrived at the river." << endl;

        // Check if conditions allow the employee to board the boat
        if (EmployeeArrives()) {
            // If conditions allow, employee crosses the river
            cout << "Employee crossing the river..." << endl;
            // Simulate crossing the river
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

int main() {
    // Create threads for hackers and employees
    vector<thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.push_back(thread(Hacker));
        threads.push_back(thread(Employee));
    }

    // Join threads
    for (auto& th : threads) {
        th.join();
    }

    return 0;
}
