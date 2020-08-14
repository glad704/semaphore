#include<stdio.h>
#include<sys/sem.h>
#include<unistd.h>

int main()
{
	int sem_id,i;	/*ID of semaphore set*/
	struct sembuf sop;	/*structure for semaphore ops*/
	sem_id=semget(10,1,IPC_CREAT|0666);
	perror("get");
	printf("Semaphore set created, Semaphore setid '%d'.\n",sem_id);
	if(semctl(sem_id,0,SETVAL,1)==-1)
	perror("semctl");
	if(fork()==0)
	{
		sop.sem_num=0;
		sop.sem_op=-1;
		sop.sem_flg=SEM_UNDO;
		semop(sem_id,&sop,1);
		for(i=1;i<=500;i++)
		{
			sleep(1);
			printf("child------%d",i);
		}
	}
	else
	{
		sop.sem_num=0;
		sop.sem_op=-1;
		sop.sem_flg=SEM_UNDO;
		semop(sem_id,&sop,1);
		for(i=1;i<=500;i++)
		{
			sleep(1);
			printf("parent------%d",i);
		}
	}
	return 0;
}
