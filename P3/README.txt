# Welcome to my Train Scheduler

## A little about me
Name: Brendon Earl  
Student #: V00797149  

## And why I wrote this code
Course: CSC 360 (Operating Systems)  
Assignment: #3 - FAT File System  

## How to use this thing
Ensure you have a C compiler setup  
run `make all` and let the makefile build it  

Run part 1 by executing `./readdisk <img_file.img>`  
Run part 2 by executing `./diskinfo <img_file.img> [<subdir>]`  
Run part 3 by executing `./diskget <img_file.img> <file_on_img> <dst_on_local>`  

where `<img_file.img>` is a local FAT img file without a bootblock.  
These parts unforuntatly do not support folders, so any additional arguments 
will not trigger searching in subdirectories.

## Expected behaviour  

If the input img file is mis-shaped (i.e. corrupted), this program is not intended 
to handle it, __it will crash__.  

For part 1, the output should look something like:  
```
➜ ./diskinfo test.img
Super block information:
Block size: 512
Block count: 6400
FAT starts: 2
FAT blocks: 50
Root directory start: 53
Root directory blocks: 8

FAT information:
Free Blocks: 6192
Reserved Blocks: 50
Allocated Blocks: 158
```

For part 2, the output should look something like:  
```
➜ ./disklist test.img
F        735                      mkfile.cc 2005/11/15 12:00:00
F       2560                        foo.txt 2005/11/15 12:00:00
F       3940                    disk.img.gz 2009/08/04 21:11:13
```

For part 3, there should be no output, unless a the file can't be found, 
if this is the case the output will look like:  
```
➜ ./diskget test.img foo.txtt test
File not found.
```

Unforuantely part 4 was not completed, nor is folder accessing.

__Happy Marking!__
