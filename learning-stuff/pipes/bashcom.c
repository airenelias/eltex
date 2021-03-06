#include "bashcom.h"

int bashcommand(char *command)
{
	int res = 0;
	int i;
	int size = 0;
	pid_t pid;

	int cmdpos[BUF_MAX_COM];
	int cmdposcar=0;
	char **cmd_tokens = malloc(size * sizeof(char**));
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
    	if(pid = fork() == 0) {
			execvp(cmd_tokens[0], cmd_tokens);
			return -1; //if failed
    	}
		else
			waitpid(pid, NULL, 0);
    }
    else
    {
	    int def_inpipe = dup(0);
	    int def_outpipe = dup(1);
	    char *path = malloc(BUF_PATH_SIZE);
	    //int pipes[cmdposcar-1][2];

	    for(i = 0; i < cmdposcar-1; i++)
	    {
	    	//pipe(pipes[i]);
	    	sprintf(path, "%s%d", ".pipe", i);
			mkfifo(path, S_IRWXU);
	    }

	    for(i = 0; i < cmdposcar; i++)
	    {
	    	if(pid = fork() == 0)
			{
				/*if(i == 0)
				{
					dup2(def_inpipe, 0);
					dup2(pipes[i][1], 1);
				}
				else if(i == cmdposcar - 1)
				{
					dup2(pipes[i-1][0], 0);
					dup2(def_outpipe, 1);
				}
				else
				{
					dup2(pipes[i-1][0], 0);
					dup2(pipes[i][1], 1);
				}*/

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

				execvp(cmd_tokens[cmdpos[i]], &cmd_tokens[cmdpos[i]]) < 0; //pointer to cmd_tokens[] position in **cmd_tokens
				return -1; //if failed
			}
	    }
	    while(wait(NULL)>0);

	    for(i = 0; i < cmdposcar-1; i++)
	    {
	    	//sprintf(path, "%s%d", ".pipe", i);
	    	//unlink(path);
	    }
	    free(command);
	    free(cmd_tokens);
	    //free(path);
	    dup2(def_inpipe, 0);
	    dup2(def_outpipe, 1);
	    close(def_inpipe);
	    close(def_outpipe);
	}
	getchar();
	return 0;
}