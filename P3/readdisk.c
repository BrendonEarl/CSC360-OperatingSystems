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
#include <time.h>

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

    return img;
}

FILE *writedisk(char *diskpath)
    {
    FILE *img;
    img = fopen(diskpath, "w");

    /* Handle failed file open */
    if (img == NULL)
    {
        printf("\nfopen() Failed with code [%s]\n\n", strerror(errno));
        return NULL;
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
    for (i = 0; i < fatlen / sizeof(fatblock); i++)
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

void parsetime(char *strout, struct dir_entry_timedate_t direntrytd)
{
    struct tm dirtimedate;
    dirtimedate.tm_year = htons(direntrytd.year) - 1900;
    dirtimedate.tm_mon = direntrytd.month - 1;
    dirtimedate.tm_mday = direntrytd.day;
    dirtimedate.tm_hour = direntrytd.hour;
    dirtimedate.tm_min = direntrytd.minute;
    dirtimedate.tm_sec = direntrytd.second;
    dirtimedate.tm_isdst = -1;

    strftime(strout, 20, "%Y/%m/%d %H:%M:%S", &dirtimedate);
}

void printfolderinfo(FILE *img, struct superblock_t sb)
{
    unsigned long blksize = htons(sb.block_size);
    unsigned long rootstart = htonl(sb.root_dir_start_block) * blksize;
    unsigned long rootend = rootstart + htonl(sb.root_dir_block_count) * blksize;
    unsigned long rootlen = rootend - rootstart;

    struct dir_entry_t direntry;
    fseek(img, rootstart, SEEK_SET);

    unsigned long i;
    for (i = 0; i < rootlen / sizeof(direntry); i++)
    {
        fread(&direntry, sizeof(struct dir_entry_t), 1, img);

        if (direntry.size > 0)
        {
            char type;
            if ((direntry.status & 0x02) == 0x02)
                type = 'F';
            else if ((direntry.status & 0x04) == 0x04)
                type = 'D';
            else
                type = '\0';

            char datetime[20];
            parsetime(datetime, direntry.modify_time);
            printf("%c %10d %30s %s\n", type, htonl(direntry.size), direntry.filename, datetime);
        }
    }
}
