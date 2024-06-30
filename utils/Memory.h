#pragma once
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

class CMemory
{
private:
	DWORD pid = 0;
	HANDLE targetHandle = 0;
	const char* procName;

	DWORD FindProcessId(const char* procName)
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnap)
			return NULL;

		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		// Durchlaufen aller aktiven Prozesse.
		while (Process32Next(hSnap, &procEntry))
		{
			// Wird der Name vom Prozess zu unserem gesuchtem Namen verglichen
			if (!strcmp(procEntry.szExeFile, procName))
			{
				CloseHandle(hSnap); // Wird geschlossen weil nicht mehr gebraucht.
				return procEntry.th32ProcessID; // Wenn ein Prozess mit dem richtigen Namen gefunden wird, wird deren id rausgegeben.
			}
		}

		CloseHandle(hSnap);
		return NULL;
	}
public:
	CMemory(const char* name) : procName(name) {}
	~CMemory()
	{
		// Aufräumen von unseren Sachen.
		if (this->targetHandle != nullptr)
			CloseHandle(this->targetHandle);
	}

	inline bool Attach()
	{
		this->pid = FindProcessId(this->procName); // Suche nach unserem Ziel.
		if (this->pid == 0)
			return false;

		// https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
		this->targetHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->pid); // Bekommen den Handle zu unserem prozess um später die Memory zu schreiben und lesen.
		if (!this->targetHandle)
			return false;

		return true;
	}

	uintptr_t FindModule(const char* modName)
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);
		if (!hSnap)
			return NULL;

		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		while (Module32Next(hSnap, &modEntry))
		{
			if (!strcmp(modEntry.szModule, modName))
			{
				CloseHandle(hSnap); // Wird geschlossen weil nicht mehr gebraucht.
				return reinterpret_cast<uintptr_t>(modEntry.modBaseAddr);
			}
		}

		CloseHandle(hSnap);
		return NULL;
	}

	template<typename T>
	T Read(uintptr_t address)
	{
		// https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-readprocessmemory
		T buffer{ };
		ReadProcessMemory(this->targetHandle, (void*)address, &buffer, sizeof(T), 0);
		return buffer;
	}

	DWORD GetPid()
	{
		return this->pid;
	}
};