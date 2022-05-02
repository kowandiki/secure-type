#include <iostream>

#include "typing.h"

#include <fstream>
#include <string>

int main(int argc, char** argv)
{
	// std::cout << rand() % 30 + (30/2) << std::endl;
	
	
	Sleep(2000);
	unsigned int maxChar;
	std::string owo;
	if (argc > 1)
	{
		owo = argv[1];
		maxChar = stoi(owo);
	}
	else
	{
		maxChar = 30;
	}
	std::ifstream file("text.txt");
	std::string str;
	Typing a;
	//unsigned int maxChar, int program, bool doGarbage, std::string word);
	while (std::getline(file, str))
	{
		str += "\n";
		a = Typing(maxChar, -1, 1, str);
		a.sendWord();
	}
	file.close();
	/*
	std::string sex = "";
	for (int i = 1; i < argc; i++)
	{
		sex += argv[i];
		sex += " ";
	} // */
	// std::string str = "abcd";
	// std::cout << str.substr(1, 3) << std::endl;
	
	
	//Typing a = Typing(30,"Prior to Windows 8: If the specified input locale identifier is not already loaded, the function loads and activates the input locale identifier for the current thread. \nBeginning in Windows 8: If the specified input locale identifier is not already loaded, the function loads and activates the input locale identifier for the system.", -1);
	//a.sendWordPreCompute(false, false, false);
	// */
	return 0;
}