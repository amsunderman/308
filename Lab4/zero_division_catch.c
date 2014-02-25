#include <signal.h>
#include <stdio.h>

/*function prototype for division by 0 signal handler*/
void zero_division();

int main()
{
	/*assign zero_division as the action for the signal
 	* SIGFPE (divide by zero) */
	signal(SIGFPE, zero_division);

	/*divide a by 0*/
	int a = 4;
	a = a/0;

	/*return*/
	return 0;
}

/*divide by 0 handler*/
void zero_division()
{
	/*print that we caught signal*/
	printf("caught a SIGFPE\n");
}
