// Note: password file line is as follows: username:password:gid:uid:fullname:HOMEDIR:program
#include "ucode.c"

int in, out, err, passwordFD, uid, gid;
char name[128], password[128], homeDirectory[128], userProgram[128];

main(int argc, char *argv[])
{
    // Close FDs 0,1 inherited from init 
    close(0);
    close(1);

    // Open   argv[1] 3 times as  in(0), out(1), err(2)
    in = open(argv[1], O_RDONLY);
    out = open(argv[1], O_WRONLY);
    err = open(argv[1], O_WRONLY);

    // settty(argv[1]) set tty name string in proc.tty
    settty(argv[1]);

    // while  1
    while (1) {

        // Printf("login") gets(name);
        printf("Login : ");
        gets(name);

        // printf("password: ") gets (password);
        printf("Password : ");
        gets(password);

        if (ValidateUser(name, password) == 1) {

            // change uid, gid to user's uid, gid; //chuid()
            chuid(uid, gid);

            // change cwd to user's home DIR // chdir()
            chdir(homeDirectory);

            // exec to program in user account // exec()
            exec(userProgram);
        }
        else {
            printf("LOGIN FAILED\n");
        }
    }
    
     exit(0);

}

int ValidateUser(char username[], char password[]) {

    // open /etc/passd file for read
    passwordFD = open("/etc/passwd", O_RDONLY);
    if (passwordFD == 0) {
        printf("Can't find password file\n");
        return 0;
    }

    char *token, *delimeter = ":\n";
    char buffer[1024];

    // Read the password file 
    read(passwordFD, buffer, 1024);

    // Get first token (user name)
    token = strtok(buffer, delimeter);

    // for each line in /etc/passwd file do
    while (token != 0) {
        if (strcmp(token, username) == 0) {

            // Get the password
            token = strtok(0, delimeter);

            // If valid account
            if (strcmp(token, password) == 0) {

                // change uid, gid to user's uid, gid; //chuid()
                uid = atoi(strtok(0, delimeter));
                gid = atoi(strtok(0, delimeter));
                
                // Move past the user's full name
                strtok(0, delimeter);

                // Get the user home directory and the program name
                strcpy(homeDirectory, strtok(0, delimeter));
                strcpy(userProgram, strtok(0, delimeter));

                // Close the password file and return success
                close(passwordFD);
                return 1;
            }
        }

        // Get the username on the next line
        token = strtok(0, delimeter);

    }

    // Close the password file and return failure
    close(passwordFD);
    return 0;

}
