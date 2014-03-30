appserver-coarse.c                                                                                  0000747 0044375 0000145 00000031761 12314422727 014707  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           #include "appserver.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_ARGUMENTS 4
#define ARGUMENT_FORMAT "appserver num_workers num_accounts out_file"
#define MAX_COMMAND_SIZE 200

/*prototype for function that parses command line arguments*/
int arg_parser(int argc, char** argv);

/*prototype for function that sets up bank accounts*/
int account_setup();

/*prototype for function that sets up cmd_buffer*/
int command_buffer_setup();

/*prototype for client loop*/
int client_loop();

/*prototype for function to print incorrect argument format to stderr*/
void incorrect_argument_format();

/*prototype for request handling worker threads*/
void * request_handler();

/*int to mark if the threads should be running or finishing up*/
int running = 1;

/*accounts*/
account * accounts;

/*command buffer*/
LinkedList * cmd_buffer;

/*num_workers and accounts*/
int num_workers;
int num_accounts;

/*lock to make str_tok threadsafe*/
pthread_mutex_t tok_lock;

/*bank lock*/
pthread_mutex_t bank_lock;

/*out file*/
FILE * out_fp;

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
	/*initialize command buffer*/
	cmd_buffer = malloc(sizeof(LinkedList));

	/*parse input arguments*/
	if(arg_parser(argc, argv))
	{
		/*argument error occured*/
		return -1;
	}

	/*initialize account space*/
	accounts = malloc(num_accounts * sizeof(account));

	/*set up bank accounts*/
	if(account_setup())
	{
		/*error encountered during bank account setup*/
		return -1;
	}

	/*set up cmd_buffer*/
	if(command_buffer_setup())
	{
		/*error encountered during command buffer setup*/
		return -1;
	}

	/*main command line loop*/
	if(client_loop())
	{
		/*error encountered during client operations*/
		return -1;
	}

	/*free stuff*/
	free(cmd_buffer);
	free(accounts);
	free_accounts();
	fclose(out_fp);

	return 0;
}

/**function used to parse command line arguments
 * @ret int: 0 = operation success, -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/04/2014*/
int arg_parser(int argc, char** argv)
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
	out_fp = fopen(argv[3], "w");

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
 * @ret int: 0 = operation success -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14*/
int account_setup()
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
 * @ret int: 0 = operation success, -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14 */
int command_buffer_setup()
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
 * @ret int: 0 = op success -1 = failure
 * @author Adam Sunderman
 * @modified 03/24/14 */
int client_loop()
{
	/*pthread workers*/
	pthread_t workers[num_workers];

	/*counter*/
	int i;

	/*size_t used for getline*/
	size_t n = 100;

	/*return value of getline*/
	ssize_t read_size = 0;

	/*entered command*/
	char * command = malloc(MAX_COMMAND_SIZE * sizeof(char));

	/*command ID*/
	int id = 1;

	/*init workers*/
	for(i = 0; i < num_workers; i++)
	{
		pthread_create(&workers[i], NULL, request_handler, NULL);
	}

	pthread_mutex_init(&tok_lock, NULL);
	pthread_mutex_init(&bank_lock, NULL);

	/*client loop*/
	while(1)
	{
		/*read line*/
		read_size = getline(&command, &n, stdin);

		/*null terminate line*/
		command[read_size - 1] = '\0';

		/*check for end*/
		if(strcmp(command, "END") == 0)
		{
			/*mark done*/
			running = 0;

			/*break loop and perform cleanup*/
			break;
		}

		/*print command id*/
		printf("ID %d\n", id);

		/*add command to buffer*/
		add_command(command, id);

		/*incremend id*/
		id++;
	}

	/*clear out command buffer*/
	while(cmd_buffer->size > 0)
	{
		/*sleep to release control to worker thread*/
		usleep(1);
	}

	/*wait for workers to finish*/
	for(i = 0; i < num_workers; i++)
	{
		pthread_join(workers[i], NULL);
	}

	/*free command*/
	free(command);

	/*return successfully*/
	return 0;
}

