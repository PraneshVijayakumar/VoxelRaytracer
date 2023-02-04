#ifndef FORGE_COMMANDS_MANAGER_H
#define FORGE_COMMANDS_MANAGER_H

#include "Command.h"
#include <Forge/Source/Core/Forge.h>
#include <Forge/Source/Core/Containers/ResizableArray.h>

namespace forge
{
	// Responsible for creating, destroying, executing, undoing and redoing commands
	class CommandsManager
	{
	public:

		void startup();

		void shutdown();

		/// <summary>
		/// Create and execute a command of the given type, and record it for potentially undoing and/or redoing it later
		/// </summary>
		/// <typeparam name="CommandType">The type of the command to create and execute</typeparam>
		/// <typeparam name="...CommandConstructorParams">The types of the parameters that will get passed to the command's constructor</typeparam>
		/// <param name="...pCommandConstructorParams">The parameters that will get passed to the command's constructor</param>
		template <typename CommandType, typename... CommandConstructorParams>
		void execute(CommandConstructorParams... pCommandConstructorParams);

		// Undo the last command that was executed or redone
		void undo();

		// Redo the last command that was undone
		void redo();

		void clear();

	private:

		/// <summary>
		/// Deletes any commands that exist, starting from the given index
		/// </summary>
		/// <param name="pIndex">The index of the command to start deleting the commands from</param>
		void deleteAllCommandsStartingFromIndex(SizeType pIndex);

		const SizeType maxNumberOfCommands = 1000;

		ResizableArray<Command*> commands;

		// The index of the next command. The command at this index,
		// if it exists, will get redone when the redo function is called.
		// The index before this is the current command index,
		// and the current command is the one that will get undone when the undo function is called
		SizeType nextCommandIndex = 0;
	};

	template <typename CommandType, typename... CommandConstructorParams>
	void CommandsManager::execute(CommandConstructorParams... pCommandConstructorParams)
	{
		// If the commands array is already full
		if(this->nextCommandIndex == this->maxNumberOfCommands)
		{
			// Delete the oldest command
			delete this->commands[0];

			// Move all the other commands one step back in the array
			for(SizeType i = 0; i < this->maxNumberOfCommands - 1; ++i)
			{
				this->commands[i] = this->commands[i + 1];
			}

			// Set the last command to nullptr
			this->commands[this->maxNumberOfCommands - 1] = nullptr;

			// Decrement the next command index
			--this->nextCommandIndex;
		}
		else
		{
			// Delete all existing commands starting from the next command index
			this->deleteAllCommandsStartingFromIndex(this->nextCommandIndex);
		}

		// Create and execute the new command
		// TODO: Is better parameter pack forwarding needed here?
		this->commands[this->nextCommandIndex] = new CommandType(pCommandConstructorParams...);
		this->commands[this->nextCommandIndex]->execute();

		// Increment the next command index
		++this->nextCommandIndex;
	}
}

#endif