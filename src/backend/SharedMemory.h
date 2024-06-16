#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <algorithm>

class SharedMemory
{
public:
    SharedMemory(const std::string &name, size_t size);
    ~SharedMemory();

    bool write(const std::string &data);
    std::string read();

private:
    HANDLE hMapFile;
    LPVOID pBuf;
    size_t memSize;
};
