#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

mutex mtx;
condition_variable cv_hydrogen, cv_oxygen;
bool hydrogen_ready = false, oxygen_ready = false;

void Make_Hydro() {
    {
        lock_guard<mutex> lock(mtx);
        cout << "H" << endl;
        hydrogen_ready = true;
        cv_hydrogen.notify_one();
    }
}

void Make_O() {
    {
        lock_guard<mutex> lock(mtx);
        cout << "O" << endl;
        oxygen_ready = true;
        cv_oxygen.notify_one();
    }
}

void Make_Water() {
    {
        unique_lock<mutex> lock(mtx);
        cv_hydrogen.wait(lock, [] { return hydrogen_ready; });
        cv_hydrogen.wait(lock, [] { return hydrogen_ready; });
        cv_oxygen.wait(lock, [] { return oxygen_ready; });

        cout << "H2O" << endl;

        // Reset states
        hydrogen_ready = false;
        oxygen_ready = false;

        cv_hydrogen.notify_one();
        cv_hydrogen.notify_one();
        cv_oxygen.notify_one();
    }
}

void MakeH() {
    while (true) {
        Make_Hydro();
        this_thread::sleep_for(chrono::seconds(1)); // Simulate the time taken to make hydrogen
    }
}

void MakeO() {
    while (true) {
        Make_O();
        this_thread::sleep_for(chrono::seconds(2)); // Simulate the time taken to make oxygen
    }
}

void MakeWaterLoop() {
    while (true) {
        Make_Water();
    }
}

int main() {
    thread hydrogen_maker(MakeH);
    thread oxygen_maker(MakeO);
    thread water_maker(MakeWaterLoop);

    hydrogen_maker.join();
    oxygen_maker.join();
    water_maker.join();

    return 0;
}
