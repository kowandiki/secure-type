#include "typing.h"


Typing::Typing()
{
	this->maxChar = 10;
	this->program = -1;
	this->sizeMult = 1.5;
	this->focusMod = 0.5;
	this->delayBetweenDirKeys = 0;
	this->delayBetweenChars = 3;
	this->delayBetweenChunks = 3;
	this->doGarbage = true;
	this->doClipboard = true;
	this->doSwitchFocus = false;


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

Typing::Typing(
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
)
{
	srand((unsigned)time(0));

	this->maxChar = maxChar < 1 ? 1 : maxChar;
	this->program = program;
	this->sizeMult = sizeMult < 1 ? 1 : sizeMult;
	this->focusMod = focusMod < 0 ? 0 : focusMod;
	this->delayBetweenDirKeys = delayBetweenDirKeys;
	this->delayBetweenChars = delayBetweenChars;
	this->delayBetweenChunks = delayBetweenChunks;
	this->doGarbage = doGarbage;
	this->doClipboard = doClipboard;
	this->doSwitchFocus = doSwitchFocus;
	this->doCtrl = doCtrl;

	this->word = word;

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

void Typing::setWord(std::string word)
{
	this->word = word;
	generateInstructions();
}