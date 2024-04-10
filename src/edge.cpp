#include "lighting_system.h"
#include <httplib.h>

class RestServer {
private:
    LightingSystem* system;
    httplib::Server server;

public:
    void setup(LightingSystem* systemInstance) {
        this->system = systemInstance;

        // Endpoint to get the current light level and LED status
        server.Get("/status", [this](const httplib::Request&, httplib::Response& res) {
            auto status = this->system->getStatus();
            res.set_content(status, "application/json");
        });

        // Endpoint to update LED state
        server.Post("/led", [this](const httplib::Request& req, httplib::Response& res) {
            bool newState = req.body == "on";
            this->system->setLedState(newState);
            res.set_content("LED state updated", "text/plain");
        });
    }

    void run() {
        server.listen("localhost", 8080);
    }
};

int main() {
    LightingSystem system;  // Instance of your LightingSystem, presumably interfacing with phys.cpp
    RestServer server;      // Create a server instance

    server.setup(&system);  // Setup server with your system
    server.run();           // Start the server

    return 0;
}
