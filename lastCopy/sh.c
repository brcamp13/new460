#include "ucode.c"

int status;


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

int doRedirect(char *command, int redirectType) {

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
