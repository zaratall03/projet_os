#ifndef FILE_OPERATION_H

#define SUCCESS 0
#define FAILURE -1
#include <stdio.h>
typedef struct {
    char* fileName;       // Nom du fichier
    FILE* filePtr;        // Descripteur de fichier
    size_t fileSize;      // Taille du fichier
    long currentPosition; // Position actuelle du pointeur de lecture/écriture
} file;

// Déclaration des prototypes des fonctions
// (vous pouvez ajouter d'autres fonctions au besoin)
int myFormat(char* partitionName);
file* myOpen(char* fileName,FileSystem* fs);
int myWrite(file* f, void* buffer, int nBytes,FileSystem* fs);
int myRead(file* f, void* buffer, int nBytes,FileSystem* fs);
void mySeek(file* f, int offset, int base,FileSystem* fs);

#endif