/**function used to print incorrect argument format to stderr
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

void * request_handler()
{
	/*current command*/
	LinkedCommand cmd;

	/*string of arguments*/
	char ** cmd_tokens = malloc(21 * sizeof(char*));

	/*current argument*/
	char * cur_tok;

	/*number of arguments in command*/
	int num_tokens = 0;

	/*stores account to check*/
	int check_account;

	/*stores amount in check command*/
	int amount;

	/*counters*/
	int i, j;

	/*flag to mark insufficient funds*/
	int ISF = 0;

	/*store timestamp*/
	struct timeval timestamp2;

	/*while main loop is running or buffer isn't empty*/
	while(running || cmd_buffer->size > 0)
	{
		/*if no commands are present then sleep to release
		* control of the thread*/
		while(cmd_buffer->size == 0 && running)
		{
			usleep(1);
		}

		/*get next command*/
		cmd = next_command();

		if(!cmd.command)
		{
			/*if command is NULL we are currently out of commands*/
			continue;
		}

		/*parse command*/
		pthread_mutex_lock(&tok_lock);
		cur_tok = strtok(cmd.command, " ");
		while(cur_tok)
		{
			cmd_tokens[num_tokens] = malloc(21 * sizeof(char));
			strncpy(cmd_tokens[num_tokens], cur_tok, 21);
			num_tokens++;
			cur_tok = strtok(NULL, " ");
		}
		pthread_mutex_unlock(&tok_lock);
		/*execute command*/
		/*is it a CHECK command?*/
		if(strcmp(cmd_tokens[0], "CHECK") == 0 && num_tokens == 2)
		{
			pthread_mutex_lock(&tok_lock);
			check_account = atoi(cmd_tokens[1]);
			pthread_mutex_unlock(&tok_lock);
			pthread_mutex_lock(&bank_lock);
			amount = read_account(check_account);
			pthread_mutex_unlock(&bank_lock);
			gettimeofday(&timestamp2, NULL);
			flockfile(out_fp);
			fprintf(out_fp, "%d BAL %d TIME %d.%06d %d.%06d\n", 
				cmd.id, amount, cmd.timestamp.tv_sec, 
				cmd.timestamp.tv_usec, timestamp2.tv_sec, 
				timestamp2.tv_usec);
			funlockfile(out_fp);
		}
		/*is it a TRANS command*/
		else if(strcmp(cmd_tokens[0], "TRANS") == 0 && num_tokens % 2 
			&& num_tokens > 1)
		{
			/*variables to store transaction info*/
			int num_trans = (num_tokens - 1) / 2;
			int trans_accounts[num_trans];
			int trans_amounts[num_trans];
			int trans_balances[num_trans];
			int temp;

			/*lock bank*/
			pthread_mutex_lock(&bank_lock);

			/*store accounts and transfer amounts*/
			for(i = 0; i < num_trans; i++)
			{
				pthread_mutex_lock(&tok_lock);
				trans_accounts[i] = atoi(cmd_tokens[i*2+1]);
				trans_amounts[i] = atoi(cmd_tokens[i*2+2]);
				pthread_mutex_unlock(&tok_lock);
			}

			/*check all transactions for sufficient funds*/
			for(i = 0; i < num_trans; i++)
			{
				trans_balances[i] = 
					read_account(trans_accounts[i]);
				if(trans_balances[i] + trans_amounts[i] < 0)
				{
					/*if ISF then let program know and
					* print to out file*/
					gettimeofday(&timestamp2, NULL);
					flockfile(out_fp);
					fprintf(out_fp, "%d ISF %d TIME " 
						"%d.06%d %d.06%d\n", 
						cmd.id, trans_accounts[i], 
						cmd.timestamp.tv_sec, 
						cmd.timestamp.tv_usec, 
						timestamp2.tv_sec, 
						timestamp2.tv_usec);
					funlockfile(out_fp);
					ISF = 1;
					break;
				}
			}
			/*if we have sufficient funds*/
			if(!ISF)
			{
				/*execute transactions*/
				for(i = 0; i < num_trans; i++)
				{
					write_account(trans_accounts[i], 
						(trans_balances[i] + 
						trans_amounts[i]));
				}
				/*print transaction success*/
				gettimeofday(&timestamp2, NULL);
				flockfile(out_fp);
				fprintf(out_fp, "%d OK TIME %d.%06d %d.%06d\n", 
					cmd.id, cmd.timestamp.tv_sec, 
					cmd.timestamp.tv_usec, 
					timestamp2.tv_sec, timestamp2.tv_usec);
				funlockfile(out_fp);
			}
			/*unlock accounts*/
			pthread_mutex_unlock(&bank_lock);
		}
		/*invalid command*/
		else
		{
			fprintf(stderr, "%d INVALID REQUEST FORMAT\n", cmd.id);
		}
		/*free command*/
		free(cmd.command);
		for(i = 0; i < num_tokens; i++)
		{
			free(cmd_tokens[i]);
		}
		num_tokens = 0;
		ISF = 0;
	}

	free(cmd_tokens);

	/*return*/
	return;
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
LinkedCommand next_command()
{
	/*temporary pointer used to free head*/
	LinkedCommand * temp_head;

	/*initialize return value*/
	LinkedCommand ret;

	/*lock command buffer*/
	pthread_mutex_lock(&(cmd_buffer->lock));

	/*are there any commands to pull?*/
	if(cmd_buffer->size > 0)
	{
		/*set return value*/
		ret.id = cmd_buffer->head->id;
		ret.command = malloc(MAX_COMMAND_SIZE * sizeof(char));
		ret.timestamp = cmd_buffer->head->timestamp;
		strncpy(ret.command, cmd_buffer->head->command, 
			MAX_COMMAND_SIZE);
		ret.next = NULL;

		/*move head and free memory from linked list*/
		temp_head = cmd_buffer->head;
		cmd_buffer->head = cmd_buffer->head->next;
		free(temp_head->command);
		free(temp_head);

		/*if head ran past tail set tail back to null*/
		if(!cmd_buffer->head)
		{
			cmd_buffer->tail = NULL;
		}

		/*update linked list size*/
		cmd_buffer->size = cmd_buffer->size - 1;
	}
	else
	{
		/*unlock command buffer*/
		pthread_mutex_unlock(&(cmd_buffer->lock));

		ret.command = NULL;

		/*no commands in buffer*/
		return ret;
	}

	/*unlock command buffer*/
	pthread_mutex_unlock(&(cmd_buffer->lock));

	/*return command*/
	return ret;
}

