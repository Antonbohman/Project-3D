

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

/*Virtual Key codes :*/
/*
vk_BackSpace = 8;
vk_Tab = 9;
vk_Return = 13;
vk_Shift = 16;
vk_Control = 17;
vk_Alt = 18;
vk_Pause = 19;
vk_CapsLock = 20;
vk_Escape = 27;
vk_Space = 32;
vk_PageUp = 33;
vk_PageDown = 34;
vk_End = 35;
vk_Home = 36;
vk_Left = 37;
vk_Up = 38;
vk_Right = 39;
vk_Down = 40;
vk_PrintScreen = 44;
vk_Insert = 45;
vk_Delete = 46;
*/
/* NOTE: vk_0..vk_9 vk_A.. vk_Z match regular ASCII codes for digits and A-Z letters */
/*
vk_0 = 48;
vk_1 = 49;
vk_2 = 50;
vk_3 = 51;
vk_4 = 52;
vk_5 = 53;
vk_6 = 54;
vk_7 = 55;
vk_8 = 56;
vk_9 = 57;
vk_A = 65;
vk_B = 66;
vk_C = 67;
vk_D = 68;
vk_E = 69;
vk_F = 70;
vk_G = 71;
vk_H = 72;
vk_I = 73;
vk_J = 74;
vk_K = 75;
vk_L = 76;
vk_M = 77;
vk_N = 78;
vk_O = 79;
vk_P = 80;
vk_Q = 81;
vk_R = 82;
vk_S = 83;
vk_T = 84;
vk_U = 85;
vk_V = 86;
vk_W = 87;
vk_X = 88;
vk_Y = 89;
vk_Z = 90;
vk_LWin = 91;
vk_RWin = 92;
vk_Apps = 93;
*/
/* numerical key pad */
/*
vk_NumPad0 = 96;
vk_NumPad1 = 97;
vk_NumPad2 = 98;
vk_NumPad3 = 99;
vk_NumPad4 = 100;
vk_NumPad5 = 101;
vk_NumPad6 = 102;
vk_NumPad7 = 103;
vk_NumPad8 = 104;
vk_NumPad9 = 105;
vk_Multiply = 106;
vk_Add = 107;
vk_Subtract = 109;
vk_Decimal = 110;
vk_Divide = 111;
*/

/* function keys */
/*
vk_F1 = 112;
vk_F2 = 113;
vk_F3 = 114;
vk_F4 = 115;
vk_F5 = 116;
vk_F6 = 117;
vk_F7 = 118;
vk_F8 = 119;
vk_F9 = 120;
vk_F10 = 121;
vk_F11 = 122;
vk_F12 = 123;
vk_F13 = 124;
vk_F14 = 125;
vk_F15 = 126;
vk_F16 = 127;

vk_NumLock = 144;
vk_ScrollLock = 145;
vk_LShift = 160;
vk_RShift = 161;
vk_LControl = 162;
vk_RControl = 163;
vk_LAlt = 164;
vk_RAlt = 165;
vk_SemiColon = 186;
vk_Equals = 187;
vk_Comma = 188;
vk_UnderScore = 189;
vk_Period = 190;
vk_Slash = 191;
vk_BackSlash = 220;
vk_RightBrace = 221;
vk_LeftBrace = 219;
vk_Apostrophe = 222;
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
	case Okey:keyToCheck = 0x4f;
		break;
	case Fkey:keyToCheck = 0x46;
		break;
	case Homekey: keyToCheck = 0x24;
		break;
	case Enterkey:keyToCheck = 0x0D;
		break;
	case Backspacekey:keyToCheck = 8;
		break;
	}
	//-32, 768 through 32, 767
	short check = GetAsyncKeyState(keyToCheck);
	if (check < 0)
	{
		toReturn = true;//presed
	}
	return toReturn;
}

