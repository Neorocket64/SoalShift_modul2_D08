#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define PANJANG 200
#define NOL 0

char *namafolder(int x)
{
    time_t wktu = time(NULL);
    struct tm waktu = *localtime(&wktu);

    static char dest[PANJANG];
    memset(&dest, 0, sizeof(dest));

    static char nama[PANJANG];
    memset(&nama, 0, sizeof(nama));

    if(x == 1)
    {
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_mday, dest, ":");
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_mon + 1, dest, ":");
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_year + 1900, dest, "-");

        if(waktu.tm_hour < 10)
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%d%s", NOL, dest, waktu.tm_hour, dest);
        }
        else
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s", waktu.tm_hour, dest);
        }
        strcat(nama, ":");

        if(waktu.tm_min < 10)
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%d%s", NOL, dest, waktu.tm_min, dest);
        }
        else
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s", waktu.tm_min, dest);
        }
    }
    else
    {
        waktu.tm_min += 30;
        mktime(&waktu);
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_mday, dest, ":");
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_mon + 1, dest, ":");
        snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%s", waktu.tm_year + 1900, dest, "-");

        if(waktu.tm_hour < 10)
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%d%s", NOL, dest, waktu.tm_hour, dest);
        }
        else
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s", waktu.tm_hour, dest);
        }
        strcat(nama, ":");

        if(waktu.tm_min < 10)
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s%d%s", NOL, dest, waktu.tm_min, dest);
        }
        else
        {
            snprintf(nama + strlen(nama), PANJANG - strlen(nama), "%d%s", waktu.tm_min, dest);
        }
    }
    return nama;
}

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

    char pathdir[PANJANG];
    memset(&pathdir, 0, sizeof(pathdir));
    snprintf(pathdir + strlen(pathdir), PANJANG - strlen(pathdir), "%s", "/home/bryan/log/");

    int status;

    pid = fork();

    if (pid == 0)
    {
        while(1)
        {
            char nama[PANJANG];
            memset(&nama, 0, sizeof(nama));
            snprintf(pathdir + strlen(pathdir), PANJANG - strlen(pathdir), "%s", namafolder(1));

            mkdir(pathdir, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);

            memset(&pathdir, 0, sizeof(pathdir));
            snprintf(pathdir + strlen(pathdir), PANJANG - strlen(pathdir), "%s", "/home/bryan/log/");

            sleep(1800);
        }
    }
    else if (pid > 0)
    {
        int counter = 0;
        int start = 1;
        char name2[PANJANG];
        memset(&name2, 0, sizeof(name2));
        while(1)
        {
            char name1[PANJANG];
            memset(&name1, 0, sizeof(name1));
            char currentname[PANJANG];
            memset(&currentname, 0, sizeof(currentname));

            strcat(currentname, namafolder(1));

            char tmp[PANJANG];
            memset(&tmp, 0, sizeof(tmp));
            snprintf(tmp + strlen(tmp), PANJANG - strlen(tmp), "%s%s", pathdir, name2);

            struct stat info;

            if (start == 1)
            {
                memset(&name1, 0, sizeof(name1));
                strcat(name1, namafolder(1));
                memset(&name2, 0, sizeof(name2));
                strcat(name2, namafolder(0));
            }
            else if ((start < 1) && (strcmp(currentname, name2) == 0) && (stat(tmp, &info) == 0))
            {
                memset(&name1, 0, sizeof(name1));
                strcat(name1, name2);
                memset(&name2, 0, sizeof(name2));
                strcat(name2, namafolder(0));
            }

            memset(&tmp, 0, sizeof(tmp));
            snprintf(tmp + strlen(tmp), PANJANG - strlen(tmp), "%s%s", pathdir, name1);

            if ((stat(tmp, &info) == 0) && (strcmp(name1, "") != 0))
            {
                while(1)
                {
                    start--;
                    char pathsystem[PANJANG];
                    char filelog[PANJANG];
                    char dest[50];
                    char ch;
                    memset(&pathsystem, 0, sizeof(pathsystem));
                    strcat(pathsystem, "/var/log/syslog");

                    memset(&filelog, 0, sizeof(filelog));
                    snprintf(filelog + strlen(filelog), PANJANG - strlen(filelog), "%s%s%s", pathdir, name1, "/log");

                    memset(&dest, 0, sizeof(dest));
                    sprintf(dest, "%d", counter + 1);
                    snprintf(filelog + strlen(filelog), PANJANG - strlen(filelog), "%s%s", dest, ".log");

                    FILE *sL=fopen((char*)pathsystem, "r");
                    FILE *aL=fopen((char*)filelog, "w");
                    while((ch = fgetc(sL)) != EOF )
                    {
                        fputc(ch, aL);
                    }
                    fclose(sL);
                    fclose(aL);
                    counter++;
                    if(counter % 30 == 0)
                    {
                        break;
                    }
                    sleep(60);
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}

