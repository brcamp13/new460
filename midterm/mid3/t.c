#include "type.h"
#include "string.c"

char *tab = "0123456789ABCDEF";
int color;

#include "exceptions.c"
#include "kernel.c"
#include "wait.c"
#include "timer.c"
#include "vid.c"
#include "tqe.c"


void copy_vectors(void) {
    extern u32 vectors_start, vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;
    while(vectors_src < &vectors_end) {
        *vectors_dst++ = *vectors_src++;
    }
}

void timer_handler();
TIMER *tp[4];

void IRQ_handler()
{
    int vicstatus = VIC_STATUS;
    int sicstatus = SIC_STATUS;
    if (vicstatus & (1<<4)) {
        if(*(tp[0]->base+TVALUE)==0) {
            timer_handler(0);
        }
        if(*(tp[1]->base+TVALUE)==0) {
            timer_handler(1);
        }
    }
    if (vicstatus & (1<<5)) {
        if (*(tp[2]->base+TVALUE)==0) {
            timer_handler(2);
        }
        if (*(tp[3]->base+TVALUE)==0) {
            timer_handler(3);
        }
    }
    if (vicstatus & 0x80000000) {
        if (sicstatus & 0x08) {
            kbd_handler();
        }
    }

}

int kpchar(char c, int ro, int co);

int main()
{
    int i;
    color = RED;
    fbuf_init();
    kbd_init();
    printf("Main starts\n");
    VIC_INTENABLE = 0;
    VIC_INTENABLE |= (1<<4);
    VIC_INTENABLE |= (1<<5);
    VIC_INTENABLE |= (1<<31);

    // enable kbd irq
    SIC_ENSET = 1 << 3;
    SIC_PICENSET = 1 << 3;
    
    timer_init();
    for (i=0; i<2; i++) {
        tp[i] = &timer[i];
        timer_start(i);
    }
    
    kernel_init();
    tqe_init();
    
    kfork((int)body, 1);
    kfork((int)body, 1);
    kfork((int)body, 1);
    kfork((int)body, 1);
    
    while(1) {
        if(readyQueue) {
            tswitch();
        }
    }
}
