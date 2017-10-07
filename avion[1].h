
#define ALTMAX 20000

#define ALTMIN 0

#define VITMAX 1000

#define VITMIN 200

#define PAUSE 2

struct coordonnees {
  int x;
  int y;
  int z;
  int altitude;
};

struct deplacement {
  int cap;
  int vitesse;
};

struct vitesse {
	  int vitX;
	  int vitY;
	  int vitZ;
};

struct avion {
	int id;
	char nom;
	char status;
	int destX;
	int destY;
	int destZ;
    struct coordonnees coord;
    struct deplacement dep;
    struct vitesse vitesse;
};
