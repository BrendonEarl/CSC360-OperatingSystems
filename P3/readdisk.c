#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    printf("Super block information:\n");
    printf("Block size: %d\n", htons(sb.block_size));
    printf("Block count: %d\n", htonl(sb.file_system_block_count));
    printf("FAT starts: %d\n", htonl(sb.fat_start_block));
    printf("FAT blocks: %d\n", htonl(sb.fat_block_count));
    printf("Root directory start: %d\n", htonl(sb.root_dir_start_block));
    printf("Root directory blocks: %d\n\n", htonl(sb.root_dir_block_count));
}

void summarizefat(FILE *img, struct superblock_t sb)
{
    unsigned long blksize = htons(sb.block_size);
    unsigned long fatstart = htonl(sb.fat_start_block) * blksize;
    unsigned long fatend = fatstart + htonl(sb.fat_block_count) * blksize;
    unsigned long fatlen = fatend - fatstart;

    uint32_t fatblock;
    fseek(img, fatstart, SEEK_SET);

    unsigned long freeblocks = 0;
    unsigned long reservedblocks = 0;
    unsigned long allocatedblocks = 0;

    unsigned long i;
    for (i = 0; i < fatlen / 4; i++)
    {
        fread(&fatblock, sizeof(uint32_t), 1, img);
        if (htonl(fatblock) == 0x00000000)
        {
            freeblocks += 1;
        }
        else if (htonl(fatblock) == 0x00000001)
        {
            reservedblocks += 1;
        }
        else
        {
            allocatedblocks += 1;
        }
    }

    printf("FAT information:\n");
    printf("Free Blocks: %lu\n", freeblocks);
    printf("Reserved Blocks: %lu\n", reservedblocks);
    printf("Allocated Blocks: %lu\n", allocatedblocks);
}

void printfileinfo(struct superblock_t sb, struct dir_entry_t *direntries)
{
    unsigned long blksize = htons(sb.block_size);
    unsigned long size = sizeof(struct dir_entry_t);
    unsigned long fatlen = (htonl(sb.fat_block_count) - htonl(sb.fat_start_block)) * blksize / size;

    unsigned long i;
    for (i = 0; i < fatlen; i++)
    {
        char type;
        if ((flipbyte(direntries[i].status) & 0x02) == 0x02)
            type = 'D';
        else
            type = 'F';
        char filename[31];
        unsigned int j;
        for (j = 0; j < 31; j++)
            filename[j] = htonb(direntries[i].filename[j]);
        printf("%c %10d %30s\n", type, htonl(direntries[i].size), filename);
    }
}
