// #include <stdlib.h>
// #include <stdio.h>
// #include <arpa/inet.h>
#include "readdisk.c"

int main()
{
    FILE *img = readdisk("test.img");
    struct superblock_t superblock = readsuperblock(img);
    printsuperblock(superblock);
    struct dir_entry_t *direntries = readfat(img, superblock);

    // printfatinfo(*superblock, direntries);
}
