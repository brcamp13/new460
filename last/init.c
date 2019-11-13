#include "ucode.c"
int console;

int parent() {
    int pid, status;
    while (1) {
        
        printf("BRANDON init : wait for ZOMBIE child\n");
        pid = wait(&status);
        // If console login process died
        if (pid == console) {
            printf("INIT: forks a new console login\n");
            console = fork(); // fork another one
            if (console) {
                continue;
            }
            else {
                exec("login /dev/tty0"); // new console login process
            }
        }
        printf("BRANDON INIT: I just buried an orphan child proc %d\n", pid);
    }
}

main() {
    int in, out; // File descriptors for terminal I/O
    in = open("/dev/tty0", O_RDONLY); // File descriptor 0
    out = open("/dev/tty0", O_WRONLY); // For display to console
    
    printf("BRANDON INIT : fork a login proc on console\n");
    
    console = fork();
    //parent
    if (console) {
        parent();
    }
    else {
        // Child: exec to login on tty0
        printf("Child doing login\n");
        exec("login /dev/tty0");
    }
}
