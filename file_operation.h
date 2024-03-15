
#define SUCCESS 0
#define FAILURE -1
#define FILENAME_MAX_LENGTH 100
#include <stdio.h>
#include<stdlib.h>

// Définition de la structure de données pour représenter un fichier
typedef struct {
    char* fileName;       // Nom du fichier
    FILE* filePtr;        // Descripteur de fichier
    size_t fileSize;      // Taille du fichier
    long currentPosition; // Position actuelle du pointeur de lecture/écriture
} file;


int myFormat(char* partitionName) {
    FILE* partition = fopen(partitionName, "wb");
    if (partition == NULL) {
        printf("Erreur lors de la création de la partition.\n");
        return FAILURE;
    }

    fclose(partition);
    return SUCCESS;
}


typedef struct {
    FILE* filePtr; // Pointeur vers le fichier
    char fileName[FILENAME_MAX_LENGTH]; // Nom du fichier
} file;

file* myOpen(char* fileName) {
    file* newFile = (file*)malloc(sizeof(file));
    if (newFile == NULL) {
        printf("Erreur lors de l'allocation de mémoire pour la structure de fichier.\n");
        return NULL;
    }

    newFile->filePtr = fopen(fileName, "r+b"); // Ouvre en lecture/écriture binaire
    if (newFile->filePtr == NULL) {
        // Le fichier n'existe pas, donc on le crée
        newFile->filePtr = fopen(fileName, "w+b"); // Crée en écriture/lecture binaire
        if (newFile->filePtr == NULL) {
            printf("Erreur lors de la création du fichier %s.\n", fileName);
            free(newFile); // Libération de la mémoire allouée pour la structure de fichier
            return NULL;
        }
    }

    // Copie du nom de fichier dans la structure de fichier
    snprintf(newFile->fileName, FILENAME_MAX_LENGTH, "%s", fileName);

    return newFile;
}

int myWrite(file* f, void* buffer, int nBytes) {
    if (f == NULL || f->filePtr == NULL || buffer == NULL || nBytes <= 0) {
        printf("Paramètres invalides pour la fonction myWrite.\n");
        return -1; // Valeur de retour pour indiquer une erreur
    }

    // Écriture des données dans le fichier à partir du tampon
    int bytesWritten = fwrite(buffer, 1, nBytes, f->filePtr);
    if (bytesWritten != nBytes) {
        printf("Erreur lors de l'écriture dans le fichier.\n");
        return -1; // Valeur de retour pour indiquer une erreur
    }

    return bytesWritten; // Retourne le nombre d'octets écrits avec succès
}

int myRead(file* f, void* buffer, int nBytes) {
    if (f == NULL || f->filePtr == NULL || buffer == NULL || nBytes <= 0) {
        printf("Paramètres invalides pour la fonction myRead.\n");
        return -1; // Valeur de retour pour indiquer une erreur
    }

    // Lecture des données du fichier dans le tampon
    int bytesRead = fread(buffer, 1, nBytes, f->filePtr);
    if (bytesRead <= 0) {
        printf("Erreur lors de la lecture depuis le fichier.\n");
        return -1; // Valeur de retour pour indiquer une erreur
    }

    return bytesRead; // Retourne le nombre d'octets lus avec succès
}


void mySeek(file* f, int offset, int base) {
    if (f == NULL || f->filePtr == NULL) {
        printf("Paramètres invalides pour la fonction mySeek.\n");
        return; // Ne rien faire si les paramètres sont invalides
    }

    // Déplacement du pointeur de lecture/écriture dans le fichier
    fseek(f->filePtr, offset, base);
}

