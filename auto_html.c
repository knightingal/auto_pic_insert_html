/* Knightingal
 * 0423
 * update: add the multiple file input function
 */
/* Knightingal
 * 0408
 * update: dynamic file name length
 */

#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>

char* img_name[] = {"jpg", "gif", "jpeg", "JPG", NULL};
char** get_all_file_name(char *dir)
{
  char** name = malloc(sizeof(char**) * 100);
  
  struct dirent *dp;
  DIR *dfd;
  int i = 0;

  if((dfd = opendir(dir)) == NULL)
  {
    printf("opendir fail\n");
    return;
  }

  while(( dp = readdir(dfd)) != NULL)
  { 
    if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
    {
      continue;
    }
    name[i] = malloc(sizeof(char) * (strlen(dp->d_name) + 1));
    sprintf(name[i], "%s", dp->d_name);
    i++;
  }
  name[i] = NULL;
  return name;
}

int cmp_file_name(char* name1, char* name2)
{
  if(strlen(name1) < strlen(name2))
  {
    return -1;
  }
  else if(strlen(name2) < strlen(name1))
  {
    return 1;
  }
  else
  {
    return strcmp(name1, name2);
  }
}

int is_img_file(char* name)
{
  char* name_p = name;
  char** img_p = img_name; 
  while(*name_p != '.' && *name_p != '\0')
  {
    name_p++;
  }
  if(*name_p == '\0')
  {
    return 0;
  }
  else
  {
    name_p++;
    while(*img_p != NULL)
    {
      if(strcmp(name_p, *img_p) == 0)
      {
        return 1;
      }
      img_p++;
    }
    return 0;
  }
}

void sort_file_name(char** name)
{
  char** name_p = name;
  char* name_tmp = NULL;
  int count = 0;
  int i, j;
  while(*name_p != NULL)
  {
    count++;
    name_p++;
  }
  for(i = 0 ; i < count ; i++)
  {
    for(j = count - 1 ; j > i ; j--)
    {
      if(cmp_file_name(name[j], name[j - 1]) < 0)
      {
        name_tmp = name[j];
        name[j] = name[j - 1];
        name[j - 1] = name_tmp;
      }
    }
  }
}

void print_all_file_name(char** name)
{
  char** name_p = name;
  while(*name_p != NULL)
  {
    printf("%s\n", *name_p);
    name_p++;
  }
}

void make_html_file(char** name, char* dir)
{
  FILE* fp;
  char* file_name = malloc(sizeof(char) * (strlen(dir) + 9));
  char ** name_p = name;
  sprintf(file_name, "%s/%s", dir, "web.htm");
  fp = fopen(file_name, "w");
  fputs("<html>\n<body>\n", fp);
  while(*name_p != NULL)
  {
    if(is_img_file(*name_p) == 1)
    {
      fputs("<img src=\"", fp);
      fputs(*name_p, fp);
      fputs("\" />\n", fp);
    }
    name_p++;
  }
  fputs("</body>\n</html>\n", fp);
  fclose(fp);
}

int main(int argc, char** argv)
{
  char** name;
  //char* dir = malloc(sizeof(char) * 32);
  char** dir;
  int i;
  if(argc == 1)
  { 
    dir = malloc(sizeof(char*) * 2);
    dir[0] = malloc(sizeof(char) * 2);
    dir[1] = NULL;
    sprintf(dir[0], "%s", ".");
  }
  else
  {
    dir = malloc(sizeof(char*) * argc);
    for(i = 1 ; i < argc ; i++)
    {
      dir[i - 1] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
      sprintf(dir[i - 1], "%s", argv[i]);
    }
    dir[argc - 1] = NULL;
  }
  i = 0;
  while(dir[i] != NULL)
  {
    name = get_all_file_name(dir[i]);
    //print_all_file_name(name);
    sort_file_name(name);
    //print_all_file_name(name);
    make_html_file(name, dir[i]);
    free(dir[i]);
    i++;
  }
  free(dir);
  return 0;
}
  
