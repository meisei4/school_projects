#include <stdio.h>
#include <stdlib.h>

#define CUSTOM_STRING

#ifdef CUSTOM_STRING

#pragma GCC diagnostic ignored "-Wincompatible-library-redeclaration"

unsigned long
strlen(const char * str)
{
  const char * s;
  for (s = str; *s; ++s);
  return s - str;
}

void
strcpy(char * d, char * s)
{
  while ((*d++ = *s++));
}

char *
strdup(char * s)
{
  char * t = malloc(strlen(s)+1);
  strcpy(t,s);
  return t;
}

int
strcmp(char * a, char * b)
{
  while (*a && (*a == *b)) {a++; b++;}
  return *a - *b;
}

char *
strchr(const char * s, int c)
{
  while (*s && *s != c) s++;
  return (*s == c) ? ((char *)s) : (NULL);
}

char *
strdel(char * str, char * delset)
{
  char * s = str;
  char * p = str;
  while (*s) {
    if (strchr(delset,*s) == NULL) {
      *p++ = *s++;
    }
    else {
      s++;
    }
  }
  *p++ = *s++;
  return str;
}

char *
strpbrk(const char * s, const char * t)
{
  while (!strchr(t,*s)) s++;
  if (*s == '\0') return NULL;
  return (char *) s;
}

char *
strstr(const char * s, const char * t)
{
  if (!*t) return (char *) s;

  for ( ; *s; s++) {
    if (*s != *t) continue;
    const char * sm = s;
    const char * tm = t;
    while (1) {
      if (*tm == 0) return (char *) s;
      if (*sm++ != *tm++) break;
    }
  }
  return NULL;
}

char * strtok_next = NULL;

char *
strtok(char * restrict s, const char * restrict sep)
{
  if (s == NULL) s = strtok_next;
  if (s == NULL) return NULL;
  
  char * t = strpbrk(s,sep);
  if (!t) {
    strtok_next = NULL;
  }
  else {
    *t = '\0';
    strtok_next = t + 1;
  }
  return s;
}

char *
strsep(char ** strp, char * delim)
{
  if (*strp == NULL) return NULL;
  
  char * ret = *strp;
  while (!strchr(delim,**strp)) (*strp)++;
  if (**strp) {
    **strp = '\0';
    (*strp)++;
  }
  else {
    *strp = NULL;
  }
  return ret;
}

size_t
strspn(const char * s, const char *t)
{
  const char * sm = s;
  while (*sm && strchr(t,*sm)) sm++;
  return (sm - s);
}
#else

#include <string.h>

#endif

int
main(int argc, char * argv[])
{

#ifdef CUSTOM_STRING
  printf("Using custom string implementation\n");
#else
  printf("Using string.h\n");
#endif

//  char str[] = "astring";
//  str[0] = 'A';
//  printf("%s\n",str);
  
  char str1[] = "abcdefg";
  char cpy[10];
  
//  printf("strlen %ld\n",strlen(str1));
  
//  strcpy(cpy,str1);
//  printf("strcpy \"%s\"\n",cpy);
  
//  printf("strchr \"%s\"\n",strchr(str1,'d'));
//  printf("strchr \"%s\"\n",strchr(str1,'\0'));
//  printf("strchr %p\n",strchr(str1,'0'));
  
//  char str2[] = "This is a test of the emergency broadcasting system.";
//  char str3[] = "aeiou";
//  printf("strrm \"%s\"\n",strdel(str2,str3));
  
//  printf("strcmp %d\n",strcmp("one","one"));
//  printf("strcmp %d\n",strcmp("one","onetwo"));
//  printf("strcmp %d\n",strcmp("onetwo","one"));
//  printf("strcmp %d\n",strcmp("onefour","onetwo"));
//  printf("strcmp %d\n",strcmp("onetwo","onefour"));
//  printf("strcmp %d\n",strcmp("",""));
//  printf("strcmp %d\n",strcmp("","onetwo"));
//  printf("strcmp %d\n",strcmp("onetwo",""));

//  char * str4 = strdup("one,two,three:un,deux,trois:eins,zwei,drei");
//  char str5[] = ",:";
//  char * strseptok;
//  while ((strseptok = strsep(&str4,str5))) {
//    printf("strseptok \"%s\"\n",strseptok);
//  }
  
  char str6[] = "is This is a little text that is short is";
  char str7[] = "is";
  char * strstrloc = strstr(str6,str7);
  while (strstrloc) {
    printf("strstr \"%s\"\n",strstrloc);
    strstrloc = strstr(strstrloc+1,str7);
  }
  printf("strstr null %p\n",strstr(str6,"exit"));
  
//  char * str8 = strdup("one,two,three:un,deux,trois:eins,zwei,drei");
//  char str9[] = ",:";
//  char * strpbrkloc = strpbrk(str8,str9);
//  while (strpbrkloc) {
//    printf("strpbrk %d\n",(int)(strpbrkloc - str8));
//    strpbrkloc = strpbrk(strpbrkloc+1,str9);
//  }
  
//  char * str10 = strdup("one,two,three:un,deux,trois:eins,zwei,drei");
//  char str11[] = ",:";
//  char * strtoktok = strtok(str10,str11);
//  while(strtoktok) {
//    printf("strtoktok %s\n",strtoktok);
//    strtoktok = strtok(NULL,str11);
//  }
  
//  int len = strspn("0127.283,2334.209@,@,","0123456789.");
//  printf("strspn %d\n",len);
  
}