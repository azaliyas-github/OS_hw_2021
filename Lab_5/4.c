#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

bool tryParseInt(char* str, int* result);

int readLine(char* buffer, int bufferSize, FILE* file, bool* eofReached);
void writeLine(char* buffer, int bufferSize, FILE* file);

void applyTerminalSettings();
void restoreTerminalSettings();

bool checkIfInputIsAvailable(long timeoutInSeconds);

void main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <file> <count of lines in a group>\n", argv[0]);
		return;
	}
	
	int linesCountInGroup;
	if (!tryParseInt(argv[2], &linesCountInGroup))
	{
		printf("Invalid number: %s\n", argv[2]);
		return;
	}
	
	FILE* file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("Can't open the file %s\n", argv[1]);
		return;
	}
	
	applyTerminalSettings();
	
	int bufferSize = 256;
	char buffer[bufferSize];
	bool fileIsRead = false;
	while (true)
	{
		for (int i = 0; linesCountInGroup == 0 || i < linesCountInGroup; ++i)
		{
			bool lineIsRead = false;
			while (!lineIsRead)
			{
				bool eofReached;
				int readCharsCount = readLine(buffer, bufferSize, file, &eofReached);
				
				writeLine(buffer, readCharsCount, stdout);
				if (eofReached)
				{
					fileIsRead = true;
					break;
				}
				if (buffer[readCharsCount - 1] == '\n')
					lineIsRead = true;
			}
			
			if (fileIsRead)
				break;
		}
		
		if (fileIsRead)
			break;

		bool waiting = true;
		while (waiting)
			if (checkIfInputIsAvailable(10))
			{
				waiting = false;
				
				do
					fgetc(stdin);
				while (checkIfInputIsAvailable(0));
			}
	}
	
	restoreTerminalSettings();
	
	int closeResult = fclose(file);
	if (closeResult != 0)
		printf("Can't close the file %s\n", argv[1]);
}

bool tryParseInt(char* str, int* resultPointer)
{
	char* endChar;
	int result = strtol(str, &endChar, 10);
	
	if (endChar == str)
		return false;
	
	*resultPointer = result;
	return true;
}

int readLine(char* buffer, int bufferSize, FILE* file, bool* eofReached)
{
	*eofReached = false;
	for (int i = 0; i < bufferSize; ++i)
	{
		char input = fgetc(file);
		int readCharsCount = i + 1;
		if (input == EOF)
		{
			*eofReached = true;
			return readCharsCount - 1;
		}
		
		buffer[i] = input;
		if (input == '\n')
			return readCharsCount;
	}
	return bufferSize;
}

void writeLine(char* buffer, int bufferSize, FILE* file)
{
	for (int i = 0; i < bufferSize; ++i)
		fputc(buffer[i], file);
}

struct termios oldSettings;

void applyTerminalSettings()
{
	struct termios newSettings;
	tcgetattr(fileno(stdin), &oldSettings);
	
	newSettings = oldSettings;
	newSettings.c_lflag &= ~ICANON;
	
	tcsetattr(fileno(stdin), TCSANOW, &newSettings);
}

void restoreTerminalSettings()
{
	tcsetattr(fileno(stdin), TCSANOW, &oldSettings);
}

bool checkIfInputIsAvailable(long timeoutInSeconds)
{
	struct timeval time;
	time.tv_sec = timeoutInSeconds;
	time.tv_usec = 0;

	fd_set set;
	FD_ZERO(&set);
	FD_SET(fileno(stdin), &set);
	
	int result = select(fileno(stdin) + 1, &set, NULL, NULL, &time);
	
	if (result > 0)
		return true;
	if (result < 0)
	{
		printf("Error\n");
		exit(1);
	}
	return false;
}