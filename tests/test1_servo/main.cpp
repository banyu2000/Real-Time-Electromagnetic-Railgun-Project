// main.cpp
#include "PCA9685.h"
#include <iostream>
#include <csignal>
#include <chrono>
#include <thread>

// Pointer to the PCA9685 driver instance
PCA9685* pca = nullptr;

// Signal handler for SIGINT (Ctrl+C)
void signalHandler(int signum) {
    if(pca) {
        // Reset the horizontal servo to 90°
        pca->setAngle(0, 90);
        // Reset the vertical servo to 90°
        pca->setAngle(1, 90);
    }
    std::cout << "\nProgram terminated. Servos reset." << std::endl;
    exit(signum);
}

int main() {
    // Register the signal handler for SIGINT
    signal(SIGINT, signalHandler);

    try {
        // Create an instance of the PCA9685 driver
        PCA9685 driver;
        pca = &driver;

        // Control example
        while(true) {
            for(int angle = 0; angle <= 180; angle += 10) {
                // Set the angle of the horizontal servo
                driver.setAngle(0, angle);
                // Set the angle of the vertical servo
                driver.setAngle(1, 180 - angle);
                std::cout << "Horizontal: " << angle 
                          << "°\tVertical: " << 180 - angle << "°" << std::endl;
                // Wait for 1 second
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}