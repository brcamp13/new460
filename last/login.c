#include "ucode.c"

int in, out, err;
char name[128], password[128];

main(int argc, char *argv[])
{
    // Close FDs 0,1 inherited from init 
    // Open   argv[1] 3 times as  in(0), out(1), err(2)
    // settty(argv[1]) set tty name string in proc.tty
    // open /etc/passd file for read
    // while  1
    // Printf("login") gets(name);
    // printf("password: ") gets (password);
    // for each line in /etc/passwd file do 
    // tokenize user account line
    // if user has a valid account
    // change uid, gid to user's uid, gid; //chuid()
    // change cwd to user's home DIR // chdir()
    // close opened /etc/passwd file // close()
    // exec to program in user account // exec()
    // else, print that the login failed
}
