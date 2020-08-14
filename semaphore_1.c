#include<stdio.h>
#include<sys/sem.h>
#include<stdlib.h>

int main()
{
	int sem_id;	/*ID of the semaphore set*/
	int i,j;	/*counter for loop operation*/
	struct sembuf sop;	/*structure for semaphore ops.*/
	int rc;
	system("clear");
	sem_id=semget(25,1,IPC_CREAT|0666);
	perror("get");
	printf("Semaphore set created, id '%d'.\n",sem_id);
	if(semctl(sem_id,0,SETVAL,1)==-1)
	{
		perror("ctl");
	}
	return 0;
}
