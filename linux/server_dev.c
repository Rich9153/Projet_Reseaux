#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 1500
#define BUFFER_SIZE 1024

// Structure pour stocker les données du client
typedef struct {
    int client_socket;
    int number;
}client_data; 

void *handleMessage(void *arg) {
    client_data *data = (client_data *)arg;
    int client_socket = data->client_socket;
    int number = data->number;
    char buffer[BUFFER_SIZE] = {0};
    int cpt = 10;
    char *message = "";
    
    while (cpt > 0 && strcmp(message, "Bravo") != 0) {
        cpt --;
        // Lire le message du client
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            printf("Client déconnecté.\n");
            break;
        }

        // Vérifier si le client veut quitter
        if (strcmp(buffer, "exit") == 0) {
            printf("Fermeture de la connexion avec le client.\n");
            break;
        }

        // Convertir l'entrée en entier
        int guess = atoi(buffer);
        

        if (guess == number) {
            message = "Bravo";
        } else if (guess > number) {
            message = "Trop haut";
        } else {
            message = "Trop bas";
        }

        // Envoyer la réponse au client
        send(client_socket, message, strlen(message), 0);
    }

    // Fermer le socket du client et libérer la mémoire
    close(client_socket);
    free(data);
    printf("Thread terminé avec succès\n");
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));
    int number = rand() % 101; // Nombre entre 0 et 100

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Attachement du socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, 5) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    while (1) {
        // Accepter une nouvelle connexion
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Erreur accept");
            continue;
        }

        printf("Nouveau client connecté.\n");

        // Allouer de la mémoire pour stocker les informations du client
        client_data *data = malloc(sizeof(client_data));
        data->client_socket = new_socket;
        data->number = number;

       // Créer un thread pour gérer le client
       pthread_t thread_id;
       if (pthread_create(&thread_id, NULL, handleMessage, (void *)data) != 0) {
           perror("Erreur pthread_create");
           free(data);
           close(new_socket);
       } else {
           pthread_detach(thread_id);  // Détacher uniquement si la création a réussi
       }
    }

    // Fermer le socket du serveur
    close(server_fd);

    return 0;
}
