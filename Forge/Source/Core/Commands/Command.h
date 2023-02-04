#ifndef FORGE_COMMAND_H
#define FORGE_COMMAND_H

namespace forge
{
	class CommandsManager;

	class Command
	{
		friend class CommandsManager;

	public:

		virtual ~Command() {};

	private:

		virtual void execute() = 0;

		virtual void undo() = 0;
	};
}

#endif