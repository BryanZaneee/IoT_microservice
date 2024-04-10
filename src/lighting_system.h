#ifndef LIGHTING_SYSTEM_HPP
#define LIGHTING_SYSTEM_HPP

#include <string>
#include <mutex>

class LightingSystem {
private:
    bool ledState;  // Current state of the LED
    int lightLevel; // Simulated light level reading
    std::mutex mtx; // Mutex for thread-safe access to the lighting system state

public:
    LightingSystem() : ledState(false), lightLevel(0) {}

    // Get the current status of the system as a JSON string
    std::string getStatus() {
        std::lock_guard<std::mutex> lock(mtx);
        // Constructing a JSON-like string (you might use a real JSON library in a full implementation)
        return "{ \"ledState\": " + std::string(ledState ? "true" : "false") +
               ", \"lightLevel\": " + std::to_string(lightLevel) + " }";
    }

    // Set the state of the LED
    void setLedState(bool state) {
        std::lock_guard<std::mutex> lock(mtx);
        ledState = state;
    }

    // Example method to simulate light level changes
    // In a real scenario, this would interface with your physical light sensor
    void setLightLevel(int level) {
        std::lock_guard<std::mutex> lock(mtx);
        lightLevel = level;
    }
};

#endif // LIGHTING_SYSTEM_HPP
