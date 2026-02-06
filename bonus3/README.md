```
bonus3@RainFall:~$ ll
total 17
dr-xr-x---+ 1 bonus3 bonus3   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 bonus3 bonus3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus3 bonus3 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 end    users  5595 Mar  6  2016 bonus3*
-rw-r--r--+ 1 bonus3 bonus3   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus3 bonus3  675 Apr  3  2012 .profile
bonus3@RainFall:~$ objdump -S -M intel ./bonus3 > /tmp/b
bonus3@RainFall:~$ vim /tmp/b
```

Let's begin.
```
80484f4:       55                      push   ebp
80484f5:       89 e5                   mov    ebp,esp
80484f7:       57                      push   edi
80484f8:       53                      push   ebx
80484f9:       83 e4 f0                and    esp,0xfffffff0
80484fc:       81 ec a0 00 00 00       sub    esp,0xa0
8048502:       ba f0 86 04 08          mov    edx,0x80486f0
8048507:       b8 f2 86 04 08          mov    eax,0x80486f2
804850c:       89 54 24 04             mov    DWORD PTR [esp+0x4],edx
8048510:       89 04 24                mov    DWORD PTR [esp],eax
8048513:       e8 f8 fe ff ff          call   8048410 <fopen@plt>
8048518:       89 84 24 9c 00 00 00    mov    DWORD PTR [esp+0x9c],eax
```
basic stack frame plus buffer of size 160
we push two unknown value so as always, we go into gdb:
```
(gdb) x/s 0x80486f0
0x80486f0:       "r"
(gdb) x/s 0x80486f2
0x80486f2:       "/home/user/end/.pass"
```

and then we call fopen.
So we have a File * opened with most likely the  the last level flag
and we store the result at 0x9c in the stack

```
804851f:       8d 5c 24 18             lea    ebx,[esp+0x18]
8048523:       b8 00 00 00 00          mov    eax,0x0
8048528:       ba 21 00 00 00          mov    edx,0x21
804852d:       89 df                   mov    edi,ebx
804852f:       89 d1                   mov    ecx,edx
8048531:       f3 ab                   rep stos DWORD PTR es:[edi],eax
```
Well Well Well do we not know that ?
so once again it's a bzero optimized for ASM, 
it bzero's a buffer which is starts at esp + 24 
So it means it's size is:
160 - 24 = 136.
the loop is made 0x21 times, 33times, plus it's a DWORD
so we initialize 4 * 33 = 132 bytes to 0, we have 4 left !

```
8048533:       83 bc 24 9c 00 00 00    cmp    DWORD PTR [esp+0x9c],0x0
804853a:       00
804853b:       74 06                   je     8048543 <main+0x4f>
804853d:       83 7d 08 02             cmp    DWORD PTR [ebp+0x8],0x2
8048541:       74 0a                   je     804854d <main+0x59>
8048543:       b8 ff ff ff ff          mov    eax,0xffffffff
8048548:       e9 c8 00 00 00          jmp    8048615 <main+0x121>
804854d:       8d 44 24 18             lea    eax,[esp+0x18]
8048551:       8b 94 24 9c 00 00 00    mov    edx,DWORD PTR [esp+0x9c]
8048558:       89 54 24 0c             mov    DWORD PTR [esp+0xc],edx
804855c:       c7 44 24 08 42 00 00    mov    DWORD PTR [esp+0x8],0x42
8048563:       00
8048564:       c7 44 24 04 01 00 00    mov    DWORD PTR [esp+0x4],0x1
804856b:       00
804856c:       89 04 24                mov    DWORD PTR [esp],eax
804856f:       e8 5c fe ff ff          call   80483d0 <fread@plt>
```
We then compare the File * to 0, just to be sure we didn't fail
if we failed, then we put -1 to eax, and return
If everything worked properly we put our buffer into eax,
we push the File * to the fourth arg, 
we push 66 into the 3rd arg
we push 1 as the 2nd arg, (which is the size of the elements we are gonna read, 1 because they are chars)
To finish we push our buffer as the first arg
and we call fread

