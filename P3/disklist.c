#include "readdisk.c"

int main()
{
    FILE *img = readdisk("test.img");
    struct superblock_t superblock = readsuperblock(img);
    printfolderinfo(img, superblock);
}
