#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>  // provides thrown errnos
#include <string.h> // provides error stringifying
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
    unsigned long i = 0;
    for (i = 0; i < (sizeof(superblock.fs_id) / sizeof(superblock.fs_id[0])); i++)
    {
        printf("%d ", superblock.fs_id[i]);
    }
}
