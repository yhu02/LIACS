//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include "../include/main.h"

char **environ;
Job jobs[JOB_LIMIT];
Stack *jobs_stack;
int current_job = -1;
int print_prompt = 0;
int print_prompt_shown = 0;

int executeProgram(char *name, char **argv)
{
	// If name is an absolute or relative path, execute it immediately
	if ((index(name, '/') != 0) || (*name == '.'))
	{
		execve(name, argv, environ);
	}
	// Else look through all PATH environment locations for the file
	else
	{
		char path_new[LINE_BUFFER];
		char *path = getenv("PATH");
		char *path_start = " ";
		int size;

		for (; *path_start != 0; path = path_start + 1)
		{
			//Start looping after every ':' character
			for (path_start = path; (*path_start != ':') && (*path_start != 0); path_start++)
			{
			};
			size = path_start - path;
			(void)strncpy(path_new, path, size);
			//Check if path already ends with /
			if (path_start[-1] != '/')
			{
				path_new[size] = '/';
				size++;
			}
			(void)strcpy(path_new + size, name);

			execve(path_new, argv, environ);
		}
	}
	return -1;
}

void init()
{
	// Check if current process is a tty
	if (isatty(STDIN_FILENO))
	{
		setsid();
		setpgrp();

		signal(SIGCHLD, signalHandlerChild);
		signal(SIGINT, signalHandlerInt);
		signal(SIGTSTP, signalHandlerTstp);
		signal(SIGCONT, signalHandlerCont);

		// Init job list
		for (int i = 0; i < JOB_LIMIT; i++)
		{
			jobs[i] = newJob(-1, -1);
		}
		jobs_stack = createStack(JOB_LIMIT);
		showPrompt();
	}
	else
	{
		printf("Error shell is not interactive\n");
		exit(1);
	}
}

// Show shell hostname
void shellPrompt()
{
	char username[LINE_BUFFER];
	char hostname[LINE_BUFFER];
	char currentDirectory[LINE_BUFFER];

	getlogin_r(username, sizeof(username));
	gethostname(hostname, sizeof(hostname));
	getcwd(currentDirectory, sizeof(currentDirectory));

	printf(GREEN "%s@%s" NONE ":" BLUE "%s" NONE "$ ", username, hostname, currentDirectory);
	fflush(stdout); // flush stdout, otherwise shellprompt doesnt print because no newline
}

// Change directory
void changeDirectory(char *args[])
{
	// Default path
	if (args[1] == NULL)
	{
		chdir(getenv("HOME"));
	}
	else
	{
		// No path
		if (chdir(args[1]) == -1)
		{
			printf("cd: %s: No such file or directory\n", args[1]);
		}
	}
}

// Fork a process
void forkProgram(char **args, char **commands, int background)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		printf("Child process could not be created\n");
		return;
	}
	else if (pid != 0) // Add to job array in parent process
	{
		signal(SIGINT, SIG_IGN);
		int job_added = 0;
		// Place new job;
		for (int i = 0; i < JOB_LIMIT; i++)
		{
			if (jobs[i].pid == -1)
			{
				jobs[i] = newJob(pid, 1);
				push(jobs_stack, i);
				current_job = peek(jobs_stack);
				job_added = 1;
				break;
			}
		}
		if (!job_added)
		{
			printf("Job limit has been reached");
			return;
		}
	}

	// Continue child process
	if (pid == 0)
	{
		// Kill process if command isn't found
		if (executeProgram(args[0], args) == -1)
		{
			printf("Command not found\n");
			freeCommands(commands);
			kill(getpid(), SIGTERM);
		}
	}

	// Wait for process to die if not in background
	if (background == 0)
	{
		waitpid(pid, NULL, 0);
		signal(SIGINT, signalHandlerInt);     //Re enable sigint when return back to parent process
	}
}

