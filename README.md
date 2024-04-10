# IoT Application README

This README provides instructions on setting up and running the IoT application code on a Raspberry Pi.

## Required Packages

To run the code successfully, you'll need to install the following packages on your Raspberry Pi:

1. C++ Compiler:
   - Install the GNU C++ compiler (g++) using the command:
     ```
     sudo apt-get install g++
     ```

2. pigpio Library:
   - Install the pigpio library and its development files using the command:
     ```
     sudo apt-get install libpigpio-dev
     ```

3. pigpiod Daemon:
   - Install the pigpiod daemon using the command:
     ```
     sudo apt-get install pigpiod
     ```

## Compilation and Execution

To compile and run the code, follow these steps:

1. Navigate to the directory where the code files (`button.cpp`, `LED.cpp`, `edge.cpp`, and `ide.cpp`) are located.

2. Compile the code files using the following commands:
    ```
    g++ -o button button.cpp -lpigpio -lrt
    ```
     ```
    g++ -o LED LED.cpp -lpigpio -lrt
    ```
    ```
    g++ -o edge edge.cpp -lpigpio -lrt
    ```
    ```
    g++ -o ide ide.cpp -lpigpio -lrt
    ```

    These commands will generate the executable files `button`, `LED`, `edge`, and `ide`. If you are receving a ` Can't lock /var/run/pigpio.pid Failed to initialize pigpio ` then run these commands: 
    ```
    sudo killall pigpiod
    ```
    ```
    sudo rm /var/run/pigpio.pid
    ```

3. Start the pigpiod daemon by running the command:
    ```
    sudo pigpiod
    ```

4. Open three separate terminal windows or tabs.

5. In the first terminal on RPi-1, run the `button` executable:
   ``` 
   ./button <ip>
   ```
   In the other terminal on RPi-2, run the `LED` executable:
   ``` 
   ./LED <ip>
   ```
    

6. In the second terminal, run the `edge` executable:
    ```
    ./edge <ip>
    ```
    This will start the edge layer code that reports services and displays service information on the dashboard.

7. In the third terminal, run the `ide` executable:
    ```
    ./ide <ip>
    ```
    This will start the IDE code that allows you to compose and execute IoT applications us

## Code Overview
The code consists of three main files that work together to create an IoT application:

1. `button.cpp`:
- This file represents the button monitoring service.
- It interacts with the buttons connected to the Raspberry Pi.
- It reads the button states and sends HTTP requests to the LED control service when buttons are pressed.

2. `LED.cpp`:
- This file represents the LED control service.
- It listens for HTTP requests to control the LEDs connected to the Raspberry Pi.
- It updates the LED states based on the received requests.

3. `edge.cpp`:
- This file represents the edge layer of the IoT application.
- It defines and reports the available services to the IDE.
- It displays the service information on the dashboard.

4. `ide.cpp`:
- This file represents the IDE (Integrated Development Environment) for composing and executing IoT applications.
- It allows the user to compose an application by selecting services from the available services reported by the edge layer.
- It executes the composed application by invoking the corresponding services.
- It interacts with the LED control service to control the LEDs based on user input.


The code files are connected through the use of common service names and the execution of services in the IDE. The edge layer reports the available services, and the IDE uses these services to compose and execute IoT applications. The physical layer provides the low-level functionality for interacting with the buttons and LEDs.

When you run the code, the physical layer (`phys`) continuously reads the button states and controls the LEDs accordingly. The edge layer (`edge`) reports the available services and displays them on the dashboard (in this case, the command line is the dashboard). The IDE (`ide`) allows you to compose an application by selecting services and executes the application by invoking the corresponding services, which in turn interact with the physical layer.
