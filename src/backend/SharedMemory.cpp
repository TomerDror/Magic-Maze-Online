#include "SharedMemory.h"
#include <iostream>
#include <thread>

SharedMemory::SharedMemory(const std::string &name, size_t size) : memSize(size)
{
    for (int attempts = 0; attempts < 10; ++attempts)
    {
        hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name.c_str());
        if (hMapFile != NULL)
        {
            break;
        }
        std::cerr << "Could not open file mapping object (" << GetLastError() << "), retrying..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (hMapFile == NULL)
    {
        std::cerr << "Could not open file mapping object after several attempts." << std::endl;
    }

    // pBuf = (char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHM_SIZE);

    

    pBuf = MapViewOfFile(
        hMapFile,            // Handle to map object
        FILE_MAP_ALL_ACCESS, // Read/write permission
        0,
        0,
        size);

    if (pBuf == NULL)
    {
        std::cerr << "Could not map view of file: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
    }
}

SharedMemory::~SharedMemory()
{
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}

void SharedMemory::write(const std::string &message) {
    if (message.length() > BUFFER_SIZE) {
        throw std::runtime_error("Message too long");
    }
    pBuf[1] = 1;  // Indicate message is ready
    std::memcpy(pBuf + 258, message.c_str(), message.length());
    std::memset(pBuf + 258 + message.length(), 0, BUFFER_SIZE - message.length());  // Clear remaining bytes
}

std::string SharedMemory::read() {
    if (pBuf[0] == 1) {
        std::string message(pBuf + 2, BUFFER_SIZE);
        auto null_char = std::find(message.begin(), message.end(), '\0');
        message.erase(null_char, message.end());
        pBuf[0] = 0;  // Clear the flag
        return message;
    }
    return "";
}