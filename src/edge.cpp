#include <iostream>
#include <vector>
#include <string>
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

void sendButtonPress(int buttonNumber, const std::string& ipAddress) {
    http_client client(U("http://" + utility::conversions::to_string_t(ipAddress) + U":8080"));
    uri_builder builder(U("/button"));
    builder.append_query(U("number"), buttonNumber);

    http_request request(methods::POST);
    request.set_request_uri(builder.to_string());

    client.request(request).then([](http_response response) {
        if (response.status_code() == status_codes::OK) {
            std::cout << "Button press sent successfully." << std::endl;
        } else {
            std::cout << "Failed to send button press." << std::endl;
        }
    });
}

int main(int argc, char* argv[]) {
    // Check if IP address is provided
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <ip_address>" << std::endl;
        return 1;
    }

    std::string ipAddress(argv[1]);

    // Report services
    reportService("RPi-1", "ButtonPress", 1, {"ButtonNumber"});
    reportService("RPi-2", "ControlLED", 2, {"LEDNumber", "OnOff"});

    // Display service information on the dashboard
    displayServiceInfo();

    // Example usage: send button press to RPi-2
    sendButtonPress(1, ipAddress);

    return 0;
}