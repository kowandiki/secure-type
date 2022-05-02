#include "character.h"

characters::characters()
{
	this->keyboardLayout = LoadKeyboardLayoutA(
		systemDefault, //default language locale as defined on the system
		0x00000010 // replace lang ? idk
	);
	
}

characters::characters(std::string str)
{

	this->keyboardLayout = LoadKeyboardLayoutA(
		systemDefault, //default language locale as defined on the system
		0x00000010 // replace lang ? idk
	);

	this->str = str;
	this->_size = str.size();
	generateCharacterList();
}

void characters::setStr(std::string str)
{
	this->str = str;
	this->_size = str.size();
	generateCharacterList();
}

void characters::generateCharacterList()
{
	character* c = new character[_size + 1];

	//set terminator
	c[_size].c = 0;
	c[_size].ch = 0;
	c[_size].shift = 0;

	for (int i = 0; i < _size; i++)
	{
		c[i] = charToVK(str[i]);
	}

	this->charList = c;
}

characters::character characters::charToVK(char c)
{
	character ret;
	if (c > 0xFF or c < 0x01)
	{
		std::cout << "panic" << std::endl;
		ret.c = 0x0A;
		ret.ch = 0;
		ret.shift = false;
		return ret;
	}
	ret.ch = c;
	int vk = VkKeyScanExA(c, keyboardLayout);

	auto test = std::div(vk, 256);
	ret.shift = test.quot;
	ret.c = test.rem;
	return ret;
}

characters::character characters::operator [] (int i)
{
	return this->charList[i];
}