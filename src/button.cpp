#include <iostream>
#include <pigpio.h>
#include <cpprest/http_client.h>
#include <string>

using namespace web;
using namespace web::http;
using namespace web::http::client;

// Button pins
const int BUTTON_1_PIN = 0;
const int BUTTON_2_PIN = 1;

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

    // Set button pins as inputs
    gpioSetMode(BUTTON_1_PIN, PI_INPUT);
    gpioSetMode(BUTTON_2_PIN, PI_INPUT);

    http_client client(utility::conversions::to_string_t(U("http://")) +
                       utility::conversions::to_string_t(ipAddress) +
                       utility::conversions::to_string_t(U(":8080")));

    while (true) {
        bool button1Pressed = gpioRead(BUTTON_1_PIN) == 1;
        bool button2Pressed = gpioRead(BUTTON_2_PIN) == 1;

        if (button1Pressed) {
            std::cout << "Button 1 pressed" << std::endl;
            uri_builder builder(U("/button"));
            builder.append_query(U("number"), 1);
            http_request request(methods::POST);
            request.set_request_uri(builder.to_string());
            client.request(request).then([](http_response response) {
                if (response.status_code() != status_codes::OK) {
                    std::cout << "Failed to send button press." << std::endl;
                }
            });
        } else if (button2Pressed) {
            std::cout << "Button 2 pressed" << std::endl;
            uri_builder builder(U("/button"));
            builder.append_query(U("number"), 2);
            http_request request(methods::POST);
            request.set_request_uri(builder.to_string());
            client.request(request).then([](http_response response) {
                if (response.status_code() != status_codes::OK) {
                    std::cout << "Failed to send button press." << std::endl;
                }
            });
        }

        gpioDelay(100000); // Delay in microseconds
    }

    // Terminate pigpio
    gpioTerminate();
    return 0;
}