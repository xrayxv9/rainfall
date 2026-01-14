```
level0@RainFall:~$ ll
total 737
dr-xr-x---+ 1 level0 level0     60 Mar  6  2016 ./
dr-x--x--x  1 root   root      340 Sep 23  2015 ../
-rw-r--r--  1 level0 level0    220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level0 level0   3530 Sep 23  2015 .bashrc
-rwsr-x---+ 1 level1 users  747441 Mar  6  2016 level0*
-rw-r--r--  1 level0 level0    675 Apr  3  2012 .profile
```

```
level0@RainFall:~$ gdb --tui ./level0
(gdb) layout asm
(gdb) b main
(gdb) run
```

If you watch close enough you will see a call to the atoi function, which is why when you launch the binary alone you get :

```
level0@RainFall:~$ ./level0
Segmentation fault (core dumped)
```

They just never check the argc
So now we read the code on gdb and we see a weird line
```
0x8048ed9 <main+25>     cmp    $0x1a7,%eax
```
What does that mean ? that means thatwe check if the number we have given on the argv, is 1a7, which is decimal 423
So we put 423

```
level0@RainFall:~$ ./level0 423
$
```
mhh weird, that looks like a shell, wait IT IS ONE !
```
level0@RainFall:~$ ./level0 423
$ ls
ls: cannot open directory .: Permission denied
$ whoami
level1
$ pwd
/home/user/level0
```
So we just have to go into /home/user/level1 ?
```
$ cd ../level1
$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
$ cat .pass
<hidden>
```
And good job, This was not the hardest let's keep going
