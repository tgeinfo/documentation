#!/bin/bash

# Script : Configuration des permissions d'accès pour le serveur Apache
# Auteur : Maxime Gillespie
# Date : 2024-10-16

# Description :
# Ce script permet de configurer les permissions d'accès pour les fichiers du serveur Apache.
# Il ajoute l'utilisateur courant au groupe www-data, change le propriétaire des fichiers Apache
# et définit des permissions pour permettre l'accès en lecture, écriture et exécution pour le groupe.
# Cela est particulièrement utile dans un contexte où plusieurs utilisateurs doivent pouvoir
# modifier ou accéder aux fichiers du serveur web dans le cadre de projets collaboratifs.

# Ajouter l'utilisateur courant au groupe www-data (groupe Apache par défaut)
sudo usermod -a -G www-data $(whoami)

# Changer le propriétaire des fichiers du répertoire web vers www-data
sudo chown www-data:www-data /var/www/html

# Donner les permissions de lecture, écriture et exécution au groupe pour le répertoire web
sudo chmod -R g+rwx /var/www/html

# Message d'information
echo "Configuration des permissions pour Apache terminée."