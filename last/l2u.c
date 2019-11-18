#include "ucode.c"

// If character is lowercase, change it to upper case
char toUpper(char character) {

    if (character >= 'a' && character <= 'z') {
        character = character - 32;
    }

    return character;
}

int main(int arc, char *argv[]) {

    char buf[256];

    // If there are no files specified (use standard in)
    int j = 0;
    if (argc == 1) {
        while (gets(buf)) {
            while(j < 256) {
                buf[j] = toUpper(buf[j]);
                j++;
            }
        }
    }
    else {

        int n;
        int source = open(argv[1], O_RDONLY);
        int destination = open(argv[2], O_WRONLY | O_CREAT);

        int i = 0;

        while (n = read(source, buf, 256)) {
            while(i < strlen(buf)) {
                buf[i] = toUpper(buf[i]);
                i++;
            }
            write(destination, buf, n);
        }
        
        close(source);
        close(destination);
    }

    return 0;
}
