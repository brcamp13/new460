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
    //
}
