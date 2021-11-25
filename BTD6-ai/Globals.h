#pragma once
#include <windows.h>
#include <vector>
#include <array>

#include "TowerName.h"
/*
0 : Easy
1 : Medium
2 : Hard
3 : Impoppable
*/
static const short DIFFICULTY = 1;

static const double Buy_Chance = 50;

static const short MaximumBuildAttempts = 50;






const int x_offset = 21;
const int x_minus_offset = 232;
const int y_offset = 92;

const int top_margin = 83;
const int bottom_margin = 117;
const int left_margin = 157;
const int right_margin = 158;

static const std::vector<int> offsetsFromSimulation = { 0x02111C98, 0x18, 0x50, 0xB8, 0x0, 0xB0, 0x10, 0x0 };
static const std::vector<int> offsetsToMoney = { 0x248, 0x18, 0x30, 0x10, 0x28 };
static const std::vector<int> offsetsToHealth = { 0x260, 0x28 };
static const std::vector<int> offsetsToRound = { 0x270, 0x98, 0x0E0, 0x28 };
static const std::vector<int> offsetsToTowerCount = { 0x68, 0x18, 0x30, 0x10 };

// VALUES BELOW ARE NOT RECOMENDED TO BE CHANGED

static std::vector<TowerName> ALLOWED_TOWERS =
{
	TowerName::Dart_Monkey,
	TowerName::Boomerang_Monkey,
	TowerName::Bomb_Shooter,
	TowerName::Tack_Shooter,
	TowerName::Ice_Monkey,
	TowerName::Glue_Gunner,
	TowerName::Sniper_Monkey,
	TowerName::Monkey_Sub,
	TowerName::Monkey_Buccaneer,
	TowerName::Monkey_Ace,
	TowerName::Heli_Pilot,
	TowerName::Mortar_Monkey,
	TowerName::Dartling_Gunner,
	TowerName::Wizard_Monkey,
	TowerName::Super_Monkey,
	TowerName::Ninja_Monkey,
	TowerName::Alchemist,
	TowerName::Druid,
	TowerName::Spike_Factory,
	TowerName::Monkey_Village,
	TowerName::Engineer_Monkey
};

static std::array<short, 3> PATHS = { 0, 1, 2 };

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

static const WORD UPGRADE_KEY_CODE[3] = {
	0xBC,
	0xBE,
	0xBF
};

static const WORD SPACE_KEY_CODE = 0x20;

// Easy = .85 math floor 5
// hard = 1.08 math floor 5
// impoppable = 1.20 math floor 5
// static_cast<int>(((TOWER_UPGRADE[1][0][i] * 0.85) + 2.5)/ 5) * 5;
const double DIFFICULTY_MULTIPLIER[4] =
{
	0.85,
	1,
	1.08,
	1.20
};
const int DIFFICULTY_ROUND[4] =
{
	40,
	60,
	80,
	100
};

// Make AI work without cumulative cash array past 100
const double CUMULATIVE_CASH[100] = {
	711,
	908,
	1046,
	1221,
	1385,
	1548,
	1730,
	1930,
	2129,
	2443,
	2632,
	2824,
	3106,
	3365,
	3631,
	3899,
	4064,
	4422,
	4682,
	4868,
	5219,
	5517,
	5794,
	5961, // Round 24 Camo
	6296,
	6629,
	7291,
	7557, // Round 28 Lead
	8283,
	8820,
	9447,
	9652,
	10564,
	11714,
	12610,
	13949,
	15226,
	16985,
	17506, // Round 40 Moab
	19687,
	20346,
	21624,
	22918,
	25340,
	26056,
	27693,
	30536,
	35294,
	38310, // Round 50
	39333,
	40852.5,
	41700.5,
	43821,
	46226.5,
	47435,
	49215.5,
	51434.5,
	53514,
	54356.5, // Round 60 BFB
	55459.7,
	56716.5,
	59412.1, // Round 63
	60130.7,
	63070.3,
	63941.7,
	64831.7,
	65475.1,
	66730.9,
	69213.7,
	70579.9,
	71946.3,
	73200.5,
	76105.3,
	78632.7,
	79807.9,
	82206.5,
	86926.1,
	93491.9,
	94748.1, // Round 80 ZOMG
	99969.3,
	104580.7,
	109183.3,
	116080.1,
	118557.5,
	119338.6,
	121797.7,
	124942.5,
	126943.4,
	127111.7,
	131130.8, // Round 91 DDT
	135495.4,
	137268.3,
	144760.8,
	148303.3,
	158082.5,
	159322.4,
	168798,
	171446.8,
	172801.4 // Round 100 BAD
};

const int TOWER_UPGRADE[23][3][5] =
{
	//Hero [Dont want AI to buy upgrades for hero]
	{
		{INT_MAX, 0, 0, 0, 0},
		{INT_MAX, 0, 0, 0, 0},
		{INT_MAX, 0, 0, 0, 0}
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
		{200, 300, 800, 2800, 35000}
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
		{350, 550, 2950, 7200, 25000},
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
	600,
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