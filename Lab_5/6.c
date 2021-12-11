#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

DIR* const openDirectory(const char* const directoryName);
void closeDirectory(DIR* const directory, const char* const directoryName);
void printDirectoryContent(DIR* const directory, const char* const directoryName);

void main(const int argc, const char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <directory>\n", argv[0]);
		return;
	}
	
	const char* const currentDirectoryName = getcwd(NULL, 0);
	DIR* const currentDirectory = openDirectory(currentDirectoryName);
	printDirectoryContent(currentDirectory, currentDirectoryName);
	closeDirectory(currentDirectory, currentDirectoryName);
	free(currentDirectoryName);
	
	const char* const argDirectoryName = argv[1];
	DIR* const argDirectory = openDirectory(argDirectoryName);
	printDirectoryContent(argDirectory, argDirectoryName);
	closeDirectory(argDirectory, argDirectoryName);
}

DIR* const openDirectory(const char* const directoryName)
{
	DIR* const directory = opendir(directoryName);
	if (directory == NULL)
	{
		printf("Can't open the directory %s\n", directoryName);
		exit(1);
	}
	return directory;
}

void closeDirectory(DIR* const directory, const char* const directoryName)
{
	const int closeResult = closedir(directory);
	if (closeResult != 0)
		printf("Can't close the directory %s\n", directoryName);
}

void printDirectoryContent(DIR* const directory, const char* const directoryName)
{
	const int oldErrno = errno;
	
	printf("Content of %s\n", directoryName);
	
	bool directoryRead = false;
	while (!directoryRead)
	{
		errno = 0;
		struct dirent* const directoryEntry = readdir(directory);
		if (errno != 0)
		{
			printf("Error\n");
			exit(1);
		}
		
		directoryRead = directoryEntry == NULL;
		if (directoryRead)
			continue;
		
		printf("%s\n", directoryEntry->d_name);
	}
	printf("\n");
	
	errno = oldErrno;
}