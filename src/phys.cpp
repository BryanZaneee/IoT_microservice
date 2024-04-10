#include <iostream>
#include <pigpio.h>

// Button and LED pin constants
const int BUTTON_1_PIN = 0;
const int BUTTON_2_PIN = 1;
const int LED_1_PIN = 2;
const int LED_2_PIN = 3;

// Button state variables
bool button1Pressed = false;
bool button2Pressed = false;

// Function to read button states
void readButtons() {
    button1Pressed = gpioRead(BUTTON_1_PIN) == HIGH;
    button2Pressed = gpioRead(BUTTON_2_PIN) == HIGH;
}

// Function to set LED states
void setLEDs(bool led1On, bool led2On) {
    gpioWrite(LED_1_PIN, led1On ? HIGH : LOW);
    gpioWrite(LED_2_PIN, led2On ? HIGH : LOW);
}

int main() {
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        std::cout << "Failed to initialize pigpio" << std::endl;
        return 1;
    }

    // Set button pins as inputs and LED pins as outputs
    gpioSetMode(BUTTON_1_PIN, PI_INPUT);
    gpioSetMode(BUTTON_2_PIN, PI_INPUT);
    gpioSetMode(LED_1_PIN, PI_OUTPUT);
    gpioSetMode(LED_2_PIN, PI_OUTPUT);

    while (true) {
        readButtons();  // Read button states

        // Perform actions based on button states
        if (button1Pressed) {
            setLEDs(true, false);  // Turn on LED 1
            std::cout << "Button 1 pressed" << std::endl;
        } else if (button2Pressed) {
            setLEDs(false, true);  // Turn on LED 2
            std::cout << "Button 2 pressed" << std::endl;
        } else {
            setLEDs(false, false);  // Turn off both LEDs
        }

        gpioDelay(100000);  // Delay to debounce buttons
    }

    gpioTerminate();  // Terminate pigpio
    return 0;
}