/**function used to add command onto linked list
 * @param LinkedList * command_buffer: command buffer to add command onto
 * @ret int: 0 = operation success -1 = operation failure
 * @author Adam Sunderman
 * @modified 03/24/2014*/
int add_command(char * given_command, int id)
{
	/*initialize new LinkedCommand to add to list*/
	LinkedCommand * new_tail = malloc(sizeof(LinkedCommand));

	/*construct the command*/
	new_tail->command = malloc(MAX_COMMAND_SIZE * sizeof(char));
	strncpy(new_tail->command, given_command, MAX_COMMAND_SIZE);
	new_tail->id = id;
	gettimeofday(&(new_tail->timestamp), NULL);
	new_tail->next = NULL;

	/*lock command_buffer*/
	pthread_mutex_lock(&(cmd_buffer->lock));

	/*is the list currently empty*/
	if(cmd_buffer->size > 0)
	{
		/*have tail point to this new command*/
		cmd_buffer->tail->next = new_tail;

		/*set new tail and increment size of linked list*/
		cmd_buffer->tail = new_tail;
		cmd_buffer->size = cmd_buffer->size + 1;
	}
	else
	{
		/*add first element and set head and tail to it*/
		cmd_buffer->head = new_tail;
		cmd_buffer->tail = new_tail;

		/*size is one*/
		cmd_buffer->size = 1;
	}

	/*unlock command buffer*/
	pthread_mutex_unlock(&(cmd_buffer->lock));

	/*return successfully*/
	return 0;
}
               Bank.c                                                                                              0000777 0044375 0000145 00000001727 12305421425 012275  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           /**  Do not modify this file  **/

#include "Bank.h"
#include <stdlib.h>


int *BANK_accounts;	//Array for storing account values

/*
 *  Intialize back accounts
 *  Input:  int n - Number of bank accounts
 *  Return:  1 if succeeded, 0 if error
 */
int initialize_accounts( int n )
{
	BANK_accounts = (int *) malloc(sizeof(int) * n);
	if(BANK_accounts == NULL) return 0;

	int i;
	for( i = 0; i < n; i++)
	{
		BANK_accounts[i] = 0;
	}
	return 1;
}

/*
 *  Read a bank account
 *  Input:  int ID - Id of bank account to read
 *  Return:  Value of bank account ID
 */
int read_account( int ID )
{
	usleep( 100000 );
	return BANK_accounts[ID - 1];
}

/*
 *  Write value to bank account
 *  Input:  int ID - Id of bank account to write to
 *  Input:  int value - value to write to account
 */
void write_account( int ID, int value)
{
	usleep( 100000 );
	BANK_accounts[ID - 1] = value;
}

/*
 * frees memory allocated for Bank Accounts
 */
