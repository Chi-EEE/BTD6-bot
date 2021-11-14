#pragma once

#include "Region.h"

#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>


class Memory
{
public:
	/// <summary>
	/// Gets process id of string
	/// </summary>
	/// <param name="procName"></param>
	/// <returns></returns>
	static DWORD GetProcessId(const TCHAR* procName)
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


	/// <summary>
	/// Gets the handle of inputted process id
	/// </summary>
	/// <param name="procId">process id</param>
	/// <returns>handle</returns>
	static HANDLE GetHandle(DWORD64 procId)
	{
		HANDLE handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, procId);
		if (!handle)
		{
			std::cout << "Cannot open process!" << std::endl;
			throw std::runtime_error("Cannot open process!");
		}
		return handle;
	}
	
	std::vector<Region> GetMemoryRegions() { return regions; }

	/// <summary>
	/// https://forum.cheatengine.org/viewtopic.php?t=563414
	/// Gets the module base address from the modName string and procId
	/// </summary>
	/// <param name="procId"></param>
	/// <param name="modName"></param>
	uintptr_t GetModuleBaseAddress(DWORD64 procId, const wchar_t* modName)
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

	/// <summary>
	/// Gets the memory regions inside of the handle
	/// </summary>
	/// <param name="handle"></param>
	void InitaliseMemoryRegions(HANDLE& handle)
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
	/// <returns></returns>
	template <class T>
	char* ScanForValue(HANDLE handle, T answer)
	{
		int regionIndex = 0;
		char* baseAddress = NULL;
		intptr_t size = 0;

		T value;
		while (true)
		{
			baseAddress = regions[regionIndex].BaseAddress;
			size = regions[regionIndex].Size;
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
			regionIndex++;
			if (regionIndex >= regions.size())
			{
				break;
			}
		}
		return nullptr;
	}

	/// <summary>
	/// Scans from the inputted base address and loops until it reaches the base address + size
	/// Checks if address has same value as answer
	/// </summary>
	/// <param name="handle"></param>
	/// <param name="answer"></param>
	/// <returns></returns>
	template <class T>
	void ScanForValues(HANDLE handle, T answer, std::vector <char*> &results)
	{
		int regionIndex = 0;
		char* baseAddress = NULL;
		intptr_t size = 0;

		T value;
		while (true)
		{
			baseAddress = regions[regionIndex].BaseAddress;
			size = regions[regionIndex].Size;
			for (char* curr = baseAddress; curr < baseAddress + size; curr += 8)
			{
				bool success = ReadProcessMemory(handle, curr, &value, sizeof(value), NULL);
				if (success)
				{
					if (value == answer)
					{
						results.push_back(curr);
					}
				}
			}
			regionIndex++;
			if (regionIndex >= regions.size())
			{
				break;
			}
		}
	}

	///https://www.cheatengine.org/forum/viewtopic.php?t=594721&sid=ae3fce06e7e08b8cae23afd8c2141974
	char* ReadOffsets(HANDLE handle, char* baseAddress, std::vector<int> offsets)
	{
		char* finalAddress = 0;
		char* tempAddress = 0;
	
		bool success = ReadProcessMemory(handle, baseAddress + offsets[0], &finalAddress, sizeof(finalAddress), NULL);
		tempAddress = finalAddress;
		if (!success)
		{
			std::cout << "oh\n";
		}

		for (int i = 1; i < offsets.size() - 1; i++)
		{
			bool success = ReadProcessMemory(handle, tempAddress + offsets[i], &finalAddress, sizeof(finalAddress), NULL);
			if (!success)
			{
				std::cout << "Invalid Offset: " << offsets[i] << std::endl;
				//throw std::runtime_error("Invalid Offset!");
				break;
			}
			tempAddress = finalAddress;
		}
		finalAddress = tempAddress + offsets[offsets.size() - 1];
		return finalAddress;
	}
private:
	std::vector<Region> regions;
};