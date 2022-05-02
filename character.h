#pragma once

#include <iostream>
#include "windows.h"
#include "winuser.h"

class characters
{
public:
	struct character
	{
		bool shift;
		unsigned int c;
		char ch;
	};
private:
	std::string str;
	character* charList;
	int _size;
	unsigned int* virtualKeys;
	const char* systemDefault = "00000800";
	HKL keyboardLayout;
public:
	characters();
	characters(std::string str);

	void generateCharacterList();
	character charToVK(char c);


	character* getCharacters() { return this->charList; }
	int size() { return this->_size; }

	void setStr(std::string str);

	character operator [] (int i);

	~characters()
	{
		delete[] charList;
		charList = nullptr;
	}
};