void free_accounts()
{
	free(BANK_accounts);
}
                                         appserver.h                                                                                         0000757 0044375 0000145 00000002364 12314361146 013435  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           #ifndef APPSERVER
#define APPSERVER

#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

#ifndef STRING
#define STRING
#include <string.h>
#endif

#ifndef PTHREAD
#define PTHREAD
#include <pthread.h>
#endif

#ifndef BANK
#define BANK
#include "Bank.h"
#endif

#ifndef TIME
#define TIME
#include <sys/time.h>
#endif

/*structure used to store a mutex lock associated with each bank account*/
typedef struct account_struct
{
	pthread_mutex_t lock;
	int value;
}account;

/*structure used to store a command within linked list*/
typedef struct LinkedCommand_struct
{
	char * command;
	int id;
	struct timeval timestamp;
	struct LinkedCommand_struct * next;
	
}LinkedCommand;

/*stucture used to store data about a linked list*/
typedef struct LinkedList_struct
{
	pthread_mutex_t lock;
	LinkedCommand * head;
	LinkedCommand * tail;
	int size;
}LinkedList;

/*function to attempt to lock an account mutex*/
int lock_account(account * to_lock);

/*function to unlock an account mutex*/
int unlock_account(account * to_unlock);

/*function to get next command in linked list*/
LinkedCommand next_command();

/*function used to add command onto linked list*/
int add_command(char * given_command, int id);

#endif
                                                                                                                                                                                                                                                                            Bank.h                                                                                              0000777 0044375 0000145 00000001417 12305421420 012271  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           /**  Do not modify this file  **/

/*
 *  These functions do not provide any error checking.
 *  If an invalid input is supplied the behavior is
 *  undefined.
 */

/*
 *  Intialize n bank accounts with IDs from 1 to n and values of 0.
 *  Input:  int n - Number of bank accounts, must be larger than 0
 *  Return:  1 if succeeded, 0 if error
 */
int initialize_accounts( int n );

/*
 *  Read a bank account
 *  Input:  int ID - Id of bank account to read
 *  Return:  Value of bank account ID
 */
int read_account( int ID );

/*
 *  Write value to bank account
 *  Input:  int ID - Id of bank account to write to
 *  Input:  int value - value to write to account
 */
void write_account( int ID, int value);

/*
 * frees memory allocated for Bank Accounts
 */
void free_accounts();
                                                                                                                                                                                                                                                 makefile                                                                                            0000777 0044375 0000145 00000001234 12314420113 012740  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           #makefile to ensure Project2 is up to date
#usage: simply type "make" to compile all files that you need recompiled
#use make clean to remove .o and executable files.

#compiler to use
CC=gcc

#all exectubles
ALL=appserver appserver-coarse

all: $(ALL)

#executables
appserver: appserver.o Bank.o
	$(CC) -pthread -g -o appserver appserver.o Bank.o
appserver-coarse:appserver-coarse.o Bank.o
	$(CC) -pthread -g -o appserver-coarse appserver-coarse.o Bank.o

#object files
appserver.o: appserver.c
	$(CC) -g -c appserver.c
appserver-coarse.o: appserver-coarse.c
	$(CC) -g -c appserver-coarse.c
Bank.o: Bank.c
	$(CC) -g -c Bank.c

#cleanup files
clean:
	rm -f $(ALL) *.o
                                                                                                                                                                                                                                                                                                                                                                    README                                                                                              0000777 0044375 0000145 00000002307 12305403730 012130  0                                                                                                    ustar   asu1351                         Domain Users                                                                                                                                                                                                           CPRE 308 Project 2: Multithreaded Server

A multithreaded server that manages access to a set of bank accounts. The
program is started with a certain number of accounts in the bank, and user
requests can perform transactions on these accounts, as well as query their
current status. The number of accounts in the bank remains fixed through the
duration of the program.

Each request requires access to multiple records from a database, and also
requires some processing time. Since many user requests may arrive
simultaneously, servicing the requests sequentially, one after another, would
cause a high average latency for the users; servicing the requests in parallel
is required to keep response times small. Thus, this program uses multiple
threads to service the requests simultaneously.

User requests are presented via terminal window. When the user types in a
request, the server records the request and immediately presents the user with a
transaction ID, and then continues to accept more requests. At a later point in
time, when the request has been processed, the results will be written out. To 
avoid interfering with user input, the delayed responses will be written to a
file rather than displayed directly.
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         