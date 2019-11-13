#define TRK 18
#define CYL 36
#define BLK 1024

typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

u16 NSEC = 2;
char buf1[BLK], buf2[BLK];

int prints(char *s)
{
    while(*s) {
        kputc(*s++);
    }
}

int gets(char *s)
{ 
    while((*s = getc()) != '\r') {
        kputc(*s++);
    }
    *s = 0;
}

// u16 getblk(u16 blk, char *buf)
// {
//     readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
// }

u16 search(INODE *ip, char *name)
{
    int i;
    char c;
    DIR *dp;
    for (i=0; i<12; i++) {
        if ((u16)ip->i_block[i]) {
            getblock((u16)ip->i_block[i], buf2);
            dp = (DIR *)buf2;
            while((char *)dp < &buf2[BLK]) {
                c = dp->name[dp->name_len]; // Save last byte
                dp->name[dp->name_len] = 0; // Make name into a string
                prints(dp->name); kputc(' ');
                if (strcmp(dp->name, name) == 0) {
                    prints("\n\r");
                    return((u16)dp->inode);
                }
                dp->name[dp->name_len] = c; // Restore last byte
                dp = (char *)dp + dp->rec_len;
            }
        }
    }
}

int load(char *filename, PROC *p)
{ 
    char *cp, *name[2], *addr;
    u16 i, ino, blk, iblk;
    u32 *up;
    GD *gp;
    INODE *ip;
    DIR *dp;

    name[0] = "boot"; name[1] = filename;
    prints("bootname: ");
    if (filename[0]==0) {
        name[1] = "mtx";
    }else {
        name[1] = filename;
    }

    addr = (char *)(p->pgdir[2048] & 0xFFF0000); // Wang page 212
    getblock(2, buf1);
    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table;
    getblock(iblk, buf1);
    ip = (INODE *)buf1 + 1;
    for (i=0; i < 2; i++) {
        ino = search(ip, name[i]) - 1; 
        if (ino < 0) {
            error();
        }
        getblock(iblk+(ino/8), buf1);
        ip = (INODE *)buf1 + (ino % 8);
    }
    if ((u16)ip->i_block[12]) {
        getblock((u16)ip->i_block[12], buf2);
    }

    for(i=0; i<12; i++) {
        getblock((u16)ip->i_block[i], addr);
        kputc('*');
        addr += 1024;
    }
    if((u16)ip->i_block[12]) {
        up = (u32 *)buf2; 
        while(*up) {
            getblock((u16)*up, addr);
            *up++;
            addr += 1024;
            kputc('.');
        }
    }
    prints("Ready to go?");
    getc();
    return 1;
} 