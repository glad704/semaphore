#include<stdio.h>
#include<sys/sem.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	int sem_id;	/*ID of the semaphore set.*/
	int i;	/*counter for loop operation.*/
	struct sembuf sop;	/*structure for semaphore ops.*/
	system("clear");
	sem_id=semget(25,1,IPC_CREAT|0666);
	perror("get");
	printf("Semaphore set created, semaphore id '%d'.\n",sem_id);
	sop.sem_num=0;
	sop.sem_op=-1;
	sop.sem_flg=SEM_UNDO;
	semop(sem_id,&sop,1);
	for(i=0;i<=10;i++)
	{
		sleep(1);
		printf("\n pr2 =%d",i);
	}
	return 0;
}
