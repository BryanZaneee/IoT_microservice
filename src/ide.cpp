#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <pigpio.h>

struct Service {
    std::string rpiId;
    std::string serviceName;
    int numInputParams;
    std::vector<std::string> inputParamNames;
};

std::vector<Service> services;

// Button pins
const int BUTTON_1_PIN = 0;
const int BUTTON_2_PIN = 1;

// LED pins
const int LED_1_PIN = 2;
const int LED_2_PIN = 3;

void registerService(const std::string& rpiId, const std::string& serviceName,
                     int numInputParams, const std::vector<std::string>& inputParamNames) {
    Service service;
    service.rpiId = rpiId;
    service.serviceName = serviceName;
    service.numInputParams = numInputParams;
    service.inputParamNames = inputParamNames;
    services.push_back(service);
}

void displayServices() {
    std::cout << "Available Services:" << std::endl;
    for (int i = 0; i < services.size(); i++) {
        std::cout << i + 1 << ". " << services[i].serviceName << " (RPi: " << services[i].rpiId << ")" << std::endl;
    }
}

void executeService(const Service& service) {
    std::cout << "Executing service: " << service.serviceName << std::endl;

    if (service.serviceName == "ButtonPress") {
        bool button1Pressed = gpioRead(BUTTON_1_PIN) == 1;
        bool button2Pressed = gpioRead(BUTTON_2_PIN) == 1;

        if (button1Pressed) {
            std::cout << "Button 1 pressed" << std::endl;
            gpioWrite(LED_1_PIN, 1);  // Turn on LED 1
            gpioDelay(1000000);  // Delay for 1 second
            gpioWrite(LED_1_PIN, 0);  // Turn off LED 1
        } else if (button2Pressed) {
            std::cout << "Button 2 pressed" << std::endl;
            gpioWrite(LED_2_PIN, 1);  // Turn on LED 2
            gpioDelay(1000000);  // Delay for 1 second
            gpioWrite(LED_2_PIN, 0);  // Turn off LED 2
        }
    }
}

void composeApplication() {
    std::vector<std::string> application;
    std::string input;

    std::cout << "Compose your IoT application by entering service names (or 'done' to finish):" << std::endl;

    while (true) {
        std::cout << "Enter a service name: ";
        std::getline(std::cin, input);

        if (input == "done") {
            break;
        }

        bool serviceFound = false;
        for (const auto& service : services) {
            if (service.serviceName == input) {
                application.push_back(input);
                serviceFound = true;
                break;
            }
        }

        if (!serviceFound) {
            std::cout << "Invalid service name. Please try again." << std::endl;
        }
    }

    std::cout << "\nApplication composed successfully!" << std::endl;
    std::cout << "Application services: ";
    for (const auto& service : application) {
        std::cout << service << " ";
    }
    std::cout << std::endl;

    std::cout << "\nExecuting the application..." << std::endl;

    for (const auto& serviceName : application) {
        for (const auto& service : services) {
            if (service.serviceName == serviceName) {
                executeService(service);  // Execute the service
                break;
            }
        }
    }

    std::cout << "\nApplication execution completed." << std::endl;
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

    // Set button pins as inputs
    gpioSetMode(BUTTON_1_PIN, PI_INPUT);
    gpioSetMode(BUTTON_2_PIN, PI_INPUT);

    // Set LED pins as outputs
    gpioSetMode(LED_1_PIN, PI_OUTPUT);
    gpioSetMode(LED_2_PIN, PI_OUTPUT);

    // Register services
    registerService("RPi-1", "ButtonPress", 0, {});

    std::string input;

    while (true) {
        displayServices();

        std::cout << "Enter 'compose' to create an application, 'execute' to run the ButtonPress service, or 'quit' to exit: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        } else if (input == "compose") {
            composeApplication();
        } else if (input == "execute") {
            executeService(services[0]);
        } else {
            std::cout << "Invalid command!" << std::endl;
        }

        std::cout << std::endl;
    }

    // Terminate pigpio
    gpioTerminate();

    return 0;
}