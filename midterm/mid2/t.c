#include "type.h"
#include "string.c"
PIPE *kpipe; // global pipe pointer

#include "uart.c"
#include "vid.c"
#include "exceptions.c"
#include "kernel.c"
#include "wait.c"
#include "pipe.c"
#include "timer.c"

void copy_vectors(void) {
    extern u32 vectors_start;
    extern u32 vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;

    while(vectors_src < &vectors_end)
       *vectors_dst++ = *vectors_src++;
}
int kprintf(char *fmt, ...);

void IRQ_handler()
{
    int vicstatus, sicstatus;
    int ustatus, kstatus;

    // read VIC status register to find out which interrupt
    vicstatus = VIC_STATUS; // VIC_STATUS=0x10140000=status reg
    sicstatus = SIC_STATUS;  
    if (vicstatus & 0x80000000){
       if (sicstatus & 0x08){
          kbd_handler();
       }
    }
}

int main() {
    fbuf_init();
    kprintf("Welcome to Wankix in ARM\n");
    uart_init();
    kbd_init();
    kpipe = &pipe;
    kpipe->head = kpipe->tail = 0;
    kpipe->room = 0;
    kpipe->room = PSIZE;
    kernel_init();
    kfork((int)pipe_writer, 1); // pipe writer process
    kfork((int)pipe_reader, 1); // pipe reader process
    printf("Forked pipes\n");
    while (1) {
        tswitch();
    }
}