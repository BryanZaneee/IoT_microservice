#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cpprest/http_client.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

struct Service {
    std::string rpiId;
    std::string serviceName;
    int numInputParams;
    std::vector<std::string> inputParamNames;
};

std::vector<Service> services;

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

void executeService(const Service& service, const std::string& ipAddress) {
    std::cout << "Executing service: " << service.serviceName << std::endl;

    if (service.serviceName == "ControlLED") {
        int ledNumber;
        bool onOff;

        std::cout << "Enter LED number: ";
        std::cin >> ledNumber;

        std::cout << "Enter LED state (1 for on, 0 for off): ";
        std::cin >> onOff;

        http_client client(U("http://" + utility::conversions::to_string_t(ipAddress) + U":8080"));
        uri_builder builder(U("/led"));
        builder.append_query(U("number"), ledNumber);
        builder.append_query(U("state"), onOff);

        http_request request(methods::POST);
        request.set_request_uri(builder.to_string());

        client.request(request).then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                std::cout << "LED control request sent successfully." << std::endl;
            } else {
                std::cout << "Failed to send LED control request." << std::endl;
            }
        });
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
}

int main(int argc, char* argv[]) {
    // Check if IP address is provided
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <ip_address>" << std::endl;
        return 1;
    }

    std::string ipAddress(argv[1]);

    // Register services
    registerService("RPi-1", "ButtonPress", 1, {"ButtonNumber"});
    registerService("RPi-2", "ControlLED", 2, {"LEDNumber", "OnOff"});

    std::string input;

    while (true) {
        displayServices();

        std::cout << "Enter 'compose' to create an application or 'quit' to exit: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            break;
        } else if (input == "compose") {
            composeApplication();

            std::cout << "\nExecuting the application..." << std::endl;

            for (const auto& serviceName : application) {
                for (const auto& service : services) {
                    if (service.serviceName == serviceName) {
                        executeService(service, ipAddress);
                        break;
                    }
                }
            }

            std::cout << "\nApplication execution completed." << std::endl;
        } else {
            std::cout << "Invalid command!" << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}