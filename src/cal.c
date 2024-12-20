#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define DAYS_IN_MONTH 30
#define MAX_LEN 100
#define MAX_TENUES_PER_DAY 10 // Nombre maximum de tenues par jour


// Catégories de vêtements
const char *hauts[] = {"T-shirt noir", "T-shirt blanc", "Pull", "Chemise"};
const char *pantalons[] = {"Jean", "Pantalon noir", "Jupe", "Short"};
const char *chaussures[] = {"Baskets blanches", "Chaussures noires", "Sandales", "Bottes"};

// Nombre d'articles par catégorie
#define NUM_HAUTS (sizeof(hauts) / sizeof(hauts[0]))
#define NUM_PANTALONS (sizeof(pantalons) / sizeof(pantalons[0]))
#define NUM_CHAUSSURES (sizeof(chaussures) / sizeof(chaussures[0]))

// Structure pour stocker plusieurs tenues par jour
typedef struct { 

    char tenues[MAX_TENUES_PER_DAY][MAX_LEN];
    int nombre_tenues;

    } JourTenues;

//Fonction pour afficher le calendrier
void afficher_calendrier(JourTenues calendrier[DAYS_IN_MONTH]) {
    printf("\nCalendrier du mois :\n");
    for (int i = 0; i < DAYS_IN_MONTH; i++) {
        printf("Jour %d :\n", i + 1);
        if (calendrier[i].nombre_tenues == 0) {
            printf(" Pas de tenue prévue\n");

        } else {
            for (int j = 0; j < calendrier[i].nombre_tenues; j++) {
                printf(" Tenue %d : %s\n", j + 1, calendrier[i].tenues[j]);
            }
     }
    }
}

// Fonction pour générer une tenue aléatoire
void generer_tenue(char *tenue) {

    // Sélection aléatoire d'un article dans chaque catégorie    
    const char *haut = hauts[rand() % NUM_HAUTS];
    const char *pantalon = pantalons[rand() % NUM_PANTALONS];
    const char *chaussure = chaussures[rand() % NUM_CHAUSSURES];

    // Concaténer les articles dans une chaîne de caractères    
    snprintf(tenue, MAX_LEN, "%s, %s, %s", haut, pantalon, chaussure);
}

// Fonction pour ajouter une tenue pour un jour spécifique
void ajouter_tenue(JourTenues calendrier[DAYS_IN_MONTH]) {
    int jour;
    char tenue[MAX_LEN];
    char confirmation;
    // Sélectionner un jour    
    printf("Entrez le jour (1-%d) pour lequel vous voulez ajouter une tenue : ", DAYS_IN_MONTH);
    if (scanf("%d", &jour) != 1) {
        printf("Entrée invalide. Veuillez entrer un numéro de jour.\n");
        while (getchar() != '\n'); // vider le buffer        
        return;
    }
    if (jour < 1 || jour > DAYS_IN_MONTH) {
        printf("Jour invalide. Veuillez réessayer.\n");
        return;
    }
    // Vérifier s'il y a encore de la place pour une nouvelle tenue ce jour-là  
    if (calendrier[jour - 1].nombre_tenues >= MAX_TENUES_PER_DAY) {
        printf("Nombre maximum de tenues atteint pour le jour %d.\n", jour);
        return;
    }
    // Générer une tenue et demander confirmation    
    generer_tenue(tenue);
    printf("Proposition de tenue pour le jour %d : %s\n", jour, tenue);
    printf("Acceptez-vous cette tenue ? (o/n) : ");
    while (getchar() != '\n'); // vider le buffer    confirmation = getchar();
        if (confirmation == 'o' || confirmation == 'O') {
        strcpy(calendrier[jour - 1].tenues[calendrier[jour - 1].nombre_tenues], tenue); 
        calendrier[jour - 1].nombre_tenues++; 
        printf("Tenue ajoutée pour le jour %d.\n", jour);
        } else {
            printf("Tenue non ajoutée.\n");
    }
}

int main() { 
    // Initialisation du tableau pour stocker les tenues pour chaque jour    
    JourTenues calendrier[DAYS_IN_MONTH] = {0};
    // Initialisation de la fonction de génération aléatoire    
    srand(time(NULL));
    int choix;
    while (1) {
        // Menu principal        
        printf("\nMenu :\n");
        printf("1. Afficher le calendrier\n");
        printf("2. Ajouter une tenue pour un jour\n"); 
            printf("3. Quitter\n"); 
            printf("Choisissez une option : ");
            if (scanf("%d", &choix) != 1) {
                printf("Entrée invalide. Veuillez entrer un numéro d'option.\n");
                while (getchar() != '\n'); // vider le buffer            
                continue;
            }
            switch (choix) {
                case 1:
                    afficher_calendrier(calendrier);
                    break;
                
                case 2:
                    ajouter_tenue(calendrier);
                    break;
                
                case 3:
                    printf("Au revoir !\n");
                    return 0;
                    
                default:
                    printf("Choix invalide. Veuillez réessayer.\n");
                    
            }
            
        }

    return 0;
}