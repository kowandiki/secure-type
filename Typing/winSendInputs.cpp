#include "typing.h"

bool Typing::setClipboard(const char* str)
{
	//open clipboard and set value to c
	//close clipboard

	if (!OpenClipboard(nullptr))
	{
		std::cout << "nullptr" << std::endl;
		return false;
	}

	//empty clipboard
	EmptyClipboard();

	//allocate space for str
	auto alloc = GlobalAlloc(GMEM_FIXED, strlen(str) + 1);
	memcpy(alloc, str, strlen(str));

	//set str in clipboard
	SetClipboardData(CF_TEXT, alloc);

	//close clipboard
	CloseClipboard();
	return true;
}

void Typing::sendClipboard()
{
	//press ctrl+v
	keyboardModifier.ki.wVk = VK_CONTROL;
	keyboardModifier.ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);

	//ctrl down
	keyboardModifier.ki.dwFlags = 0;
	SendInput(1, &keyboardModifier, sizeof(INPUT));

	//v down

	keyboardInput.ki.wVk = 'V';
	keyboardInput.ki.wScan = MapVirtualKey('V', MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));

	//v up
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));

	//ctrl up
	keyboardModifier.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardModifier, sizeof(INPUT));
}

void Typing::sendchar(characters::character c)
{
	// if (c.c > 0xFE)
	// {
	// 	 std::cout << "invalid vk" << c.c << std::endl;
	// 	 return;
	// }
	if (c.shift)
	{
		//set keys
		keyboardModifier.ki.wVk = VK_SHIFT;
		keyboardModifier.ki.wScan = MapVirtualKey(VK_SHIFT, MAPVK_VK_TO_VSC);

		//shift down
		keyboardModifier.ki.dwFlags = 0;
		SendInput(1, &keyboardModifier, sizeof(INPUT));
	}

	keyboardInput.ki.wVk = c.c;
	keyboardInput.ki.wScan = MapVirtualKey(c.c, MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));

	if (c.shift)
	{
		//shift up
		keyboardModifier.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &keyboardModifier, sizeof(INPUT));
	}
}

void Typing::sendWord()
{

	//get handle
	this->handle = GetForegroundWindow();

	//block user input (only works when run as admin)
	BlockInput(true);

	//release all keys
	releaseAllKeys();

	//*
	for (int i = 0; i < this->instructionsArrSize; i++)
	{

		for (int j = 0; this->instructionsArr[i][j].offset != this->terminatorOffset; j++)
		{
			int offset = instructionsArr[i][j].offset;
			int del = instructionsArr[i][j].del;

			// SetFocus(instructionsArr[i][j].handle);
			if (instructionsArr[i][j].handle == NULL)
			{
				SetForegroundWindow(GetDesktopWindow());
			}
			else
			{
				SetForegroundWindow(this->handle);
			}


			while (offset < 0)
			{
				left();
				offset++;
				Sleep(this->delayBetweenDirKeys);
			}
			while (offset > 0)
			{
				right();
				offset--;
				Sleep(this->delayBetweenDirKeys);
			}
			if (this->doGarbage)
			{
				while (del < 0)
				{
					delback();
					del++;
					Sleep(this->delayBetweenDirKeys);
				}
				while (del > 0)
				{
					delForward();
					del--;
					Sleep(this->delayBetweenDirKeys);
				}
			}

			Sleep(this->delayBetweenChars);

			//50/50 on whether to use clipboard ot send next char or sendchar()

			if (this->doClipboard)
			{
				if (rand() % 1)
				{
					sendchar(this->instructionsArr[i][j].c);
				}
				else
				{
					if (setClipboard(new const char[2]{ instructionsArr[i][j].c.ch, '\0' }))
						sendClipboard();
					else
						sendchar(this->instructionsArr[i][j].c);
				}
				continue;
			}
			sendchar(this->instructionsArr[i][j].c);
		}
		Sleep(this->delayBetweenChunks);
		//make sure its always tabbed in at the end of each chunk
		SetForegroundWindow(this->handle);
	} // */

	//unblock user input
	BlockInput(false);
}




void releaseAllKeys()
{
	//toggle caps lock if its on
	if (GetKeyState(VK_CAPITAL))
	{
		INPUT keyboardInput;

		keyboardInput.type = INPUT_KEYBOARD;
		keyboardInput.ki.time = 0;
		keyboardInput.ki.dwExtraInfo = 0;

		keyboardInput.ki.wVk = VK_CAPITAL;
		keyboardInput.ki.wScan = MapVirtualKey(VK_CAPITAL, MAPVK_VK_TO_VSC);
		keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
		SendInput(1, &keyboardInput, sizeof(INPUT));
		keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &keyboardInput, sizeof(INPUT));
	}
	//use setKeyboardState to release everything else

	//TODO: Fix memory leak
	//I'm pretty sure this just doesn't work
	LPBYTE arr = new BYTE[256];
	for (int i = 0; i < 256; i++)
	{
		arr[i] = 0;
	}

	SetKeyboardState(arr);
}


void Typing::left()
{
	//left arrow key = VK_LEFT
	keyboardInput.ki.wVk = VK_LEFT;
	keyboardInput.ki.wScan = MapVirtualKey(VK_LEFT, MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));
}

void Typing::right()
{
	//right arrow key = VK_RIGHT
	keyboardInput.ki.wVk = VK_RIGHT;
	keyboardInput.ki.wScan = MapVirtualKey(VK_RIGHT, MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));
}

void Typing::delForward()
{
	keyboardInput.ki.wVk = VK_DELETE;
	keyboardInput.ki.wScan = MapVirtualKey(VK_DELETE, MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));
}

void Typing::delback()
{
	keyboardInput.ki.wVk = VK_BACK;
	keyboardInput.ki.wScan = MapVirtualKey(VK_BACK, MAPVK_VK_TO_VSC);
	keyboardInput.ki.dwFlags = 0; // 0 = key down, KEYEVENTF_KEYUP = key up
	SendInput(1, &keyboardInput, sizeof(INPUT));
	keyboardInput.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardInput, sizeof(INPUT));
}

void Typing::ctrlDir(bool doRight)
{
	//set keys
	keyboardModifier.ki.wVk = VK_LCONTROL;
	keyboardModifier.ki.wScan = MapVirtualKey(VK_CONTROL, MAPVK_VK_TO_VSC);

	//ctrl down
	keyboardModifier.ki.dwFlags = 0;
	SendInput(1, &keyboardModifier, sizeof(INPUT));

	//press arrow
	doRight ? this->right() : this->left();

	//release ctrl
	keyboardModifier.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &keyboardModifier, sizeof(INPUT));
}


