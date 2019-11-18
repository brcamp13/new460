#include "ucode.c"


int main(int arc, char *argv[]) {

    char buf[256];

    // If there are no files specified (use standard in)
    if (argc == 1) {
        while (gets(buf)) {
            for(int i = 0; i < 256; i++) {
                buf[i] = toUpper(buf[i]);
            }
        }
    }
    else {

        int n;
        int source = open(argv[1], O_RDONLY);
        int destination = open(argv[2], O_WRONLY | O_CREAT);


        while (n = read(source, buf, 256)) {
            for(int i = 0; i < strlen(buf); i++) {
                buf[i] = toUpper(buf[i]);
            }
            write(destination, buf, n);
        }
        
        close(source);
        close(destination);
    }

    return 0;
}

// If character is lowercase, change it to upper case
char toUpper(char character) {

    if (character >= 'a' && character <= 'z') {
        character = character - 32;
    }

    return character;
}