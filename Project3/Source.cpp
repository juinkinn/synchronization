#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

mutex chassis_mutex; // Mutex for chassis production
mutex tires_mutex;   // Mutex for tire production
condition_variable cv;
int num_tires_ready = 0; // Number of tires ready for chassis production

void Produce_tire() {
    cout << "Producing tire" << endl;
    this_thread::sleep_for(chrono::seconds(1)); // Simulate tire production time
}

void Put_tire_to_chassis() {
    this_thread::sleep_for(chrono::seconds(1)); // Simulate tire attaching time
    num_tires_ready++;
    if (num_tires_ready == 4) {
        cv.notify_one(); // Signal chassis production when enough tires are ready
    }
}

void Produce_chassis() {
    // Produce chassis
    cout << "Producing chassis" << endl;
    this_thread::sleep_for(chrono::seconds(1)); // Simulate chassis production time
}

void MakeChassis() {
    while (true) {
        unique_lock<mutex> lock(tires_mutex);
        cv.wait(lock, [] { return num_tires_ready >= 4; }); // Wait until enough tires are ready
        lock.unlock();

        // Call Produce_chassis function
        Produce_chassis();

        // Reset the number of tires ready
        num_tires_ready = 0;
    }
}

void MakeTires() {
    while (true) {
        lock_guard<mutex> lock(chassis_mutex);
        Produce_tire();
        Put_tire_to_chassis();
    }
}

int main() {
    const int num_chassis = 3;

    thread tires_thread(MakeTires);
    thread chassis_thread(MakeChassis);

    // Joining threads
    tires_thread.join();
    chassis_thread.join();

    return 0;
}
