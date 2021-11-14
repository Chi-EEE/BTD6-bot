#pragma once
#include <Windows.h>

class Keyboard
{
public:
	/*static void Initalise();*/

	static INPUT keyPress(WORD scanCode);
	static void keyRelease(INPUT input);

	/*static UINT TOWER_SCAN_CODE[23];
	static UINT UPGRADE_SCAN_CODE[3];
	static UINT SPACE_SCAN_CODE;*/
private:
};