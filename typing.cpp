#include "typing.h"


Typing::Typing()
{
	this->instructionsArr = nullptr;
	this->instructionsArrSize = 0;

	srand((unsigned)time(0));
	this->program = -1;
	//no need to define alpha, or special

	keyboardInput.type = INPUT_KEYBOARD;
	keyboardInput.ki.wScan = 0; //MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	keyboardInput.ki.time = 0;
	keyboardInput.ki.dwExtraInfo = 0;
	keyboardInput.ki.dwFlags = 0;
	keyboardInput.ki.wVk = 0x0A; //reserved key, wont do anything

	keyboardModifier.type = INPUT_KEYBOARD;
	keyboardModifier.ki.wScan = 0; //MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	keyboardModifier.ki.time = 0;
	keyboardModifier.ki.dwExtraInfo = 0;
	keyboardModifier.ki.dwFlags = 0;
	keyboardModifier.ki.wVk = 0x0A; //reserved key, wont do anything
	characters garbage = characters(this->nonsense);
	this->garbage = garbage.getCharacters();
}

Typing::Typing(unsigned int maxChar, int program, bool doGarbage, std::string word)
{
	srand((unsigned)time(0));
	this->maxChar = maxChar;
	this->word = word;
	this->program = program;
	this->doGarbage = doGarbage;
	this->instructionsArr = nullptr;
	this->instructionsArrSize = 0;

	characters garbage = characters(this->nonsense);
	this->garbage = garbage.getCharacters();

	this->generateInstructions();

	keyboardInput.type = INPUT_KEYBOARD;
	keyboardInput.ki.wScan = 0; //MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	keyboardInput.ki.time = 0;
	keyboardInput.ki.dwExtraInfo = 0;
	keyboardInput.ki.dwFlags = 0;
	keyboardInput.ki.wVk = 0x0A; //reserved key, wont do anything

	keyboardModifier.type = INPUT_KEYBOARD;
	keyboardModifier.ki.wScan = 0; //MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
	keyboardModifier.ki.time = 0;
	keyboardModifier.ki.dwExtraInfo = 0;
	keyboardModifier.ki.dwFlags = 0;
	keyboardModifier.ki.wVk = 0x0A; //reserved key, wont do anything

	//required due to slight differences between different programs
	switch (this->program)
	{
	case 0:
		//chrome
		alpha = "abcdefghijklmnopqrstuvwxyz1234567890_";
		special = "~`!@#$%&*()-+=[]\\{}|;:'\",<.>/?^";
		xSpecial = " ";
		xSpecial2 = "";
		break;
	case 1:
		//firefox
		alpha = "abcdefghijklmnopqrstuvwxyz1234567890_";
		special = "~`!@#$%&*()-+=[]\\{}|;:'\",<.>/?";
		xSpecial = " ";
		xSpecial2 = "^";
		break;
	case 2:
		//discord
		alpha = "abcdefghijklmnopqrstuvwxyz1234567890";
		special = "~`!@#$%&*()-+=[]\\{}|;:'\",<.>/?^_ ";
		xSpecial = "";
		xSpecial2 = "";
		break;
	}
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

	//*
	for (int i = 0; i < this->instructionsArrSize; i++)
	{
		
		for (int j = 0; this->instructionsArr[i][j].offset != this->terminatorOffset; j++)
		{
			int offset = instructionsArr[i][j].offset;
			int del = instructionsArr[i][j].del;
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
			// std::cout << " |" << instructionsArr[i][j].c.ch << instructionsArr[i][j].c.c << "| ";
			sendchar(this->instructionsArr[i][j].c);
				
		}
		Sleep(this->delayBetweenChunks);
	} // */
}

