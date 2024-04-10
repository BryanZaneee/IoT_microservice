#include <iostream>
#include <cstdlib>
#include <pigpio.h>

// Button pins
const int BUTTON_1_PIN = 0;
const int BUTTON_2_PIN = 1;

// LED pins
const int LED_1_PIN = 2;
const int LED_2_PIN = 3;

// Button states
bool button1Pressed = false;
bool button2Pressed = false;

// Function to read button states
void readButtons() {
    button1Pressed = gpioRead(BUTTON_1_PIN) == 1;
    button2Pressed = gpioRead(BUTTON_2_PIN) == 1;
}

// Function to set LED states
void setLEDs(bool led1On, bool led2On) {
    gpioWrite(LED_1_PIN, led1On ? 1 : 0);
    gpioWrite(LED_2_PIN, led2On ? 1 : 0);
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

    while (true) {
        readButtons();

        // Perform actions based on button states
        if (button1Pressed) {
            setLEDs(true, false);
            // Perform action for button 1 press
            std::cout << "Button 1 pressed" << std::endl;
        } else if (button2Pressed) {
            setLEDs(false, true);
            // Perform action for button 2 press
            std::cout << "Button 2 pressed" << std::endl;
        } else {
            setLEDs(false, false);
        }

        // Small delay to debounce buttons
        gpioDelay(100000); // Delay in microseconds
    }

    // Terminate pigpio
    gpioTerminate();

    return 0;
}