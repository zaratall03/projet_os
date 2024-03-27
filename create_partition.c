#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("partition.dat", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Erreur lors de la création du fichier de partition");
        return 1;
    }

    // Taille de la partition (en octets)
    off_t partition_size = 1000000; // 1 Mo

    // Tronquer le fichier à la taille spécifiée
    if (ftruncate(fd, partition_size) == -1) {
        perror("Erreur lors du tronquage du fichier de partition");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