void Typing::generateInstructions()
{
	if (this->instructionsArr != nullptr)
	{
		//instructions have already been made, so overwrite
		for (int i = 0; i < this->instructionsArrSize; i++)
		{
			delete[] this->instructionsArr[i];
			this->instructionsArr[i] = nullptr;
		}
		delete[] instructionsArr;
		instructionsArr = nullptr;
	}

	// split up this->word into this->maxChar sized chunks if doGarbage is false
	// 
	// if doGarbage is true, generate a random number between 
	// this->maxChar / this->sizeMult and this->maxChar (i.e 30/2, 30)
	// random num will be the number of characters to type, remaining will be garbage characters
	// 
	// Generate instructions for each list, increment instructionsArrSize, add to instructionsArr
	// make sure this is all stored in the heap, so it can be accessed whenever hotkey is pressed
	// 
	// calling function again should generate a new random list of instructions
	// 
	// may look into an entropy esque thing that can quantify how securely or insecurily
	// something will be typed

	int* amountOfGarbage = new int[this->word.size()];
	amountOfGarbage[0] = -1;
	int remainder;

	std::string* wordList;

	if (this->doGarbage)
	{
		

		// if doGarbage is true, generate a random number between 
		// this->maxChar / this->sizeMult and this->maxChar (i.e 30/2, 30)
		// random num will be the number of characters to type, remaining will be garbage characters
		int charRemaining = this->word.size();
		int charSent = 0;

		int itr;
		int max = 0;
		for (itr = 0; charRemaining > 0; itr++)
		{
			int randNum = rand() % (int)(this->maxChar / this->sizeMult);
			int impChars = this->maxChar - randNum;

			if (impChars > charRemaining)
			{
				randNum = this->maxChar - charRemaining;
			}
			amountOfGarbage[itr] = randNum;
			this->instructionsArrSize++;
			charRemaining -= impChars;
		}
		amountOfGarbage[itr + 1] = -1;


		wordList = new std::string[this->instructionsArrSize];

		int startIndex = 0;
		for (int i = 0; i < this->instructionsArrSize; i++)
		{
			int amount = this->maxChar - amountOfGarbage[i];
			wordList[i] = std::string(this->maxChar, 1);
			std::string str = this->word.substr(startIndex, amount);
			// assign characters from this->word to wordList[i]
			for (int j = 0; j < amount; j++)
			{
				wordList[i][j] = str[j];
			}
			startIndex += amount;
		}

	}
	else
	{
		this->instructionsArrSize = this->word.size() / this->maxChar;
		remainder = this->word.size() % this->maxChar;

		if (remainder == 0)
		{
			remainder = this->maxChar;
		}
		else
		{
			//remainder = remainder
			this->instructionsArrSize++;
		}

		wordList = new std::string[this->instructionsArrSize];

		int startIndex = 0;
		int substrSize;

		//split this->word into maxChar char chunks to reduce likelyhood of mistakes
		if (this->instructionsArrSize > 1)
		{
			substrSize = this->maxChar;
		}
		else
		{
			substrSize = remainder;
		}

		for (int i = 0; i < this->instructionsArrSize; i++)
		{
			wordList[i] = this->word.substr(startIndex, substrSize);

			startIndex += substrSize;
			if (i - 1 == this->instructionsArrSize)
			{
				substrSize = remainder;
			}
		}

	}
	

	this->instructionsArr = new instruction* [this->instructionsArrSize];


	// Generate instructions for each list, increment instructionsArrSize, add to instructionsArr
	// make sure this is all stored in the heap, so it can be accessed whenever hotkey is pressed

	// create each word chunk
	// value of 1 = char to be randomely generated
	
	
	//scramble and keep track of swaps
	int** swaps = new int* [this->instructionsArrSize];

	for (int i = 0; i < this->instructionsArrSize; i++)
	{
		swaps[i] = new int[wordList[i].size()];
		for (int j = 0; j < wordList[i].size(); j++)
		{
			char c = wordList[i][j];
			int randNum = rand() % wordList[i].size();
			wordList[i][j] = wordList[i][randNum];
			wordList[i][randNum] = c;
			swaps[i][j] = randNum;
		}
	}

	int* finalIndexes = new int[this->maxChar];
	int* finalIndex = new int[this->maxChar];
	for (int iteration = 0; iteration < this->instructionsArrSize; iteration++)
	{
		

		//find final index of each value
		//this is done by following each swap
		//can probably be optimized
		int index = 0;
		for (int i = 0; i < wordList[iteration].size(); i++)
		{
			index = i;
			//follow value of i and set final index
			for (int j = 0; j < wordList[iteration].size(); j++)
			{
				if (j == index)
				{
					index = swaps[iteration][j];
				}
				else if (swaps[iteration][j] == index)
				{
					index = j;
				}
			}
			finalIndexes[i] = index;
		}
		//get different version of finalindexes
		for (int i = 0; i < wordList[iteration].size(); i++)
		{
			finalIndex[finalIndexes[i]] = i;
		}

		//create array of instructions
		this->instructionsArr[iteration] = new instruction[wordList[iteration].size() + 2];
		//set terminator
		this->instructionsArr[iteration][wordList[iteration].size() + 1].del = this->terminatorOffset;
		this->instructionsArr[iteration][wordList[iteration].size() + 1].offset = this->terminatorOffset;
		this->instructionsArr[iteration][wordList[iteration].size() + 1].ctrl = false;


		//set valid instructions
		int currentIndex = 0;
		int targetIndex = 0;
		int offset = 0;
		characters charList(wordList[iteration]);
		for (int i = 0; i < wordList[iteration].size(); i++)
		{
			targetIndex = 0;
			//if finalIndex[x] < finalIndex[i]
			//targetIndex++
			// if not, do nothing
			for (int x = i; x >= 0; x--)
			{
				if (finalIndex[x] < finalIndex[i])
				{
					targetIndex++;
				}
			}


			//offset = finalIndex[i] - currentIndex;
			offset = targetIndex - currentIndex;


			//currentIndex = finalIndex[i] + 1;
			currentIndex = targetIndex + 1;

			//save offset into instruction array
			this->instructionsArr[iteration][i].offset = offset;
			this->instructionsArr[iteration][i].del = 0;
			if (charList[i].ch == 1)
			{
				int owo = rand() % this->garbageSize;
				char uwu = nonsense[owo];
				charList.getCharacters()[i] = charList.charToVK(uwu);
			}
			this->instructionsArr[iteration][i].c = charList[i];
			
			
		}
		//move cursor to last character sent
		this->instructionsArr[iteration][wordList[iteration].size()].offset = wordList[iteration].size() - currentIndex;
		this->instructionsArr[iteration][wordList[iteration].size()].del = -amountOfGarbage[iteration];
		this->instructionsArr[iteration][wordList[iteration].size()].ctrl = false;
		this->instructionsArr[iteration][wordList[iteration].size()].c = characters::character{0,0x0A, 0};
	}
	delete[] finalIndexes;
	delete[] finalIndex;
}

