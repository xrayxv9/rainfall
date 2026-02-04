```
bonus2@RainFall:~$ ll
total 17
dr-xr-x---+ 1 bonus2 bonus2   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 bonus2 bonus2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus2 bonus2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus3 users  5664 Mar  6  2016 bonus2*
-rw-r--r--+ 1 bonus2 bonus2   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus2 bonus2  675 Apr  3  2012 .profile
bonus2@RainFall:~$ objdump -S -M intel ./bonus2 > /tmp/c
bonus2@RainFall:~$ vim /tmp/c
```

Ok So as usual we begin with a simple stack frame, to make sure everything is aligned and working properly
```
8048532:       81 ec a0 00 00 00       sub    esp,0xa0
```
We have a 160bytes of buffer, good to know

```
8048532:       81 ec a0 00 00 00       sub    esp,0xa0
8048538:       83 7d 08 03             cmp    DWORD PTR [ebp+0x8],0x3
804853c:       74 0a                   je     8048548 <main+0x1f>
804853e:       b8 01 00 00 00          mov    eax,0x1
8048543:       e9 e8 00 00 00          jmp    8048630 <main+0x107>
```
For the first time, we check whether our user has entered input values or not.
So if we have 3 args, we go to 8048548 which is the next step if not, we just return.

```
8048548:       8d 5c 24 50             lea    ebx,[esp+0x50]
804854c:       b8 00 00 00 00          mov    eax,0x0
8048551:       ba 13 00 00 00          mov    edx,0x13
8048556:       89 df                   mov    edi,ebx
8048558:       89 d1                   mov    ecx,edx
```
ebx is now a buffer of 80 bytes
eax, is NULL
edx, is 19
edi, is a pointer to the buffer
ecx, is 19 too

```
804855a:       f3 ab                   rep stos DWORD PTR es:[edi],eax
804855c:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804855f:       83 c0 04                add    eax,0x4
8048562:       8b 00                   mov    eax,DWORD PTR [eax]
8048564:       c7 44 24 08 28 00 00    mov    DWORD PTR [esp+0x8],0x28
```
Ok so here we do a while,
this while writes into edi, for 19 characters (19 because ecx is the counter) it writes eax, which is 0.
So we fill edi with NULL
So we have a memset, and more precisly a bzero
