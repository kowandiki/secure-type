#pragma once
#include <iostream>
#include "windows.h"
#include "winuser.h"
#include "..\character.h"

#include <string.h>

struct instruction
{
	int offset;
	int del;
	bool ctrl;
	characters::character c;
	HWND handle;
};
struct group
{
	std::string characters;

};
void releaseAllKeys();



class Typing
{
private:

	instruction** instructionsArr;
	int instructionsArrSize;

	int terminatorOffset = (unsigned int)-1 >> 1;

	//cannot be less than 1
	float sizeMult = 1.25;
	unsigned int maxChar;
	unsigned int delayBetweenDirKeys = 3;
	unsigned int delayBetweenChars = 3;
	unsigned int delayBetweenChunks = 5;

	float focusMod = 0.5;

	bool doGarbage;
	bool doClipboard;
	bool doSwitchFocus;
	bool doCtrl;

	HWND handle;

	INPUT keyboardInput;
	INPUT keyboardModifier;

	std::string alpha;
	std::string special;
	std::string xSpecial;
	std::string xSpecial2;

	std::string nonsense = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()`~-_=+[{]}\\|;:'\",<.>/?";
	characters::character* garbage;
	unsigned int garbageSize = 94;
	// 0 = chrome, 1 = firefox, 2 = discord
	int program;
	std::string word;
public:
	
	Typing();
	Typing(std::string filePath);
	Typing(
		unsigned int maxChar,
		int program,
		float sizeMult,
		float focusMod,
		unsigned int delayBetweenDirKeys,
		unsigned int delayBetweenChars,
		unsigned int delayBetweenChunks,
		bool doGarbage,
		bool doClipboard,
		bool doSwitchFocus,
		bool doCtrl,
		std::string word
	);
	//~Typing();
	void left();
	void right();
	void ctrlDir(bool right);
	void sendchar(characters::character c);
	void sendWord();
	void generateInstructions();
	void delForward();
	void delback();

	bool setClipboard(const char* str);
	void sendClipboard();

	void setHandle(HWND handle) { this->handle = handle; }

	void setWord(std::string word);

};

/*
CHROME
textboxes:
	go until first type switch (unless its a space)
	"|abc " going right will end after space "abc |"
	"| abc" going right will end after space " |abc"
	"abc |" going left will end after a		 "|abc "
	" abc|" going left will end after a		 " |abc"
	treats special & alphanumeric the same
	going right stops after space (1 or 2 type switches)
	going left stops after 1 type switch unless starting on space, in which case its after 2 type switches|| space -> alpha ->(stops)special/another space


FIREFOX
textboxes

DISCORD
starting on alpha, go until 1st type switch
starting on special, go until 2nd type switch

GOOGLE DOCS
	behaves the same as chrome textbox
*/
