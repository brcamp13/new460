// uart.c file

#define UDR   0x00
#define UFR   0x18

#define RXFE 0x10
#define TXFF 0x20

typedef volatile struct uart{
  char *base;
  int n;
}UART;

UART uart[4];

int uart_init()
{
    int i;
    UART *up;
    for(i=0; i<4; i++){
        up = &uart[i];
        up->base = (char *)(0x101F1000 + i * 0x1000);
        up->n = i;
    }
    uart[3].base = (char *)(0x10009000);
}

int ugetc(UART *up)
{
    while (*(up->base + UFR) & RXFE)
    return *(up->base + UDR);
}

int uputc(UART *up, char c)
{
    while (*(up->base + UFR) & TXFF)
      ;
    *(up->base + UDR) = c;
}

int ugets(UART *up, char s[])
{
    while ((*s = ugetc(up)) != '\r') {
        uputc(up, *s);
        s++;
    }
    *s = 0;
}

int uprints(UART *up, char *s)
{
    while (*s) {
        uputc(up, *s++);
    }
}

int urpu(UART *up, int x)
{  
    char c;
    char *tab = "0123456789ABCDEF";
    if (x){
       c = tab[x % 10];
       urpu(up, (x / 10));
       uputc(up, c);
    }
}

int uprintu(UART *up, int x)
{
   (x==0)? uputc(up, '0') : urpu(up, x);
   uputc(up, ' ');
}

int uprintd(UART *up, int x){
   if(x<0){
      uputc(up, '-');
      urpu(up, abs(x));
      uputc(up, ' ');
   }else{
      (x==0)? uputc(up, '0') : urpu(up, x);
      uputc(up, ' ');
   }
}

int urpx(UART *up, int x)
{  
    char c;
    char *tab = "0123456789ABCDEF";
    if (x){
       c = tab[x % 16];
       urpx(up, (x / 16));
       uputc(up, c);
    }
}

int uprintx(UART *up, int x){
   if(x==0){
      uputc(up, '0');
      uputc(up, ' ');
   }else{
      uputc(up, '0');
      uputc(up, 'x');
      urpx(up, x);
      uputc(up, ' ');
   }
}

//~~~~~~~~~~My PRINT~~~~~~~~~~
int ufprintf(UART *up, char *fmt, ...){

   int i = 0; 
   char *cp = fmt; //Access the format string; 
   int *ip = (int *)&fmt + 1; //Access the first argv variable in the stack 

   //If you encounter a '%' in the format string, use the correct print function to print the current variable pointed to by ip in the stack
   for (i = 0; i < strlen(cp); i++){
      if(cp[i] == '%'){
         
         i++; 

         switch(cp[i]) {
            case 's' :
               uprints(up, *ip);
               break;
            case 'c' :
               uputc(up, *ip);
               break;
            case 'u' :
               uprintu(up, *ip);
               break;
            case 'd' :
               uprintd(up, *ip);
               break;
            case 'x' :
                uprintx(up, *ip);
                break;
         }

         ip++;

      }else if(cp[i] == '\\' && cp[i+1] == 'n'){     //If you encounter '\n', put '\r' instead

         uputc(up, "\r");
         i++; 
         continue;

      }else{
         uputc(up, cp[i]);   //If just a non-special character, print it out
      }
   }

}
