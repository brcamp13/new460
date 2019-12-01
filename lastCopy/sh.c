#include "ucode.c"

int main(int argc, char *argv[]) {

    char cmd1[32];
    char cmd2[32];
    int pid;

    // From book, do_pipe algorithm:
    int pid, pd[2];
    pipe(pd); // Create a pipe: pd[0] = READ, pd[1] = WRITE
    pid = fork(); // Fork a child to share the pipe

    if (pid) {  // Parent as pipe READER
        close(pd[1]); // Close pipe WRITE end
        dup2(pd[0], 0); // Redirect stdin to pipe READ end
        exec(cmd2);
    }
    else {  // Child: as pipe WRITER
        close(pd[0]); // close pipe READ end
        dup2(pd[1], 1); // redirect stdout to pipe write end
        exec(cmd1);
    }

}