#include <windows.h>
#include <iostream>

// Функция, определяющая приоритет текущего процесса
DWORD WINAPI ThreadFunc(LPVOID lpParam) {

    // Дескриптор текущего процесса
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Класс приоритета текущего процесса
    DWORD priorityClass = GetPriorityClass(hCurrentProcess);

    if (priorityClass == 0) {
        std::cerr << "Error: Failed to retrieve priority class. Error code: "
            << GetLastError() << "\n";
        return 1;
    }

    // Вывод класса приоритета
    std::cout << "Current process priority class: ";
    switch (priorityClass) {
    case NORMAL_PRIORITY_CLASS:       std::cout << "NORMAL_PRIORITY_CLASS\n"; break;
    case IDLE_PRIORITY_CLASS:         std::cout << "IDLE_PRIORITY_CLASS\n"; break;
    case HIGH_PRIORITY_CLASS:         std::cout << "HIGH_PRIORITY_CLASS\n"; break;
    case REALTIME_PRIORITY_CLASS:     std::cout << "REALTIME_PRIORITY_CLASS\n"; break;
    case BELOW_NORMAL_PRIORITY_CLASS: std::cout << "BELOW_NORMAL_PRIORITY_CLASS\n"; break;
    case ABOVE_NORMAL_PRIORITY_CLASS: std::cout << "ABOVE_NORMAL_PRIORITY_CLASS\n"; break;
    default:                          std::cout << "Unknown Class (" << priorityClass << ")\n"; break;
    }

    return 0;
}

int main() {
    DWORD threadId;
    // Create the separate thread running ThreadFunc
    HANDLE hThread = CreateThread(
        NULL,           // Default security attributes
        0,              // Default stack size (1MB)
        ThreadFunc,     // Pointer to the thread function
        NULL,           // Argument passed to the thread function (void*)
        0,              // Default creation flags (start running immediately)
        &threadId       // Variable to receive the thread identifier
    );

    // Error handling for thread creation
    if (hThread == NULL) {
        std::cerr << "Error: Failed to create thread. Error code: "
            << GetLastError() << "\n";
        return 1;
    }

    // Wait for the worker thread to finish its work before main exits
    WaitForSingleObject(hThread, INFINITE);

    // Clean up the thread handle kernel object
    CloseHandle(hThread);

    return 0;
}