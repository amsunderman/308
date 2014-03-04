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

/*structure used to store data in a linked list*/
typedef struct LinkedList_struct
{
	char * command;
	struct LinkedList_struct * next;
}LinkedList;

/*function to attempt to lock an account mutex*/
int lock_account(account * to_lock);

/*function to unlock an account mutex*/
int unlock_account(account * to_unlock);

/*function to get next element in linked list*/
LinkedList * next(LinkedList * current);

/*function used to search a linked list for a specific value*/
LinkedList * search(LinkedList * head, int value);

#endif
