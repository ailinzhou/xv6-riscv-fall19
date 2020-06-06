#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *file, char *name);

char *obtainFileName(char *path)
{
    char *p = path + strlen(path);
    for (; *p != '/' && p >= path; p--)
        ;
    p++;
    return p;
}

void findFile(char *name, char *fileName)
{
    char *p = obtainFileName(fileName);
    for (; *p != '\0'; p++)
    {
        char *t = p;
        char *n = name;
        for (; *n != '\0'; n++, t++)
        {
            if (*n != *t)
            {
                break;
            }
        }
        if (*n == '\0')
        {
            fprintf(1,"%s\n", fileName);
            return;
        }
    }
}

void dirFind(char *name, int fd, char *path)
{
    struct dirent de;
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum <= 0)
        {
            continue;
        }
        char *file = de.name;
        if (*file == '.' || *file == '/')
        {
            continue;
        }
        char all[strlen(de.name) + strlen(path) + 2];
        strcpy(all, path);
        int i = (strlen(path));
        all[i++] = '/';
        for (; (all[i++] = *file) != '\0'; file++)
            ;
        int cfd;
        cfd = open(all, 0);
        if (cfd < 0)
        {
            continue;
        }
        find(all, name);
        close(cfd);
    }
    return;
}

void find(char *path, char *name)
{
    int fd;
    struct stat st;
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        findFile(name, path);
        close(fd);
        break;
    case T_DIR:
        dirFind(name, fd, path);
        close(fd);
        break;
    default:
        break;
    }
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "find: param error\n");
        exit();
    }
    find(argv[1], argv[2]);
    exit();
}