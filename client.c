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
	int seat_number;
	ticket *temp;

	if ((semid = semget (SEM_ID, 1, PERMS | IPC_CREAT)) < 0)
		sys_err ("server: can not create semaphore");
	
	if ((shmid = shmget (SHM_ID, sizeof(temp), PERMS|IPC_CREAT)) < 0)
		sys_err ("client: can not get shared memory segment");

	if ((temp = (ticket *) shmat(shmid, 0, 0)) == NULL)
		sys_err ("client: shared memory attach error");
  
	printf("Available seats in a non-smoking room:\n");
  
	for (int i = 0; i < n; i++)
	{
		if (temp[i].buy == 0)
		printf("Seat # %d\n", temp[i].num);
	}
	
	scanf("Seat number: %s", seat_number);
	
	while (semctl (semid, 0, GETVAL, 0))
		
	semctl (semid, 0, SETVAL, 1);
	
	temp[seat_number].buy = 1;
	
	semctl (semid, 0, SETVAL, 0);
	
	shmdt (temp);
	
	exit (0);
}