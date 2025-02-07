#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <psapi.h>

//Get PID of a process by its name//
DWORD GetPidByName(const wchar_t* pName) {
	PROCESSENTRY32 pEntry;
	HANDLE snapshot;

	pEntry.dwSize = sizeof(PROCESSENTRY32);
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // Take a snapshot of all processes

	if (snapshot == INVALID_HANDLE_VALUE) {
		printf("Failed to create process snapshot.\n");
		return 0;
	}

	if (Process32First(snapshot, &pEntry)) { // Start iterating through processes
		do {
			if (_wcsicmp(pEntry.szExeFile, pName) == 0) { // Case-insensitive comparison
				DWORD pid = pEntry.th32ProcessID;
				CloseHandle(snapshot); // Close handle before returning
				return pid;
			}
		} while (Process32Next(snapshot, &pEntry)); // Continue iteration
	}

	CloseHandle(snapshot); // Ensure the snapshot handle is closed
	return 0;
}

//Main Funtion Creation
int main(void) {

// Declare structures for process creation
	STARTUPINFOEX info = { sizeof(0) };
	PROCESS_INFORMATION processInfo;

// Variables for attribute list and process handles
	SIZE_T cbAttributeListSize = 0;
	PPROC_THREAD_ATTRIBUTE_LIST pAttributeList = NULL;
	HANDLE hExplorerProcess = NULL;
	DWORD dwExplorerPid = 0;

// Get the PID of notepad.exe(trusted Windows process)
	dwExplorerPid = GetPidByName(L"notepad.exe");

// If notepad.exe is not found, use the current process as the parent
	if (dwExplorerPid == 0) {
		dwExplorerPid = GetCurrentProcessId();
	}

	//Initialize the process attribute list
	InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize);
	pAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, cbAttributeListSize);
	InitializeProcThreadAttributeList(pAttributeList, 1, 0, &cbAttributeListSize);

	//Open a handle to the notepad.exe process
	hExplorerProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwExplorerPid);
	UpdateProcThreadAttribute(pAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hExplorerProcess, sizeof(HANDLE), NULL, NULL); //Assigning notepad.exe as the PP

	info.lpAttributeList = pAttributeList;

	//Creating a New Process(mspaint.exe) with the Spoofed PPID
	CreateProcessA(
		NULL,
		(LPSTR)"mspaint.exe",
		NULL,
		NULL,
		FALSE,
		EXTENDED_STARTUPINFO_PRESENT, //applies the modified PP info
		NULL,
		NULL,
		(LPSTARTUPINFOA)&info.StartupInfo, &processInfo);

	printf("Current PID: %d\n", GetCurrentProcessId()); //Current running program
	printf("notepad PID: %d\n", dwExplorerPid); //spoofed parent process
	printf("mspaint PID: %d\n", processInfo.dwProcessId); //newly created process

	WaitForSingleObject(processInfo.hProcess, INFINITE);

	Sleep(3000);

	DeleteProcThreadAttributeList(pAttributeList);
	CloseHandle(hExplorerProcess);
}