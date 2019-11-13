/************* kbd.c file **************/

#include "keymap2"
#include "keymap"

#define KCNTL 0x00
#define KSTAT 0x04
#define KDATA 0x08
#define KCLK  0x0C
#define KISTA 0x10

#define CAPSLOCK         0x3A
#define LSHIFT           0x2A
#define RSHIFT           0x36
#define CONTROL          0x1D

typedef volatile struct kbd{
  char *base;
  char buf[128];
  int head, tail, data, room;
}KBD;

volatile KBD kbd;

int count;
int shift;
int control;
int capslock;
int release;

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
  shift = 0;
  control = 0;
  release = 0;
}

void kbd_handler()
{
  u8 scode, c;
  int i;
  KBD *kp = &kbd;

  color = YELLOW;
  scode = *(kp->base + KDATA);

  if (scode & 0x80){ // key release: ONLY catch release of shift, control
    scode &= 0x7F;   // mask out bit 7
    if (scode == LSHIFT || scode == RSHIFT)
      shift = 0;    // released the shift key
    if (scode == CONTROL)
      control = 0;  // released the Control key
    return;
  }

  // from here on, must be key press 
  if (scode == LSHIFT || scode == RSHIFT){
    shift = 1;
    return;
  }
  if (scode == CONTROL){
    control = 1;
    return;
  }

  if (scode == 0x3A){
    if (capslock) {
      capslock = 0;
    }else {
      capslock = 1;
    }
    return;
  }

  c = (shift ? sh[scode] : unsh[scode]);

  /* (1). write code to handle caps lock key *******/

  if (capslock) {
    printf("Capslock\n");
    c = sh[scode];
  }
  

  /* (2). write code to recognize Control-C key *******/

  if (control && (scode == 0x2E)){    // Control-C keys on PC, these are 2 keys
    printf("Control-c key\n");
    c = '\n'; // force a line, let proc handle #2 signal when exit Kmode
  }
  
  /* (3). write code to recognize Control-D key *******/

  if (control && (scode == 0x20)){  // Control-D, these are 2 keys
    printf("Control-d key\n");
    c = 4;   // Control-D
  }

  if (c != '\r') {
    printf("kbd interrupt: c=%x %c\n", c, c);
  } 

  kp->buf[kp->head++] = c;
  kp->head %= 128;
  kp->data--;
  kp->room++;
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
