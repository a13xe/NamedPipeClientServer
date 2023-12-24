#include <windows.h>
#include <iostream>

#define PIPE_NAME "\\\\.\\pipe\\MyPipe"

int main() {
    HANDLE hPipe, hEvent;
    OVERLAPPED overlap = { 0 };
    char buffer[128];
    DWORD written;

    // Create a named pipe
    hPipe = CreateNamedPipe(
        PIPE_NAME, 
        PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, 
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 
        1, 128, 128, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create pipe." << std::endl;
        return 1;
    }

    // Create an event object for asynchronous operations
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        std::cerr << "Failed to create event." << std::endl;
        CloseHandle(hPipe);
        return 1;
    }
    overlap.hEvent = hEvent;

    // Server loop
    int choice;
    bool connected = false;
    while (true) {
        std::cout << "1. Connect to pipe\n"
                  << "2. Write to pipe\n"
                  << "3. Disconnect\n"
                  << "4. Exit\n"
                  << "Enter choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Connect to the pipe
                if (!ConnectNamedPipe(hPipe, &overlap)) {
                    if (GetLastError() == ERROR_IO_PENDING || GetLastError() == ERROR_PIPE_CONNECTED) {
                        WaitForSingleObject(hEvent, INFINITE);
                    } else {
                        std::cerr << "Failed to connect to pipe." << std::endl;
                    }
                }
                connected = true;
                std::cout << "Connected to pipe." << std::endl;
                break;

            case 2:
                // Write to the pipe
                if (connected) {
                    std::cout << "Enter message: ";
                    std::cin.ignore(); // Flush the newline character from the buffer
                    std::cin.getline(buffer, sizeof(buffer));

                    WriteFile(hPipe, buffer, strlen(buffer) + 1, &written, &overlap);
                    WaitForSingleObject(hEvent, INFINITE);
                    std::cout << "Message sent." << std::endl;
                } else {
                    std::cout << "Not connected to any client." << std::endl;
                }
                break;

            case 3:
                // Disconnect the pipe
                DisconnectNamedPipe(hPipe);
                connected = false;
                std::cout << "Disconnected." << std::endl;
                break;

            case 4:
                // Exit
                CloseHandle(hPipe);
                CloseHandle(hEvent);
                return 0;

            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }
}
