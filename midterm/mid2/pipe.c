PIPE pipe[NPIPE];

int read_pipe(PIPE *p, char *buf, int n) {
    int r = 0;
    if (n <= 0) {
        return 0;
    }

        while (n) {
            while (p->data) {
                *buf++ = p->buf[p->tail++]; //read a byte to buf
                p->tail %= PSIZE;
                p->data--;
                p->room++;
                r++;
                n--;
                if (n==0) {
                    break;
                }
            }
        kwakeup(&p->room); // wakeup writers
        if (r) {    // if has read some data
            return r;
        }
        ksleep(&p->data); // sleep for datav
    }
}

int write_pipe(PIPE *p, char *buf, int n) {
    int r = 0;
    if (n <= 0) {
        return 0;
    }

    while (n) {
        while (p->room) {
            p->buf[p->head++] = *buf++; // write a byte to pipe
            p->head %= PSIZE;
            p->data++;
            p->room--;
            r++;
            n--;
            if (n == 0) {
                break;
            }
        }
        kwakeup(&p->data); // wakeup readers if any
        if (n == 0) {
            return r; // finished writing n bytes
        }
        // still has data to write but pipe has no room
        ksleep(&p->room); //sleep for room
    }
}

int pipe_writer() {  // pipe reader task code
    struct uart *up  = &uart[0];
    char line[] = "Brandon Campbell\0";
    while (1) {
        printf("task%d waits for line from UART0\n", running->pid);
        uprints(up, "\r\n");
        printf("task%d wrote lines=[%s] to pipe\n", running->pid, line);
        write_pipe(kpipe, line, strlen(line));
        break;
    }
}

int pipe_reader() {  // pipe writer task code
    char line[128];
    int i, n;
    while(1) {
        printf("task%d reading from pipe\n", running->pid);
        n = read_pipe(kpipe, line, 20);
        printf("task%d read n=%d bytes from pipe : [", running->pid, n);

        for (i=0; i<n; i++) {
            kputc(line[i]);
        }
        printf("]\n");
    }
}
