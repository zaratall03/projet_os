#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_operation.h"
#define PARTITION_FILE_NAME "partition.bin"
#define FILENAME_MAX_LENGTH 100
#define SEEK_SET 0

// Définition de la structure de données pour représenter un fichier

typedef struct {
    FILE* partitionFile; // Descripteur de fichier de la partition
} FileSystem;

// Fonction de formatage de la partition
int myFormat(char *partitionName) {
    FILE* partition = fopen(PARTITION_FILE_NAME, "wb");
    if (partition == NULL) {
        printf("Erreur lors de la création de la partition.\n");
        return -1;
    }
    fclose(partition);
    return 0;
}

// Fonction pour ouvrir ou créer un fichier dans la partition
file* myOpen(char* fileName, FileSystem* fs) {
    // Vérifier si le fichier existe dans la partition, le cas échéant, ouvrez-le
    // Si le fichier n'existe pas, créez-le
    FILE* partition = fopen(PARTITION_FILE_NAME, "rb+");
    if (partition == NULL) {
        printf("Erreur lors de l'ouverture de la partition.\n");
        return NULL;
    }

    file* newFile = (file*)malloc(sizeof(file));
    if (newFile == NULL) {
        printf("Erreur lors de l'allocation de mémoire pour la structure de fichier.\n");
        fclose(partition);
        return NULL;
    }

    // Vérifier si le fichier existe dans la partition
    fseek(partition, 0, SEEK_END);
    long fileSize = ftell(partition);
    fseek(partition, 0, SEEK_SET);

    while (fread(newFile, sizeof(file), 1, partition)) {
        if (strcmp(newFile->fileName, fileName) == 0) {
            // Le fichier existe déjà dans la partition, retourne sa structure
            fclose(partition);
            return newFile;
        }
    }

    // Le fichier n'existe pas dans la partition, donc on le crée
    strcpy(newFile->fileName, fileName);
    newFile->fileSize = 0;
    newFile->currentPosition = 0;

    fseek(partition, 0, SEEK_END);
    fwrite(newFile, sizeof(file), 1, partition);

    fclose(partition);

    return newFile;
}

// Fonction pour écrire dans un fichier dans la partition
int myWrite(file* f, void* buffer, int nBytes, FileSystem* fs) {
    FILE* partition = fopen(PARTITION_FILE_NAME, "rb+");
    if (partition == NULL) {
        printf("Erreur lors de l'ouverture de la partition.\n");
        return -1;
    }

    fseek(partition, sizeof(file), SEEK_SET);
    while (fread(f, sizeof(file), 1, partition)) {
        if (strcmp(f->fileName, fileName) == 0) {
            // Le fichier est trouvé dans la partition, on peut écrire dedans
            fseek(partition, sizeof(file), SEEK_CUR);
            fwrite(buffer, 1, nBytes, partition);
            fclose(partition);
            return nBytes;
        }
    }

    // Le fichier n'est pas trouvé dans la partition
    fclose(partition);
    return -1;
}

// Fonction pour lire depuis un fichier dans la partition
int myRead(file* f, void* buffer, int nBytes, FileSystem* fs) {
    FILE* partition = fopen(PARTITION_FILE_NAME, "rb+");
    if (partition == NULL) {
        printf("Erreur lors de l'ouverture de la partition.\n");
        return -1;
    }

    fseek(partition, sizeof(file), SEEK_SET);
    while (fread(f, sizeof(file), 1, partition)) {
        if (strcmp(f->fileName, fileName) == 0) {
            // Le fichier est trouvé dans la partition, on peut lire dedans
            fseek(partition, sizeof(file), SEEK_CUR);
            int bytesRead = fread(buffer, 1, nBytes, partition);
            fclose(partition);
            return bytesRead;
        }
    }

    // Le fichier n'est pas trouvé dans la partition
    fclose(partition);
    return -1;
}

// Fonction pour déplacer le pointeur de lecture/écriture dans un fichier dans la partition
void mySeek(file* f, int offset, int base, FileSystem* fs) {
    FILE* partition = fopen(PARTITION_FILE_NAME, "rb+");
    if (partition == NULL) {
        printf("Erreur lors de l'ouverture de la partition.\n");
        return;
    }
}
    fseek(partition, sizeof(file), SEEK_SET);
    while (fread(f, sizeof(file), 1, partition)) {
        if (strcmp(f->fileName, fileName) == 0) {
            // Le fichier est trouvé dans la partition, on peut déplacer le pointeur
            fseek(partition, sizeof(file), SEEK_CUR);
            fseek(partition, offset, base);
        }
    }    