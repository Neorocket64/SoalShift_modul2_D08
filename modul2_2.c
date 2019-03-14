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
#include <pwd.h>

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
		struct stat buf;
		struct passwd *p;
		char *attach;
		
		static const char tempat[] = "/home/agarpac/hatiku/", pembanding[] = "www-data";
		//memory allocation needed
		size_t dlen = strlen(tempat) + 1;
		
		if ((d = opendir(tempat)) != 0)
		{
			while ((dir = readdir(d)) != NULL)
			{
				//check if contains elen.ku (is this neccessary?)
				attach = strstr(dir->d_name, "elen.ku");
				if(attach != NULL)
				{
					//this is where allocation starts to roll!
					char *str = malloc(dlen + strlen(dir->d_name));
					if (str != 0)
					{
						strcpy(str, tempat);
						strcat(str, dir->d_name);
						if (stat(str, &buf) == 0)
						{
							p = getpwuid(buf.st_uid);
							if( p != NULL)
							{
								//comparing username to www-data never been so complicated (i blame errors)
								char uname[100] = "";
								stpcpy(uname, p->pw_name);
								if(strcmp(uname, pembanding) == 0)
								{
									remove(str);
									if (chmod (str, 777) < 0) printf("error in chmod");
									else 
									{
										remove(str);
									}
								}
							}
						}
					}
				}
			}
			closedir(d);
		}

		sleep(3);
	}

	exit(EXIT_SUCCESS);
}
