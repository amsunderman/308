#include "appserver.h"

#define NUM_ARGUMENTS 4
#define ARGUMENT_FORMAT "appserver num_workers num_accounts out_file"

/*prototype for function to print incorrect argument format to stderr*/
void incorrect_argument_format();

/**program used to initiate bank account server and take requests. will output
 * request results to file provided as arv[3]
 * @param argv[1]: integer that represents number of working threads
 * @param argv[2]: integer that represents number of accounts
 * @param argv[3]: string that represents name of output file
 * @ret int: 0 = operation success, -1 = error encountered
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int main(int argc, char** argv)
{
	/*pointer to typedef'd account structure to store locks for all
 	* accounts*/
	account * accounts;

	/*typedef'd linked list for command buffer*/
	LinkedList cmd_buffer;

	/*integers to store num_workers and num_accounts*/
	int num_workers, num_accounts;

	/*file pointer to write program output to*/
	FILE * out_fp;

	/*parse input arguments*/

	/*check for correct number of arguments*/
	if(argc != NUM_ARGUMENTS)
	{
		fprintf(stderr, "error (appserver): incorrect # of command " 
			"line arguments\n");
		fprintf(stderr, "appserver expected format: ");
		fprintf(stderr, ARGUMENT_FORMAT);
		fprintf(stderr, "\nappserver: exiting program\n");
		return -1;
	}

	/*store arguments*/
	if(!sscanf(argv[1], "%d", &num_workers))
	{
		/*sscanf failed to read an integer*/
		incorrect_argument_format();
		return -1;
	}

	if(!sscanf(argv[2], "%d", &num_accounts))
	{
		/*sscanf failed to read an integer*/
		incorrect_argument_format();
		return -1;
	}

	/*open output file*/
	out_fp = fopen(argv[3], "w+");

	if(!out_fp)
	{
		/*failed to open file*/
		fprintf(stderr, "error (appserver): failed to open out_file\n");
		fprintf(stderr, "appserver: exiting program\n");
		return -1;
	}

	/*return successfully*/
	return 0;
}

/**function used to print incorrect argument format to stderr
 * @param void
 * @ret void
 * @author Adam Sunderman
 * @modified 03/04/2014*/
void incorrect_argument_format()
{
	fprintf(stderr, "error(appserver): incorrect argument format\n");
	fprintf(stderr, "appserver expected format: ");
	fprintf(stderr, ARGUMENT_FORMAT);
	fprintf(stderr, "\nappserver: exiting program\n");
}
