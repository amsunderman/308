#include "appserver.h"

#define NUM_ARGUMENTS 4
#define ARGUMENT_FORMAT "appserver num_workers num_accounts out_file"
#define MAX_COMMAND_SIZE 100

/*prototype for function that parses command line arguments*/
int arg_parser(int argc, char** argv, int * num_workers, int * num_accounts, 
	FILE * out_fp);

/*prototype for function that sets up bank accounts*/
int account_setup(account * accounts, int num_accounts);

/*prototype for function that sets up cmd_buffer*/
int command_buffer_setup(LinkedList * cmd_buffer);

/*prototype for client loop*/
int client_loop(LinkedList * cmd_buffer, int num_workers, int num_accounts, 
	FILE * out_file);

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
	LinkedList * cmd_buffer = malloc(sizeof(LinkedList));

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

	/*initialize account space*/
	accounts = malloc(num_accounts * sizeof(account));

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
	if(client_loop(cmd_buffer, num_workers, num_accounts, out_fp))
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

/*function used to set up accounts
 * @param account * accounts: account array to store locks
 * @param int num_accounts: number of accounts to initialize
 * @ret int: 0 = operation success -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14*/
int account_setup(account * accounts, int num_accounts)
{
	/*counter*/
	int i;

	/*initialize BANK_accounts*/
	initialize_accounts(num_accounts);

	/*loop through num_accounts and create accounts for each*/
	for(i = 0; i < num_accounts; i++)
	{
		pthread_mutex_init(&(accounts[i].lock), NULL);
		accounts[i].value = 0;
	}

	return 0;
}

/*Function used to initialize command buffer
 * @param LinkedList * cmd_buffer: list to be initialized.
 * @ret int: 0 = operation success, -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14 */
int command_buffer_setup(LinkedList * cmd_buffer)
{
	/*initialize command buffer*/
	pthread_mutex_init(&(cmd_buffer->lock), NULL);
	cmd_buffer->head = NULL;
	cmd_buffer->tail = NULL;
	cmd_buffer->size = 0;

	/*return successfully*/
	return 0;
}

/*function that loops and executes commands as threads until the exit
 * function is called. After exit is called, the rest of the commands are ran
 * and the threads are joined before the function returns
 * @param LinkedList * cmd_buffer: command buffer
 * @param int num_workers: number of worker threads to use
 * @param int num_accounts: number of accounts in bank
 * @param FILE * out_file: file to write transactions to
 * @ret int: 0 = op success -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14 */
int client_loop(LinkedList * cmd_buffer, int num_workers, int num_accounts, 
	FILE * out_file)
{
	/*TODO*/

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

/**function used to attempt to lock an account mutex
 * @param account * to_lock: account structure to attempt to lock
 * @ret int: 0 = operation success; -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/24/2014*/
int lock_account(account * to_lock)
{
	if(pthread_mutex_trylock(&(to_lock->lock)))
	{
		/*lock failed return -1*/
		return -1;
	}

	/*locked, return success*/
	return 0;
}

/**function to unlock an account mutex
 * @param account * to_unlock: account structure to attempt to unlock
 * @ret int: 0 = operation success; -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/24/2014*/
int unlock_account(account * to_unlock)
{
	/*unlock account*/
	pthread_mutex_unlock(&(to_unlock->lock));

	/*return successfully*/
	return 0;
}

/**function to get next element in linked list
 * @param LinkedList * command_buffer: linked list to pull from
 * @ret char * command: will be next command in LinkedList (NULL if no command 
 * exists)
 * @author Adam Sunderman
 * @modified 03/24/2014*/
char * next_command(LinkedList * command_buffer)
{
	/*temporary pointer used to free head*/
	LinkedCommand * temp_head;

	/*initialize return value*/
	char * ret = malloc(MAX_COMMAND_SIZE * sizeof(char));

	/*lock command buffer*/
	pthread_mutex_lock(&(command_buffer->lock));

	/*are there any commands to pull?*/
	if(command_buffer->size > 0)
	{
		/*set return value*/
		strncpy(ret, command_buffer->head->command, MAX_COMMAND_SIZE);

		/*move head and free memory from linked list*/
		temp_head = command_buffer->head;
		command_buffer->head = command_buffer->head->next;
		free(temp_head->command);
		free(temp_head);

		/*if head ran past tail set tail back to null*/
		if(!command_buffer->head)
		{
			command_buffer->tail = NULL;
		}

		/*update linked list size*/
		command_buffer->size = command_buffer->size - 1;
	}
	else
	{
		/*unlock command buffer*/
		pthread_mutex_unlock(&(command_buffer->lock));

		/*no commands in buffer*/
		return NULL;
	}

	/*unlock command buffer*/
	pthread_mutex_unlock(&(command_buffer->lock));

	/*return command*/
	return ret;
}

/**function used to add command onto linked list
 * @param LinkedList * command_buffer: command buffer to add command onto
 * @ret int: 0 = operation success -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/24/2014*/
int add_command(LinkedList * command_buffer, char * given_command)
{
	/*initialize new LinkedCommand to add to list*/
	LinkedCommand * new_tail = malloc(sizeof(LinkedCommand));

	/*construct the command*/
	new_tail->command = malloc(MAX_COMMAND_SIZE * sizeof(char));
	strncpy(new_tail->command, given_command, MAX_COMMAND_SIZE);
	new_tail->next = NULL;

	/*lock command_buffer*/
	pthread_mutex_lock(&(command_buffer->lock));

	/*is the list currently empty*/
	if(command_buffer->size > 0)
	{
		/*have tail point to this new command*/
		command_buffer->tail->next = new_tail;

		/*set new tail and increment size of linked list*/
		command_buffer->tail = new_tail;
		command_buffer->size = command_buffer->size + 1;
	}
	else
	{
		/*add first element and set head and tail to it*/
		command_buffer->head = new_tail;
		command_buffer->tail = new_tail;

		/*size is one*/
		command_buffer->size = 1;
	}

	/*unlock command buffer*/
	pthread_mutex_unlock(&(command_buffer->lock));

	/*return successfully*/
	return 0;
}
