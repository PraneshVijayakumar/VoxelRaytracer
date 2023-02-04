#ifndef FORGE_TEXT_FILE_EDITOR_H
#define FORGE_TEXT_FILE_EDITOR_H

#include "FileEditor.h"

namespace forge
{
	class TextFileEditor : public FileEditor
	{
	public:

		~TextFileEditor();

		virtual bool openFile(const char *pFilePath) override;

		const char* getTextString() const;

	private:

		// A String that holds a copy of the text from the file that the instance of this class opens
		char *textString = nullptr;
	};

	inline
	TextFileEditor::~TextFileEditor()
	{
		delete this->textString;
	}

	inline
	const char* TextFileEditor::getTextString() const
	{
		return textString;
	}
}

#endif