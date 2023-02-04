#include "CommandsManager.h"

namespace forge
{
	void CommandsManager::startup()
	{
		// Allocate the commands array
		this->commands.setNumberOfElements(this->maxNumberOfCommands);
		for(SizeType i = 0; i < this->maxNumberOfCommands; ++i)
		{
			this->commands[i] = nullptr;
		}
	}

	void CommandsManager::shutdown()
	{
		// Delete any commands that exist
		this->deleteAllCommandsStartingFromIndex(0);
	}

	void CommandsManager::undo()
	{
		if(this->nextCommandIndex > 0)
		{
			this->commands[this->nextCommandIndex - 1]->undo();
			--this->nextCommandIndex;
		}
	}
	
	void CommandsManager::redo()
	{
		if(this->nextCommandIndex < this->maxNumberOfCommands && this->commands[this->nextCommandIndex])
		{
			this->commands[this->nextCommandIndex]->execute();
			++this->nextCommandIndex;
		}
	}

	void CommandsManager::deleteAllCommandsStartingFromIndex(SizeType pIndex)
	{
		// Delete all the existing commands starting from the next command index
		for(SizeType i = pIndex; i < this->maxNumberOfCommands; ++i)
		{
			// If the command exists, delete the command
			if(this->commands[i])
			{
				delete this->commands[i];
				this->commands[i] = nullptr;
			}
			// If the command does not exist, then none of the subsequent commands can exist, so the loop can be exited
			else
			{
				break;
			}
		}
	}

	void CommandsManager::clear()
	{
		this->deleteAllCommandsStartingFromIndex(0);
		this->nextCommandIndex = 0;
	}
}