#include "ucode.c"


int GetLine(char *buffer, int fd) {
    int i = 0;
    char temp[5] = "";

    // Clear the buffer
    strcpy(buffer, "");
    while (strcmp(temp, "\n") != 0) {
        if (read(fd, temp, 1) > 0) {
            strcat(buffer, temp);
            i++;
        }
        else {
            break;
        }
    }
    return i;
}

int main(int argc, char *argv[]) {

    // So the general idea is to just read input from the file, line-by-line
    // When you encounter a line with the pattern that you are looking for, then you print that line
    // Use strstr for this
    // So read 1024 at a time and then do strstr for that line, and if it returns something, then print that line
    char line[1024];
    int n, fd;

    // Stdin (no argv specified)
    if (argc == 2) {
        while (gets(line)) {
            if (strstr(line, argv[1])) {
                prints(line);
            }
            return 0;
        }
    }
    else {
        fd = open(argv[2], O_RDONLY);
        while (n = GetLine(line, fd)) {
            if (strstr(line, argv[1])) {
                printf("%s", line);
            }
        }
    }

    // Close file descriptor and return success
    close(fd);
    return 0;
}