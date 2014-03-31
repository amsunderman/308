/*******************************************************************************
*
* CprE 308 Scheduling Lab
*
* scheduling.c
*******************************************************************************/

#include <stdio.h>
#include <string.h>

#define NUM_PROCESSES 20

struct process
{
  /* Values initialized for each process */
  int arrivaltime;  /* Time process arrives and wishes to start */
  int runtime;      /* Time process requires to complete job */
  int priority;     /* Priority of the process */

  /* Values algorithm may use to track processes */
  int starttime;
  int endtime;
  int flag;
  int remainingtime;
};

/* Forward declarations of Scheduling algorithms */
void first_come_first_served(struct process *proc);
void shortest_remaining_time(struct process *proc);
void round_robin(struct process *proc);
void round_robin_priority(struct process *proc);

int main()
{
  int i;
  struct process proc[NUM_PROCESSES],      /* List of processes */
                 proc_copy[NUM_PROCESSES]; /* Backup copy of processes */

  /* Seed random number generator */
  /*srand(time(0));*/  /* Use this seed to test different scenarios */
  srand(0xC0FFEE);     /* Used for test to be printed out */

  /* Initialize process structures */
  for(i=0; i<NUM_PROCESSES; i++)
  {
    proc[i].arrivaltime = rand()%100;
    proc[i].runtime = (rand()%30)+10;
    proc[i].priority = rand()%3;
    proc[i].starttime = 0;
    proc[i].endtime = 0;
    proc[i].flag = 0;
    proc[i].remainingtime = 0;
  }

  /* Show process values */
  printf("Process\tarrival\truntime\tpriority\n");
  for(i=0; i<NUM_PROCESSES; i++)
    printf("%d\t%d\t%d\t%d\n", i, proc[i].arrivaltime, proc[i].runtime,
           proc[i].priority);

  /* Run scheduling algorithms */
  printf("\n\nFirst come first served\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  first_come_first_served(proc_copy);

  printf("\n\nShortest remaining time\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  shortest_remaining_time(proc_copy);

  printf("\n\nRound Robin\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin(proc_copy);

  printf("\n\nRound Robin with priority\n");
  memcpy(proc_copy, proc, NUM_PROCESSES * sizeof(struct process));
  round_robin_priority(proc_copy);

  return 0;
}

void first_come_first_served(struct process *proc)
{
	/*counters*/
	int i, j;

	/*int to store running total of completion time*/
	int completiontime_running_total = 0;

	/*int to store average completion time*/
	int avg_completion_time;

	/*int to store system time*/
	int sys_time = 0;

	/*keep track of first come*/
	int first_come;

	/*loop through num processes to execute them*/
	for(i = 0; i < NUM_PROCESSES; i++)
	{
		/*initialize first_come*/
		first_come = i;

		/*loop through and find first come*/
		for(j = 0; j < NUM_PROCESSES; j++)
		{
			/*if first_come is already complete (flag = 1) then 
 			* overwrite it*/
			if(proc[first_come].flag)
			{
				first_come = j;
			}

			/*if first_come arrival time is after current process 
 			* and current process hasn't been executed (flag = 0) 
 			* then overwrite it*/
			else if(proc[first_come].arrivaltime > 
				proc[j].arrivaltime && !proc[j].flag)
			{
				first_come = j;
			}
		}

		/*advance system time if it hasn't caught up to arrival time*/
		if(sys_time < proc[first_come].arrivaltime)
			sys_time = proc[first_come].arrivaltime;

		/*execute process*/
		/*set first_come start time*/
		proc[first_come].starttime = sys_time;

		/*advance system time*/
		sys_time += proc[first_come].runtime;

		/*set first_come end time*/
		proc[first_come].endtime = sys_time;

		/*keep track of completion time running total*/
		completiontime_running_total += (proc[first_come].endtime - 
			proc[first_come].arrivaltime);

		/*mark first_come as completed*/
		proc[first_come].flag = 1;

		/*print process star and finish*/
		printf("Process %d started at time %d\n", first_come, 
			proc[first_come].starttime);
		printf("Process %d finished at time %d\n", first_come, 
			proc[first_come].endtime);
	}

	/*calculate average completion time*/
	avg_completion_time = completiontime_running_total/NUM_PROCESSES;

	/*print out average arrival to finish time*/
	printf("Average time from arrival to completion is %d seconds\n", 
		avg_completion_time);
}

void shortest_remaining_time(struct process *proc)
{
  /* Implement scheduling algorithm here */
}

void round_robin(struct process *proc)
{
  /* Implement scheduling algorithm here */
}

void round_robin_priority(struct process *proc)
{
  /* Implement scheduling algorithm here */
}

