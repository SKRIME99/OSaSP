#define _GNU_SOURCE
#define MAX_LEN 255

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

void dir_walk( char *path, char *flags);

int main(int argc, char *argv[])
{
    char *flags = (char*)calloc(5, sizeof(char));
    char *path = ".";
    if (argc > 1)
    {
        for(int i = 1; i < argc; ++i)
        {
            if (strstr(argv[i],"-")) 
                strcat(flags,argv[i]);
            else if(argv[i][0] == '/')
                path = argv[i];
        }
    }
    else strcat(flags,"-ldf");

    if (!strstr(flags,"l") && !strstr(flags,"d") && !strstr(flags,"f"))
        strcat(flags,"-ldf");

    if (strstr(flags, "d"))
        printf("d - %s/\n", path);
        
    dir_walk(path, flags);
    free(flags);
    return 0;
}

void dir_walk(char *path, char *flags)
{
    DIR *dir = opendir(path);
    struct dirent *read_dir;
    struct stat st;
    if (!dir)
    {
        if (errno)
        {
            fprintf(stderr, "lab1 : '%s' : Permission denied\n", path);
            errno = 0;
            return;
        }
    }
    while ((read_dir = readdir(dir)))
    {
        if (!(strcmp(read_dir->d_name, ".") && strcmp(read_dir->d_name, "..")))
            continue;
        char tmp[MAX_LEN];
        strcpy(tmp, path);
        strcat(tmp, "/");
        strcat(tmp, read_dir->d_name);
        lstat(tmp, &st);
        if (S_ISDIR(st.st_mode))
        {
            if (strstr(flags, "d"))
                printf("d - %s\n", tmp);
            dir_walk(tmp, flags);
        }
        if (S_ISREG(st.st_mode))
        {
            if (strstr(flags, "f"))
                printf("f - %s\n", tmp);
        }
        if (S_ISLNK(st.st_mode))
        {
            if (strstr(flags, "l"))
                printf("l - %s\n", tmp);
        }
    }
    if (!read_dir)
    {
        if(errno)
        {
            fprintf(stderr, "lab1: '%s' : File can't be readed\n", path);
            errno = 0;
        }
    }
    closedir(dir);
}