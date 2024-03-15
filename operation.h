#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define PARTITION_SIZE 1024
#define MAX_FILES 10

// Structure représentant les informations d'un fichier
typedef struct {
    char name[100];
    int size;
    int start_block;
    bool is_free;
} FileInfo;

// Structure représentant la partition de fichiers
typedef struct {
    FileInfo files[MAX_FILES];  // Tableau des fichiers
    int free_blocks;            // Nombre de blocs libres dans la partition
    int next_free_block;        // Prochain bloc libre disponible
} Partition;

FILE* partition_file;  // Pointeur vers le fichier de la partition
Partition partition;   // Instance de la partition

// Fonction pour formater une nouvelle partition avec un nom spécifié
int myFormat(char* partitionName) {
    partition_file = fopen(partitionName, "wb");  // Ouvre le fichier en mode écriture binaire
    if (!partition_file) {
        return 0;  // Échec de l'ouverture du fichier
    }

    // Initialise la partition à zéro
    memset(&partition, 0, sizeof(Partition));
    partition.free_blocks = PARTITION_SIZE;  // Initialise le nombre de blocs libres
    partition.next_free_block = 0;           // Initialise le prochain bloc libre

    // Écrit la partition dans le fichier
    fwrite(&partition, sizeof(Partition), 1, partition_file);
    fclose(partition_file);  // Ferme le fichier

    return 1;  // Formatage réussi
}

// Fonction pour ouvrir un fichier avec un nom spécifié
file* myOpen(char* fileName) {
    int i;
    for (i = 0; i < MAX_FILES; i++) {
        if (strcmp(partition.files[i].name, fileName) == 0) {
            return (file*) (i + 1);  // Le fichier existe déjà, renvoie son index
        }
    }

    // Vérifie s'il reste des blocs libres dans la partition
    if (partition.free_blocks < 1) {
        return NULL;  // Aucun bloc libre disponible, renvoie NULL
    }

    // Crée un nouveau fichier avec le nom spécifié
    strcpy(partition.files[i].name, fileName);
    partition.files[i].size = 0;                 // Initialise la taille du fichier à zéro
    partition.files[i].start_block = partition.next_free_block;  // Définit le bloc de départ
    partition.files[i].is_free = false;           // Marque le fichier comme occupé

    partition.free_blocks--;  // Décrémente le nombre de blocs libres
    partition.next_free_block++;  // Met à jour l'indice du prochain bloc libre
    return (file*) (i + 1);  // Retourne le pointeur vers le fichier ouvert
}

// Fonction pour écrire dans un fichier ouvert
int myWrite(file* f, void* buffer, int nBytes) {
    int file_index = (int) f - 1;  // Calcule l'index du fichier à partir du pointeur
    int blocks_needed = (nBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;  // Calcule le nombre de blocs nécessaires

    // Vérifie si l'écriture dépasse la taille maximale du fichier
    if (partition.files[file_index].size + blocks_needed > BLOCK_SIZE) {
        return 0;  // Écriture impossible, dépassement de la taille maximale
    }

    int start_block = partition.files[file_index].start_block;  // Bloc de départ
    int current_block = start_block;

    for (int i = 0; i < blocks_needed; i++) {
        // Déplace le curseur du fichier au début du bloc
        if (fseek(partition_file, start_block * BLOCK_SIZE, SEEK_SET) != 0) {
            return 0;  // Échec du déplacement du curseur
        }

        // Écrit les données dans le fichier
        int bytes_written = fwrite(buffer, 1, nBytes, partition_file);
        if (bytes_written == 0) {
            return 0;  // Écriture échouée
        }

        nBytes -= bytes_written;  // Met à jour le nombre d'octets restants à écrire
        buffer += bytes_written;   // Avance le pointeur du buffer

        current_block++;
        start_block++;
    }

    partition.files[file_index].size += blocks_needed * BLOCK_SIZE;  // Met à jour la taille du fichier

    return blocks_needed * BLOCK_SIZE;  // Retourne le nombre total d'octets écrits
}

// Fonction pour lire à partir d'un fichier ouvert
int myRead(file* f, void* buffer, int nBytes) {
    int file_index = (int) f - 1;  // Calcule l'index du fichier à partir du pointeur
    int blocks_needed = (nBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;  // Calcule le nombre de blocs nécessaires

    int start_block = partition.files[file_index].start_block;  // Bloc de départ
    int current_block = start_block;

    for (int i = 0; i < blocks_needed; i++) {
        // Déplace le curseur du fichier au début du bloc
        if (fseek(partition_file, start_block * BLOCK_SIZE, SEEK_SET) != 0) {
            return 0;  // Échec du déplacement du curseur
        }

        // Lit les données à partir du fichier dans le buffer
        int bytes_read = fread(buffer, 1, nBytes, partition_file);
        if (bytes_read == 0) {
            return 0;  // Lecture échouée
        }

        nBytes -= bytes_read;  // Met à jour le nombre d'octets restants à lire
        buffer += bytes_read;   // Avance le pointeur du buffer

        current_block++;
        start_block++;
    }

    return blocks_needed * BLOCK_SIZE;  // Retourne le nombre total d'octets lus
}

// Fonction pour déplacer le curseur de lecture/écriture d'un fichier
void mySeek(file* f, int offset, int base) {
    int file_index = (int) f - 1;  // Calcule l'index du fichier à partir du pointeur
    int start_block = partition.files[file_index].start_block;  // Bloc de départ
    int block_size = BLOCK_SIZE;  // Taille du bloc

    if (base == SEEK_SET) {
        start_block += offset / block_size;  // Déplace le curseur au début du fichier
    } else if (base == SEEK_CUR) {
        start_block += (offset / block_size) + partition.files[file_index].size / block_size;  // Déplace le curseur en fonction de la position actuelle
    } else if (base == SEEK_END) {
        start_block += (offset / block_size) + (PARTITION_SIZE / block_size);  // Déplace le curseur à la fin du fichier
    }

    fseek(partition_file, start_block * block_size, SEEK_SET);  // Déplace effectivement le curseur
}
