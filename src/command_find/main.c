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
    if(argc>2){
        do_dir(argv[1],argv);
    }
    return 0;
}

void do_dir(const char * dir_name, const char * const * parms)
{
    char* subdir;
    int paramCase=0;
    DIR* currentDir = opendir(dir_name);
    struct dirent *curr_ent;
    if(strcmp(parms[2],PARMS1))
    {
        paramCase=1;
    }
    if ( currentDir == NULL )
    {
        printf("\n!\n");
        do_file(dir_name,parms);
        printf("\n!!\n");
        return;
    }

    while((curr_ent =readdir(currentDir)) != NULL)
    {

        switch(paramCase)
        {
        case 1:
            if(curr_ent->d_type == DT_REG)
            {
                return;
            }
            else
            {
                printf("%s\n", curr_ent->d_name);
            }
            break;
        default:
            printf("%s\n", curr_ent->d_name);

        }

        if(!is_dot_or_dot_dot(curr_ent->d_name)&&curr_ent->d_type == DT_DIR)
        {
            subdir = malloc(strlen(dir_name) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, dir_name);
            strcat(subdir, "/");
            strcat(subdir, curr_ent->d_name);
            do_dir(subdir,parms);
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

    int paramCase=0;
    struct stat file_stats;
    if(!strcmp(file_name,""))
    {
        printf("\n1%s\n",file_name);
        return;
    }

    stat(file_name, &file_stats);

    if(!strcmp(parms[2],PARMS1))
    {
        printf("\n3\n");

        paramCase=1;
    }
    switch(paramCase)
    {
    case 1:
        printf("%s", file_name);
        break;
    default:
        printf("\n4\n");

    }


}
