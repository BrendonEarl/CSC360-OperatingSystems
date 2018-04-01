#include <stdlib.h>
// #include <stdio.h>
// #include <stdint.h>
// #include <sys/types.h>
// #include <time.h>

// Super block
struct __attribute__((__packed__)) superblock_t
{
    uint8_t fs_id[8];
    uint16_t block_size;
    uint32_t file_system_block_count;
    uint32_t fat_start_block;
    uint32_t fat_block_count;
    uint32_t root_dir_start_block;
    uint32_t root_dir_block_count;
};

// Time and date entry
struct __attribute__((__packed__)) dir_entry_timedate_t
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

// Directory entry
struct __attribute__((__packed__)) dir_entry_t
{
    uint8_t status;
    uint32_t starting_block;
    uint32_t block_count;
    uint32_t size;
    struct dir_entry_timedate_t modify_time;
    struct dir_entry_timedate_t create_time;
    uint8_t filename[31];
    uint8_t unused[6];
};

// struct stat
// {
//     dev_t st_dev;         /* ID of device containing file */
//     ino_t st_ino;         /* inode number */
//     mode_t st_mode;       /* protection */
//     nlink_t st_nlink;     /* number of hard links */
//     uid_t st_uid;         /* user ID of owner */
//     gid_t st_gid;         /* group ID of owner */
//     dev_t st_rdev;        /* device ID (if special file) */
//     off_t st_size;        /* total size, in bytes */
//     blksize_t st_blksize; /* blocksize for file system I/O */
//     blkcnt_t st_blocks;   /* number of 512B blocks allocated */
//     time_t st_atime;      /* time of last access */
//     time_t st_mtime;      /* time of last modification */
//     time_t st_ctime;      /* time of last status change */
// };
