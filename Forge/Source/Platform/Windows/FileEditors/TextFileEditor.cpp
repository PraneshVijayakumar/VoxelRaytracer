#include "TextFileEditor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <Forge/Source/Core/Forge.h>

namespace forge
{
	bool TextFileEditor::openFile(const char *pFilePath)
	{
		// If a file has already been opened, close it first
		if(this->textString != nullptr)
		{
			delete this->textString;
		}

		// Open an input filestream for the file
		std::ifstream inputFileStream(pFilePath);

		// Check if the file stream is currently associated to a file
		if(!inputFileStream.is_open())
		{
			return false;
		}

		std::string fileString;

		// Read each line from the file and add it to the text string
		std::string lineString;
		while(std::getline(inputFileStream, lineString))
		{
			// Manually adding a newline because std::getline gets a line without the newline
			fileString += lineString + "\n";
		}

		// Create the textString array and copy the file string into it
		{
			this->textString = new char[fileString.length() + 1];
			for(SizeType i = 0; i < fileString.length(); ++i)
			{
				this->textString[i] = fileString.c_str()[i];
			}
			// Set the final character to be the null character
			this->textString[fileString.length() - 1] = '\0';
		}
		
		// The TextFileEditor has been created successfully
		return true;
	}
}