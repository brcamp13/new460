#include "ucode.c"

int main(int argc, char *argv[]) {
    

    char fileBuf[2];
    int fd = 0;
    int n;

    // No file to cat, do stdin
    if (argc == 1) {
        while (1) {
            gets(fileBuf);
            prints(fileBuf);
            return 0;
        }
    }
    else {

        // Open the file, read and display everything
        fd = open(argv[1], O_RDONLY);
        while(n = read(fd, fileBuf, 1)) {
            fileBuf[n] = 0;
            printf("%s", fileBuf);
        }
    }

    close(fd);

}
