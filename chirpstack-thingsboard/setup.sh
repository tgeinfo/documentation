# Author: Maxime Gillespie
# Date: 2024-09-18
# Version: 1.0

# Description: Ce script permet d'installer les mises à jour d'Ubuntu 24.04, Docker, Docker-compose et Git. Il permet également de cloner le dépôt chirpstack-thingsboard.
# Ce script est utilisé dans le cadre du projet avec les maduino zero dans le cours PPS de l'année 2024-2025 au département de génie électrique du cégep de l'Outaouais.
# Usage: bash setup.sh

#!/bin/bash

#install ubuntu 24.04 updates
sudo apt-get update
sudo apt-get upgrade -y

#install docker
sudo apt-get install docker.io -y
sudo systemctl start docker
sudo systemctl enable docker

#install docker-compose
sudo apt-get install docker-compose -y

#install git
sudo apt-get install git subversion -y

#clone chirpstack-thingsboard repository
svn export https://github.com/username/repo/trunk/path/to/folder chirpstack-thingsboard