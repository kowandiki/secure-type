#pragma once

#include <iostream>

class ctrlLogic
{

public:
	struct group
	{
		std::string characters;
		int** interactionsR;
		int** interactionsL;
		// each array is a special casem each value within the array corresponds to
		// the next valid group to allow it to continue moving, if nothing matches it stops
		// this is a really bad approach but I cannot think of a better way to allow custom cases
		// **nullptr will act as terminator**
	};
private:
	group* groups;
	int numOfGroups;

	ctrlLogic();
	// will probably need to make this read from file or find
	ctrlLogic(int groupSet);

	int indexAfterCtrl(std::string sent, int currentIndex, bool right);

	int inGroup(char c, group* group);
};