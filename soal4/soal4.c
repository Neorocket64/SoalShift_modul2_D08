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

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  char pathFile[] = "home/bryan/Documents/makanan/makan_enak.txt";
  int namafile = 1;

  while(1) {
    char nama[3];
    sprintf(nama,"%d",namafile);
    struct stat statFile;
    stat (pathFile, &statFile);
    time_t timeFile2 = statFile.st_atime;

    //mengecek waktu terakhir file dibuka
    if (difftime(time(NULL), timeFile2) <= 30) {
	//membuat file diet

	char pathHasil[]="/home/bryan/Documents/makanan/makan_sehat";
	snprintf(pathHasil + strlen(pathHasil), "%s%s", nama, ".txt");
	//strcat(pathHasil, nama);
	//strcat(pathHasil, ".txt");

	FILE *hasil = fopen(pathHasil, "w");
	fclose(hasil);
	namafile++;
	}

    sleep(5);
  }
  exit(EXIT_SUCCESS);
}
