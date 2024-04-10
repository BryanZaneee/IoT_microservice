#include <iostream>
#include <vector>
#include <string>
#include <pigpio.h>

// Service structure
struct Service {
    std::string rpiId;
    std::string serviceName;
    int numInputParams;
    std::vector<std::string> inputParamNames;
};

std::vector<Service> services;  // Vector to store services

// Function to report a service
void reportService(const std::string& rpiId, const std::string& serviceName,
                   int numInputParams, const std::vector<std::string>& inputParamNames) {
    Service service;
    service.rpiId = rpiId;
    service.serviceName = serviceName;
    service.numInputParams = numInputParams;
    service.inputParamNames = inputParamNames;
    services.push_back(service);
}

// Function to display service information
void displayServiceInfo() {
    std::cout << "Available Services:" << std::endl;
    for (const auto& service : services) {
        std::cout << "RPi ID: " << service.rpiId << std::endl;
        std::cout << "Service Name: " << service.serviceName << std::endl;
        std::cout << "Number of Input Parameters: " << service.numInputParams << std::endl;
        std::cout << "Input Parameter Names: ";
        for (const auto& paramName : service.inputParamNames) {
            std::cout << paramName << " ";
        }
        std::cout << std::endl << std::endl;
    }
}

int main() {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        std::cout << "Failed to initialize pigpio" << std::endl;
        return 1;
    }

    // Report services
    reportService("RPi-1", "ReadTemperature", 1, {"F_or_C"});
    reportService("RPi-2", "ControlLED", 2, {"LEDNumber", "OnOff"});

    displayServiceInfo();  // Display service information on the dashboard

    gpioTerminate();  // Terminate pigpio
    return 0;
}