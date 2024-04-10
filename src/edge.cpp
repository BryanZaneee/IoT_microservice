#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <pigpio.h>

struct Service {
    std::string rpiId;
    std::string serviceName;
    int numInputParams;
    std::vector<std::string> inputParamNames;
};

std::vector<Service> services;

void reportService(const std::string& rpiId, const std::string& serviceName,
                   int numInputParams, const std::vector<std::string>& inputParamNames) {
    Service service;
    service.rpiId = rpiId;
    service.serviceName = serviceName;
    service.numInputParams = numInputParams;
    service.inputParamNames = inputParamNames;
    services.push_back(service);
}

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

int main(int argc, char* argv[]) {
    int port = 8888;  // Default port number

    if (argc > 1) {
        port = std::atoi(argv[1]);  // Get port number from command-line argument
    }

    // Initialize pigpio with the specified port number
    if (gpioInitialise_ex("localhost", port) < 0) {
        std::cout << "Failed to initialize pigpio" << std::endl;
        return 1;
    }

    // Report services
    reportService("RPi-1", "ReadTemperature", 1, {"F_or_C"});
    reportService("RPi-2", "ControlLED", 2, {"LEDNumber", "OnOff"});

    // Display service information on the dashboard
    displayServiceInfo();

    // Terminate pigpio
    gpioTerminate();

    return 0;
}