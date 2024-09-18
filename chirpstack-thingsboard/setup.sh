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

sudo apt-get install ca-certificates curl
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc

echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo apt-get update

sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

sudo systemctl enable docker
sudo systemctl start docker

# Récupère les fichiers de configuration pour chirpstack
git clone https://github.com/chirpstack/chirpstack-docker.git

# Déplace et créé le dossier souhaité vers le répertoire parent et nettoie
cp chirpstack-docker/configuration ./configuration
rm -rf chirpstack-docker

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

# Démarre les docker avec docker-compose
sudo docker compose up



