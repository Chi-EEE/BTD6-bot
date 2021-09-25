#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>
class ProcessMemory
{
private:
	DWORD64 ReadOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);


public:
	DWORD GetProcessId(const HWND hwnd);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	HANDLE GetHandle(DWORD procId);
	int GetIntFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
	double GetDoubleFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
	float GetFloatFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
	bool GetBoolFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);

};

