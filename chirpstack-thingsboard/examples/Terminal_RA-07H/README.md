Voici une documentation détaillée des commandes AT par rapport à l'utilisation avec le Maduino :

1. **AT+CRESTORE**
   - Réinitialise le module LoRaWAN (comme le Maduino) à ses paramètres d'usine. Cela permet de partir d'une configuration propre.

2. **AT+CADR=1**
   - Active l'Adaptive Data Rate (ADR). ADR est utilisé pour optimiser le débit de données et la consommation d'énergie en fonction des conditions du réseau.

3. **AT+CFREQBANDMASK=0002**
   - Définit les bandes de fréquences à utiliser. Par exemple, la valeur `0002` spécifie les bandes spécifiques selon la région ou le profil réseau.

4. **AT+CTXP=5**
   - Configure la puissance de transmission du module à 5 dBm. Ce réglage permet d'ajuster la portée et la consommation du dispositif selon les exigences du réseau.

5. **AT+CDEVEUI=b2288c2ab6dee8ab**
   - Configure le Device EUI (DEVEUI), qui est l'identifiant unique de l'appareil sur le réseau LoRaWAN. Cette valeur est essentielle pour permettre l'authentification et la gestion des appareils par le réseau.

6. **AT+CAPPEUI=0000000000000000**
   - Configure l'App EUI (APPEUI), identifiant de l'application à laquelle l'appareil est lié. L'APPEUI permet de regrouper des appareils sous une même application dans un contexte LoRaWAN.

7. **AT+CAPPKEY=c072f75a6f5d3940335836d73a8728fa**
   - Configure l'App Key (APPKEY), qui est une clé secrète utilisée pour l'authentification lors de l'activation OTAA (Over The Air Activation). Cela garantit la sécurité des communications entre l'appareil et le réseau.

8. **AT+CSAVE**
   - Sauvegarde la configuration actuelle, y compris les modifications des paramètres comme l'ADREUI, l'APPEUI et l'APPKEY. Cette commande assure que les paramètres sont persistants après un redémarrage du module.

9. **AT+CJOIN=1,0,10,8**
   - Demande à l'appareil de rejoindre le réseau LoRaWAN en utilisant OTAA. Les paramètres fournis spécifient le nombre de tentatives, la méthode d'activation, et d'autres configurations pour optimiser la jonction.

10. **AT+DTRX=1,2,4,01670110**
    - Envoie des données sur le réseau LoRaWAN. Dans cette commande :
     - `1` spécifie le type de port utilisé,
     - `2` est le mode de transmission,
     - `4` est la taille du payload,
     - `01670110` représente les données à envoyer.

Ces commandes sont utiles pour configurer le Maduino pour des applications LoRaWAN, telles que l'envoi de capteurs environnementaux ou la communication longue portée dans un cadre éducatif. Assurez-vous de respecter les règles du réseau lors de l'utilisation de ces commandes.