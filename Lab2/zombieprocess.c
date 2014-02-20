#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv)
{
	pid_t child;

	child = fork();

	if(child == 0)
	{
		return 0;
	}
	else
	{
		sleep(60);
		wait();
		return 0;
	}
}
