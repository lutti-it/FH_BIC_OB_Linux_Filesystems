#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
void do_dir(const char * dir_name, const char * const * parms);
int is_dot_or_dot_dot(char const* name);
int main(int argc, char* argv[])
{
    if(argc>1){
        do_dir(argv[1],'\0');
    }
    return 0;
}

void do_dir(const char * dir_name, const char * const * parms)
{
    char* subdir;
    DIR* currentDir = opendir(dir_name);
    struct dirent *curr_ent;
    if ( currentDir == NULL )
        return;

    while((curr_ent =readdir(currentDir)) != NULL)
    {
        printf("%s\n", curr_ent->d_name);
        if(!is_dot_or_dot_dot(curr_ent->d_name)&&curr_ent->d_type == DT_DIR)
        {
            subdir = malloc(strlen(dir_name) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, dir_name);
            strcat(subdir, "/");
            strcat(subdir, curr_ent->d_name);
            do_dir(subdir,'\0');
            free(subdir);
        }
    }
    closedir(currentDir);
}
int is_dot_or_dot_dot(char const* name)
{
    return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}



/*

void do_file(const char * file_name, const char * const * parms);
void do_dir(const char * dir_name, const char * const * parms);
int is_dot_or_dot_dot(char const* name);

char cwd[1024];

int main(int argc, char* argv[])
{
    //do_file("test.txt","test");
    //do_dir("debug","");

    //Eingabe

    //if(argc == 1) do_file(argv[1],'\0'); //nur command_find

    //printf("%c\n", argv[0][0]);
    char * name;
    char * param;

    if(argc > 1)
    {
        DIR *dirp = opendir(argv[1]);

        if (dirp != NULL)
        {
            //chdir
            int i;
            for(i = 2; i <= argc; i++)
            {

                if (argv[i-1][0] == '-')
                {
                    param = argv[i-1];
                    do_file(name,param);
                }
                else
                {
                    name = argv[2];
                }
            }
        }
        else
        {
            name = argv[1];
        }
    }
    return 0;
}

void do_file(const char * file_name, const char * const * parms){

    char* path = getcwd(cwd, sizeof(cwd));
    DIR *dirp = opendir(path);
    struct dirent *curr_ent;

    if (dirp != NULL)
    {
        if((curr_ent =readdir(dirp)) != NULL){
            if(strcmp(file_name,curr_ent->d_name) == 1){
                printf("%s\n", file_name);
            }
            else{

                // Suche erweitern auf die Methode do_dir
                do_dir(file_name,parms);
            }
        }
        else{
            //Achtung es gibt keine wieteren Verzeichnisse
        }
    }
    else{
        //Achtung Error: Der Pfad ist nicht richtig
    }
    closedir(dirp);
}

void do_dir(const char * dir_name, const char * const * parms){

    char* path = getcwd(cwd, sizeof(cwd));
    DIR *dirp = opendir(path);
    struct dirent *curr_ent;

    if (dirp != NULL)
    {
        char * subdir;

        do_file(dir_name,parms);

        while((curr_ent =readdir(dirp)) != NULL){

            subdir = malloc(strlen(path) + strlen(curr_ent->d_name) + 2);
            strcpy(subdir, path);
            subdir = strcat(subdir, "/");
            subdir = strcat(subdir, curr_ent->d_name);

            struct stat path_stat;
            stat(subdir, &path_stat);

            if(!is_dot_or_dot_dot(curr_ent->d_name) && S_ISDIR(path_stat.st_mode)){


                printf("%s\n", "test hoch 3");
//                chdir(subdir);
//                do_file(dir_name,parms);
            }
            free(subdir);
        }
    }
    closedir(dirp);
}

int is_dot_or_dot_dot(char const* name)
{
    return (strcmp(name, ".") == 0 || strcmp(name, "..") == 0 );
}*/
