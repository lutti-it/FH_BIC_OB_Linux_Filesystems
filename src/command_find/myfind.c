#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <fnmatch.h>
#include <ctype.h>

//-print 1, -user 2, -name 3, -type 4, -ls 5, -nouser 6, -path 7
#define PARMS1 "-print"
#define PARMS2 "-user"
#define PARMS3 "-name"
#define PARMS4 "-type"
#define PARMS5 "-ls"
#define PARMS6 "-nouser"
#define PARMS7 "-path"

static int do_file(const char * file_name, const char * const * parms);
static void do_dir(const char * dir_name, const char * const * parms);
static int is_dot_or_dot_dot(char const* name);
static int my_print (const char* file_name, const char* const* parms);
static int do_user (const char* const* userIdentification, struct stat *file_stats,const char* file_name,const char* const* parms);
static int do_name ();
static int do_type(const char* file_name, const char* const* parms);
//argc = Anzahl der Parameter
//argv = die einzelnen übergebenen Werte werden in einem char-array gespeichert (argument values)
//Beispiel Programmaufruf ./foo kurt hannes --> argc 3 (1. ./foo, 2. kurt, 3. hannes)
//argv[0] = ./foo, argv[1] = kurt, argv[2] = hannes

int main(int argc, char* argv[])
{

    if(argc>=2) //wenn argument count größer gleich 2, z.b. myfind -name
    {
      return do_file(argv[1],argv); //do_file statt do_dir //"myfind" = argv[0], "-name" = argv[1] --> const char * file_name, argv --> const char * const * parms
    }
    else
    {
      return EXIT_FAILURE;
    }
}

void do_dir(const char * dir_name, const char * const * parms)
{
    char* subdir;
    DIR* currentDir = opendir(dir_name); //DIR *opendir(const char *dirname);
    struct dirent *curr_ent; //readdir --> struct dirent *entry

    if ( currentDir == NULL )
    {
        return;
    }
    while((curr_ent = readdir(currentDir)) != NULL)
    {
        if(!is_dot_or_dot_dot(curr_ent->d_name)&&curr_ent->d_type == DT_DIR)
        {
            subdir = malloc(strlen(dir_name) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, dir_name);
            char str[] = "/";
            if(strncmp(dir_name, str, 1) != 0)
            {
                strcat(subdir, "/"); // slash abfrage! dir name kann schon ein "/" beinhalten
            }
            strcat(subdir, curr_ent->d_name);
            do_file(subdir,parms); //do_file
            free(subdir);
        }
    }
    closedir(currentDir);
}
int is_dot_or_dot_dot(char const* name)
{
    return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}
