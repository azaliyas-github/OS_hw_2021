#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

void printInfo()
{
	struct timeval rawTime;
	struct timezone timezone;
	gettimeofday(&rawTime, &timezone);
	struct tm currentTime = *localtime(&rawTime.tv_sec);

	printf(
		"PID: %d\n"
		"PID of parent: %d\n"
		"Current time: %i:%i:%i:%li\n\n",
		getpid(),
		getppid(),
		currentTime.tm_hour,
		currentTime.tm_min,
		currentTime.tm_sec,
		rawTime.tv_usec / 1000);
}

void main()
{
	pid_t pid = fork();
	if (pid < 0)
		return;
	
	if (pid > 0)
	{
		pid = fork();
		if (pid < 0)
			return;
		
		printInfo();
		if (pid > 0)
			system("ps -x");
	}
	else
		printInfo();
}