// Handle multiple pipelines
void pipeHandler(char **args, char **commands)
{
	if (args == NULL)
	{
		printf("Pipe has no commands");
		return;
	}
	int n = 1;

	// Calculate number of piped commands
	for (int i = 0; args[i] != NULL; i++)
	{
		if (strcmp(args[i], "|") == 0)
		{
			if (i == 0)
			{
				printf("Pipe is missing input file\n");
				return;
			}
			else if (args[i + 1] == NULL)
			{
				printf("Pipe is missing output file\n");
				return;
			}
			n++;
		}
		i++;
	}

	// File descriptors
	int pipe_input[2];
	int pipe_output[2];
	pid_t pid;
	int i = 0;

	// pipes will be set for each command before and after a pipe

	for (int j = 0; args[j] != NULL;)
	{
		char *command[LINE_BUFFER];

		// Split pipe command by | token and execute each command individually
		for (int k = 0;;)
		{
			// No more commands
			if (args[j] == NULL)
			{
				command[k] = NULL;
				break;
			}
			// Pipe found
			else if (strcmp(args[j], "|") == 0)
			{
				command[k] = NULL;
				j++;
				break;
			}
			command[k] = args[j];
			k++;
			j++;
		}

		// 1 Of the pipes will be used in 1 iteration,
		// and the other pipe will be used in the next iteration
		// corresponding with the left and right side of |
		if (i & 1)
		{
			pipe(pipe_input); // for odd i
		}
		else
		{
			pipe(pipe_output); // for even i
		}

		pid = fork();
		if (pid == -1)
		{
			if (i != n - 1)
			{
				// Close input of pipe
				if (i & 1)
				{
					close(pipe_input[1]);
				}
				else
				{
					close(pipe_output[1]);
				}
			}
			return;
		}

		// In child process
		if (pid == 0)
		{
			// First command
			if (i == 0)
			{
				dup2(pipe_output[1], STDOUT_FILENO);
			}
			// Right side of final pipe (last command) to stdout
			else if (i == n - 1)
			{
				if (n & 1)
				{
					dup2(pipe_input[0], STDIN_FILENO);
				}
				else
				{
					dup2(pipe_output[0], STDIN_FILENO);
				}
			}
			// Middle command (in between pipes)
			// Needs 2 iterations for piping output to input
			else
			{
				if (i & 1)
				{
					dup2(pipe_output[0], STDIN_FILENO);
					dup2(pipe_input[1], STDOUT_FILENO);
				}
				else
				{
					dup2(pipe_input[0], STDIN_FILENO);
					dup2(pipe_output[1], STDOUT_FILENO);
				}
			}

			// Kill child if command in pipeline can't be executed
			if (executeProgram(command[0], command) == -1)
			{
				freeCommands(commands);
				kill(getpid(), SIGTERM);
			}
		}
		else
		{
			int status;

			// Terminate the rest of the command if child was killed
			waitpid(pid, &status, 0);
			if (status)
			{
				printf("Error a command in the pipe could not be found\n");
				return;
			}

			closeFD(i, n - 1, pipe_input, pipe_output);
			i++;
		}
	}
}

void closeFD(int i, int last, int pipe1[], int pipe2[])
{
	// Close file descriptors
	// First iteration
	if (i == 0)
	{
		close(pipe2[1]);
	}
	// Last iteration
	else if (i == last)
	{
		if (i & 1)
		{
			close(pipe2[0]);
		}
		else
		{
			close(pipe1[0]);
		}
	}
	else
	{
		if (i & 1)
		{
			close(pipe2[0]);
			close(pipe1[1]);
		}
		else
		{
			close(pipe1[0]);
			close(pipe2[1]);
		}
	}
}

