//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include "../include/job.h"

void listJobs()
{
	for (int i = 0; i < JOB_LIMIT; i++)
	{
		// 0 = no job
		// 1 = running,
		// 2 = stopped
		switch (jobs[i].status)
		{
		case (-1):
			break;
		case (0):
			printf("[%d] Stopped %d\n", i, jobs[i].pid);
			break;
		case (1):
			printf("[%d] Running %d\n", i, jobs[i].pid);
			break;
		default:
			printf("EXiT %d", jobs[i].status);
			exit(1);
		}
	}
}

Job newJob(int pid, int status)
{
	Job new_job;
	new_job.pid = pid;
	new_job.status = status; //-1 = inactive, 0 = stopped, 1 = running

	return new_job;
}

// Delete a job from the array. 0 = fail, 1 = success
int deleteJob(int pid)
{
	for (int i = 0; i < JOB_LIMIT; i++)
	{
		if (jobs[i].pid == pid)
		{
			jobs[i] = newJob(-1, -1);
			return 1;
		}
	}
	return 0;
}

void fg(char **args)
{
	int i = 0;
	while (args[i] != 0)
	{
		i++;
	}

	if (i >= 3)
	{
		printf("fg has too many arguments");
	}
	else if (i == 1)
	{
		printf("fg is missing an argument");
	}
	else if (i == 2)
	{
		// to be implemented... or not
	}
}

void bg(char **args)
{
	int i = 0;
	while (args[i] != 0)
	{
		i++;
	}

	if (i >= 3)
	{
		printf("bg has too many arguments\n");
	}
	else if (i == 1)
	{
		printf("bg is missing an argument\n");
	}
	else if (i == 2)
	{
		int jid = atoi(args[1]);

		if (jobs[jid].pid != -1)
		{
			printf("success\n");
			jobs[jid].status = 1;
		}
		else
		{
			printf("Error job %d doesnt exist\n", jid);
		}
		//signal(SIGINT, signalHandlerInt); // Re enable SIGINT for parent process
	}
}