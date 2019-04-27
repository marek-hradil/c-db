#include <stdio.h>
#include <time.h>

void log(char * msg)
{
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("[%s] %s\n", asctime(timeinfo), msg);
}
