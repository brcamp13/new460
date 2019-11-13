/*********  t.c file *********************/
// Brandon Campbell Fall 2019

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int prints(char *s)
{
    for(int i=0; i < strlen(s); i++){
        putc(s[i]);
    }
}

int gets(char s[])
{
    while(*s = getc() != '\r') {
        *s++;
    }
    return s;
}

main()
{
    char ans[64];
    while(1){
        prints("What's your name? ");
        gets(ans);  
        if (ans[0]==0){
            break;
        }
        prints("Welcome "); 
        prints(ans);
        prints("\n\r");
    }
    prints("return to assembly and hang\n\r");
}