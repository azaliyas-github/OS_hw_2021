#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

const int readFile(char* const buffer, const int bufferSize, FILE* const file, bool* const eofReached);
void writeFile(const char* const buffer, const int bufferSize, FILE* const file);

FILE* const openFile(const char* const fileName, const char* const mode);
void closeFile(FILE* const file, const char* const fileName);

void copyFile(FILE* const sourceFile, FILE* const destinationFile);
void copyPermissions(FILE* const sourceFile, FILE* const destinationFile);

void main(const int argc, const char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <source file> <destination file>\n", argv[0]);
		return;
	}
	
	FILE* const sourceFile = openFile(argv[1], "r");
	FILE* const destinationFile = openFile(argv[2], "w");
	
	copyFile(sourceFile, destinationFile);
	copyPermissions(sourceFile, destinationFile);

	closeFile(destinationFile, argv[2]);
	closeFile(sourceFile, argv[1]);
}

const int readFile(char* const buffer, const int bufferSize, FILE* const file, bool* const eofReached)
{
	*eofReached = false;
	for (int i = 0; i < bufferSize; ++i)
	{
		const char input = fgetc(file);
		const int readCharsCount = i + 1;
		if (feof(file) != 0)
		{
			*eofReached = true;
			return readCharsCount - 1;
		}
		
		buffer[i] = input;
	}
	return bufferSize;
}

void writeFile(const char* const buffer, const int bufferSize, FILE* const file)
{
	for (int i = 0; i < bufferSize; ++i)
		fputc(buffer[i], file);
}

FILE* const openFile(const char* const fileName, const char* const mode)
{
	FILE* const file = fopen(fileName, mode);
	if (file == NULL)
	{
		printf("Can't open the file %s\n", fileName);
		exit(1);
	}
	return file;
}

void closeFile(FILE* const file, const char* const fileName)
{
	const int closeResult = fclose(file);
	if (closeResult != 0)
		printf("Can't close the file %s\n", fileName);
}

void copyFile(FILE* const sourceFile, FILE* const destinationFile)
{
	bool eofReached = false;
	while (!eofReached)
	{
		const int bufferSize = 256;
		char buffer[bufferSize];
		
		const int readCharsCount = readFile(buffer, bufferSize, sourceFile, &eofReached);
		writeFile(buffer, readCharsCount, destinationFile);
	}
}

void copyPermissions(FILE* const sourceFile, FILE* const destinationFile)
{
	struct stat sourceFileInfo;
	fstat(fileno(sourceFile), &sourceFileInfo);
	fchmod(fileno(destinationFile), sourceFileInfo.st_mode);
}