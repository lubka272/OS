#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *findMe)
{

    const char irregularName[3] = {".."};

    char buf[512];
    char *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("ls: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {  
        
        if (de.inum == 0)
            continue;       
        
        memmove(p, de.name, DIRSIZ);
        p[strlen(de.name)+1] = '\0';
        if (stat(buf, &st) < 0)
        {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }        

        if (strcmp(p, findMe) == 0)
        {
            printf("%s\n", buf);
            return;
        }
        else if(st.type == 1 && strcmp(p, irregularName) != 0 && strcmp(p, irregularName+1) != 0 ){
            find(buf,findMe);
            return;
        }          
    }

    printf("File or directory with such name does not exist\n");

    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        find(".", argv[1]);
        exit(0);
    }
    else if (argc == 3)
    {
        find(argv[1], argv[2]);
    }
    else
    {
        printf("Too much arguments");
    }

    exit(0);
}
