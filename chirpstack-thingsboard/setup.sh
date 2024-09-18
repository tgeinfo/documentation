#!/bin/bash

# Ce script met à jour et met à niveau Ubuntu, installe Docker et Docker Compose,
# clone un dossier spécifique d'un dépôt GitHub, et déplace le dossier cloné
# vers le répertoire parent avant de nettoyer les fichiers temporaires.

# Étapes du script:
# 1. Met à jour et met à niveau Ubuntu.
# 2. Installe Docker et Docker Compose, puis démarre et active Docker.
# 3. Clone un dossier spécifique du dépôt GitHub 'tgeinfo/scripts' en utilisant
#    une extraction sparse pour ne récupérer que le dossier 'chirpstack-thingsboard'.
# 4. Déplace le contenu du dossier cloné vers le répertoire parent et supprime
#    les fichiers temporaires et le dossier .git.

# Met à jour et met à niveau Ubuntu
sudo apt-get update && sudo apt-get upgrade -y

# Installe Docker et Docker Compose
sudo apt-get install -y docker.io docker-compose
sudo systemctl start docker
sudo systemctl enable docker

# Clone le dossier spécifique du dépôt
git clone --depth 1 --filter=blob:none --sparse https://github.com/tgeinfo/scripts.git chirpstack-thingsboard
cd chirpstack-thingsboard
git sparse-checkout set chirpstack-thingsboard

# Déplace le dossier souhaité vers le répertoire parent et nettoie
mv chirpstack-thingsboard/* .
rm -rf arduino documentation chirpstack-thingsboard .git

# Crée les répertoires nécessaires et modifie les permissions
mkdir -p ./mytb-data && sudo chown -R 799:799 ./mytb-data
mkdir -p ./mytb-logs && sudo chown -R 799:799 ./mytb-logs

# Télécharge l'image Docker de ThingsBoard avec PostgreSQL
sudo docker pull thingsboard/tb-postgres

# Exécute le script de mise à niveau de ThingsBoard
sudo docker run -it -v ./mytb-data:/data --rm thingsboard/tb-postgres upgrade-tb.sh

# Supprime le conteneur 'mytb' s'il existe
sudo docker-compose rm mytb

# Démarre les services définis dans le fichier docker-compose
sudo docker-compose up
