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

	clientPosition = Vector2{ windowPosition.left + x_offset, windowPosition.top + y_offset }; //Top left corner of playable window without hitting ui
	clientSize = Vector2{ windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

	DebuildPosition = Vector2{ clientSize.X + 200, clientSize.Y + 101 };
	DeupgradePosition = Vector2{ clientSize.X + 175, clientSize.Y + 101 };
}

void Game::GetMemoryAddresses()
{
	memory.InitaliseMemoryRegions(handle);

	intptr_t moduleAddress = memory.GetModuleBaseAddress(processId, L"GameAssembly.dll");

	simulationAddress = memory.ReadOffsets(handle, (char*)moduleAddress, offsetsFromSimulation);
	std::cout << (void*)simulationAddress << "\n";
	moneyAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToMoney);

	healthAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToHealth);

	roundAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToRound);

	towerCountAddress = memory.ReadOffsets(handle, simulationAddress, offsetsToTowerCount);
}

double Game::GetMoney()
{
	ReadProcessMemory(handle, moneyAddress, &money, sizeof(money), NULL);
	return money;
}

double Game::GetHealth()
{
	ReadProcessMemory(handle, healthAddress, &health, sizeof(health), NULL);
	return health;
}

double Game::GetRoundCount()
{
	ReadProcessMemory(handle, roundAddress, &roundCount, sizeof(roundCount), NULL);
	return roundCount;
}

int Game::GetTowerCount()
{
	ReadProcessMemory(handle, towerCountAddress, &towerCount, sizeof(towerCount), NULL);
	return towerCount;
}

int Game::MultiplyDefaultPrice(double defaultPrice)
{
	return static_cast<int>(((defaultPrice * DIFFICULTY_MULTIPLIER[DIFFICULTY])) / 5) * 5;
}

Tower* Game::GetRandomTower()
{
	//if (randomTowerIndexes.size() != towers.size())
	//{
		randomTowerIndexes.clear();
		for (short i = 0; i < towers.size(); i++)
		{
			randomTowerIndexes.push_back(i);
		}
	//}
	std::shuffle(randomTowerIndexes.begin(), randomTowerIndexes.end(), Random::GetEngine());
	return &towers[randomTowerIndexes[0]];
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
		INPUT input = Keyboard::keyPress(MapVirtualKeyA(TOWER_KEY_CODE[towerId],4)); // Getting the SCAN Code in real time
		
		Mouse::setPosition(Position);
		Mouse::leftMouseDown();

		Clock::wait(.3f);
		Keyboard::keyRelease(input);
		Mouse::leftMouseUp();
		Mouse::setPosition(DebuildPosition);

		// Loop to check if the tower count has increased
		int checkAttempts = 0;
		while (checkAttempts <= 5)
		{
			if (previousTowerCount < GetTowerCount())
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

bool Game::UpgradeTower(Tower* tower, short path)
{
	short LatestPath = tower->GetUpgradePath()[path];
	if (tower->IsValidPath(path) && money >= MultiplyDefaultPrice(TOWER_UPGRADE[tower->GetId()][path][LatestPath]))
	{
		std::cout << "Path " << path << " cost / Latest Path: " << LatestPath << ": " << MultiplyDefaultPrice(TOWER_UPGRADE[tower->GetId()][path][LatestPath]) << " bought with " << money << "\n";
		// use the , . / buttons to upgrade.
		Mouse::setPosition(tower->GetPosition());

		Mouse::leftMouseDown();
		Clock::wait(.1f);

		Mouse::leftMouseUp();
		Clock::wait(.6f);

		INPUT input = Keyboard::keyPress(MapVirtualKeyA(UPGRADE_KEY_CODE[path], 4));
		Clock::wait(.3f);

		Keyboard::keyRelease(input);

		Mouse::setPosition(DeupgradePosition);
		Mouse::leftMouseDown();
		Clock::wait(.1f);
		Mouse::leftMouseUp();

		tower->IncreasePath(path);
		return true;
	}
	return false;
}

bool Game::CanBuildTower(TowerName TowerName)
{
	int towerId = static_cast<int>(TowerName);
	return TOWER_BASE_COST[towerId] <= money;
}

void Game::StartNextRound()
{
	INPUT input = Keyboard::keyPress(MapVirtualKeyA(SPACE_KEY_CODE, 4));
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