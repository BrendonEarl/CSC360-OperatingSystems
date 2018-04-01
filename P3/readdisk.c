#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
// #include <fcntl.h>  // file control
#include <errno.h>  // provides thrown errnos
#include <string.h> // error stringifying
#include "disk.h"
// #include <sys/types.h>

FILE *readdisk(char *diskpath)
{
    FILE *img;
    img = fopen(diskpath, "r");

    /* Handle failed file open */
    if (img == NULL)
    {
        printf("\nfopen() Failed with code [%s]\n", strerror(errno));
        return NULL;
    }
    else
    {
        printf("\nfopen() Successful\n");
    }

    return img;
}

struct superblock_t readsuperblock(FILE *img)
{
    struct superblock_t superblock;
    fread(&superblock, sizeof(superblock), 1, img);

    return superblock;
}

void printsuperblock(struct superblock_t superblock)
{
    unsigned long i;

    printf("Super block information:\n");
    printf("Block size: %d\n", -1);
    printf("Block count: %d\n", -1);
    printf("FAT starts: %d\n", -1);
    printf("FAT blocks: %d\n", -1);
    printf("Root directory start: %d\n", -1);
    printf("Root directory blocks: %d\n", -1);

    printf("FAT information:");
    printf("Free Blocks: %d\n", -1);
    printf("Reserved Blocks: %d\n", -1);
    printf("Allocated Blocks: %d\n", -1);

    for (i = 0; i < (sizeof(superblock.fs_id) / sizeof(superblock.fs_id[0])); i++)
    {
        printf("%d ", superblock.fs_id[i]);
    }
}
