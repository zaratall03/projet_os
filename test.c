#include <stdio.h>
#include <stdlib.h>
#include "file_operation.h" 

int main() {
    char choix;
    char fileName[100];
    file* fichier;
    int result;

    // Boucle du menu interactif
    while (1) {
        printf("\nMenu:\n");
        printf("1. Créer ou formater une partition\n");
        printf("2. Ouvrir un fichier\n");
        printf("3. Lire depuis un fichier\n");
        printf("4. Ecrire dans un fichier\n");
        printf("5. Déplacer le pointeur de lecture/écriture dans un fichier\n");
        printf("6. Quitter\n");
        printf("Choix: ");
        scanf(" %c", &choix);

        switch (choix) {
            case '1':
                printf("Entrez le nom de la partition à créer ou formater: ");
                scanf("%s", fileName);
                result = myFormat(fileName);
                if (result == SUCCESS)
                    printf("Partition créée ou formatée avec succès.\n");
                else
                    printf("Échec de la création ou du formatage de la partition.\n");
                break;
            case '2':
                printf("Entrez le nom du fichier à ouvrir: ");
                scanf("%s", fileName);
                fichier = myOpen(fileName);
                if (fichier != NULL)
                    printf("Fichier ouvert avec succès.\n");
                else
                    printf("Échec de l'ouverture du fichier.\n");
                break;
            case '3':
                // Code pour lire depuis un fichier
                break;
            case '4':
                // Code pour écrire dans un fichier
                break;
            case '5':
                // Code pour déplacer le pointeur de lecture/écriture dans un fichier
                break;
            case '6':
                printf("Au revoir !\n");
                exit(0);
            default:
                printf("Choix invalide. Veuillez choisir une option valide.\n");
        }
    }

    return 0;
}
