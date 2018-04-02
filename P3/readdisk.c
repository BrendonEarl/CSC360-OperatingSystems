#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     // file control open
#include <sys/stat.h>  // stat struct
#include <sys/mman.h>  // mem managment
#include <arpa/inet.h> // byte endian conversion
#include <string.h>    // error stringifying
#include <errno.h>     // provides thrown errnos
#include <math.h>      // pow function
#include "disk.h"

FILE *readdisk(char *diskpath)
{
    FILE *img;
    img = fopen(diskpath, "r");

    /* Handle failed file open */
    if (img == NULL)
    {
        printf("\nfopen() Failed with code [%s]\n\n", strerror(errno));
        return NULL;
    }
    else
    {
        printf("\nfopen() Successful\n\n");
    }

    return img;
}

struct superblock_t readsuperblock(FILE *img)
{
    struct superblock_t superblock;
    fread(&superblock, sizeof(superblock), 1, img);

    return superblock;
}

void printsuperblock(struct superblock_t sb)
{
    unsigned long i;

    printf("Super block information:\n");
    unsigned long id = 0;
    unsigned long id_size = (sizeof(sb.fs_id) / sizeof(sb.fs_id[0]));
    for (i = 0; i < id_size; i++)
    {
        id += sb.fs_id[i] * pow(2, (id_size - i));
    }
    printf("Block size: %d\n", htons(sb.block_size));
    printf("Block count: %d\n", htonl(sb.file_system_block_count));
    printf("FAT starts: %d\n", htonl(sb.fat_start_block));
    printf("FAT blocks: %d\n", htonl(sb.fat_block_count));
    printf("Root directory start: %d\n", htonl(sb.root_dir_start_block));
    printf("Root directory blocks: %d\n\n", htonl(sb.root_dir_block_count));
}

void printfatinfo(struct superblock_t sb)
{
    unsigned long blksize = htons(sb.block_size);
    unsigned long size = sizeof(struct dir_entry_t);
    unsigned long fatlen = (htonl(sb.fat_block_count) - htonl(sb.fat_start_block)) * blksize / size;

    unsigned long freeblocks = 0;
    unsigned long reservedblocks = 0;
    unsigned long allocatedblocks = 0;

    unsigned long i;
    for (i = 0; i < fatlen; i++)
    {
        // int h = direntries[i].status;
    }
    printf("FAT information:\n");
    printf("Free Blocks: %d\n", -1);
    printf("Reserved Blocks: %d\n", -1);
    printf("Allocated Blocks: %d\n", -1);
}
