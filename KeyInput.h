#pragma once

#pragma once
#include <windows.h>
//#include <Mouse.h>
//#include <Keyboard.h>
//
//std::unique_ptr<DirectX::Mouse>m_mouse;
//std::unique_ptr<DirectX::Keyboard>m_keyboard; //NOT IMPLEMENTED

enum KEY {
	Akey,
	Bkey,
	Ckey,
	Dkey,
	Ekey,
	Fkey,
	Gkey,
	Hkey,
	Ikey,
	Jkey,
	Kkey,
	Lkey,
	Mkey,
	Nkey,
	Okey,
	Pkey,
	Qkey,
	Rkey,
	Skey,
	Tkey,
	Ukey,
	Vkey,
	Wkey,
	Xkey,
	Ykey,
	Zkey,
	//SPECIAL
	Homekey,
	Enterkey,
	Backspacekey

};
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

bool KeyInput(KEY akey);

//void Update();

				//UPDATE
				//auto mouse = m_mouse->GetState();
				/*if (mouse.positionMode == Mouse::MODE_RELATIVE) {
					float delta [3] = { float(mouse.x)* ROTATION_GAIN, float(mouse.y)* ROTATION_GAIN, 0.0f };

					//vector3

					pitch -= delta[1]; //Y
					yaw -= delta[0]; //X

					float limit = XM_PI / 2.0f - 0.01f;
					pitch = max(-limit, pitch);
					pitch = min(+limit, pitch);

					if (yaw > XM_PI)
					{
						yaw -= XM_PI * 2.0f;
					}
					else if (yaw < -XM_PI)
					{
						yaw += XM_PI * 2.0f;
					}

				}
				m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);

				float y = sinf(pitch);
				float r = cosf(pitch);
				float z = r * cosf(yaw);
				float x = r * sinf(yaw);

				cameraFocus = cameraPosition + XMVECTOR{ x, y, z, 0.0f };

				*/


				//OLD CONTROLS
				/*
				//{
				//auto kb = m_keyboard->GetState();
				//if (kb.Escape)
				//{
				//	cameraPosition = cameraOriginalPostion;
				//}
				//CHANGE CAMERA POSITION without delta



			//	if (KeyInput(Okey)) //camera postion locking at focus
			//	{
			//		orbital = true;
			//	}
			//	if (KeyInput(Fkey)) //Focus change
			//	{
			//		orbital = false;
			//	}

			//	if (orbital == true)
			//	{
			//		//movement in Z-axis W+ S-
			//		if (KeyInput(Wkey))
			//		{
			//			cameraPosition += {0.0f, 0.0f, 0.001f, 0.0f};
			//			cameraFocus += {0.0f, 0.0f, 0.001f, 0.0f};
			//		}
			//		if (KeyInput(Skey))
			//		{
			//			cameraPosition -= {0.0f, 0.0f, 0.001f, 0.0f};
			//			cameraFocus -= {0.0f, 0.0f, 0.001f, 0.0f};
			//		}
			//		//movement in X-axis D+ A-
			//		if (KeyInput(Akey))
			//		{
			//			cameraPosition -= {0.0f, 0.001f, 0.0f, 0.0f};
			//			cameraFocus -= {0.0f, 0.001f, 0.0f, 0.0f};
			//		}
			//		if (KeyInput(Dkey))
			//		{
			//			cameraPosition += {0.0f, 0.001f, 0.0f, 0.0f};
			//			cameraFocus += {0.0f, 0.001f, 0.0f, 0.0f};
			//		}
			//		//movement in Y-axis Q+ E-
			//		if (KeyInput(Qkey))
			//		{
			//			cameraPosition += {0.001f, 0.0f, 0.0f, 0.0f};
			//			cameraFocus += {0.001f, 0.0f, 0.0f, 0.0f};
			//		}
			//		if (KeyInput(Ekey))
			//		{
			//			cameraPosition -= {0.001f, 0.0f, 0.0f, 0.0f};
			//			cameraFocus -= {0.001f, 0.0f, 0.0f, 0.0f};
			//		}
			//		//rest position with HOME
			//		if (KeyInput(Homekey))
			//		{
			//			cameraPosition = cameraOriginalPostion;
			//		}
			//	}
			//	else
			//	{
			//		//movement in Z-axis W+ S-
			//		if (KeyInput(Wkey))
			//		{
			//			cameraFocus += {0.0f, 0.0f, 0.001f, 0.0f};
			//		}
			//		if (KeyInput(Skey))
			//		{
			//			cameraFocus -= {0.0f, 0.0f, 0.001f, 0.0f};
			//		}
			//		//movement in X-axis D+ A-
			//		if (KeyInput(Akey))
			//		{
			//			cameraFocus -= {0.0f, 0.001f, 0.0f, 0.0f};
			//		}
			//		if (KeyInput(Dkey))
			//		{
			//			cameraFocus += {0.0f, 0.001f, 0.0f, 0.0f};
			//		}
			//		//movement in Y-axis Q+ E-
			//		if (KeyInput(Qkey))
			//		{
			//			cameraFocus += {0.001f, 0.0f, 0.0f, 0.0f};
			//		}
			//		if (KeyInput(Ekey))
			//		{
			//			cameraFocus -= {0.001f, 0.0f, 0.0f, 0.0f};
			//		}
			//		//rest position with HOME
			//		if (KeyInput(Homekey))
			//		{
			//			cameraFocus = cameraOriginalFocus;
			//		}
			//	}

			//	//rotate ENTER // BACKSPACE
			//	if (KeyInput(Enterkey))
			//	{
			//		rotationValue = 0.01f;
			//	}
			//	if (KeyInput(Backspacekey))
			//	{
			//		rotationValue = 0.0f;
			//	}
			//}*/