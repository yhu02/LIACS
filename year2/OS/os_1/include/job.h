#ifndef JOB_H
#define JOB_H

//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include <stdio.h>
#include <stdlib.h>
#include "./constants.h"
#include "./stack.h"

typedef struct Job
{
	int pid;
	int status;
} Job;

extern Job jobs[JOB_LIMIT];
extern Stack* jobs_stack;
//List all jobs stopped, and running
void listJobs();

//Delete a job with pid
int deleteJob(int pid);

//Return a Job structure
Job newJob(int pid, int status);

//Move job number in args[1] to background
void bg(char **args);

//Move job number in args[1] to foreground
void fg(char **args);
#endif