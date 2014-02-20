#include <stdio.h>
#include <pthread.h>

/*function prototype for thread 1 to run*/
void * thread1();

/*function prototype for thread 2 to run*/
void * thread2();

/*creates two threads and runs two correlating functions*/
int main(int argc, char* argv)
{
	/*declare thread variables*/
	pthread_t i1;
	pthread_t i2;

	/*creation of threads 1 and 2*/
	pthread_create(&i1, NULL, thread1, NULL);
	pthread_create(&i2, NULL, thread2, NULL);

	/*wait for threads 1 and 2 to complete*/
	pthread_join(i1, NULL);
	pthread_join(i2, NULL);

	/*identify self as main process*/
	printf("Hello, I am the main process\n");

	return 0;
}

/*identifies self as thread 1*/
void * thread1()
{
	printf("Hello, I am thread 1\n");
}

/*identifies self as thread 2*/
void * thread2()
{
	printf("Hello, I am thread 2\n");
}
