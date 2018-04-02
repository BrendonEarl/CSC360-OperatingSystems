#include "readdisk.c"

int main()
{
    FILE *img = readdisk("test.img");
    struct superblock_t superblock = readsuperblock(img);
    copyout(img, superblock, "foo.txt", "test");
}
