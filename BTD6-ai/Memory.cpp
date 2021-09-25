#include "Memory.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD Memory::GetProcessId(const HWND hwnd)
{
	DWORD pId = 0;
	GetWindowThreadProcessId(hwnd, &pId);
	return pId;
}

//https://forum.cheatengine.org/viewtopic.php?t=563414
uintptr_t Memory::GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

HANDLE Memory::GetHandle(DWORD procId)
{
	HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, procId);
	if (!handle)
	{
		std::cout << "Cannot open process!" << std::endl;
		throw std::runtime_error("Cannot open process!");
	}
	return handle;
}

//https://www.cheatengine.org/forum/viewtopic.php?t=594721&sid=ae3fce06e7e08b8cae23afd8c2141974
DWORD64 Memory::ReadOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
{
	DWORD64 finalAddress = 0;
	DWORD64 tempAddress = 0;

	bool success = ReadProcessMemory(handle, (LPVOID)(baseAddress + firstOffset), &finalAddress, sizeof(finalAddress), NULL);
	tempAddress = finalAddress;

	for (int i : offsets)
	{
		bool success = ReadProcessMemory(handle, (PVOID)(tempAddress + i), &finalAddress, sizeof(finalAddress), NULL);
		if (!success)
		{
			std::cout << "Invalid Offset: " << std::hex << i << std::endl;
			throw std::runtime_error("Invalid Offset!");
		}
		tempAddress = finalAddress;
	}
	finalAddress = tempAddress + finalOffset;
	return finalAddress;
}

double Memory::GetDoubleFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
{
	double value = 0;
	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
	if (!success)
	{
		std::cout << "Couldn't find value from offsets!" << std::endl;
	}
	return value;
}

float Memory::GetFloatFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
{
	float value = 0;
	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
	if (!success)
	{
		std::cout << "Couldn't find value from offsets!" << std::endl;
	}
	return value;
}
