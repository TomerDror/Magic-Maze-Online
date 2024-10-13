#include <windows.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <algorithm>

const int SHM_SIZE = 514;  // 2 buffers of 256 bytes each + 2 flags
const int BUFFER_SIZE = 256;

HANDLE hMapFile;
char *pBuf;

void send_to_python(const std::string &message) {
    if (message.length() > BUFFER_SIZE) {
        throw std::runtime_error("Message too long");
    }
    pBuf[1] = 1;  // Indicate message is ready
    std::memcpy(pBuf + 258, message.c_str(), message.length());
    std::memset(pBuf + 258 + message.length(), 0, BUFFER_SIZE - message.length());  // Clear remaining bytes
}

std::string recv_from_python() {
    if (pBuf[0] == 1) {
        std::string message(pBuf + 2, BUFFER_SIZE);
        auto null_char = std::find(message.begin(), message.end(), '\0');
        message.erase(null_char, message.end());
        pBuf[0] = 0;  // Clear the flag
        return message;
    }
    return "";
}

void listen_for_python_messages() {
    while (true) {
        std::string message = recv_from_python();
        if (!message.empty()) {
            std::cout << "Received from Python: " << message << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

bool run_python_script(const char* script_name) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Construct the command line to run the Python script
    std::string command = "python ";
    command += script_name;

    // Start the Python script process
    if (!CreateProcessA(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")." << std::endl;
        return false;
    }

    // Close process and thread handles (Python script will continue to run independently)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

int main() {
    // Run the Python script
    if (!run_python_script("CommsPython.py")) {
        return 1;
    }

    // Retry opening the shared memory block created by the Python script
    for (int attempts = 0; attempts < 10; ++attempts) {
        hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "my_shared_memory");
        if (hMapFile != NULL) {
            break;
        }
        std::cerr << "Could not open file mapping object (" << GetLastError() << "), retrying..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (hMapFile == NULL) {
        std::cerr << "Could not open file mapping object after several attempts." << std::endl;
        return 1;
    }

    pBuf = (char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);
    if (pBuf == NULL) {
        std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    std::thread listener_thread(listen_for_python_messages);
    listener_thread.detach();

    try {
        while (true) {
            std::string user_input;
            std::cout << "Enter message to send to Python : ";
            std::getline(std::cin, user_input);
            send_to_python(user_input);
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return 0;
}
