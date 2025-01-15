#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TENUES 100  // Nombre maximum de tenues à charger

// Structure pour stocker les informations d'une tenue (haut, bas, chaussure)
typedef struct {
    char haut[100];
    char bas[100];
    char chaussure[100];
} Tenue;

// Fonction pour charger les tenues depuis un fichier CSV
int charger_tenues(const char* fichier, Tenue tenues[]) {
    FILE* file = fopen(fichier, "r");  // Ouvre le fichier CSV en mode lecture
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", fichier);
        return 0;  // Retourne 0 si l'ouverture du fichier échoue
    }

    char ligne[256];
    int i = 0;

    // Ignorer la première ligne (les en-têtes)
    fgets(ligne, sizeof(ligne), file);

    // Lire chaque ligne et remplir les données des tenues dans le tableau
    while (fgets(ligne, sizeof(ligne), file) && i < MAX_TENUES) {
        // On lit les données séparées par des virgules et on les stocke dans la structure
        sscanf(ligne, "%99[^,],%99[^,],%99[^\n]", tenues[i].haut, tenues[i].bas, tenues[i].chaussure);
        i++;  // Incrémente l'index des tenues
    }

    fclose(file);  // Ferme le fichier une fois la lecture terminée
    return i;  // Retourne le nombre de tenues chargées
}

// Fonction pour afficher une tenue dans l'interface GTK
void afficher_tenue(GtkWidget* window, GtkWidget* image_haut, GtkWidget* image_bas, GtkWidget* image_chaussure, Tenue tenues[], int index) {
    char path_haut[200], path_bas[200], path_chaussure[200];

    // Construire les chemins complets des images à partir des noms de fichiers
    snprintf(path_haut, sizeof(path_haut), "./images/%s", tenues[index].haut);
    snprintf(path_bas, sizeof(path_bas), "./images/%s", tenues[index].bas);
    snprintf(path_chaussure, sizeof(path_chaussure), "./images/%s", tenues[index].chaussure);

    // Charger et afficher les images des vêtements dans l'interface GTK
    gtk_image_set_from_file(GTK_IMAGE(image_haut), path_haut);
    gtk_image_set_from_file(GTK_IMAGE(image_bas), path_bas);
    gtk_image_set_from_file(GTK_IMAGE(image_chaussure), path_chaussure);
}

// Fonction pour gérer l'événement du bouton "Générer une tenue"
void on_generer_tenue(GtkWidget* button, gpointer user_data) {
    Tenue* tenues = (Tenue*)user_data;  // Récupérer les tenues à partir des données utilisateur
    static int index = 0;  // L'index de la tenue actuelle (incrémenté à chaque clic)
    int nombre_tenues = charger_tenues("tenues.csv", tenues);  // Charger les tenues depuis le fichier CSV

    // Si des tenues ont été chargées, afficher une tenue au hasard
    if (nombre_tenues > 0) {
        afficher_tenue(GTK_WIDGET(button), image_haut, image_bas, image_chaussure, tenues, index);
        index = (index + 1) % nombre_tenues;  // Passer à la tenue suivante en boucle
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *image_haut, *image_bas, *image_chaussure;
    Tenue tenues[MAX_TENUES];  // Tableau pour stocker les tenues

    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Créer la fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Générateur de tenues");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);  // Taille de la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);  // Fermer l'application lorsque la fenêtre est fermée

    // Créer une boîte verticale pour contenir les widgets (images et bouton)
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    // Créer des widgets pour afficher les images des vêtements
    image_haut = gtk_image_new();
    image_bas = gtk_image_new();
    image_chaussure = gtk_image_new();

    // Ajouter les images à la boîte verticale
    gtk_box_pack_start(GTK_BOX(box), image_haut, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), image_bas, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), image_chaussure, TRUE, TRUE, 0);

    // Créer un bouton pour générer une nouvelle tenue
    button = gtk_button_new_with_label("Générer une tenue");
    g_signal_connect(button, "clicked", G_CALLBACK(on_generer_tenue), tenues);  // Connecter le signal "clicked" du bouton à la fonction
    gtk_box_pack_start(GTK_BOX(box), button, FALSE, FALSE, 0);

    // Afficher la fenêtre et tous ses widgets
    gtk_widget_show_all(window);

    // Lancer la boucle principale de GTK
    gtk_main();

    return 0;
}
