# Jeu de Devinette en Réseau (C - Sockets)

Un projet réseau en C mettant en place un jeu de devinette multijoueur via des sockets(TCP pour notre projet). 
Un serveur génère un nombre aléatoire entre 1 et 100, et plusieurs clients peuvent se connecter pour tenter de le deviner en temps réel.

## 📌 Fonctionnalités Principales

### ✅ Serveur de jeu :
- Génère un nombre aléatoire entre 1 et 100.
- Gère plusieurs connexions clients simultanées.
- Reçoit les propositions des clients et renvoie :
  - `"Trop haut"`
  - `"Trop bas"`
  - `"Correct"`
- Annonce le gagnant et termine la partie pour tous.

### ✅ Client de jeu :
- Se connecte au serveur.
- Permet à l’utilisateur de deviner le nombre via le terminal.
- Affiche les réponses et indications envoyées par le serveur.

### ✅ Communication en temps réel :
- Réponses immédiates du serveur après chaque proposition.
- Tous les clients sont informés dès qu’un joueur gagne.

### ✅ Gestion des erreurs :
- Détection des déconnexions inattendues côté client/serveur.
- Affichage de messages d’erreur clairs en cas de problème de connexion.

## 🛠️ Technologies utilisées

- C (langage principal)
- Sockets (communication réseau bas niveau)
- Linux (Ubuntu recommandé)

## 🚀 Pour exécuter le projet

### 1. Compiler les fichiers

```bash
gcc -o serveur serveur.c
gcc -o client client.c
