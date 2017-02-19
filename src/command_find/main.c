#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void listdir(char const* dirname);
int is_dot_or_dot_dot(char const* name);
int main(int argc, char* argv[])
{
    if(argc>1)
        listdir(argv[1]);
    else
    {
        listdir("/home/leo/test1");
    }
}

void listdir(char const* dirname){
    char* subdir;
    DIR* currentDir = opendir(dirname);
    struct dirent *curr_ent;
    if ( currentDir == NULL )
        return;

    while((curr_ent =readdir(currentDir)) != NULL)
    {
        printf("%s\n", curr_ent->d_name);
        if(!is_dot_or_dot_dot(curr_ent->d_name)&&curr_ent->d_type == DT_DIR)
        {
            subdir = malloc(strlen(dirname) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, dirname);
            strcat(subdir, "/");
            strcat(subdir, curr_ent->d_name);
            listdir(subdir);
            free(subdir);
        }
    }
    closedir(currentDir);
}
int is_dot_or_dot_dot(char const* name)
{
    return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}
