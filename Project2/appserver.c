#include "appserver.h"

#define NUM_ARGUMENTS 4
#define ARGUMENT_FORMAT "appserver num_workers num_accounts out_file"

/*prototype for function that parses command line arguments*/
int arg_parser(int argc, char** argv, int * num_workers, int * num_accounts, 
	FILE * out_fp);

/*prototype for function that sets up bank accounts*/
int account_setup(account * accounts, int num_accounts);

/*prototype for function that sets up cmd_buffer*/
int command_buffer_setup(LinkedList * cmd_buffer);

/*prototype for client loop*/
int client_loop(int num_workers, int num_accounts, FILE * out_file);

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
	LinkedList * cmd_buffer;

	/*integers to store num_workers and num_accounts*/
	int num_workers, num_accounts;

	/*file pointer to write program output to*/
	FILE * out_fp;

	/*parse input arguments*/
	if(arg_parser(argc, argv, &num_workers, &num_accounts, out_fp))
	{
		/*argument error occured*/
		return -1;
	}

	/*set up bank accounts*/
	if(account_setup(accounts, num_accounts))
	{
		/*error encountered during bank account setup*/
		return -1;
	}

	/*set up cmd_buffer*/
	if(command_buffer_setup(cmd_buffer))
	{
		/*error encountered during command buffer setup*/
		return -1;
	}

	/*main command line loop*/
	if(client_loop(num_workers, num_accounts, out_fp))
	{
		/*error encountered during client operations*/
		return -1;
	}

	/*return successfully*/
	return 0;
}

/**function used to parse command line arguments
 * @param int argc: passed from main to count arguments
 * @param int argv: passed from main to access cmd arguments
 * @param int * num_workers: variable to store argv[1]
 * @param int * num_accounts: variable to store argv[2]
 * @param FILE * out_fp: file pointer to open outfile specified by argv[3]
 * @ret int: 0 = operation success, -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int arg_parser(int argc, char** argv, int * num_workers, int * num_accounts, 
	FILE * out_fp)
{
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
	if(!sscanf(argv[1], "%d", num_workers))
	{
		/*sscanf failed to read an integer*/
		incorrect_argument_format();
		return -1;
	}

	if(!sscanf(argv[2], "%d", num_accounts))
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

/*TODO*/
int account_setup(account * accounts, int num_accounts)
{
	/*TODO*/
}

/*TODO*/
int command_buffer_setup(LinkedList * cmd_buffer)
{
	/*TODO*/
}

/*TODO*/
int client_loop(int num_workers, int num_accounts, FILE * out_file)
{
	/*TODO*/
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

/**function used to attempt to lock an account mutex
 * @param account * to_lock: account structure to attempt to lock
 * @ret int: 0 = operation success; -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int lock_account(account * to_lock)
{
	/*TODO*/
}

/**function to unlock an account mutex
 * @param account * to_unlock: account structure to attempt to unlock
 * @ret int: 0 = operation success; -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int unlock_account(account * to_unlock)
{
	/*TODO*/
}

/**function to get next element in linked list
 * @param LinkedList * command_buffer: linked list to pull from
 * @ret char * command: will be next command in LinkedList
 * @author Adam Sunderman
 * @modified 03/04/2014*/
char * next_command(LinkedList * command_buffer)
{
	/*TODO*/
}

/**function used to add command onto linked list
 * @param LinkedList * command_buffer: command buffer to add command onto
 * @ret int: 0 = operation success -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int add_command(LinkedList * command_buffer)
{
	/*TODO*/
}
