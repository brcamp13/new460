/*******************************************************
*                      t.c file                        *
*******************************************************/

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
#include <string.h>
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
GD    *gp;
INODE *ip;
DIR   *dp;

char buf1[BLK], buf2[BLK];
int color = 0x0A;
u8 ino;

main()
{ 
  u16    i, iblk, notEmpty;
  char   c, temp[64];

  prints("read block# 2 (GD)\n\r");
  getblk(2, buf1);

  // 1. WRITE YOUR CODE to get iblk = bg_inode_table block number
  getblk((u16)2, buf1);
  gp = (GD *)buf1;
  iblk = gp->bg_inode_table;
  getblk(iblk, buf1);
  prints("inode_block="); putc(iblk+'0'); prints("\n\r"); 

  // 2. WRITE YOUR CODE to get root inode
  ip = (INODE *)buf1 + 1;
  prints("read inodes begin block to get root inode\n\r");

  // 3. WRITE YOUR CODE to step through the data block of root inode
   for(i=0; i<15; i++) {
     if (ip->i_block[i]) {
       prints("Block: ");
       putc(i + '0');
     }
   }
   prints("read data block of root DIR\n\r");
   
  // 4. print file names in the root directory /
  getblk((u16)ip->i_block[0], buf2);
  dp = (DIR *)buf2;
  gp = (GD *)buf2;
  i=0;

  while(i<13) {
    strncpy(temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;
    if (temp == 0) {
      break;
      i++;
    } else {
      prints(temp);
      putc('\n');
      dp = (DIR *)((char *)dp + dp->rec_len);
      memset(temp, 0, strlen(temp));
      i++;
    }
  }


}  

int prints(char *s)
{
  u16 i;
  for(i=0; i < strlen(s); i++) {
    putc(s[i]);
  }
}

int gets(char s[]) 
{ 
  while(*s = getc() != '\r') {
    *s++;
  }
}

int getblk(u16 blk, char buf[])
{
  readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
}
