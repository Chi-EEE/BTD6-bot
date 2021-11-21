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

	clientPosition = Vector2{ windowPosition.left + x_offset, windowPosition.top + y_offset}; //Top left corner of playable window without hitting ui
	clientSize = Vector2{ windowSize.right - x_offset - x_minus_offset, windowSize.bottom - y_offset }; // Playable section of screen without hitting the store ui

	playBoxPosition = Vector2{ clientPosition.X + left_margin, clientPosition.Y + top_margin };
	playBoxSize = Vector2{ clientSize.X - left_margin - right_margin, clientSize.Y - top_margin - bottom_margin };

	DebuildPosition = Vector2{ clientSize.X + 200, clientSize.Y + 101 };
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

void Game::RandomizeTowers() // Error when there is no tower in array
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
}

Tower* Game::GetRandomTower(short indexCount)
{
	return &towers[randomTowerIndexes[indexCount]];
}

short Game::GetRandomTowerIndex(short indexCount)
{
	return randomTowerIndexes[indexCount];
}

/// <summary>
/// Places tower down without verifying that the bot has enough money
/// MUST VERIFY BEFORE USING THIS FUNCTION
/// </summary>
/// <param name="TowerName"></param>
/// <param name="Position"></param>
/// <returns></returns>
bool Game::PlaceTower(TowerName TowerName, Vector2 Position)
{
	int towerId = static_cast<int>(TowerName);
	const int TowerBuildCost = MultiplyDefaultPrice(TOWER_BASE_COST[towerId]);
	const int previousTowerCount = towerCount;
	INPUT input = Keyboard::keyPress(MapVirtualKeyA(TOWER_KEY_CODE[towerId], 4)); // Getting the SCAN Code in real time

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
			Tower newTower = Tower{ TowerName, Position, roundCount };
			towers.push_back(newTower);
			std::cout << "Built tower id: " << static_cast<int>(TowerName) << "\n";
			return true;
		}
		checkAttempts++;
		Clock::wait(0.1f);
	}
	return false;
}

bool Game::UpgradeTower(Tower* tower, short path)
{
	short LatestPath = tower->GetUpgradePath()[path];
	const int TowerUpgradeCost = MultiplyDefaultPrice(TOWER_UPGRADE[tower->GetId()][path][LatestPath]);
	if (tower->IsValidPath(path) && money >= TowerUpgradeCost)
	{
		std::cout << "Path " << path << " cost / Latest Path: " << LatestPath << ": " << TowerUpgradeCost << " bought with " << money << "\n";
		// use the , . / buttons to upgrade.
		Mouse::setPosition(tower->GetPosition());

		Mouse::leftMouseDown();
		Clock::wait(.1f);

		Mouse::leftMouseUp();
		Clock::wait(.6f);

		INPUT input = Keyboard::keyPress(MapVirtualKeyA(UPGRADE_KEY_CODE[path], 4));
		Clock::wait(.3f);

		Keyboard::keyRelease(input);

		Mouse::leftMouseDown();
		Clock::wait(.1f);
		Mouse::leftMouseUp();

		tower->IncreasePath(path, roundCount);
		Clock::wait(.4f); // So we can buy multiple upgrades without hitting the ui when buying other tower upgrades
		return true;
	}
	return false;
}

bool Game::CanBuildTower(TowerName TowerName)
{
	int towerId = static_cast<int>(TowerName);
	
	return MultiplyDefaultPrice(TOWER_BASE_COST[towerId]) <= money;
}

void Game::StartNextRound()
{
	INPUT input = Keyboard::keyPress(MapVirtualKeyA(SPACE_KEY_CODE, 4));
	Clock::wait(.1f);
	Keyboard::keyRelease(input);
}

void Game::AddTower(Tower tower)
{
	towers.push_back(tower);
}