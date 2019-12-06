#include "ucode.c"

int status;

void doRedirect(char *command, int redirectType);

int containsPipe(char *command) {
    char *c = command;

    while (*c) {
        if (*c == '|') {
            return 1;
        }
        *c++;
    }

    return 0;
}

int doPipe(char *command) {
    int pid, pd[2];
    int i = 0, j = 0;
    char head[256], tail[256], temp[256];
    char *token, *command1, *command2;

    // Preserve the original command string
    strcpy(temp, command);

    // Get first command (head)
    while (temp[i] != '|') {
        head[i] = temp[i];
        i++;
    }
    head[i - 1] = 0;

    // Get second command (tail)
    i += 2;
    while (temp[i] != 0 ) {
        tail[j] = temp[i];
        j++;
        i++;
    }
    tail[j] = 0;

    pipe(pd); // Create a pipe: pd[0]=READ, pd[1]=WRITE
    pid = fork();

    // If parent process
    if (pid) {
        close(pd[1]); // Close pipe WRITE end
        dup2(pd[0], 0); // redirect stdin to pipe READ end

        // Check if there is a multi-pipe (pipes recursively execute right to left if so)
        if (containsPipe(tail) == 1) {
            doPipe(tail);
        }
        else {
            // Check if there is a redirect as there are no multi-pipes
            doRedirect(tail, getRedirectType(tail));
            exec(tail);
        }
    }
    else { // If child process as pipe reader
        close(pd[0]); // close pipe read end
        dup2(pd[1], 1); // redirect stdout to pipe write end

        // Check if there is a redirect
        doRedirect(head, getRedirectType(head));
        exec(head);
    }

    return;
}

// Returns 1 if >>, 2 if >, 3 if <, 0 if none
int getRedirectType(char *command) {
    char *c = command;
    
    while (*c) {
        if (*c == '>') {
            *c++;
            if (*c == '>') {
                return 1;
            }

            return 2;
        }

        if (*c == '<') {
            return 3;
        }

        *c++;
    }

    return 0;
}

void doRedirect(char *command, int redirectType) {

    int i, j, k;
    int fd;
    char temp[256], cmd[256], fileLocation[256];

    // Preserve the original command string
    strcpy(temp, command);

    // Appending to existing file
    if (redirectType == 1) {

        // Get the command
        for (i = 0; temp[i] != '>'; i++) {
            cmd[i] = temp[i];
        }
        command[i-1] = '\0';

        // Get the file name/location
        j = i + 3;
        for (k = 0; j < strlen(command); k++) {
            fileLocation[k] = temp[j];
            j++;
        }
        fileLocation[k] = '\0';

        fd = open(fileLocation, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd, 1); // Redirect stdout to file pointed to by fd
        exec(command);
    }

    // Creating new file
    if (redirectType == 2) {

        // Get the command
        for (i = 0; temp[i] != '>'; i++) {
            cmd[i] = temp[i];
        }
        command[i-1] = '\0';

        // Get the file name/location
        j = i + 2;
        for (k = 0; j < strlen(command); k++) {
            fileLocation[k] = temp[j];
            j++;
        }
        fileLocation[k] = '\0';

        fd = open(fileLocation, O_WRONLY | O_CREAT);
        dup2(fd, 1); // Redirect stdout to file pointed to by fd
        exec(command);
    }

    // Reading input from file
    if (redirectType == 3) {

        // Get the command
        for (i = 0; temp[i] != '>'; i++) {
            cmd[i] = temp[i];
        }
        command[i-1] = '\0';

        // Get the file name/location
        j = i + 3;
        for (k = 0; j < strlen(command); k++) {
            fileLocation[k] = temp[j];
            j++;
        }
        fileLocation[k] = '\0';

        fd = open(fileLocation, O_RDONLY);
        dup2(fd, 0); // Redirect stdin to file pointed to by fd
        exec(command);
    }

    // No redirect
    if (redirectType == 0) {
        printf("Redirect not found in command\n");
        return;
    }
}

int main(int argc, char *argv[]) {

    char command[256], temp[256];
    int pid;
    char *token;

    while(1) {
        printf("~~~~~~~~~~~~~~~BRANDON SHELL~~~~~~~~~~~~~~~~~~~");
        printf("Enter a command: ");
        gets(command);

        // If the user pressed enter, cycle through the loop
        if (command[0] == 0 || command[0] == ' ') {
            continue;
        }

        // Preserve the command
        strcpy(temp, command);

        token = strtok(temp, " ");

        if (strcmp("cd", token) == 0) {
            token = strtok(0, " ");
            chdir(token);
        }
        else { 
            pid = fork();

            if (pid < 0) {
                printf("No child processes available. Exiting....\n");
                exit(1);
            }

            if (pid) {
                // Parent sh waits for the child to die
                pid = wait(&status);
            } 
            else {
                printf("Command = %s \n", command);

                if (containsPipe(command) == 0) {
                    doRedirect(command, getRedirectType(command));
                    exec(command);
                }
                else {
                    printf("DOING PIPE\n");
                    doPipe(command);
                }
            }
        }
    }
}
