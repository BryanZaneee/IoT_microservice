#include <iostream>
#include <string>
#include <curl/curl.h> // Make sure to link against libcurl

// Function to send API requests to the edge layer
void sendRequest(const std::string& endpoint, const std::string& method, const std::string& data = "") {
    CURL* curl = curl_easy_init();
    if(curl) {
        CURLcode res;
        std::string url = "http://localhost:8080" + endpoint;
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        // Always cleanup
        curl_easy_cleanup(curl);
    }
}

int main() {
    std::string input;
    std::cout << "Simple IoT IDE for Emergency Lighting System" << std::endl;

    while (true) {
        std::cout << "Enter command ('status' to get light status, 'ledon' to turn LED on, 'ledoff' to turn LED off, 'exit' to quit): ";
        std::cin >> input;

        if (input == "exit") {
            break;
        } else if (input == "status") {
            sendRequest("/status", "GET");
        } else if (input == "ledon") {
            sendRequest("/led", "POST", "on");
        } else if (input == "ledoff") {
            sendRequest("/led", "POST", "off");
        } else {
            std::cout << "Unknown command." << std::endl;
        }
    }

    return 0;
}
