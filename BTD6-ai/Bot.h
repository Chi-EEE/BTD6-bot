#pragma once
#include "Memory.h"

class Bot
{
public:
	Bot();

private:
	HANDLE handle;
	DWORD64 processId;

	void GetMemoryAddresses();

	void run();

	Memory memory = Memory{};
	// Memory Addresses
	char* simulationAddress;

	char* moneyAddress;
	char* healthAddress;
	char* roundAddress;
	char* towerCountAddress;
};

