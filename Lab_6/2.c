#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

void printInfo()
{
	printf("PID: %d\n", getpid());
	printf("PID of parent: %d\n", getppid());
	
	struct timeval rawTime;
	struct timezone timezone;
	gettimeofday(&rawTime, &timezone);
	struct tm currentTime = *localtime(&rawTime.tv_sec);
	printf(
		"Current time: %i:%i:%i:%li\n\n",
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
		if (pid > 0) {
			wait(NULL);
			system("ps -x");
		}
	}
	else
		printInfo();
}