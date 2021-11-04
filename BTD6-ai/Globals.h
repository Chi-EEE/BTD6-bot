#pragma once
#include <windows.h>
#include <string>

static double Buy_Chance = 100;
static double Upgrade_Chance = 0;










// VALUES BELOW ARE NOT RECOMENDED TO BE CHANGED
enum class TowerName {
	Hero,
	Dart_Monkey,
	Boomerang_Monkey,
	Bomb_Shooter,
	Tack_Shooter,
	Ice_Monkey,
	Glue_Gunner,
	Sniper_Monkey,
	Monkey_Sub,
	Monkey_Buccaneer,
	Monkey_Ace,
	Heli_Pilot,
	Mortar_Monkey,
	Dartling_Gunner,
	Wizard_Monkey,
	Super_Monkey,
	Ninja_Monkey,
	Alchemist,
	Druid,
	Banana_Farm,
	Spike_Factory,
	Monkey_Village,
	Engineer_Monkey
};

//http://www.kbdedit.com/manual/low_level_vk_list.html
static const WORD TOWER_KEY_CODE[23] =
{
	0x55,
	0x51,
	0x57,
	0x45,
	0x52,
	0x54,
	0x59,
	0x5A,
	0x58,
	0x43,
	0x56,
	0x42,
	0x4E,
	0x4D,
	0x41,
	0x53,
	0x44,
	0x46,
	0x47,
	0x48,
	0x4A,
	0x4B,
	0x4C
};

static UINT TOWER_SCAN_CODE[23];

static const WORD UPGRADE_KEY_CODE[3] = {
	0xBC,
	0xBE,
	0xBF
};

static UINT UPGRADE_SCAN_CODE[3];

static const WORD SPACE_KEY_CODE = 0x20;
static UINT SPACE_SCAN_CODE;

// Easy = .85 math floor 5
// hard = 1.08 math floor 5
// impoppable = 1.20 math floor 5
// static_cast<int>(((TOWER_UPGRADE[1][0][i] * 0.85) + 2.5)/ 5) * 5;
const float DIFFICULTY[4] =
{
	0.85f,
	1.f,
	1.08f,
	1.20f
};
const int TOWER_UPGRADE[23][3][5] =
{
	//Hero
	{
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	},
	{ // Dart Monkey
		{140, 220, 300, 1800, 15000},
		{100, 190, 400, 8000, 45000},
		{90, 200, 625, 2000, 25000}
	},
	{ // Boomerang Monkey
		{200, 280, 1300, 3000, 32400},
		{175, 250, 1600, 4000, 35000},
		{100, 300, 1300, 2200, 60000}
	},
	{ // Bomb Shooter
		{350, 650, 1200, 3600, 55000},
		{250, 400, 900, 3200, 25000},
		{200, 300, 800, 2400, 35000}
	},
	{ // Tack Shooter
		{150, 300, 600, 3500, 45500},
		{100, 225, 550, 2700, 15000},
		{100, 100, 450, 3200, 20000}
	},
	{ // Ice Tower
		{100, 300, 1500, 3000, 30000},
		{225, 350, 2900, 3000, 20000},
		{175, 225, 1950, 2000, 30000}
	},
	{ // Glue Gunner
		{200, 300, 2500, 5000, 22000},
		{100, 1800, 3250, 3500, 15000},
		{120, 400, 3400, 3000, 28000}
	},
	{ // Sniper Monkey
		{350, 1500, 3000, 5000, 34000},
		{300, 450, 3200, 7200, 13000},
		{400, 400, 3500, 4750, 14000}
	},
	{ // Monkey Sub
		{130, 500, 500, 2500, 40000},
		{450, 300, 1400, 13000, 32000},
		{380, 1000, 1100, 3000, 25000}
	},
	{ // Monkey Buccaneer
		{295, 550, 2950, 7500, 25000},
		{550, 500, 900, 4500, 21000},
		{180, 400, 2300, 5500, 23000}
	},
	{ // Monkey Ace
		{650, 650, 1000, 3000, 40000},
		{200, 350, 900, 14000, 35000},
		{500, 300, 2200, 24000, 100000}
	},
	{ // Heli Pilot
		{800, 500, 1750, 19600, 45000},
		{300, 600, 3000, 12000, 30000},
		{250, 350, 3500, 8500, 35000}
	},
	{ // Mortar Monkey
		{500, 650, 1100, 8000, 28000},
		{300, 500, 900, 5500, 30000},
		{200, 500, 700, 11000, 40000}
	},
	{ //Dartling Gunner
		{300, 900, 4250, 11000, 95000},
		{250, 950, 5250, 5100, 60000},
		{150, 1200, 3400, 12000, 58000}
	},
	{ //Wizard Monkey
		{150, 600, 1300, 10900, 32000},
		{300, 900, 3000, 4000, 54000},
		{300, 300, 1700, 2800, 24000}
	},
	{ //Super Monkey
		{2500, 4500, 22000, 100000, 500000},
		{1000, 1400, 8400, 19000, 90000},
		{3000, 1200, 5500, 60000, 240000}
	},
	{ //Ninja Monkey
		{300, 350, 850, 2750, 35000},
		{350, 500, 900, 5200, 22000},
		{250, 400, 2750, 4500, 40000}
	},
	{ //Alchemist
		{250, 350, 1250, 3000, 60000},
		{250, 475, 3000, 4500, 45000},
		{650, 600, 1000, 2750, 40000}
	},
	{ //Druid
		{250, 1000, 1850, 5100, 80000},
		{250, 350, 950, 5000, 35000},
		{100, 300, 600, 2500, 45000}
	},
	{ //Banana Farm
		{500, 600, 3000, 19000, 100000},
		{300, 800, 3500, 7500, 100000},
		{250, 200, 2900, 15000, 60000}
	},
	{ //Spike Factory
		{800, 600, 2300, 9500, 150000},
		{600, 800, 2500, 5000, 40000},
		{150, 400, 1400, 3500, 30000}
	},
	{ //Monkey Village
		{400, 1500, 800, 2500, 25000},
		{250, 2000, 7500, 20000, 40000},
		{500, 500, 10000, 3000, 100000} //Monkeyopolis is not needed
	},
	{ //Engineer Monkey
		{500, 400, 525, 2500, 32000},
		{250, 350, 800, 13500,120000},
		{450, 220, 500, 3500, 54000}
	}
};

const int TOWER_BASE_COST[23] =
{
	10000000,
	200,
	325,
	525,
	280,
	500,
	275,
	350,
	325,
	500,
	800,
	1600,
	750,
	1000,
	450,
	2500,
	500,
	550,
	425,
	1250,
	1000,
	1200,
	450
};