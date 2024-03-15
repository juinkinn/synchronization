#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// Constants
const int MAX_CARS_ON_BRIDGE = 3;
const int NO_DIRECTION = -1;

// Global variables
mutex mtx;
condition_variable cv;
int carsOnBridge = 0;
int directionOnBridge = NO_DIRECTION;

// Function to simulate a car arriving at the bridge
void ArriveBridge(int direction) {
    unique_lock<mutex> lock(mtx);
    cv.wait(lock, [&] {
        return carsOnBridge < MAX_CARS_ON_BRIDGE &&
            (directionOnBridge == NO_DIRECTION || directionOnBridge == direction);
        });

    // Increment the count of cars on the bridge and update direction
    carsOnBridge++;
    directionOnBridge = direction;

    // Output arrival message
    cout << "Car arrived on bridge, direction: " << direction
        << ", cars on bridge: " << carsOnBridge << endl;
}

// Function to simulate a car exiting from the bridge
void ExitBridge() {
    unique_lock<mutex> lock(mtx);

    // Decrement the count of cars on the bridge
    carsOnBridge--;

    // Reset direction if no cars are left on the bridge
    if (carsOnBridge == 0) {
        directionOnBridge = NO_DIRECTION;
    }

    // Notify waiting threads
    cv.notify_all();

    // Output exit message
    cout << "Car exited from bridge, cars on bridge: " << carsOnBridge << endl;
}

// Function representing the actions a car takes
void Car(int direction) {
    ArriveBridge(direction);
    // Simulate passing the bridge
    ExitBridge();
}

int main() {
    // Create a vector to hold car threads
    vector<thread> cars;

    // Spawn threads representing cars
    for (int i = 0; i < 10; ++i) {
        cars.emplace_back(Car, i % 2); // Alternating direction
    }

    // Join car threads with the main thread
    for (auto& car : cars) {
        car.join();
    }

    return 0;
}
