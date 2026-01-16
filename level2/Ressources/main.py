import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
print(shellcode, len(shellcode))
