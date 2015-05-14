#ifndef STRUCT_H
#define STRUCT_H


/* Déclaration des structures et constantes relatives au projet */
enum DIRECTION { RIGHT = 0, LEFT = 1, DOWN = 2, UP = 3, UPLEFT = 4, UPRIGHT = 5, DLEFT = 6, DRIGHT = 7, NONE = 8 };

typedef struct Jeu{				//Structure d'une partie
	struct Equipe** equipes;		//Tableau d'équipes
	int nbEquipe;					//Nombre d'équipe
	struct Terrain* pMapTerrain;	//Carte en cours
	int temps;						//Durée
	char nomMap[100];				//Nom de la map
} Jeu;

typedef struct Terrain{				//Structure terrain 
	SDL_Texture* imageBackground;		//background
	SDL_Surface* collisionMapSurface;
	SDL_Surface* globalMapSurface;
} Terrain;

typedef struct Equipe{		//Structure équipe
	struct Worms** worms;		//Tableau de Worms
	int nbWormsStart;			//Nombre de Worms au début de la partie
	int vie;					//Somme des points de vie de l'équipe
	char nom[50];				//Nom d'équipe
	SDL_Color color;			//Couleur d'équipe
	//logo?
} Equipe;

typedef struct Worms{			//Structure worms
	int vie;						//Point de vie
	char nom[20];					//Nom du joueur
	char* arme;						//Arme en cours d'utilisation
	//Arme* invent[TAILLEINV];		//Inventaire des armes et objets disponibles
	enum DIRECTION dirSurface;
	SDL_Surface* wormsSurfaceLeft;
	SDL_Surface* wormsSurfaceRight;
	SDL_Surface* wormsSurfaceTomb;
	SDL_Surface* texteLifeSurface;
	SDL_Surface* texteNameSurface;
	struct KaamObject* wormsObject;
	SDL_Color* color;
	int indexAnim;
	struct Equipe* team;
	char random;
	char randomCounter;
	//son?
} Worms;

/* Déclaration des structures concernant les armes */
typedef struct Munition{	//Structure munition
	char* nom;					//Nom de la munition
	int posx;					//Position en X
	int posy;					//Position en Y
	float angle;				//Angle de la munition
	//image?
} Munition;

typedef struct Arme{			//Structure arme
	char* nom;						//Nom de l'arme
	float degats;					//Dégats de l'arme
	int radius;						//Diamètre d'action de l'arme
	int range;						//Ce paramètre ne s'applique que pour les armes non lancées (mitrailleuse, ce genre de truc)
	int nbmuni;						//Nombre de munitions
	struct Munition muni;			//Munition en cours d'utilisation
	float angle;					//Angle de l'arme (sa position est celle du worms)
	struct KaamObject* weaponObject;//Pour une grenade : A initialiser avec la vitesse horizontale et la vitesse verticale correspondant au tir
	/*Dépendent de la puissance et l'inclinaison du tir, les vitesses sont exprimées en A.cos(angle) pour X et B.sin(angle) pour Y*/
	//image?
	//son?
} Arme;

typedef struct Caisse{
	char* nom;			//Nom de la caisse
	int posx;			//Position en X
	int posy;			//Position en Y
	struct KaamObject* boxObject;
	//image?
} Caisse;

/*Structures globales*/
typedef struct Point{		//structure Point
	int x;
	int y;
}Point;

typedef struct Cercle{
	float x;
	float y;
	float rayon;
}Cercle;

typedef struct Vector{
	float x;
	float y;
	double norm;
}Vector;

typedef struct Cursor{ //structure Curseur
	struct Point before;
	struct Point now;
	int motion;
	char currentCursor;
	SDL_Cursor * cursor1;
	SDL_Cursor * cursor2;
}Cursor;

typedef struct Input{		//Structure gérant les différentes inputs
	char jump;					//touche de saut
	char jumpOnGoing;			//indique qu'un saut est en cours et désactive les touches de direction
	char bend;					//touche pour s'accroupir
	char menu;					//touche de menu principal
	enum DIRECTION direction;	//stocke la direction du déplacement du worms
	char rclick;				//click droite de la souris
	char lclick;				//click gauche de la souris
	char quit;					//quitte le programme
	char weaponTab;
	char wheelUp;				//zoomIn
	char wheelDown;				//zoomOut
	char raffraichissement;
	char windowResized;
	char acceleration;
	char bombe;
	char deplacement;
	char screenshot;
	char camCentrer;
	char changeWorms;
	char arme;
	char soundAllowed;
	char musicAllowed;
	char textInput[100];
	unsigned char textCounter;
	struct Cursor cursor;		//cursor avec position actuelle et précédente de la souris
}Input;

/* Structure objet KaamEngine */
typedef struct KaamObject{
	SDL_Rect objectBox;
	struct Point absoluteCoordinate;
	struct Point precedentCoordinate;
	enum DIRECTION motionDirection;
	float Xspeed;
	float Yspeed;
	int weapon;
	int reactToBomb;
	int rightOk;
	int leftOk;
	int falling;
	int rebound;
	int startMotion;
	int relativeTime;
	SDL_Surface* objectSurface;
}KaamObject;


typedef struct GlobalVariable{		//structure gérant les polices
	TTF_Font * FontName[2];
	SDL_Color colorTab[4];

	int nbEquipe;
	int nbWormsEquipe[4];
	int nbWormsTotal;
	int gameEnd;
	Uint32 timeLastWormsChange;

	//Varibles d'index
	int teamPlaying;
	int wormsPlaying[4];
	int indexWormsTab;

	//Initialisation du jeu
	char savePath[100];
	char teamNames[4][50];
	char wormsNames[16][50];

}GlobalVariable;

#endif
