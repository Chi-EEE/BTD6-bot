#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <TlHelp32.h>
class Memory
{
private:
	DWORD64 ReadOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);

public:
	DWORD GetProcessId(const char* windowName);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
	HANDLE GetHandle(DWORD procId);
	double GetDoubleFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
	float GetFloatFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);

};

