#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t child1, child2, child3;
  int status, i, pipefd[4];
  
  char *extrect[] = {"unzip", "campur2.zip", NULL};
  char *grepin[] = {"grep", ".txt$", NULL};
  char *listin[] = {"ls", "/home/agarpac/campur2/", NULL};
  
  pipe(pipefd); // sets up 1st pipe
  pipe(pipefd + 2); // sets up 2nd pipe
	
  child1 = fork();

  if (child1 == 0)
  {
    // this will go first
	execv("/usr/bin/unzip", extrect);
  } 
  else 
  {
	// wait pls
	while ((wait(&status)) > 0);
	
	child2 = fork();
	if (child2 == 0)
	{
		// piping
		
		dup2(pipefd[1], 1);
		
		close(pipefd[0]);
		close(pipefd[1]);
		close(pipefd[2]);
		close(pipefd[3]);	
		
		execv("/bin/ls", listin);
	}
	else
	{
		child3 = fork();
		if (child3 == 0)
		{
			dup2(pipefd[0], 0);
	
			dup2(pipefd[3], 1);
			
			//tutup karena tidak diperlukan
			close(pipefd[0]);
			close(pipefd[1]);
			close(pipefd[2]);
			close(pipefd[3]);
			
			execv("/bin/grep", grepin);
		}
		else
		{
			// piping - keep pipe[2] open
			
			close(pipefd[0]);
			close(pipefd[1]);
			close(pipefd[3]);
			
			char filetxt[1000];
			read(pipefd[2],filetxt,sizeof(filetxt));
			close(pipefd[2]);
			
			FILE *out = fopen("daftar.txt","w+");
			fputs(filetxt, out);
			fclose(out);
		}
	}
  }
  close(pipefd[0]);
  close(pipefd[1]);
  close(pipefd[2]);
  close(pipefd[3]);
}