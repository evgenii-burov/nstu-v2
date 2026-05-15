#include <windows.h>
#include <iostream>

// Функция, определяющая приоритет текущего процесса
DWORD WINAPI ThreadFunc(LPVOID lpParam) {

    // Получение дескриптора текущего процесса
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Получение класса приоритета текущего процесса
    DWORD priorityClass = GetPriorityClass(hCurrentProcess);

    if (priorityClass == 0) {
        std::cerr << "Error: Failed to retrieve priority class. Error code: "
            << GetLastError() << "\n";
        return 1;
    }

    // Вывод класса приоритета
    std::cout << "Current process priority class: ";
    switch (priorityClass) {
    case NORMAL_PRIORITY_CLASS:
        std::cout << "NORMAL_PRIORITY_CLASS\n";
        break;
    case IDLE_PRIORITY_CLASS:
        std::cout << "IDLE_PRIORITY_CLASS\n";
        break;
    case HIGH_PRIORITY_CLASS:
        std::cout << "HIGH_PRIORITY_CLASS\n";
        break;
    case REALTIME_PRIORITY_CLASS:
        std::cout << "REALTIME_PRIORITY_CLASS\n";
        break;
    case BELOW_NORMAL_PRIORITY_CLASS:
        std::cout << "BELOW_NORMAL_PRIORITY_CLASS\n";
        break;
    case ABOVE_NORMAL_PRIORITY_CLASS:
        std::cout << "ABOVE_NORMAL_PRIORITY_CLASS\n";
        break;
    default:
        std::cout << "Unknown Class (" << priorityClass << ")\n";
        break;
    }

    return 0;
}

int main() {
    DWORD threadId;
    // Создание присоединённого потока
    HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &threadId);

    if (hThread == NULL) {
        std::cerr << "Error: Failed to create thread. Error code: "
            << GetLastError() << "\n";
        return 1;
    }

    // Ожидание завершения созданного потока
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);

    return 0;
}