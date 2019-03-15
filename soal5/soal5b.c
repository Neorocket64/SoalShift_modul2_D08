#include <stdio.h>
#include <unistd.h>

int main () {
  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"mati", "killall", "/home/bryan/sisop/modul2/soal5/soal5a", NULL};
  execv("/usr/bin/killall", argv);

  printf("Proses Dihentikan\n");

  return 0;
}
