// Note: password file line is as follows: username:password:gid:uid:fullname:HOMEDIR:program
#include "ucode.c"

int in, out, err, passwordFD, uid, gid;
char username[128], password[128];
char tokens[7][64];

main(int argc, char *argv[])
{
    // Close FDs 0,1 inherited from init 
    close(0);
    close(1);

    // Open   argv[1] 3 times as  in(0), out(1), err(2)
    in = open(argv[1], 0);
    out = open(argv[1], 1);
    err = open(argv[1], 2);

    // settty(argv[1]) set tty name string in proc.tty
    settty(argv[1]);

    // while  1
    while (1) {

        // Printf("login") gets(name);
        printf("Login : ");
        gets(username);

        // printf("password: ") gets (password);
        printf("Password : ");
        gets(password);

        if (ValidateUser(username, password) == 1) {

            // change uid, gid to user's uid, gid; //chuid()
            chuid(uid, gid);

            // change cwd to user's home DIR // chdir()
            chdir(tokens[5]);

            // exec to program in user account // exec()
            exec("sh");
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

    char *token;
    char line[256];
    int n = 0;

    // for each line in /etc/passwd file do
    while (n = GetLine(line)) {

        TokenizeLine(line);

        // If it is a match
        if (strcmp(username, tokens[0]) == 0 && strcmp(password, tokens[1]) == 0) {

            uid = atoi(tokens[2]);
            gid = atoi(tokens[3]);
    
            // Close password file and return success
            close(passwordFD);
            return 1;
            
        }
    }

    // Close the password file and return failure
    close(passwordFD);
    return 0;

}

int GetLine(char *buffer) {
    int i = 0;
    char temp[5] = "";

    // Clear the buffer
    strcpy(buffer, "");
    while (strcmp(temp, "\n") != 0) {
        if (read(passwordFD, temp, 1) > 0) {
            strcat(buffer, temp);
            i++;
        }
        else {
            break;
        }
    }
    return i;
}


int TokenizeLine(char *line)
{

    int i = 0;
    int j = 0;

    while (*line != 0) {

        if (*line != ':') {

            tokens[i][j] = *line;
            j++;

        } else {

            tokens[i][j++] = 0;
            i++;
            j = 0;
            
        }

        *line++;

    }

    tokens[i][j] = 0;

}