// Handle commands
int commandHandler(char **args, char **commands)
{
	int background = 0;

	if (strcmp(args[0], "exit") == 0)
	{
		freeCommands(commands);
		exit(0);
	}
	else if (strcmp(args[0], "pwd") == 0)
	{
		system("pwd");
	}
	else if (strcmp(args[0], "clear") == 0)
	{
		system("clear");
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		changeDirectory(args);
	}
	else if (strcmp(args[0], "jobs") == 0)
	{
		listJobs();
	}
	else if (strcmp(args[0], "fg") == 0)
	{
		fg(args);
	}
	else if (strcmp(args[0], "bg") == 0)
	{
		bg(args);
	}
	else
	{
		for (int i = 0; args[i] != NULL; i++)
		{
			// Background enabled
			if (strcmp(args[i], "&") == 0)
			{
				if (i == 0)
				{
					printf("Error unknown syntax: &");
					return 0;
				}
				args[i] = NULL;
				background = 1;
				break;
			}
			// Piping detected
			if (strcmp(args[i], "|") == 0)
			{
				pipeHandler(args, commands);
				return 0;
			}
		}
		// Execute program if not piped
		forkProgram(args, commands, background);
	}
	// Dont reprint prompt when executed in background
	return background;
}

// Split a string into tokens delimited by characters in delim,
// And where you also save the characters in keep as a token
char **makeTokens(char line[], int buffer, char *delim, char *keep)
{
	int t = 0;
	char **tokens = malloc(buffer * sizeof(char));

	memset(tokens, '\0', buffer);
	while (1)
	{
		if (*line == '\0')
		{
			tokens[t] = NULL;
			break;
		}
		char *p;

		// Save the characters in keep as a token
		if (strchr(keep, *line) != NULL)
		{
			p = malloc(2);
			memcpy(p, line, 1);
			p[1] = '\0';
			tokens[t] = p;
			t++;
			line++;
			continue;
		}

		// Save the string before the first occurrence of a delim character as a token
		int len = strcspn(line, delim);
		if (len > 0)
		{
			p = malloc(len + 1);

			if (p != NULL)
			{
				memcpy(p, line, len);
				p[len] = '\0';

				tokens[t] = p;
				t++;
			}
			line += len;
		}
		else
		{
			line++;
		}
	}
	return tokens;
}

// Free memory
void freeCommands(char **commands)
{
	for (int i = 0; commands[i] != NULL; i++)
	{
		free(commands[i]);
	}

	free(commands);
}

void showPrompt()
{
	// Read user input and show prompt
	while (1)
	{
		char line[LINE_BUFFER]; // buffer for the user input
		char **commands;		// array for the different tokens in the command

		memset(line, '\0', LINE_BUFFER); // Reset line buffer

		if (print_prompt == 0) // print hostname when needed
		{
			shellPrompt();
			print_prompt_shown = 1;
		}
		else if (print_prompt == 1) // Reset
		{
			print_prompt = 0;
		}

		fgets(line, LINE_BUFFER, stdin); // Get input

		commands = makeTokens(line, LINE_BUFFER, " |&;\n\t\0", ";|&");

		// Handle EOF Ctrl+D
		if (feof(stdin))
		{
			freeCommands(commands);
			exit(1);
		}

		// Continue if command is empty
		if (commands[0] == NULL)
		{
			free(commands);
			continue;
		}

		// Separate commands by ; token
		for (int current = 0;;)
		{
			char *command[LINE_BUFFER];
			for (int i = 0;; i++)
			{
				// End reached by either NULL or ;. Remove NULL or ; and treat as separate command
				if (commands[i + current] == NULL || strcmp(commands[i + current], ";") == 0)
				{
					command[i] = NULL;
					if (i == 0)
					{
						current += 1;
					}
					else
					{
						print_prompt = commandHandler(command, commands);
						current += i;
					}
					break;
				}
				else
				{
					command[i] = commands[i + current];
					// End reached by &. Enable background execution
					if (strcmp(commands[i + current], "&") == 0)
					{
						print_prompt = commandHandler(command, commands);
						current += i + 1;
						break;
					}
				}
			}

			// Last command reached exit
			if (commands[current] == NULL)
			{
				freeCommands(commands);
				break;
			}
		}
	}
}

int main(int argc, char *argv[], char **envp)
{
	(void)(argc || argv);

	environ = envp;
	init();

	return 0;
}
