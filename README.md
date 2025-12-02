# Match-3

# 🍬 CANDY CRUSH EN C : JEU DE TYPE MATCH-3

[![GitHub language count](https://img.shields.io/github/languages/count/VotreNomUtilisateur/repo-candy-crush-c?style=flat-square)](https://github.com/VotreNomUtilisateur/repo-candy-crush-c)
[![GitHub top language](https://img.shields.io/github/languages/top/VotreNomUtilisateur/repo-candy-crush-c?color=blue&style=flat-square)](https://github.com/VotreNomUtilisateur/repo-candy-crush-c)
[![Licence](https://img.shields.io/github/license/VotreNomUtilisateur/repo-candy-crush-c?style=flat-square)](LICENSE)

## 📝 Description du Projet

Ce projet est une implémentation du célèbre jeu de réflexion **Match-3** (communément appelé "Candy Crush") entièrement développé en **langage C**. Il a été conçu pour explorer les concepts de la programmation structurée, la gestion de la mémoire, et la création d'algorithmes de détection de motifs complexes sur une grille 2D.

Le jeu se joue en ligne de commande ou utilise une bibliothèque simple pour l'affichage graphique (à spécifier si applicable).

### ✨ Fonctionnalités Principales

* **Grille de Jeu $N \times M$ :** Configuration facile des dimensions de la grille.
* **Système de Swap :** Échange de bonbons adjacents par coordonnées.
* **Algorithme de Détection :** Identification et validation de **matches de 3 bonbons ou plus** (horizontal et vertical).
* **Mécanique de Gravité :** Chute des bonbons après un match et génération de nouveaux bonbons pour remplir la grille.
* **Vérification de Coups Valides :** Le jeu n'autorise un échange que s'il résulte en un match.
* **Gestion du Score :** Attribution de points en fonction de la taille des matches.

## 🛠️ Technologies Utilisées

* **Langage :** C (Standard C99 / ANSI C)
* **Compilateur :** GCC (GNU Compiler Collection)
* **Bibliothèque Graphique (Optionnel) :** *[Si vous utilisez SDL, Allegro, etc., listez-le ici. Sinon, vous pouvez supprimer cette ligne ou la remplacer par :* `Interface en ligne de commande (ASCII Art).` *]*

## 🚀 Installation et Démarrage

### Prérequis

Assurez-vous d'avoir installé le compilateur GCC sur votre système :

```bash
gcc --version
