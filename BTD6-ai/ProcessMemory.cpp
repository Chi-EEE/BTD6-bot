#include "ProcessMemory.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>


ProcessMemory::ProcessMemory()
{
}

//DWORD ProcessMemory::GetProcessId(const HWND hwnd)
//{
//	DWORD pId = 0;
//	GetWindowThreadProcessId(hwnd, &pId);
//	return pId;
//}
DWORD ProcessMemory::GetProcessId(const TCHAR* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_tcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));

		}
	}
	CloseHandle(hSnap);
	return procId;
}


//https://forum.cheatengine.org/viewtopic.php?t=563414
void ProcessMemory::GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
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
					Region newRegion;
					newRegion.BaseAddress = (char*)modEntry.modBaseAddr;
					newRegion.Size = modEntry.modBaseSize;
					regions.push_back(newRegion);
					CloseHandle(hSnap);
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
}

HANDLE ProcessMemory::GetHandle(DWORD procId)
{
	HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, procId);
	if (!handle)
	{
		std::cout << "Cannot open process!" << std::endl;
		throw std::runtime_error("Cannot open process!");
	}
	return handle;
}


//Doesn't work for BTD 6 
////https://www.cheatengine.org/forum/viewtopic.php?t=594721&sid=ae3fce06e7e08b8cae23afd8c2141974
//DWORD64 ProcessMemory::ReadOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
//{
//	DWORD64 finalAddress = 0;
//	DWORD64 tempAddress = 0;
//
//	bool success = ReadProcessMemory(handle, (LPVOID)(baseAddress + firstOffset), &finalAddress, sizeof(finalAddress), NULL);
//	tempAddress = finalAddress;
//
//	for (int i : offsets)
//	{
//		bool success = ReadProcessMemory(handle, (PVOID)(tempAddress + i), &finalAddress, sizeof(finalAddress), NULL);
//		if (!success)
//		{
//			std::cout << "Invalid Offset: " << std::hex << i << std::endl;
//			//throw std::runtime_error("Invalid Offset!");
//			break;
//		}
//		tempAddress = finalAddress;
//		std::cout << finalAddress << std::endl;
//	}
//	finalAddress = tempAddress + finalOffset;
//	return finalAddress;
//}
//
//int ProcessMemory::GetIntFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
//{
//	int value = 0;
//	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
//	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
//	if (!success)
//	{
//		std::cout << "Couldn't find value from offsets!" << std::endl;
//	}
//	return value;
//}
//
//double ProcessMemory::GetDoubleFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
//{
//	double value = 0;
//	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
//	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
//	if (!success)
//	{
//		std::cout << "Couldn't find value from offsets!" << std::endl;
//	}
//	return value;
//}
//
//float ProcessMemory::GetFloatFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
//{
//	float value = 0;
//	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
//	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
//	if (!success)
//	{
//		std::cout << "Couldn't find value from offsets!" << std::endl;
//	}
//	return value;
//}
//
//bool ProcessMemory::GetBoolFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset)
//{
//	bool value = 0;
//	DWORD64 finalAddress = ReadOffsets(handle, baseAddress, firstOffset, offsets, finalOffset);
//	bool success = ReadProcessMemory(handle, (PVOID)(finalAddress), &value, sizeof(value), NULL);
//	if (!success)
//	{
//		std::cout << "Couldn't find value from offsets!" << std::endl;
//	}
//	return value;
//}

/// <summary>
/// Gets the memory regions inside of the handle
/// </summary>
/// <param name="handle"></param>
void ProcessMemory::InitaliseMemoryRegions(HANDLE &handle)
{
	unsigned char* addr = 0;
	MEMORY_BASIC_INFORMATION mbi;
	while (VirtualQueryEx(handle, addr, &mbi, sizeof(mbi)))
	{
		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS && mbi.Protect != PAGE_GUARD)
		{
			Region newRegion;
			newRegion.BaseAddress = (char*)mbi.BaseAddress;
			newRegion.Size = mbi.RegionSize;
			regions.push_back(newRegion);
		}
		addr += mbi.RegionSize;
	}
}

/// <summary>
/// Scans from the inputted base address and loops until it reaches the base address + size
/// Checks if address has same value as answer
/// </summary>
/// <param name="handle"></param>
/// <param name="answer"></param>
/// <param name="baseAddress">Base address of memory region</param>
/// <param name="size">Size of the memory region</param>
/// <returns></returns>
char* ProcessMemory::ScanForDoubleValue(HANDLE handle, double answer, char* baseAddress, intptr_t size)
{
	double value;
	for (char* curr = baseAddress; curr < baseAddress + size; curr += 8)
	{
		bool success = ReadProcessMemory(handle, curr, &value, sizeof(value), NULL);
		if (success)
		{
			if (value == answer)
			{
				return curr;
			}
		}
	}
	return nullptr;
}