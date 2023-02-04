#ifndef FORGE_FILE_EDITOR_H
#define FORGE_FILE_EDITOR_H

namespace forge
{
	class FileEditor
	{
	public:

		/// <summary>
		/// Loads a file at the given path from the hard drive
		/// </summary>
		/// <param name="pFilePath">The path of the file to load</param>
		/// <returns>True if the file was opened successfully, false if it was not</returns>
		virtual bool openFile(const char *pFilePath) = 0;
	};
}

#endif