static int do_file(const char * file_name, const char * const * parms) //in der main ist der erste parameter hier unser argv[1], der zweite argv (das gesamte array)
{
    int paramCase=0;
    int j=2;
    int i=0;
    int return_code=EXIT_FAILURE;

    struct stat file_stats;

    if(strcmp(file_name,"") == 0) //int strcmp(char *str1, char *str2)
		{
			return;
		}
    //Abfrage der länge des Arrays parms!
    while(parms[i]!=NULL)
		{
			i++;
		}
    if(i<j)
		{
			printf(" %s ", file_name);
		}
    else
    {
        stat(file_name, &file_stats); //stat lstat? //int stat(const char *path, struct stat *buf);
        j=1;
        while(parms[j]!=NULL && paramCase == 0)
        {
            paramCase=0;
            if(strcmp(parms[j],PARMS1) == 0)
            {
                paramCase=1;
            }
            else if(strcmp(parms[j],PARMS2) == 0)
            {
                paramCase=2;
            }
            else if(strcmp(parms[j], PARMS3) == 0)
            {
                paramCase = 3;
            }
            else if(strcmp(parms[j], PARMS4) == 0)
            {
                paramCase = 4;
            }
            else if(strcmp(parms[j], PARMS5) == 0)
            {
                paramCase = 5;
            }
            else if(strcmp(parms[j], PARMS6) == 0)
            {
                paramCase = 6;
            }
            else if(strcmp(parms[j], PARMS7) == 0)
            {
                paramCase = 7;
            }
            if(paramCase != 0)
            {
              return_code = EXIT_SUCCESS;
            }
            //-print 1, -user 2, -name 3, -type 4, -ls 5, -nouser 6, -path 7
            switch(paramCase)
            {
            case 1:
                if(strcmp(parms[1],"-print") == 0) //Wenn kein Verzeichnis angegeben wurde
                {
                  my_print(".",parms);
                }
                else
                {
                  my_print(parms[1],parms);
                }
                break;
            case 2:
                do_user(parms[j+1],&file_stats,file_name,parms);
                break;
            case 3:
                break;
            case 4:
                if(strcmp(parms[1],"-type") == 0) //Wenn kein Verzeichnis angegeben wurde
                {
                  do_type(".",parms);
                }
                else
                {
                  do_type(parms[1],parms);
                }
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            }
            j++;
        }
    }
    do_dir(file_name,parms);

    return return_code;
}
//printf Fehlerbehandlung einbauen! --> ?? fprintf( stderr, "that didn't go well\n" );
static int my_print (const char* file_name, const char* const* parms)
{
	DIR *dir;
	struct dirent *dirpointer;
	char * temp_dir;


    if ((dir=opendir(file_name)) == NULL)
    {
      fprintf(stderr, "Fehler beim Öffnen der Directory %s %s\n", file_name, strerror(errno));
      return(EXIT_FAILURE);
    }
    struct stat file_stats;

    while ((dirpointer = readdir(dir)) != NULL)
    {
      temp_dir = malloc(strlen(file_name)+strlen(dirpointer->d_name)+2);

      strcpy(temp_dir,file_name);
      strcat(temp_dir,"/");
      strcat(temp_dir,dirpointer->d_name);

      stat(temp_dir, &file_stats);

      if(S_ISDIR(file_stats.st_mode) && is_dot_or_dot_dot(dirpointer->d_name) == 0)
      {
        my_print(temp_dir,parms);
      }
      if(is_dot_or_dot_dot(dirpointer->d_name) == 0)
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }

      free(temp_dir);
    }

    closedir(dir);
    return EXIT_SUCCESS;
}
static int do_user (const char* const* userIdentification,struct stat *file_stats,const char* file_name,const char* const* parms)
{
    /*struct passwd *pwd;
    pwd=getpwnam(userIdentification);
    if(pwd)
    {
        if(pwd->pw_uid==file_stats->st_uid)
        {
            my_print(file_name,parms);
        }
    }
    else
    {
        pwd=getpwuid(userIdentification);
        if(pwd)
        {
            if(pwd->pw_uid==file_stats->st_uid)
            {
                my_print(file_name,parms);
            }
        }

    }*/
}

static int do_name ()
{


}

static int do_type(const char* file_name, const char* const* parms)
{
	DIR *dir;
	struct dirent *dirpointer;
	char * temp_dir;
	char type=NULL;

  if (strcmp(parms[1],"-type") == 0 && parms[2] != NULL)
  {
    type=*parms[2];
  }
  else if (strcmp(parms[2],"-type") == 0 && parms[3] != NULL)
  {
    type=*parms[3];
  }
  else
  {
    printf("find: Fehlendes Argument für »-type«");
  }
  if(type != NULL)
  {
    if ((dir=opendir(file_name)) == NULL)
    {
      fprintf(stderr, "Fehler beim Öffnen der Directory %s\n", strerror(errno));
      return(EXIT_FAILURE);
    }
    struct stat file_stats;

    while ((dirpointer = readdir(dir)) != NULL)
    {
      temp_dir = malloc(strlen(file_name)+strlen(dirpointer->d_name)+2);

      strcpy(temp_dir,file_name);
      strcat(temp_dir,"/");
      strcat(temp_dir,dirpointer->d_name);

      stat(temp_dir, &file_stats);
/*
              b      block (buffered) special

              c      character (unbuffered) special

              d      directory

              p      named pipe (FIFO)

              f      regular file

              l      symbolic  link;  this is never true if the -L option or the -follow option is in effect, unless the symbolic link is broken.  If you want to search for symbolic links when -L is in
                     effect, use -xtype.

              s      socket


*/
      if(S_ISDIR(file_stats.st_mode) && is_dot_or_dot_dot(dirpointer->d_name) == 0)
      {
        do_type(temp_dir,parms);
        free(temp_dir);
      }
      if(S_ISREG(file_stats.st_mode) && type == 'f')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISDIR(file_stats.st_mode) && type == 'd')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISFIFO(file_stats.st_mode) && type == 'p')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISLNK(file_stats.st_mode) && type == 'l')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISCHR(file_stats.st_mode) && type == 'c')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISBLK(file_stats.st_mode) && type == 'b')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
      else if(S_ISSOCK(file_stats.st_mode) && type == 's')
      {
        printf("%s/%s\n",file_name,dirpointer->d_name);
      }
    }

    closedir(dir);
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
