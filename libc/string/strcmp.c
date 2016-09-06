#include <string.h>
int strcmp(char *lhs, char *rhs)
{
  if(lhs==rhs) return 0;
  if (*lhs!=*rhs) return *lhs-*rhs;
  while(*lhs)
  {
    lhs++;rhs++;
    if (*lhs!=*rhs) return *lhs-*rhs;
  }
  return 0;
}
int strncmp(char * lhs, char * rhs,size_t n)
{
  if(lhs==rhs) return 0;
  if (*lhs!=*rhs) return *lhs-*rhs;
  lhs++;rhs++;
  n--;
  for(;!n;n--)
  {
    if (*lhs!=*rhs) return *lhs-*rhs;
    lhs++;rhs++;
  }
  return 0;
}
