// Note: password file line is as follows: username:password:gid:uid:fullname:HOMEDIR:program
#include "ucode.c"

int in, out, err, passwordFD, uid, gid;
char username[128], password[128];
char *passwordFileLine[6];

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
        gets(username);

        // printf("password: ") gets (password);
        printf("Password : ");
        gets(password);

        if (ValidateUser(username, password) == 1) {

            // change uid, gid to user's uid, gid; //chuid()
            chuid(uid, gid);

            // change cwd to user's home DIR // chdir()
            chdir(passwordFileLine[5]);

            // exec to program in user account // exec()
            char userProgram[64];
            strcpy(userProgram, "/bin/");
            strcat(userProgram, passwordFileLine[6]);
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

    char *token;
    char line[256];
    int n = 0;

    // for each line in /etc/passwd file do
    while (n = GetLine(line)) {

        TokenizeLine(line);

        // If it is a match
        if (strcmp(username, passwordFileLine[0]) == 0 && strcmp(password, passwordFileLine[1]) == 0) {

            uid = atoi(passwordFileLine[2]);
            gid = atoi(passwordFileLine[3]);
    
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

// int TokenizeLine(char *line) {
//     char *cp = line;
//     char *filler[64];
//     int i = 0;

//     while (*cp != 0) {
//         while (*cp != ':') {
//             strcat(passwordFileLine[i], *cp);
//         }
//         i++;
//         *cp++;
//     }
// }

int TokenizeLine(char *line)
{
  char *cp = line; 
  argc = 0;
  while (*cp != 0){
    while (*cp == ':')  *cp++ = 0;
    if (*cp != 0)// skip over blanks // token start
      passwordFileLine[argc++] = cp; // pointed by argv[ ]
    while (*cp != ':' && *cp != 0) // scan token chars
      cp++;
    if (*cp != 0)
      *cp = 0;
    else // end of token
      break; // end of line
    cp++;// continue scan
  } //end outer while
  passwordFileLine[argc] = 0; // argv[argc]=0
}
