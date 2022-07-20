#ifndef SIGNAL_H
#define SIGNAL_H

//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "./constants.h"
#include "./job.h"
#include "./main.h"

extern int print_prompt;
extern int print_prompt_shown;
extern Job jobs[JOB_LIMIT];
extern int current_job;

void signalHandlerChild();
void signalHandlerInt();
void signalHandlerTstp();
void signalHandlerCont();

#endif