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

static void do_file(const char * file_name, const char * const * parms);
static void do_dir(const char * dir_name, const char * const * parms);
static int is_dot_or_dot_dot(char const* name);
static int my_print (const char* file_name, const char* const* parms);
static int do_user (const char* const* userIdentification,struct stat *file_stats,const char* file_name,const char* const* parms);
//argc = Anzahl der Parameter
//argv = die einzelnen übergebenen Werte werden in einem char-array gespeichert (argument values) 
//Beispiel Programmaufruf ./foo kurt hannes --> argc 3 (1. ./foo, 2. kurt, 3. hannes)
//argv[0] = ./foo, argv[1] = kurt, argv[2] = hannes

int main(int argc, char* argv[])
{

    if(argc>=2) //wenn argument count größer gleich 2, z.b. myfind -name
    {
        do_file(argv[1],argv); //do_file statt do_dir //"myfind" = argv[0], "-name" = argv[1] --> const char * file_name, argv --> const char * const * parms
    }
    return 0;
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
void do_file(const char * file_name, const char * const * parms) //in der main ist der erste parameter hier unser argv[1], der zweite argv (das gesamte array) 
{
    int paramCase=0;
	int j=2;
	int i=0;
	
    struct stat file_stats; //system struct that is defined to store information about files
	//st_mode, st_ino (inode), st_dev, st_uid, st_gid, st_atime, st_ctime, st_mtime, st_nlink, st_size
    int paramCase=0;
    int j=2;
    int i=0;

    struct stat file_stats; //system struct that is defined to store information about files
    //st_mode, st_ino (inode), st_dev, st_uid, st_gid, st_atime, st_ctime, st_mtime, st_nlink, st_size

    if(!strcmp(file_name,"")) //int strcmp(char *str1, char *str2)
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

        while(parms[j]!=NULL)
        {
			paramCase=1;
            if(!strcmp(parms[j],PARMS1))
            {
                paramCase=1;
            }
            if(!strcmp(parms[j],PARMS2))
            {
                paramCase=2;
            }
            if (!strcmp(parms[j], PARMS3))
            {
                paramCase = 3;
            }
            if (!strcmp(parms[j], PARMS4))
            {
                paramCase = 4;
            }
            if (!strcmp(parms[j], PARMS5))
            {
                paramCase = 5;
            }
            if (!strcmp(parms[j], PARMS6))
            {
                paramCase = 6;
            }
            if (!strcmp(parms[j], PARMS7))
            {
                paramCase = 7;
            }
            //-print 1, -user 2, -name 3, -type 4, -ls 5, -nouser 6, -path 7
            switch(paramCase)
            {
            case 1:
                my_print(file_name,parms);
                break;
            case 2:
                do_user(parms[j+1],&file_stats,file_name,parms);
                break;
            case 3:
                break;
            case 4:
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
}
//printf Fehlerbehandlung einbauen! --> ?? fprintf( stderr, "that didn't go well\n" );
static int my_print (const char* file_name, const char* const* parms)
{
    if (fprintf(stdout, "%s\n", file_name) < 0)
    {
        fprintf(stderr, "%s:  %s:  %s:  \n", parms[2], file_name, strerror(errno));
    }
    return 0; //0 means success
}
static int do_user (const char* const* userIdentification,struct stat *file_stats,const char* file_name,const char* const* parms)
{
    struct passwd *pwd;
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

    }
}
//printf Fehlerbehandlung einbauen! --> ?? fprintf( stderr, "that didn't go well\n" );
static int my_print (const char* file_name, const char* const* parms)
{
	if (fprintf(stdout, "%s\n", file_name < 0)
	{
		fprintf(stderr, "%s:  %s:  %s:  \n", parms[0], file_name, strerror(errno));
	}
	return 0; //0 means success
}

