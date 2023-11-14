#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

int main() {
    // Replace /dev/cu.usbmodem* with the correct port for your Arduino Uno on macOS
    const std::string port = "/dev/tty.usbmodem144101";

    // Open the serial port
#ifdef _WIN32
    HANDLE hSerial = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }
#else
    int hSerial = open(port.c_str(), O_RDWR | O_NOCTTY);
    if (hSerial == -1) {
        std::cerr << "Error opening serial port." << std::endl;
        return 1;
    }
    struct termios tty;
    tcgetattr(hSerial, &tty);
    tty.c_cflag &= ~PARENB; // Clear parity bit
    tty.c_cflag &= ~CSTOPB; // Clear stop field
    tty.c_cflag |= CS8;     // 8 data bits
    tcsetattr(hSerial, TCSANOW, &tty);
#endif

    // Set the baud rate
    const int baudRate = 9600;
#ifdef _WIN32
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state." << std::endl;
        return 1;
    }
    dcbSerialParams.BaudRate = baudRate;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state." << std::endl;
        return 1;
    }
#else
    cfsetispeed(&tty, baudRate);
    cfsetospeed(&tty, baudRate);
#endif

    // Main loop for sending commands
    for (int speed = -100; speed <= 100; ++speed) {
        // Format the speed command
        std::string command = std::to_string((speed + 100) * 10) + '\n';

        // Send the command over serial
#ifdef _WIN32
        DWORD bytesWritten;
        WriteFile(hSerial, command.c_str(), command.size(), &bytesWritten, NULL);
#else
        write(hSerial, command.c_str(), command.size());
#endif

        // Optional delay to control the timing
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the serial port
#ifdef _WIN32
    CloseHandle(hSerial);
#else
    close(hSerial);
#endif

    return 0;
}
