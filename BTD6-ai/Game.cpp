#include <algorithm>
#include <random>

#include "Game.h"
#include "Random.h"
#include "Globals.h"

Game::Game()
{
	processId = Memory::GetProcessId(L"BloonsTD6.exe");
	handle = Memory::GetHandle(processId);

	GetMemoryAddresses();
	GetPositions();
}

void Game::GetPositions()
{
	HWND hwnd = Window::GetHwnd("BloonsTD6");
	RECT windowPosition = Window::GetRect(hwnd); // Window Position
	RECT windowSize = Window::GetSizeRect(hwnd); // Playable Window Size

	RECT clientPosition = RECT{ windowPosition.left + x_offset, windowPosition.top + y_offset, windowPosition.right, windowPosition.bottom }; //Top left corner of playable window without hitting ui
	RECT clientSize = RECT{ 0, 0, windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

	DeselectPosition = Vector2{ clientSize.right + 200, clientSize.bottom + 101 };
	CloseHandle(hwnd);
}

void Game::GetMemoryAddresses()
{
	memory.InitaliseMemoryRegions(handle);

	intptr_t moduleAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");

	simulationAddress = memory.ReadOffsets(handle, (char*)moduleAddress, offsetsFromSimulation);

	moneyAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToMoney);
	healthAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToHealth);
	roundAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToRound);
	towerCountAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToTowerCount);
}

void Game::GetMoney(double* variable)
{
	ReadProcessMemory(handle, moneyAddress, &variable, sizeof(variable), NULL);
	money = *variable;
}

void Game::GetHealth(double* variable)
{
	ReadProcessMemory(handle, healthAddress, &variable, sizeof(variable), NULL);
	health = *variable;
}

void Game::GetRoundCount(int* variable)
{
	ReadProcessMemory(handle, roundAddress, &variable, sizeof(variable), NULL);
	roundCount = *variable;
}

void Game::GetTowerCount(int* variable)
{
	ReadProcessMemory(handle, towerCountAddress, &variable, sizeof(variable), NULL);
	towerCount = *variable;
}

int Game::MultiplyDefaultPrice(double defaultPrice)
{
	return static_cast<int>(((defaultPrice * DIFFICULTY_MULTIPLIER[DIFFICULTY]) + 2.5) / 5) * 5;
}

Tower Game::GetRandomTower()
{
	if (randomTowerIndexes.size() != towers.size())
	{
		randomTowerIndexes.clear();
		for (short i = 0; i < towers.size(); i++)
		{
			randomTowerIndexes.push_back(i);
		}
	}
	std::shuffle(randomTowerIndexes.begin(), randomTowerIndexes.end(), Random::GetEngine());
	return towers[randomTowerIndexes[0]];
}

Tower Game::GetNextRandomTower(short indexCount)
{
	return towers[randomTowerIndexes[indexCount]];
}

bool Game::PlaceTower(TowerName TowerName, Vector2 Position)
{
	int towerId = static_cast<int>(TowerName);
	if (TOWER_BASE_COST[towerId] <= money)
	{
		const int previousTowerCount = towerCount;
		INPUT input = Keyboard::keyPress(TOWER_SCAN_CODE[towerId]);

		Mouse::setPosition(Position);
		Mouse::leftMouseDown();

		Clock::wait(.3f);
		Keyboard::keyRelease(input);
		Mouse::leftMouseUp();
		Mouse::setPosition(DeselectPosition);

		// Loop to check if the tower count has increased
		int checkAttempts = 0;
		while (checkAttempts <= 20)
		{
			GetTowerCount(&towerCount);
			if (previousTowerCount < towerCount)
			{
				Tower newTower = Tower{ TowerName, Position };
				towers.push_back(newTower);
				return true;
			}
			checkAttempts++;
			Clock::wait(0.1f);
		}
	}
	return false;
}

void Game::StartNextRound()
{
	INPUT input = Keyboard::keyPress(SPACE_SCAN_CODE);
	Clock::wait(.1f);
	Keyboard::keyRelease(input);
}

//void Game::moveMouseToFarms()
//{ 
//	for (Vector2 position : farmPositions)
//	{
//		Clock::wait(.1f);
//		Mouse::setPosition(position);
//	}
//}

void Game::AddTower(Tower tower)
{
	towers.push_back(tower);
}