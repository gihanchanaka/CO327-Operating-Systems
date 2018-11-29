//
// Created by gihanchanaka on 11/21/18.
//

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
/**
 * Executes the command "cat fixtures | grep <search_term> | cut -b 1-9".
 */

int main(int argc, char **argv)
{
    if(argc!=2){
        printf("ERROR: Worng shell parameter passing\n");
        printf("USAGE: %s searchTerm\n" ,argv[0]);
        return 0;
    }


	int status;
	int i;

	// arguments for commands
	char *cat_args[3];
	char *grep_args[4];
	char *cut_args[4];

	cat_args[0]="cat";cat_args[1]="fixtures";cat_args[2]=NULL;
	grep_args[0]="grep";grep_args[1]="-i";grep_args[2]=argv[1];grep_args[3]=NULL;
	cut_args[0]="cut";cut_args[1]="-b";cut_args[2]="1-9";cut_args[3]=NULL;


    /*printf("%s %s %s \n",cat_args[0],cat_args[1],cat_args[2]);
    printf("%s %s %s %s\n",grep_args[0],grep_args[1],grep_args[2],grep_args[3]);
    printf("%s %s %s %s\n",cut_args[0],cut_args[1],cut_args[2],cut_args[3]);*/

    // make 2 pipes (cat to grep and grep to cut); each has 2 fds

	int pipes[4];

	// sets up 1st pipe
    pipe(pipes);

    // sets up 2nd pipe
    pipe(&pipes[2]);

    // we now have 4 fds:
	// pipes[0] = read end of cat->grep pipe (read by grep)
	// pipes[1] = write end of cat->grep pipe (written by cat)
	// pipes[2] = read end of grep->cut pipe (read by cut)
	// pipes[3] = write end of grep->cut pipe (written by grep)

	// fork the first child (to execute cat)

    //printf("SUCEESS: Parent process work finished\n");

	if (fork() == 0)
	{
        //printf("SUCCESS: Cat process started\n");
        // replace cat's stdout with write part of 1st pipe
        dup2(pipes[WRITE_END],fileno(stdout));

        // close all pipes (very important!); end we're using was safely copied
        close(pipes[READ_END]);
        close(pipes[2+WRITE_END]);
        close(pipes[2+READ_END]);

		// Execute the cat command
        execvp(cat_args[0],cat_args);
        //printf("SUCCESS: Cat process finished\n");
        return 0;
	}
	else
	{
		// fork second child (to execute grep)

		if (fork() == 0)
		{
            //printf("SUCCESS: Grep process started\n");
            wait(NULL);
            // replace grep's stdin with read end of 1st pipe
            dup2(pipes[READ_END],fileno(stdin));

        	// replace grep's stdout with write end of 2nd pipe
            dup2(pipes[2+WRITE_END],fileno(stdout));

            // close all ends of pipes
			close(pipes[WRITE_END]);
			close(pipes[2+READ_END]);

			// execute grep command
			execvp(grep_args[0],grep_args);
            //printf("SUCCESS: grep process finished\n");
            return 0;
		}
		else
		{
			// fork third child (to execute cut)

			if (fork() == 0)
			{

			    //printf("SUCCESS: Cut process started\n");
                wait(NULL);
                // replace cut's stdin with input read of 2nd pipe
				dup2(pipes[2+READ_END],fileno(stdin));

				// close all ends of pipes
				close(pipes[2+WRITE_END]);
                close(pipes[READ_END]);
                close(pipes[WRITE_END]);


				// execute cut command
				execvp(cut_args[0],cut_args);
                //printf("SUCCESS: Cut process finished\n");
                return 0;
			}
		}
	}

	// only the parent gets here and waits for 3 children to finish
	// It's a good idea to close all your pipes (the parent needs none!)
	// before waiting for your children!
    close(pipes[READ_END]);close(pipes[WRITE_END]);
    close(pipes[2+READ_END]);close(pipes[2+WRITE_END]);
    wait(NULL);

    return 0;
}

