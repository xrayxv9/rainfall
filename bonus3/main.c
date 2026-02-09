#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *fp;
    char buffer[130]; // Espace réservé par sub esp, 0xa0
    // L'assembleur utilise lea ebx,[esp+0x18] pour le début du buffer
    char *pass_from_file = &buffer[0];    // Offset 0x18
    char *extra_buffer = &buffer[66];      // Offset 0x18 + 0x42 (66 en décimal)

    // Initialisation du buffer à zéro (rep stos)
    memset(buffer, 0, 0x21 * 4); // 132 octets mis à zéro

    // Ouverture du fichier de mot de passe
    fp = fopen("/home/user/end/.pass", "r");
    
    // Vérification : argc == 2 (nom du prog + 1 argument) et fichier ouvert
    if (fp == NULL || argc != 2) {
        return -1;
    }

    // Premier fread : lit 66 octets (0x42) dans le début du buffer
    fread(pass_from_file, 1, 0x42, fp);
    
    // Sécurise la fin de la première lecture
    buffer[65] = '\0'; // mov BYTE PTR [esp+0x59], 0x0 (0x18 + 0x41)

    // atoi sur l'argument utilisateur (argv[1])
    int index = atoi(argv[1]);
    
    // TRÈS SUSPECT : Écrit un octet nul à un index contrôlé par l'utilisateur
    // mov BYTE PTR [esp+eax*1+0x18], 0x0
    pass_from_file[index] = '\0';

    // Second fread : lit 65 octets (0x41) dans la suite du buffer
    fread(extra_buffer, 1, 0x41, fp);

    fclose(fp);

    // Comparaison entre le contenu du fichier et l'argument utilisateur
    if (strcmp(pass_from_file, argv[1]) == 0) {
        // Si ça matche, on lance un shell
        execl("/bin/sh", "sh", NULL);
    } else {
        // Sinon on affiche la seconde partie du fichier
        puts(extra_buffer);
    }

    return 0;
}
