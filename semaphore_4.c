#include<stdio.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SEMNAME "/tmp/license"
#define N 8

union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
};

int acquire(int);
int release(int);

int main()
{
	int key,id,i,status;
	int pid,num_waiting;
	int uid=getuid();
	union semun argument;

	/*Only 3 processes can execute the program at a time.*/
	argument.val=3;

	if((open(SEMNAME,O_CREAT))<0)
	{
		perror("Could not create file\n");
	}

	if((key=ftok(SEMNAME,uid))<0)
	{
		perror("ftok");
		exit(1);
	}

	id=semget(key,1,IPC_CREAT|0666);
	if(id<0)
	{
		fprintf(stderr,"Unable to obtain semaphore\n");
	}

	/*Note that only one process initializes the semaphore*/
	if(semctl(id,0,SETVAL,argument)<0)
	{
		fprintf(stderr,"Cannot set semaphore value.\n");
	}
	else
	{
		fprintf(stderr,"Semaphore %d initialized.\n",key);
	}

	for(i=0;i<N;i++)
	{
		if((pid=fork())==0)
		{
			if((acquire(id))==0)
			{
				execlp("date","date",0);
			}
			else
			{
				exit(1);
			}
		}
		else
		{
			num_waiting=semctl(id,0,GETNCNT);
			printf("There are %d processes waiting for the license\n",num_waiting);
		}
	}

	while((pid=wait(&status))>0)
	{
		if((release(id))<0)
		{
			printf("Error: could not release license for %d\n",pid);
		}
		else
		{
			printf("Process %d no longer holds license\n",pid);
		}
		num_waiting=semctl(id,0,GETNCNT);
		printf("Now there are %d processes waiting for the license\n",num_waiting);
	}

	/*Remove the semaphore*/
	if((semctl(id,0,IPC_RMID))<0)
	{
		perror("semctl");
	}
	else
	{
		printf("Removed semaphore\n");
	}

	/*Remove the file*/
	if((unlink("/tmp/license"))<0)
	{
		perror("Remove File");
		exit(1);
	}

	return (0);
}

int acquire(int id)
{
	int retval;
	struct sembuf operations[1];

	/*Set up the sembuf structure*/
	operations[0].sem_num=0;	/*index into semaphore array*/
	operations[0].sem_op=-1;
	operations[0].sem_flg=0;	/*wait if semaphore is <=0*/

	retval=semop(id,operations,1);

	if(retval!=0)
	{
		printf("acquire operation did not succeed\n");
		perror("REASON");
	}
	return retval;
}

int release(int id)
{
	int retval;
	struct sembuf operations[1];

	/*Set up the sembuf structure*/
	operations[0].sem_num=0;
	operations[0].sem_op=-1;
	operations[0].sem_flg=0;

	retval=semop(id,operations,1);

	if(retval!=0)
	{
		printf("Release did not succeed.\n");
		perror("REASON");
	}
	return retval;
}
