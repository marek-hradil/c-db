#include <stdio.h>
#include <time.h>

void log(char * msg)
{
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char * stamp = asctime(timeinfo);
  stamp[strlen(stamp) - 1] = '\0';
  printf("[%s] %s\n", stamp, msg);
}
