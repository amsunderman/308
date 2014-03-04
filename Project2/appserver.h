#ifndef APPSERVER
#define APPSERVER

#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

#ifndef PTHREAD
#define PTHREAD
#include <pthread.h>
#endif

#ifndef BANK
#define BANK
#include "Bank.h"
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
char * next_command(LinkedList * command_buffer);

/*function used to add command onto linked list*/
int add_command(LinkedList * command_buffer);

#endif
