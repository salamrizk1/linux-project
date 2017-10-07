#include ¨saca.h¨
#include ¨avion.h¨
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct sockadr_in adresse;
int sock = 0, val_lire, lire;

struct sockadr_in, serv_adr;


// caractéristiques du déplacement de l'avion
struct deplacement dep;


// coordonnées spatiales de l'avion
struct coordonnees coord;


// numéro de vol de l'avion : code sur 5 caractères
char numero_vol[6];

char donnee[1024];
char commande[1024];

/********************************
 ***  3 fonctions à implémenter
 ********************************/

int ouvrir_communication() {
    // fonction à implémenter qui permet d'entrer en communication via TCP
    // avec le gestionnaire de vols
    sock = socket(AF_INET,SOCK_STREAM,0);
    memset(&serv_adr, ´0´, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(PORT);

    inet_pton(AF_INET, ¨130.0.0.1¨, &serv_adr.sin_adr);
    if(connect(sock, (struct sockadr *)&serv_adr, sizeof(serv_adr)) < 0)
	{
	    printf(¨\n Connection failed¨);
	    return -1;
	}
    else
	{
    	    return 1;
	}

void fermer_communication() {
    // fonction à implémenter qui permet de fermer la communication
    // avec le gestionnaire de vols
	close(sock);
}

void envoyer_caracteristiques() {
    // fonction à implémenter qui envoie l'ensemble des caractéristiques
    // courantes de l'avion au gestionnaire de vols
	sprintf(donnee, ¨%s,%d,%d,%d,%d,%d¨,numero_vol, coord.x, coord.y, coord.altitude, dep.vitesse, dep.cap);
	send(sock, donnee, 1024, 0);
}

/********************************

 ***  Fonctions gérant le déplacement de l'avion : ne pas modifier
 ********************************/

// initialise aléatoirement les paramètres initiaux de l'avion

void initialiser_avion() {
    // initialisation aléatoire du compteur aléatoire
    int t;
    srand(time(NULL));
    t = rand();

    // intialisation des paramètres de l'avion
    coord.x = 1000 + t % 1000;
    coord.y = 1000 + t % 1000;
    coord.altitude = 900 + t % 100;
    dep.cap = t % 360;
    dep.vitesse = 600 + t % 200;

    // Affichage des donnees

	const char charset[] = ¨ABCDEFGHIJKLMNOPQRSTUVWXYZ¨;
	const char nums[] = ¨0123456789¨;
	for(size_a n=1; n < 6; n++)
	{
	    srand(time(NULL)) * n * n);
	    t = rand();
	    if(n < 3)
	    {
		int key = t % (int) (sizeof charset -1);
		numero_vol[n-1] = charset[key];
	    }
	    else
	    {
		int key = t % (int) (sizeof nums -1);
		numero_vol[n-1] = nums[key];
	    }
	}
    }



    // initialisation du numéro de l'avion : chaine de 5 caractères 
    // formée de 2 lettres puis 3 chiffres
    //numero_vol[0] = (random() % 26) + 'A';
    //numero_vol[1] = (random() % 26) + 'A';
    //sprintf(&numero_vol[2], "%03d", (random() % 999) + 1);
    //numero_vol[5] = 0;
//}

// modifie la valeur de l'avion avec la valeur passée en paramètre

void changer_vitesse(int vitesse) {
    if (vitesse < 0)
        dep.vitesse = 0;
    else if (vitesse > VITMAX)
        dep.vitesse = VITMAX;
    else dep.vitesse = vitesse;
}

// modifie le cap de l'avion avec la valeur passée en paramètre

void changer_cap(int cap) {
    if ((cap >= 0) && (cap < 360))
        dep.cap = cap;
}

// modifie l'altitude de l'avion avec la valeur passée en paramètre

void changer_altitude(int alt) {
    if (alt < 0)
        coord.altitude = 0;
    else if (alt > ALTMAX)
        coord.altitude = ALTMAX;
    else coord.altitude = alt;
}

// affiche les caractéristiques courantes de l'avion

void afficher_donnees() {
    printf("Avion %s -> localisation : (%d,%d), altitude : %d, vitesse : %d, cap : %d\n",
            numero_vol, coord.x, coord.y, coord.altitude, dep.vitesse, dep.cap);
}

// recalcule la localisation de l'avion en fonction de sa vitesse et de son cap

void calcul_deplacement() {
    float cosinus, sinus;
    float dep_x, dep_y;
    int nb;

    if (dep.vitesse < VITMIN) {
        printf("Vitesse trop faible : crash de l'avion\n");
        fermer_communication();
        exit(2);
    }
    if (coord.altitude == 0) {
        printf("L'avion s'est ecrase au sol\n");
        fermer_communication();
        exit(3);
    }
    //cos et sin ont un paramètre en radian, dep.cap en degré nos habitudes francophones
    /* Angle en radian = pi * (angle en degré) / 180 
       Angle en radian = pi * (angle en grade) / 200 

       Angle en grade = 200 * (angle en degré) / 180 
       Angle en grade = 200 * (angle en radian) / pi 

       Angle en degré = 180 * (angle en radian) / pi 
       Angle en degré = 180 * (angle en grade) / 200 
     */

    cosinus = cos(dep.cap * 2 * M_PI / 360);
    sinus = sin(dep.cap * 2 * M_PI / 360);

    //newPOS = oldPOS + Vt
    dep_x = cosinus * dep.vitesse * 10 / VITMIN;
    dep_y = sinus * dep.vitesse * 10 / VITMIN;

    // on se déplace d'au moins une case quels que soient le cap et la vitesse
    // sauf si cap est un des angles droits
    if ((dep_x > 0) && (dep_x < 1)) dep_x = 1;
    if ((dep_x < 0) && (dep_x > -1)) dep_x = -1;

    if ((dep_y > 0) && (dep_y < 1)) dep_y = 1;
    if ((dep_y < 0) && (dep_y > -1)) dep_y = -1;

    //printf(" x : %f y : %f\n", dep_x, dep_y);

    coord.x = coord.x + (int) dep_x;
    coord.y = coord.y + (int) dep_y;

    afficher_donnees();
}

// fonction principale : gère l'exécution de l'avion au fil du temps

void se_deplacer() {
    while (1) {
        sleep(PAUSE);
        calcul_deplacement();
        envoyer_caracteristiques();
    }
}

int main(int argc, char const *argv[]) {
    // on initialise l'avion
    initialiser_avion();
	char *val = (char*)malloc(sizeof(char)*5);

    afficher_donnees();
    // on quitte si on arrive à ne pas contacter le gestionnaire de vols
    if (!ouvrir_communication()) {
        printf("Impossible de contacter le gestionnaire de vols\n");
        exit(1);
    }

    // on se déplace une fois toutes les initialisations sont faites
    se_deplacer();
}


