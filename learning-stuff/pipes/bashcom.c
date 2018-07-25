#include "../include/bashcom.h"

int bashcommand(char *command)
{
	int fd[2];
	int i;
	int size = 0;
	pid_t pid;
	pipe(fd);

	int cmdpos[32];
	int cmdposcar=0;
	char **cmd_tokens = malloc(size * sizeof(char**));
	scanf("%[^\n]s", command);
	char *token;
	token = strtok(command, " ");
	if(token != NULL) {
    	cmdpos[cmdposcar]=size;
    	cmdposcar++;
	}
    while(token != NULL)
    {
    	size++;
    	cmd_tokens = realloc(cmd_tokens, size * sizeof(char**));
    	if(strcmp(token, "|") == 0) {
    		cmd_tokens[size-1] = NULL;
    		cmdpos[cmdposcar]=size;
    		cmdposcar++;
    	}
    	else
	    	cmd_tokens[size-1] = token;
    	token = strtok(NULL, " ");
    }
	size++;
    cmd_tokens = realloc(cmd_tokens, size * sizeof(char**));
    cmd_tokens[size-1] = NULL;

    if(cmdposcar == 1) //if single command
    {
    	if(pid = fork() == 0)
			execvp(cmd_tokens[0], cmd_tokens);
		else
			waitpid(pid, NULL, 0);
    }
    else
    {
	    int def_inpipe = dup(0);
	    int def_outpipe = dup(1);
	    char *path = malloc(1024);

	    for(i = 0; i < cmdposcar-1; i++)
	    {
	    	sprintf(path, "%s%d", ".pipe", i);
			mkfifo(path, S_IRWXU);
	    }

	    for(i = 0; i < cmdposcar; i++)
	    {
	    	if(pid = fork() == 0)
			{
				int fd_w, fd_r;

				sprintf(path, "%s%d", ".pipe", i);
				fd_w = open(path, O_WRONLY);
				sprintf(path, "%s%d", ".pipe", i-1);
				fd_r = open(path, O_RDONLY);

				if(fd_w >= 0)
					dup2(fd_w, 1);
				else
					dup2(def_outpipe, 1);
				if(fd_r >= 0)
					dup2(fd_r, 0);
				else
					dup2(def_inpipe, 0);
				
				execvp(cmd_tokens[cmdpos[i]], &cmd_tokens[cmdpos[i]]); //pointer to cmd_tokens[] position in **cmd_tokens
			}
	    }
	    while(wait(NULL)>0);

	    for(i = 0; i < cmdposcar-1; i++)
	    {
	    	sprintf(path, "%s%d", ".pipe", i);
	    	unlink(path);
	    }
	    free(command);
	    free(cmd_tokens);
	    free(path);
	    dup2(def_inpipe, 0);
	    dup2(def_outpipe, 1);
	    close(def_inpipe);
	    close(def_outpipe);
	    getchar();
	}
	return 0;
}