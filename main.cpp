#include <iostream>

//#include "windows.h"

#include "typing.h"
#include <fstream>
#include <string>

#include <iostream>
#include <windows.h>

// #include <thread>

using T = void(Typing::*)();
void hotkeyReadMsg(int* ids, T* fcnArr, Typing* a, int size);

int main(int argc, char** argv)
{
	// SecureType.exe								SecureType.exe
	// chunk size									15
	// what program to use							-1
	// size multiplier (must be greater than 1)		1.5
	// focus mulitplier (must not be less than 0)	0.5
	// delay between arrow keys						0
	// delay between each character					3
	// delay between each chunk						0
	// send garbage									true
	// use the clipboard							true
	// switch focus									true
	// use ctrl										true
	// vk of hotkey (int)							check windows list of vks
	// modifiers (int)								int
	// what you want to be sent / file path			literally anything
	// min args: 14
	
	//Typing b(8, -1, 1.5, 0.5, 0, 20, 0, false, true, true, true, "meowdy");
	//Sleep(2000);
	//b.sendWord();
	//return 0;

	if (argc < 14)
	{
		std::cout << "Required Parameters:" << std::endl;
		std::cout << "1) chunk size (int)" << std::endl;
		std::cout << "2) what program to use (int)" << std::endl;
		std::cout << "3) size multiplier (float > 1)" << std::endl;
		std::cout << "4) focus swap multiplier (float !< 0)" << std::endl;
		std::cout << "5) delay between arrow keys (int)" << std::endl;
		std::cout << "6) delay between characters (int)" << std::endl;
		std::cout << "7) delay between chunks (int)" << std::endl;
		std::cout << "8) send garbage (t/f)" << std::endl;
		std::cout << "9) use clipboard (t/f)" << std::endl;
		std::cout << "10) swap focus (t/f)" << std::endl;
		std::cout << "11) use ctrl (t/f)" << std::endl;
		std::cout << "12) vk of hotkey (int)" << std::endl;
		std::cout << "13) modifier keys (int)" << std::endl;
		std::cout << "14) file path / string to be sent" << std::endl;
		return -1;
	}
	int chunksize; 
	int program;
	float sizeMult;
	float focusMod;
	int delayArrow;
	int delayChar;
	int delayChunk;
	int vk;
	int mod;
	try
	{
		chunksize = std::stoi(argv[1]);
	}
	catch (...)
	{
		std::cout << "invalid chunk size" << std::endl;
		return 1;
	}
	try
	{
		program = std::stoi(argv[2]);
	}
	catch (...)
	{
		std::cout << "invalid program" << std::endl;
		return 2;
	}
	try
	{
		sizeMult = std::stof(argv[3]);
		if (sizeMult < 1)
			throw 1;
	}
	catch (...)
	{
		std::cout << "invalid size mult" << std::endl;
		return 3;
	}
	try
	{
		focusMod = std::stof(argv[4]);
		if (focusMod < 0)
			throw 1;
	}
	catch (...)
	{
		std::cout << "invalid focus mod" << std::endl;
		return 4;
	}
	
	try
	{
		delayArrow = std::stoi(argv[5]);
	}
	catch (...)
	{
		std::cout << "invalid arrow key delay" << std::endl;
		return 5;
	}
	
	try
	{
		delayChar = std::stoi(argv[6]);
	}
	catch (...)
	{
		std::cout << "invalid char delay" << std::endl;
		return 6;
	}
	
	try
	{
		delayChunk = std::stoi(argv[7]);
	}
	catch (...)
	{
		std::cout << "invalid chunk delay" << std::endl;
		return 7;
	}

	bool doGarbage = argv[8][0] == 't' ? true : false;
	bool doClipboard = argv[9][0] == 't' ? true : false;
	bool doFocus = argv[10][0] == 't' ? true : false;
	bool doCtrl = argv[11][0] == 't' ? true : false;
	
	try
	{
		vk = std::stoi(argv[12]);
	}
	catch (...)
	{
		std::cout << "invalid vk" << std::endl;
		return 8;
	}
	
	try
	{
		mod = std::stoi(argv[13]);
	}
	catch (...)
	{
		std::cout << "invalid mod" << std::endl;
		return 9;
	}
	std::string word = "";

	for (int i = 14; i < argc; i++)
	{
		word += argv[i];
		word += " ";
	}
	//word[word.size() - 1] = '\0';
	word = word.substr(0, word.size() - 1);

	std::ifstream file(word);

	if (file.is_open())
	{
		std::cout << "file exists" << std::endl;
		std::string str;
		word = "";
		while (std::getline(file, str))
		{
			str += "\n";
			word += str;
		}
		word = word.substr(0, word.size() - 2);
		file.close();
	}
	else
	{
		std::cout << "unable to find file, treating as string" << std::endl;
	}
	
	Typing a;
	a = Typing(
		10,
		-1,
		1.5,
		0.5,
		0,
		3,
		3,
		doGarbage,
		doClipboard,
		doFocus,
		doCtrl,
		word
	);

	//set up hotkey stuff
	RegisterHotKey(NULL, 1, mod, vk);

	T* arr = new T[1]{ &Typing::sendWord };
	int* ids = new int[1]{ 1 };

	hotkeyReadMsg(ids, arr, &a, 1);

	return 0;
}