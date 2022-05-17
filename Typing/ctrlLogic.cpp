#include "typing.h"
#include "ctrlLogic.h"

int ctrlLogic::indexAfterCtrl(std::string sent, int currentIndex, bool right)
{
	// handle edge cases
	// going right when there are no characters to the right
	if (currentIndex == sent.size() && right)
	{
		return currentIndex;
	}
	// going left when there are no characters to the left
	if (currentIndex == 0 && !right)
	{
		return currentIndex;
	}

	if (right)
	{
		// when going right, sent[currentIndex] is the first character checked
		// find out what group the first character belongs to
		// go until first swap, then compare that swap to all possible comparisons (from groups)
		// if it matches anything, continue this process
		// if it doesn't match anything, return the number of characters checked
		int group;
		char c = sent[currentIndex];

		for (group = 0; this->groups[group].characters.size() != 0; group++)
		{
			if (inGroup(c, &this->groups[group]) != -1)
			{
				break;
			}
		}
		int ret;

		// every value in this, is an index in interactionsR that should be considered
		// will be adjusted after every group change
		int* validInteractions;

		int numOfInteractions;
		// find how big array is
		for (numOfInteractions = 0; this->groups[group].interactionsR[numOfInteractions] != nullptr; numOfInteractions++);

		validInteractions = new int[numOfInteractions];
		for (int i = 0; i < numOfInteractions; i++)
		{
			// set every group as valid
			validInteractions[i] = i;
		}

		// will want to start counting from 0, this makes it easier
		int swaps = -1;
		for (ret = currentIndex; ret < sent.size(); ret++)
		{
			// keep iterating through characters until the character[ret] isn't part of the same group
			// when this happens, check all interationsR and isolate which ones aren't immediately nullified
			// repeat this process using only the newly pruned interationsR
			// do this until none of the interactionsR have more branches, then break the loop

			// if its in the same group so ignore it and go to next loop
			// this assumes there will not be any cases where characters of the same group
			// stop each other
			if (inGroup(sent[ret], &this->groups[group]) != -1)
			{
				continue;
			}
			swaps++;
			int newGroup;
			for (newGroup = 0; this->groups[newGroup].characters.size() != 0; newGroup++)
			{
				if (inGroup(sent[ret], &this->groups[newGroup]) != -1)
				{
					// found the group being swapped to
					break;
				}
			}

			// now find all the valid interactionsR
			// make sure to reset numOfInteractions
			// overwrite index 0 through numOfInteractions with new group numbers
			numOfInteractions = 0;
			for (int i = 0;	i < numOfInteractions; i++)
			{
				if (this->groups[group].interactionsR[i][swaps] == newGroup)
				{
					validInteractions[numOfInteractions] = i;
					numOfInteractions++;
				}
			}
			
			// nothing else matches anymore, so it cannot move right anymore
			if (numOfInteractions == 0)
			{
				break;
			}
			// if numOfInteractions is greater than 0, keep looping since moving right is still valid

		}

		return ret;
	}
	else
	{
		// when moving left

		// when going left, sent[currentIndex - 1] is the first character checked
		// find out what group the first character belongs to
		// go until first swap, then compare that swap to all possible comparisons (from groups)
		// if it matches anything, continue this process
		// if it doesn't match anything, return the number of characters checked
		int group;

		currentIndex--;
		char c = sent[currentIndex];

		for (group = 0; this->groups[group].characters.size() != 0; group++)
		{
			if (inGroup(c, &this->groups[group]) != -1)
			{
				break;
			}
		}
		int ret;

		// every value in this, is an index in interactionsL that should be considered
		// will be adjusted after every group change
		int* validInteractions;

		int numOfInteractions;
		// find how big array is
		for (numOfInteractions = 0; this->groups[group].interactionsR[numOfInteractions] != nullptr; numOfInteractions++);

		validInteractions = new int[numOfInteractions];
		for (int i = 0; i < numOfInteractions; i++)
		{
			// set every group as valid
			validInteractions[i] = i;
		}

		// will want to start counting from 0, this makes it easier
		int swaps = -1;
		for (ret = currentIndex; ret >= 0; ret--)
		{
			// keep iterating through characters until the character[ret] isn't part of the same group
			// when this happens, check all interationsL and isolate which ones aren't immediately nullified
			// repeat this process using only the newly pruned interationsL
			// do this until none of the interactionsL have more valid branches, then break the loop

			// if its in the same group so ignore it and go to next loop
			// this assumes there will not be any cases where characters of the same group
			// stop each other
			if (inGroup(sent[ret], &this->groups[group]) != -1)
			{
				continue;
			}
			swaps++;
			int newGroup;
			for (newGroup = 0; this->groups[newGroup].characters.size() != 0; newGroup++)
			{
				if (inGroup(sent[ret], &this->groups[newGroup]) != -1)
				{
					// found the group being swapped to
					break;
				}
			}

			// now find all the valid interactionsR
			// make sure to reset numOfInteractions
			// overwrite index 0 through numOfInteractions with new group numbers
			numOfInteractions = 0;
			for (int i = 0; i < numOfInteractions; i++)
			{
				if (this->groups[group].interactionsR[i][swaps] == newGroup)
				{
					validInteractions[numOfInteractions] = i;
					numOfInteractions++;
				}
			}

			// nothing else matches anymore, so it cannot move right anymore
			if (numOfInteractions == 0)
			{
				break;
			}
			// if numOfInteractions is greater than 0, keep looping since moving right is still valid

		}

		return ret;
	}
}



int ctrlLogic::inGroup(char c, group* g)
{
	for (int i = 0; g->characters.size(); i++)
	{
		if (g->characters[i] == c)
		{
			return i;
		}
	}
	return -1;
}




ctrlLogic::ctrlLogic()
{
	this->groups = new group;
	this->numOfGroups = 0;
}

ctrlLogic::ctrlLogic(int groupSet)
{
	switch (groupSet)
	{
	case 0:
		// this can be chrome/google docs
		"abcdefghijklmnopqrstuvwxyz0123456789'";
		break;
	}
}