
#ifndef MAIN_H
#define MAIN_H

//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "../include/constants.h"
#include "../include/job.h"
#include "../include/signal_handler.h"
#include "../include/main.h"
#include "../include/stack.h"

void showPrompt();
void shellPrompt();
void changeDirectory(char **args);
void freeCommands(char **commands);
int executeProgram(char *name, char **argv);
void closeFD(int i, int last, int pipe1[], int pipe2[]);
void forkProgram(char **args, char **commands, int background);
void pipeHandler(char **args, char **commands);
void init();

#endif