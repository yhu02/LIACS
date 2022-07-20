//Yvo Hu			: 2962802
//Seyed Salqin Zeidi: 2982048

#include "../include/signal_handler.h"

void signalHandlerChild()
{
    int jid = jobs_stack->top;
    pid_t pid = jobs[jid].pid;

    int status;
    waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))	
    {
        deleteJob(pid);
        pop(jobs_stack);

        if (print_prompt_shown == 0)
        {
            shellPrompt();
            print_prompt_shown = 1;
            print_prompt = 0;
        }
    }
}

void signalHandlerInt()
{
    int jid = current_job;
    pid_t pid = jobs[jid].pid;

    if (pid > 0) // Ignore SIGINT if shell is on top of stack
    {
        kill(pid, SIGKILL);
    }
    shellPrompt();
}

void signalHandlerTstp()
{
    int jid = current_job;
    pid_t pid = jobs[jid].pid;

    if (pid > 0) // Ignore SIGTSTP if shell is on top of stack
    {
        jobs[jid].status = 0;
        kill(pid, SIGSTOP);
    }
    showPrompt();
}

void signalHandlerCont()
{
}
