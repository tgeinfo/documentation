# Documentation : Intégration de Laird LoRaWAN, ChirpStack, ThingBoard et Maduino LoRaWAN

## Introduction

Ce document a pour objectif d'expliquer la relation et le fonctionnement des composants suivants dans un réseau IoT basé sur la technologie LoRaWAN :

- **Gateway Laird LoRaWAN**
- **ChirpStack**
- **ThingBoard**
- **Maduino LoRaWAN**

L'objectif est de fournir une référence claire permettant de comprendre comment ces éléments interagissent pour collecter, gérer et visualiser des données provenant de capteurs connectés.

## Composants du Système

### 1. Gateway Laird LoRaWAN

**Description :**  
La Gateway Laird LoRaWAN est un dispositif matériel qui sert de point de communication entre les capteurs LoRaWAN (comme le Maduino) et le réseau Internet. Elle capte les messages radio des capteurs et les transmet à un serveur réseau via une connexion Internet (Ethernet, Wi-Fi, etc.).

**Fonctionnalités :**
- Réception et transmission de données LoRaWAN.
- Gestion des protocoles de communication LoRaWAN.
- Interface pour la configuration réseau et la gestion des appareils connectés.

**Accès :**  
L'interface de gestion de la Gateway Laird LoRaWAN est généralement accessible via une adresse IP spécifique, par exemple :  
`https://10.3.63.13/`  
*Note : Cette URL est indicative et doit être configurée selon le réseau utilisé.*

### 2. ChirpStack

**Description :**  
ChirpStack est une plateforme open-source pour la gestion des réseaux LoRaWAN. Elle comprend plusieurs composants tels que le Network Server, l’Application Server, et le Gateway Bridge.

**Fonctionnalités :**
- **Network Server** : Gère la communication entre les gateways et les appareils LoRaWAN.
- **Application Server** : Traite les données des capteurs et les envoie vers des applications tierces.
- **Gateway Bridge** : Sert de pont entre les gateways et le serveur réseau.

**Rôle dans le Système :**  
ChirpStack reçoit les données du Gateway Laird, gère l'authentification et la sécurité, et transmet les données au backend pour traitement ultérieur.

**Accès :**  
L'interface web de ChirpStack est accessible via :  
`http://10.3.159.3:8080/`

### 3. ThingBoard

**Description :**  
ThingBoard est une plateforme IoT open-source pour la gestion, le contrôle et la visualisation des données provenant des appareils connectés.

**Fonctionnalités :**
- Tableaux de bord personnalisables pour la visualisation des données.
- Gestion des appareils et des utilisateurs.
- Intégration avec des bases de données et des services externes via des API.

**Rôle dans le Système :**  
ThingBoard reçoit les données traitées par ChirpStack et permet de visualiser, analyser et interagir avec les données des capteurs via des tableaux de bord interactifs.

**Accès :**  
L'interface web de ThingBoard est accessible via :  
`http://10.3.159.3:8081/`

### 4. Maduino LoRaWAN

**Description :**  
Le Maduino LoRaWAN est une carte de développement basée sur Arduino équipée d'un module LoRaWAN, permettant de connecter des capteurs et de transmettre des données via le réseau LoRaWAN.

**Fonctionnalités :**
- Interfaces pour connecter divers capteurs (température, humidité, etc.).
- Transmission de données via LoRaWAN vers la Gateway Laird.
- Programmabilité via l'environnement Arduino pour personnaliser les fonctionnalités.

**Rôle dans le Système :**  
Le Maduino collecte les données environnementales et les envoie via LoRaWAN au Gateway Laird, initiant ainsi le flux de données dans le système.

## Flux de Données et Interactions

1. **Collecte des Données :**
   - Le **Maduino LoRaWAN** collecte des données à partir des capteurs connectés (par exemple, température, humidité).

2. **Transmission des Données :**
   - Les données collectées sont transmises sans fil via le protocole **LoRaWAN** au **Gateway Laird LoRaWAN**.

3. **Réception et Traitement Initial :**
   - Le **Gateway Laird LoRaWAN** reçoit les messages LoRaWAN et les transmet via une connexion Internet au **ChirpStack**.

4. **Gestion du Réseau LoRaWAN :**
   - **ChirpStack** gère la réception des données, l'authentification des appareils, et assure la sécurité des communications.
   - Les données sont ensuite traitées et formatées par **ChirpStack**.

5. **Envoi vers la Plateforme IoT :**
   - **ChirpStack** transmet les données traitées à **ThingBoard** via des API ou des intégrations spécifiques.

6. **Visualisation et Analyse :**
   - **ThingBoard** affiche les données sur des tableaux de bord interactifs, permettant de visualiser, analyser et surveiller les informations en temps réel.

## Schéma d'Architecture

```
[ Maduino LoRaWAN ]
        |
        | (LoRaWAN)
        v
[ Gateway Laird LoRaWAN ]
        |
        | (Internet)
        v
[ ChirpStack Network Server ]
        |
        v
[ ChirpStack Application Server ]
        |
        v
[ ThingBoard IoT Platform ]
```

## Avantages de cette Architecture

- **Portée Étendue :** La technologie LoRaWAN permet une communication sur de longues distances avec une faible consommation d'énergie.
- **Scalabilité :** Grâce à ChirpStack et ThingBoard, le système peut être facilement étendu pour gérer un grand nombre d'appareils.
- **Flexibilité :** Les plateformes open-source permettent une personnalisation et une adaptation selon les besoins spécifiques.
- **Visualisation Intuitive :** ThingBoard offre des outils puissants pour la création de tableaux de bord et l'analyse des données.

## Cas d'Utilisation

- **Surveillance Environnementale :** Collecte de données telles que la température, l'humidité, la qualité de l'air, etc.
- **Gestion de l'Énergie :** Suivi de la consommation énergétique des appareils connectés.
- **Agriculture Intelligente :** Surveillance des conditions du sol et des cultures pour optimiser les rendements.

## Conclusion

L'intégration du **Gateway Laird LoRaWAN**, **ChirpStack**, **ThingBoard** et **Maduino LoRaWAN** offre une solution robuste et flexible pour la création de réseaux IoT basés sur LoRaWAN. Comprendre les rôles et les interactions de chaque composant permet de développer des projets IoT efficaces et évolutifs.

---

## Ressources Supplémentaires

- **Documentation Laird LoRaWAN :** [Lien vers la documentation officielle](https://www.lairdconnect.com/)
- **ChirpStack Documentation :** [https://www.chirpstack.io/docs/](https://www.chirpstack.io/docs/)
- **ThingBoard Documentation :** [https://thingsboard.io/](https://thingsboard.io/)

[Documentation des TTPs](https://github.com/tgeinfo/scripts/tree/main/chirpstack-thingsboard)