//#pragma once
//#include <map>
//#include <windows.h>
//#include <vector>
//#include <string>
//#include <TlHelp32.h>
//
//struct Region {
//	char* BaseAddress;
//	intptr_t Size;
//};
//template <class T>
//class ProcessMemory
//{
//private:
//
//	std::vector<Region> regions;
//
//public:
//	ProcessMemory();
//	//void InitaliseMemoryRegions(HANDLE handle);
//
//	//DWORD GetProcessId(const HWND hwnd);
//	DWORD GetProcessId(const TCHAR* procName);
//	void GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
//	HANDLE GetHandle(DWORD procId);
//	void InitaliseMemoryRegions(HANDLE &handle);
//	char* ScanForDoubleValue(HANDLE handle, double answer, char* baseAddress, intptr_t size);
//	char* ScanForALongValue(HANDLE handle, long answer, char* baseAddress, intptr_t size);
//	void ScanForLongValues(HANDLE handle, long answer, char* baseAddress, intptr_t size, std::vector <char*> results);
//	//DWORD64 ReadOffsets(HANDLE handle, unsigned long long baseAddress, std::vector<long> offsets);
//
//	std::vector<Region> GetMemoryRegions()
//	{
//		return regions;
//	}
//	/*int GetIntFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
//	double GetDoubleFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
//	float GetFloatFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);
//	bool GetBoolFromOffsets(HANDLE handle, DWORD64 baseAddress, DWORD64 firstOffset, std::vector<DWORD64> offsets, DWORD64 finalOffset);*/
//
//};
//
