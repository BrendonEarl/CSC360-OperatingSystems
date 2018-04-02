// #include <stdlib.h>
// #include <stdio.h>
// #include <arpa/inet.h>
#include "readdisk.c"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    FILE *img = readdisk(argv[1]);
    struct superblock_t superblock = readsuperblock(img);
    printsuperblock(superblock);
    summarizefat(img, superblock);
    return 0;
}
