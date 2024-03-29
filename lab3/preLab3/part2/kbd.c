/************* kbd.c file **************/

#include "keymap2"
#include "keymap"

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

typedef volatile struct kbd{
  char *base;
  char buf[128];
  int head, tail, data, room;
}KBD;

volatile KBD kbd;
int count;
int release = 0;

int kbd_init()
{
  char scode;
  KBD *kp = &kbd;
  kp->base = (char *)0x10006000;
  *(kp->base + KCNTL) = 0x10; // bit4=Enable bit0=INT on
  *(kp->base + KCLK)  = 8;
  kp->head = kp->tail = 0;
  kp->data = 0; kp->room = 128;
  count = 0;  // number of KBD interrupts
  release = 0; // release key pressed?
}

void kbd_handler()
{
  u8 scode, c;
  int i;
  KBD *kp = &kbd;
  color = YELLOW;
  scode = *(kp->base + KDATA);

  if (scode & 0x80) {
    return;
  }

  c = unsh[scode];

  if (c != '\r') {
    printf("kbd interrupt: c=%x %c\n", c, c);
    kp->buf[kp->head++] = c;
    kp->head %= 128;
    kp->data--;
    kp->room++;
  }  
}

int kgetc()
{
  char c;
  KBD *kp = &kbd;
  unlock();
  while(kp->data <= 0);
  lock();
    c = kp->buf[kp->tail++];
    kp->tail %= 128;
    kp->data--; kp->room++;
    unlock();
    return c;
}

int kgets(char s[])
{
  char c;
  while((c=kgetc()) != '\r'){
    *s++ = c;
  }
  *s=0;
  return strlen(s);
}
