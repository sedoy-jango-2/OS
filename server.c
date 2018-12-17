#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_ID	2002

typedef struct
{
	int num;
	int smoke;
	int buy;
} ticket;

void sys_err (char *msg)
{
	puts (msg);
	exit (1);
}

int main(int argc, char **argv)
{
	int shmid; 
	ticket *temp;           

	if ((shmid = shmget (SHM_ID, sizeof(temp), 0666|IPC_CREAT)) < 0)
		sys_err ("client: can not get shared memory segment");

	if ((temp = (ticket *) shmat(shmid, 0, 0)) == NULL)
		sys_err ("client: shared memory attach error");

	temp->num = 0;
	temp->smoke = 0;
	temp->buy = 0;
  
	shmdt (temp);
	exit (0);
}
