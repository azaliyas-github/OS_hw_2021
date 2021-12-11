#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

void applyTerminalSettings(bool echo);
void restoreTerminalSettings();

bool checkIfInputIsAvailable(long timeoutInSeconds);

void main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <output file>\n", argv[0]);
		return;
	}
	
	FILE* file = fopen(argv[1], "w");
	if (file == NULL)
	{
		printf("Can't create the file %s\n", argv[1]);
		return;
	}
	
	applyTerminalSettings(true);
	
	while (true)
		if (checkIfInputIsAvailable(10))
		{
			char input = fgetc(stdin);
			if (input == 6)
				break;
			
			fputc(input, file);
			if (ferror(file) != 0)
			{
				printf("Can't write to the file %s\n", argv[1]);
				break;
			}
		}

	restoreTerminalSettings();
	
	int closeResult = fclose(file);
	if (closeResult != 0)
		printf("Can't close the file %s\n", argv[1]);
}

struct termios oldSettings;

void applyTerminalSettings(bool echo)
{
	struct termios newSettings;
	tcgetattr(fileno(stdin), &oldSettings);
	
	newSettings = oldSettings;
	newSettings.c_lflag &= ~ICANON;
	if (!echo)
		newSettings.c_lflag &= ~ECHO;
	
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