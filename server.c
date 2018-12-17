#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define SHM_ID  2002
#define SEM_ID	2001
#define PERMS	0666

typedef struct
{
	int num;
	int smoke;
	int buy;
}ticket;

void sys_err (char *msg)
{
	puts (msg);
	exit (1);
}

int main(int argc, char **argv)
{
	int shmid, semid;
	int n = 7;
	ticket *temp, *temp_local;
	
	if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
		sys_err ("server: can not create semaphore");
	
	if ((shmid = shmget (SHM_ID, sizeof(temp), PERMS|IPC_CREAT)) < 0)
		sys_err ("client: can not get shared memory segment");

	if ((temp = (ticket *) shmat(shmid, 0, 0)) == NULL)
		sys_err ("client: shared memory attach error");
	
	semctl (semid, 0, SETVAL, 1); /* установить блокировку */
	
	for (int i = 0; i < n; i++)
	{
		temp[i].num = temp_local[i].num = i;
		temp[i].smoke = temp_local[i].smoke = 0;
		temp[i].buy = temp_local[i].buy = 0;
	}
	
	semctl (semid, 0, SETVAL, 0); /* снять блокировку */
        
	loop:
		for (int i = 0; i < n; i++)
		{
			if (temp[i].buy != temp_local[i].buy)
			{
				printf("Куплен билет № %d\n", i);
				temp_local[i].buy = temp[i].buy;
			}
		}
	goto loop;
	
	shmdt (temp);
	exit (0);
}