```
8048574:       c6 44 24 59 00          mov    BYTE PTR [esp+0x59],0x0
8048579:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804857c:       83 c0 04                add    eax,0x4
804857f:       8b 00                   mov    eax,DWORD PTR [eax]
8048581:       89 04 24                mov    DWORD PTR [esp],eax
8048584:       e8 a7 fe ff ff          call   8048430 <atoi@plt>
8048589:       c6 44 04 18 00          mov    BYTE PTR [esp+eax*1+0x18],0x0
804858e:       8d 44 24 18             lea    eax,[esp+0x18]
8048592:       8d 50 42                lea    edx,[eax+0x42]
8048595:       8b 84 24 9c 00 00 00    mov    eax,DWORD PTR [esp+0x9c]
804859c:       89 44 24 0c             mov    DWORD PTR [esp+0xc],eax
80485a0:       c7 44 24 08 41 00 00    mov    DWORD PTR [esp+0x8],0x41
80485a7:       00
80485a8:       c7 44 24 04 01 00 00    mov    DWORD PTR [esp+0x4],0x1
80485af:       00
80485b0:       89 14 24                mov    DWORD PTR [esp],edx
80485b3:       e8 18 fe ff ff          call   80483d0 <fread@plt>```
```
So here we put a '\0' in the middle of our buffer
0x59 - 0x18 = 65
So index 65 = '\0'
then we do an atoi of our av[1]
Ok so now what do we do ?
We put a '\0' at the index esp + 24 + atoi(av[1]) * 1
This is the equivalent of 
```
buffer[atoi(av[1])] = '\0'
```
after that we take our buffer, displace it's start to esp + 0x18 + 0x42 = 90 which means buffer size 70
we read again in the same file but this time
we still read characters, but only 0x41 (65) instead of 0x42, they are still 5 remaining

```
80485b8:       8b 84 24 9c 00 00 00    mov    eax,DWORD PTR [esp+0x9c]
80485bf:       89 04 24                mov    DWORD PTR [esp],eax
80485c2:       e8 f9 fd ff ff          call   80483c0 <fclose@plt>
```
Do I need to explain ?
(we close the File \*)
```
80485c7:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
80485ca:       83 c0 04                add    eax,0x4
80485cd:       8b 00                   mov    eax,DWORD PTR [eax]
80485cf:       89 44 24 04             mov    DWORD PTR [esp+0x4],eax
80485d3:       8d 44 24 18             lea    eax,[esp+0x18]
80485d7:       89 04 24                mov    DWORD PTR [esp],eax
80485da:       e8 d1 fd ff ff          call   80483b0 <strcmp@plt>
80485df:       85 c0                   test   eax,eax
80485e1:       75 1e                   jne    8048601 <main+0x10d>
```
now now now,
we take the av, add 4 to make it av[1] put it into eax.
we compare what we have put to what the file contains
Of course if we fail, we get beaten up (we just return)
```
80485e3:       c7 44 24 08 00 00 00    mov    DWORD PTR [esp+0x8],0x0
80485ea:       00
80485eb:       c7 44 24 04 07 87 04    mov    DWORD PTR [esp+0x4],0x8048707
80485f2:       08
80485f3:       c7 04 24 0a 87 04 08    mov    DWORD PTR [esp],0x804870a
80485fa:       e8 21 fe ff ff          call   8048420 <execl@plt>
80485ff:       eb 0f                   jmp    8048610 <main+0x11c>
8048601:       8d 44 24 18             lea    eax,[esp+0x18]
8048605:       83 c0 42                add    eax,0x42
8048608:       89 04 24                mov    DWORD PTR [esp],eax
804860b:       e8 d0 fd ff ff          call   80483e0 <puts@plt>
8048610:       b8 00 00 00 00          mov    eax,0x0
8048615:       8d 65 f8                lea    esp,[ebp-0x8]
```
Well no need to really check we just exec the /bin/sh
Let's try somthing

```
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cd ../end
$ ls -la
total 13
dr-xr-x---+ 1 end  end     80 Sep 23  2015 .
dr-x--x--x  1 root root   340 Sep 23  2015 ..
-rw-r--r--  1 end  end    220 Apr  3  2012 .bash_logout
-rw-r--r--  1 end  end   3489 Sep 23  2015 .bashrc
-rwsr-s---+ 1 end  users   26 Sep 23  2015 end
-r--r-----+ 1 end  end     65 Sep 23  2015 .pass
-rw-r--r--  1 end  end    675 Apr  3  2012 .profile
$ cat .pass
<hidden>
```
Why does that works ?
Because we put at the index 0 a \0, plus we have as an av[1] a '\0'
so when we strcmp, we stop at '\0'
Good job, this is a funny troll
