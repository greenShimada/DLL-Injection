#include <stdio.h>
#include <windows.h>

STARTUPINFO startupInfo;
PROCESS_INFORMATION processInformation;

int Inject_DLL(char* pathdll, char* exe_name) {

    if (!CreateProcess(exe_name, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInformation)) {
        printf("Erro ao criar processo\n");
        return FALSE;
    }
    int nLength = strlen(pathdll) + 1; 
    LPVOID lpRemoteString = VirtualAllocEx(processInformation.hProcess, NULL, nLength, MEM_COMMIT, PAGE_READWRITE);

    if (!lpRemoteString) {
        printf("Erro ao alocar memória no processo remoto\n");
        CloseHandle(processInformation.hProcess);
        return FALSE;
    }
    
    if (!WriteProcessMemory(processInformation.hProcess, lpRemoteString, pathdll, nLength, NULL)) {
        printf("Erro ao escrever na memória do processo remoto\n");
        VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);
        CloseHandle(processInformation.hProcess);
        return FALSE;
    }

    // Pega o endereço de LoadLibraryA dentro da thread do outro processo.
    LPVOID lpLoadLibraryA = GetProcAddress(GetModuleHandleW(L"KERNEL32.DLL"), "LoadLibraryA");
    HANDLE hThread = CreateRemoteThread(processInformation.hProcess, NULL, 0, 
        (LPTHREAD_START_ROUTINE)lpLoadLibraryA, lpRemoteString, 0, NULL);
        
    if (!hThread) {
        printf("Erro ao criar thread remota\n");
        VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);
        CloseHandle(processInformation.hProcess);
        return FALSE;
    }
    
    WaitForSingleObject(hThread, 4000);
    TerminateThread(hThread, 0);


    ResumeThread(processInformation.hThread);
    
    VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);
    CloseHandle(processInformation.hThread);
    CloseHandle(processInformation.hProcess);
    printf("Injeção de DLL concluída com sucesso\n");
    return TRUE;
}

int main() {
    Inject_DLL("mylib.dll", "msgbox.exe");
    return 0;
}
