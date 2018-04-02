#include "readdisk.c"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    FILE *img = readdisk(argv[1]);
    struct superblock_t superblock = readsuperblock(img);
    copyout(img, superblock, argv[2], argv[3]);
    return 0;
}
