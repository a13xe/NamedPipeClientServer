#include <windows.h>
#include <iostream>

#define PIPE_NAME "\\\\.\\pipe\\MyPipe"

int main() {
    HANDLE hPipe;
    OVERLAPPED overlap = { 0 };
    char buffer[128];
    DWORD read;

    // Connect to the named pipe
    hPipe = CreateFile(
        PIPE_NAME, 
        GENERIC_READ, 
        0, NULL, OPEN_EXISTING, 
        FILE_FLAG_OVERLAPPED, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to connect to pipe." << std::endl;
        return 1;
    }

    // Client loop
    int choice;
    while (true) {
        std::cout << "1. Read from pipe\n"
                  << "2. Exit\n"
                  << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Read from the pipe
                if (ReadFile(hPipe, buffer, sizeof(buffer), &read, &overlap)) {
                    WaitForSingleObject(overlap.hEvent, INFINITE);
                    std::cout << "Received message: " << buffer << std::endl;
                } else {
                    std::cerr << "Failed to read from pipe." << std::endl;
                }
                break;

            case 2:
                // Exit
                CloseHandle(hPipe);
                return 0;

            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }
}
