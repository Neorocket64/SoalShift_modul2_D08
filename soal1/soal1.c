#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>

int main()
{
	pid_t pid, sid;

	pid = fork();

	if (pid < 0)
	{
		exit(EXIT_FAILURE);
	}

	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();

	if (sid < 0)
	{
		exit(EXIT_FAILURE);
	}

	if ((chdir("/")) < 0)
	{
		exit(EXIT_FAILURE);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	while(1)
	{
		// main program here
		
		DIR *d;
		struct dirent *dir;
		char dir_o[100] = "/home/agarpac/Pictures/", 
			dir_n[100] = "/home/agarpac/modul2/gambar/", 
			dir_e1[100], dir_e2[100],
			replacement[10] = "_grey", 
			extension[5] = ".png",
			*attach, *fname;
		d = opendir("/home/agarpac/Pictures");
		if (d)
		{
			while ((dir = readdir(d)) != NULL)
			{
				attach = strstr(dir->d_name, ".png");
				if(attach != NULL)
				{
					strcpy( dir_e1, dir_o );
					strcat( strcat( dir_e2, strtok( dir->d_name, "." ) ), extension );
					strcpy( dir_e2, dir_n );
					strcat( strcat ( strcat( dir_e2, strtok( dir->d_name, "." ) ), replacement ), extension );
					printf("%s\n", dir_e2);
					//printf(" old : %s\n new : %s\n", dir_e1, dir_e2);
					rename(dir_e1, dir_e2);
				}
			}
			closedir(d);
		}

		sleep(1);
	}

	exit(EXIT_SUCCESS);
}
