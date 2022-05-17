#include "typing.h"

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

	this->instructionsArr = new instruction * [this->instructionsArrSize];


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

		int nullFocusCount = (wordList[iteration].size() * this->focusMod) * doSwitchFocus;
		int currentInstSize = wordList[iteration].size() + nullFocusCount;


		//create array of instructions
		this->instructionsArr[iteration] = new instruction[currentInstSize + 2];
		//set terminator
		this->instructionsArr[iteration][currentInstSize + 1].del = this->terminatorOffset;
		this->instructionsArr[iteration][currentInstSize + 1].offset = this->terminatorOffset;
		this->instructionsArr[iteration][currentInstSize + 1].ctrl = false;
		this->instructionsArr[iteration][currentInstSize + 1].handle = this->handle;

		//set stuff for null focus swap
		for (int i = 0; i < nullFocusCount;)
		{
			int num = rand() % currentInstSize;
			if (instructionsArr[iteration][num].handle != (HWND)-1)
			{
				instructionsArr[iteration][num].handle = (HWND)-1;
				i++;
			}
		}


		//set valid instructions
		int currentIndex = 0;
		int targetIndex = 0;
		int offset = 0;
		characters charList(wordList[iteration]);
		int charListLocation;
		for (int i = 0, charListLocation = 0; i < currentInstSize; i++)
		{

			if (instructionsArr[iteration][i].handle == (HWND)-1)
			{
				//generate random instructions to throw off keyloggers
				this->instructionsArr[iteration][i].c = charList.charToVK(nonsense[rand() % this->garbageSize]);

				if (rand() % 1)
				{
					//go left
					this->instructionsArr[iteration][i].offset = (rand() % (i - currentIndex + 1)) * -1;
				}
				else
				{
					//go right
					this->instructionsArr[iteration][i].offset = rand() % (i - currentIndex + 1);
				}

				this->instructionsArr[iteration][i].handle = NULL;
				this->instructionsArr[iteration][i].del = 0;
				this->instructionsArr[iteration][i].ctrl = 0;
				continue;
			}
			targetIndex = 0;
			//if finalIndex[x] < finalIndex[i]
			//targetIndex++
			// if not, do nothing
			for (int x = charListLocation; x >= 0; x--)
			{
				if (finalIndex[x] < finalIndex[charListLocation])
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
			if (charList[charListLocation].ch == 1)
			{
				int owo = rand() % this->garbageSize;
				char uwu = nonsense[owo];
				charList[charListLocation] = charList.charToVK(uwu);
			}
			this->instructionsArr[iteration][i].c = charList[charListLocation];
			// this->instructionsArr[iteration][i].handle = this->handle;
			// make sendword() handle getting the target window
			this->instructionsArr[iteration][i].handle = (HWND)1;
			this->instructionsArr[iteration][i].ctrl = 0;

			charListLocation++;

		}
		//move cursor to last character sent
		this->instructionsArr[iteration][currentInstSize].offset = wordList[iteration].size() - currentIndex;
		this->instructionsArr[iteration][currentInstSize].del = -amountOfGarbage[iteration];
		this->instructionsArr[iteration][currentInstSize].ctrl = false;
		this->instructionsArr[iteration][currentInstSize].c = characters::character{ 0,0x0A, 0 };
		this->instructionsArr[iteration][currentInstSize].handle = (HWND)1;
		//this->instructionsArr[iteration][currentInstSize].handle = this->handle;
	}
	delete[] finalIndexes;
	delete[] finalIndex;
}