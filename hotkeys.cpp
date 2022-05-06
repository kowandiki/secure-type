#include "windows.h"
#include "typing.h"


using T = void(Typing::*)();
void hotkeyReadMsg(int* ids, T* fcnArr, Typing* a, int size)
{
	if (size < 1)
	{
		//close threads, don't create a new one	
		return;
	}
	// create separate thread and monitor for when hotkey is pressed
	// if pressed, & matches id, perform attached func
	// take array of ids & func ptrs
	// ideally, if this is called again, close the previous thread (if there was one)
	// and replace it with a new one
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_HOTKEY)
		{
			for (int i = 0; i < size; i++)
			{
				if (msg.wParam == ids[i])
				{
					(a->*fcnArr[i])();
					break;
				}
			}

		}
	}
}