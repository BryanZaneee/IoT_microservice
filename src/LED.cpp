#include <iostream>
#include <pigpio.h>
#include <cpprest/http_listener.h>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// LED pins
const int LED_1_PIN = 2;
const int LED_2_PIN = 3;

// Handle control requests for LEDs
void handleLEDControl(http_request request) {
    // Check if the path is for LED control
    auto path = request.relative_uri().path();
    if (path == U("/led")) {
        auto query = uri::split_query(request.request_uri().query());
        int ledNumber = std::stoi(query["number"]);
        bool onOff = query["state"] == "1";

        if (ledNumber == 1) {
            gpioWrite(LED_1_PIN, onOff ? 1 : 0);
        } else if (ledNumber == 2) {
            gpioWrite(LED_2_PIN, onOff ? 1 : 0);
        }

        request.reply(status_codes::OK);
    } else {
        request.reply(status_codes::NotFound);
    }
}

int main(int argc, char* argv[]) {
    // Check if IP address is provided
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <ip_address>" << std::endl;
        return 1;
    }

    std::string ipAddress(argv[1]);

    // Initialize pigpio
    if (gpioInitialise() < 0) {
        std::cout << "Failed to initialize pigpio" << std::endl;
        return 1;
    }

    // Set LED pins as outputs
    gpioSetMode(LED_1_PIN, PI_OUTPUT);
    gpioSetMode(LED_2_PIN, PI_OUTPUT);

    http_listener listener(utility::conversions::to_string_t("http://") + utility::conversions::to_string_t(ipAddress) + utility::conversions::to_string_t(":8080"));

    listener.support(methods::POST, handleLEDControl);

    try {
        listener.open().wait();
        std::cout << "Listening for LED control requests on " << ipAddress << "..." << std::endl;

        while (true) {
            // Keep the server running
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Terminate pigpio
    gpioTerminate();

    return 0;
}
