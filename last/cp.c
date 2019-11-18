#include "ucode.c"

int main(int argc, char *argv[]) {

    int source, destination, n;
    char buf[256];

    // Open source and destination FDs
    source = open(argv[1], O_RDONLY);
    destination = open(argv[2], O_WRONLY);

    // Read 256 bytes from source, write 256 bytes to destination
    while (n = read(source, buf, 256)) {
        write(destination, buf, 256);
    }

    // Close both FDs and return success
    close(source);
    close(destination);
    return 0;

}
