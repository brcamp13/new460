#include "ucode.c"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";
struct stat mystat, *sp;

int ls_file(char *fname) {

    struct stat fstat, *sp = &fstat;

    int r, i;
    char sbuf[4096];

    r = stat(fname, sp);

    // Print "d" if directory, "-" otherwise
    if ((sp->st_mode & 0040000) == 0040000) {
        prints("d");
    }
    else {
        prints("-");
    }

    // Print permission bits
    i = 8;
    while (i >= 0) {
        if (sp->st_mode & (1 << i)) {
            mputc(t1[i]);
        }
        else {
            mputc(t2[i]);
        }
    }

    printf(" %d", sp->st_nlink); // Link count
    printf(" %d", sp->st_uid); // UID
    printf(" %d", sp->st_gid); // GID
    printf(" %d", sp->st_size); // File size
    printf(" %s\n\r", fname); // File name

}

int ls_dir(char *dname, char *cwd) {
    
    int n, fd;
    char buf[1024], fname[128], temp[32];
    char *cp;
    DIR *dp;

    fd = open(argv[1], O_RDONLY);


    while( (n = read(fd, buf, 1024))) {

        cp = buf;
        dp = (DIR*)cp;

        while (cp < &buf[1024]) {
            strncpy(fname, dp->name, dp->name_len);
            fname[dp->name_len] = 0;

            strcpy(temp, dname);
            strcat(temp, "/");
            strcat(temp, fname);

            ls_file(fname);
        }

        cp += dp->rec_len;
        dp = (DIR*)cp;
    }

    close(fd);
}


int main(int argc, char *argv) {

    struct stat mystat, *sp;
    int r; 
    int fd;
    char *s;
    char cwd[1024], fileName[128];

    printf("DOING LS!!!!!!!!!!!!!!\n");

    s = argv[1];

    // Ls the current directory as nothing specified
    if (argc == 1) {
        s = "./";
    }

    sp = &mystat;
    
    if ((r = stat(s, sp)) < 0) {
        printf("Error opening file lol\n");
        exit(1);
    }

    strcpy(fileName, s);

    if (s[0] != '/') { // Filename relative to CWD
        getcwd(cwd);
        strcpy(fileName, cwd);
        strcat(fileName, "/");
        strcat(fileName, s); // Construct CWD/filename
    }

    // If the mode is a file, then ls file
    if ((sp->st_mode & 0100000) == 0100000) {
        ls_file(fileName);
    }
    else {
        // Otherwise, make sure that it's a dir and ls the dir
        if ((sp->st_mode & 0040000) == 0040000) {
            ls_dir(fileName, cwd);
        }
    }
}
