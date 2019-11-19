#include "ucode.c"

STAT utat, *sp;
int fd, n;
DIR *dp;
char f[32], cwdname[64], file[64], buf[1024];
char *cp;


int main(int argc, char *argv) {

    sp = &utat;
    int fileStat;

    printf("==========Brandon LS ==========");

    // Ls the current directory as nothing specified
    if (argc == 1) {
        strcpy(file, "./");
    }
    else { // Ls the filename/dirname that was given by user
        strcpy(file, argv[1]);
    }

    fileStat = stat(file, sp);

    // If the mode is a file, then ls file
    if ((sp->st_mode & 0100000) == 0100000) {
        ls_file(sp, file, file);
    }
    else {
        // Otherwise, make sure that it's a dir and ls the dir
        if ((sp->st_mode & 0040000) == 0040000) {
            ls_dir(sp, file);
        }
    }
}