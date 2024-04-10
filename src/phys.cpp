#include <iostream>
#include <wiringPi.h>

// GPIO pin numbers using WiringPi's numbering scheme
const int ledPin = 0;  // LED connected to GPIO pin 0 (WiringPi pin number)
const int lightSensorPin = 1;  // Light sensor connected to GPIO pin 1 (WiringPi pin number)

void setup() {
    wiringPiSetup();  // Initialize WiringPi
    pinMode(ledPin, OUTPUT);  // Set the LED pin to output mode
    pinMode(lightSensorPin, INPUT);  // Set the light sensor pin to input mode
    digitalWrite(ledPin, LOW); // Ensure LED is off initially
    std::cout << "System Initialized.\n";
}

void loop() {
    // Read the light sensor value
    int lightLevel = digitalRead(lightSensorPin);

    // If the sensor detects low light, turn on the LED
    if (lightLevel == LOW) {
        digitalWrite(ledPin, HIGH);  // Low light detected, turn on the LED
        std::cout << "Low light detected - LED on.\n";
    } else {
        digitalWrite(ledPin, LOW);   // Adequate light detected, turn off the LED
        std::cout << "Adequate light - LED off.\n";
    }

    delay(1000);  // Wait for a second before the next reading
}

int main() {
    setup();
    while (true) {
        loop();
    }

    return 0;
}
