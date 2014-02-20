#include <stdio.h>

int main(int argc, char argv[])
{
	fork();
	fork();
	usleep(1);
	printf("Process %d's parent process ID is %d\n", getpid(), getppid());
	return 0;
}
