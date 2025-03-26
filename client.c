#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 1. Création du socket TCP
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // 2. Configuration de l'adresse du serveur
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Adresse IP introuvable");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // 3. Connexion au serveur
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur %s:%d\n", SERVER_IP, PORT);
    printf("Devinez un nombre entre 1 et 100 !\n");

    // Boucle de 10 tentatives
    for (int tentative = 10; tentative > 0; tentative--) {
        printf("Il vous reste %d tentatives. Entrez un nombre : ", tentative);
        fgets(buffer, BUFFER_SIZE, stdin);

        // Envoi du nombre au serveur
        if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
            perror("Erreur lors de l'envoi du message");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // Réception de la réponse du serveur
        int received_bytes = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (received_bytes == -1) {
            perror("Erreur lors de la réception de la réponse");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[received_bytes] = '\0'; // Assurer la terminaison correcte de la chaîne
        printf("Réponse du serveur : %s\n", buffer);

        // Vérifier si le nombre est trouvé
        if (strcmp(buffer, "Bravo\n") == 0 || strcmp(buffer, "trouvé") == 0) {
            printf("🎉 Félicitations, vous avez trouvé le nombre ! 🎉\n");
            break; // Quitter la boucle
        }
    }

    printf("Fin du jeu. Merci d'avoir joué !\n");

    // 6. Fermeture du socket
    close(client_socket);
    printf("Connexion fermée.\n");

    return 0;
}
