#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>

#define PORT 1500
#define BUFFER_SIZE 1024

// Ajouter la bibliothèque Winsock
#pragma comment(lib, "Ws2_32.lib")

// Structure pour stocker les données du client
typedef struct {
    SOCKET client_socket;
    int number;
} client_data;
#include <windows.h>

// Fonction thread pour Windows
DWORD WINAPI handleMessage(LPVOID arg) {
    client_data *data = (client_data *)arg;
    SOCKET client_socket = data->client_socket;
    int number = data->number;
    char buffer[BUFFER_SIZE] = {0};
    int cpt = 10;
    char *message = "";

    while (cpt > 0 && strcmp(message, "Bravo") != 0) {
        cpt--;
        // Lire le message du client
        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(client_socket, buffer, BUFFER_SIZE, 0);
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
    closesocket(client_socket);
    free(data);
    printf("Thread terminé avec succès\n");
    return 0;
}


int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("Erreur WSAStartup");
        exit(EXIT_FAILURE);
    }

    // Initialisation du générateur de nombres aléatoires
    srand((unsigned int)time(NULL));
    int number = rand() % 101; // Nombre entre 0 et 100

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        perror("Erreur socket");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Attachement du socket au port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        perror("Erreur bind");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Écoute des connexions entrantes
    if (listen(server_fd, 5) == SOCKET_ERROR) {
        perror("Erreur listen");
        closesocket(server_fd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexion...\n");

    while (1) {
        // Accepter une nouvelle connexion
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            perror("Erreur accept");
            continue;
        }

        printf("Nouveau client connecté.\n");

        // Allouer de la mémoire pour stocker les informations du client
        client_data *data = malloc(sizeof(client_data));
        data->client_socket = new_socket;
        data->number = number;

        // Créer un thread pour gérer le client
        HANDLE thread_handle;
        thread_handle = CreateThread(NULL, 0, handleMessage, (void *)data, 0, NULL);
        if (thread_handle == NULL) {
            perror("Erreur CreateThread");
            free(data);
            closesocket(new_socket);
        } else {
            CloseHandle(thread_handle); // Facultatif si vous n'avez pas besoin d'attendre le thread
        }
        
    }

    // Fermer le socket du serveur
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
