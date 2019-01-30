

#pragma once
#include <windows.h>
#include "KeyInput.h"

//VALUES
/*0x41 A key
0x42 B key
0x43 C key
0x44 D key
0x45 E key
0x46 F key
0x47 G key
0x48 H key
0x49 I key
0x4A J key
0x4B K key
0x4C L key
0x4D M key
0x4E N key
0x4F O key
0x50 P key
0x51 Q key
0x52 R key
0x53 S key
0x54 T key
0x55 U key
0x56 V key
0x57 W key
0x58 X key
0x59 Y key
0x5A Z key

VK_HOME 0x24 Home key
VK_RETURN 0x0D ENTER KEY
*/

bool KeyInput(KEY akey) {
	
	int keyToCheck = 0;
	bool toReturn = false;
	switch (akey) {
	case Wkey: keyToCheck = 0x57;
		break;
	case Akey: keyToCheck = 0x41;
		break;
	case Skey: keyToCheck = 0x53;
		break;
	case Dkey: keyToCheck = 0x44;
		break;
	case Qkey: keyToCheck = 0x51;
		break;
	case Ekey: keyToCheck = 0x45;
		break;
	case Homekey: keyToCheck = 0x24;
		break;
	case Enterkey:keyToCheck = 0x0D;
	}
	//-32, 768 through 32, 767
	short check = GetAsyncKeyState(keyToCheck);
	if (check < 0)
	{
		toReturn = true;//presed
	}
	return toReturn;
}