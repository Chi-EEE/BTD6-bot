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
	static HANDLE GetHandle(DWORD procId)
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
	void GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
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
		//CloseHandle(hSnap);
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
	/// <param name="baseAddress">Base address of memory region</param>
	/// <param name="size">Size of the memory region</param>
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
	/// <param name="baseAddress">Base address of memory region</param>
	/// <param name="size">Size of the memory region</param>
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
private:
	std::vector<Region> regions;
};