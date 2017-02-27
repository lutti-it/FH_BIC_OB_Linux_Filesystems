#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#define PARMS1 "-print"
#define PARMS2 "-user"
void do_file(const char * file_name, const char * const * parms);
void do_dir(const char * dir_name, const char * const * parms);
int is_dot_or_dot_dot(char const* name);
int main(int argc, char* argv[])
{
    if(argc>=2){
        do_file(argv[1],argv); //do_file statt do_dir
    }
    return 0;
}

void do_dir(const char * dir_name, const char * const * parms)
{
    char* subdir;
    int paramCase=0;
    DIR* currentDir = opendir(dir_name);
    struct dirent *curr_ent;
    if ( currentDir == NULL )
    {
        return;
    }
    while((curr_ent =readdir(currentDir)) != NULL)
    {
        if(!is_dot_or_dot_dot(curr_ent->d_name)&&curr_ent->d_type == DT_DIR)
        {
            subdir = malloc(strlen(dir_name) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, dir_name);
            strcat(subdir, "/"); // slash abfrage!
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
void do_file(const char * file_name, const char * const * parms){

    int paramCase=0,j=2,i=0;
    struct stat file_stats;
    if(!strcmp(file_name,""))
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
        return;
    }
    stat(file_name, &file_stats); //stat lstat?

    while(parms[j]!=NULL)
    {
        if(!strcmp(parms[i],PARMS1))
        {

            paramCase=1;
        }
        if(!strcmp(parms[i],PARMS2))
        {
            paramCase=2;
        }

        switch(paramCase)
        {
        case 1:
            printf(" %s ", file_name);
            break;
        case 2:
            printf(" %d ", file_stats.st_uid);
            break;

        }
        j++;
    }
    do_dir(file_name,parms);
}

