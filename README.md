# CY TECH - Hardware System Interface - Projet HSI

Ce projet consiste en une application simulant le fonctionnement du boîtier
central de gestion d'un véhicule (BCVG). Cette application communique avec le
véhicule au travers du multiplexeur (MUX) en UDP, ainsi qu'avec les commodos et
le boîtier de gestion des feux (BGF) en liaison série. Elle est chargée du
traitement des informations reçues du MUX et des commodos afin de contrôler
l'allumage des feux (au travers du BGF) et des voyants lumineux du tableau de
bord (au travers du MUX).

L'application implémente des fonctions d'encodage et de décodages de champs de
bits nécessaire au traitement des trames reçue ainsi que des automates finis
encodant les états et transitions des feux et des essuie-glace du véhicule.

## Architecture du projet

* __bin/ :__ fichiers compilés exécutables (application et driver)
* __src/ :__ fichiers sources
* __lib/ :__ librairies statiques et fichiers `.h` associés
* __lib/python/ :__ sous-projet de génération du dictionnaire de données
* __docker/ :__ configuration docker-compose pour la récupération et l'affichage
  des données de l'application

## Détail des étapes de développement

### 1. Elaboration d'un dictionnaire de données

La première étape du projet fut d'analyser l'énoncé du projet et d'en extraire
la totalité des types et des variables globales de l'application.
Principalement, il y a une variable pour chaque information encodée dans les
trames entrantes et sortantes, une variable pour chaque dispositif contrôlé par
un automate et un timer pour chaque automate en ayant besoin.

Ce dictionnaire de
données a été amené à être modifié tout au long du projet, voyant certains types
ou variables ajoutés ou supprimés. Nous avons réalisé qu'il était bien plus
difficile que ce qu'il en a l'air de prévoir l'ensemble des données nécessaires
à l'application avant même de commencer le développement de celle-ci, et nous
aurions été bien embarrassés si nous avions été contraints de nous en tenir au
dictionnaire de données tel que défini initialement.

### 2. Développement du script de génération du dictionnaire de données

Une fois le dictionnaire de données élaboré (`lib/python/types.csv`
, `lib/python/variables.csv`), nous avons développé un script python3 permettant
de générer le code de la librairie statique. Ce script utilise notamment le
module `csnake` permettant d'automatiser la génération de code C. Il lit
initialement les CSV dans des listes, puis les utilises pour créer les objets
correspondants (types, variables, fonctions getter et setter) et délègue la
génération du code au module `csnake`.
> Note : Le module n'étant pas encore à sa v1 et manquant certaines
> fonctionnalités, certaines lignes de codes sont générées manuellement
> (notamment les commentaires doxygen et les typedef)

L'exécution du script, le formatage et la compilation des fichiers générés sont
délégués à un Makefile
(voir [la section correspondante](#5-cration-des-makefile)).

### 3. Premier `main()`

Le premier `main()` développé n'implémente que la lecture et l'affichage de la
trame UDP envoyée périodiquement toutes les 10ms. Cette fonction n'est plus dans
la version actuelle du projet, en voici le code :

```C
__attribute__((noreturn))
int main(void) {

    int32_t driver_fd = DRV_ERROR;
    uint8_t out_udp_frame[DRV_UDP_10MS_FRAME_SIZE];

    driver_fd = drv_open();

    if (driver_fd == DRV_ERROR) {
        perror("Unable to open connection to driver");
    }

    while (1) {

        drv_read_udp_10ms(driver_fd, out_udp_frame);

#pragma unroll
        for (size_t i = 0; i < DRV_UDP_10MS_FRAME_SIZE; i++) {
            (void) fprintf(stdout, "%08" PRIX32, out_udp_frame[i]);
        }
        (void) fputs("\n", stdout);
    }
}
```

### 4. Développement des automates finis

Le projet implémente (dans `src/state_machines/`) trois automates
distincts : `fsm_lights` pour gérer l'allumage de chacun des
feux, `fsm_blinkers` pour l'allumage des clignotants gauche et droite
et `fsm_wipers` pour les essuie-glace.

Ces implémentations sont déviées de l'implémentation type fournie dans `fsm.c` ;
chaque automate définit sa liste d'états, sa liste d'évènements transitoires et
sa liste de transitions, une transition étant définie par son état de départ,
son état d'arrivée et son évènement déclencheur. Chaque automate implémente
également une fonction pour chaque dispositif régi par cet automate, qui
détermine l'évènement en cours en fonction des variables globales de
l'application et met à jour ces données en fonction de l'état résultant de
l'automate.

### 5. Création des Makefile

Le projet utilise un Makefile sur 3 niveaux :

* __lib/python/Makefile :__ Ce Makefile crée un virtualenv python et exécute le
  script générant le dictionnaire de données, puis supprime le virtualenv.
* __lib/Makefile :__ Ce Makefile compile les différentes librairies statiques
    * `data-dictionary.a` est créé en compilant le fichier `.c` généré en
      appelant le Makefile sous `lib/python/`
    * `crc8.a` est créé en téléchargeant les fichiers `checksum.h` et `crc8.c`
      depuis GitHub et en les compilant
* __./Makefile :__ Le Makefile top level compile les fichiers sources en
  intégrant les librairies statiques générées par le Makefile sous `lib/`

### 6. Fonctions de décodage

Il y a trois fonctions de décodage utilisées pour le projet :

* `decode_commodos(const uint8_t*, size_t)`: Cette fonction lit d'abord le crc8
  sur le premier octet de la trame, puis décode les informations sur le second
  en utilisant les masques définis à cet effet. Elle affiche un message
  d'avertissement lorsque le crc8 ne correspond pas à celui recalculé, mais
  n'interromps pas le traitement.
* `decode_mux(const uint8_t)`: cette fonction décode les quatorze octets de la
  trame envoyée par le MUX en utilisant des macro paramétrées extrayant les
  différentes informations sur la trame.
* `decode_bgf(const uint8_t*, size_t)`: Cette fonction lit les messages
  d'acquittement envoyés par le BGF en utilisant les constantes définissant les
  id de messages du BGF.

### 7. Fonctions d'encodage

Il y a deux fonctions d'encodage utilisées pour le projet :

* `encode_bgf(lns_frame_t*)`: Cette fonction encode les cinq messages à envoyer
  au BGF pour commander l'allumage des feux en utilisant les constantes
  définissant les indices et les offset pour l'encodage/décodage des trames du
  BGF.
* `encode_mux(const uint8_t)`: cette fonction encode les neuf octets transmis au
  MUX en utilisant les constantes définissant les offset des différents champs
  et les macros paramétrées permettant d'encoder chaque octet du compteur de
  kilomètre.

### 8. Premier livrable

Une fois les fonctions ci-dessus développées, nous avons pu remplacer
le `main()` temporaire développé à [la section trois](#3-premier-main) par une
implémentation complète de la séquence du programme.

À l'exécution, le seul problème que nous ayons constaté est que les feux
semblent ne jamais s'allumer. En s'aidant du debugger, nous avons remarqué que
les messages d'acquittements n'étaient jamais reçus (tous les messages reçus par
le LNS portaient le serial number 12, celui des commodos). Faute de temps, nous
n'avons pas pu identifier l'origine de cette faute, ni si celle-ci cachait
encore d'autres fautes.