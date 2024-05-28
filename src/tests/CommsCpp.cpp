#include <windows.h>
#include <iostream>


int main() {
    // Open the shared memory block created by the Python script
    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,   // Read/write access
        FALSE,                 // Do not inherit the name
        "my_shared_memory");  // Name of the shared memory object

    if (hMapFile == NULL) {
        std::cerr << "Could not open file mapping object (" << GetLastError() << ")." << std::endl;
        return 1;
    }

    // Map the shared memory block to the process's address space
    char *pBuf = (char *) MapViewOfFile(hMapFile,   // Handle to map object
                                         FILE_MAP_ALL_ACCESS, // Read/write permission
                                         0,                   
                                         0,                   
                                         1024);              // Size of the mapping

    if (pBuf == NULL) {
        std::cerr << "Could not map view of file (" << GetLastError() << ")." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    // Access the data in shared memory
    std::cout << "Data in shared memory: ";
    for (int i = 0; i < 4; ++i) {
        std::cout << (int)pBuf[i] << " ";
    }
    std::cout << std::endl;

    // Clean up
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
