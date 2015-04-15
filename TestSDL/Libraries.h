#ifndef LIBRARIES_H
#define LIBRARIES_H

/*Bibliothèques de base*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*Bibliothèques supplémentaires*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

/*Macros*/
#define MY_ABS(a) ((a) < 0 ? (-a) : (a))
#define SWAP(x,y) ((x ^= y), (y ^= x), (x ^= y))
#define CARRE(a) ((a) * (a))

/*Constantes*/
#define pi 3.14159265358
#define RMASK 0x000000FF
#define GMASK 0x0000FF00
#define BMASK 0x00FF0000
#define AMASK 0xFF000000

/*Chemins de map*/
#define cMAP		"../assets/pictures/map.png"
#define cMAP_HD		"../assets/pictures/mapHD2.png"
#define cMAP_TEST	"../assets/pictures/maptest.png"
#define cMAP_TEST2	"../assets/pictures/maptest2.png"
#define cMAP_TEST3	"../assets/pictures/maptest3.png"
#define cMAP_TEST4	"../assets/pictures/maptest4.png"
#define ICONE "../assets/pictures/icone.png"

/*Structures globales*/
typedef struct{		//structure Point
	int x;
	int y;
}Point;

typedef struct{
	float x;
	float y;
	float rayon;
}Cercle;

typedef struct{
	float x;
	float y;
	double norm;
}Vector;

typedef struct{ //structure Curseur
	Point before;
	Point now;
	int motion;
}Cursor;

enum DIRECTION { RIGHT = 0, LEFT = 1, DOWN = 2, UP = 3, UPLEFT = 4, UPRIGHT = 5, DLEFT = 6, DRIGHT = 7, NONE = 8 };

/*Structure gérant les différentes inputs*/
typedef struct{		
	char jump;	//touche de saut
	char jumpOnGoing;	//indique qu'un saut est en cours et désactive les touches de direction
	char bend;	//touche pour s'accroupir
	char menu;	//touche de menu principal
	enum DIRECTION direction;	//stocke la direction du déplacement du worms
	char rclick;		//click droite de la souris
	char lclick;	//click gauche de la souris
	char quit;	//quitte le programme
	char weaponTab;
	char wheelUp; //zoomIn
	char wheelDown; //zoomOut
	char raffraichissement;
	char windowResized;
	char acceleration;
	char bombe;
	int wormsPlaying;
	char deplacement;
	char screenshot;
	char TestCentrer;
	Cursor cursor; //cursor avec position actuelle et précédente de la souris
}Input;


/*Structure objet KaamEngine*/
typedef struct{
	SDL_Rect objectBox;
	Point absoluteCoordinate;
	enum DIRECTION motionDirection;
	float Xspeed;
	float Yspeed;
	int weapon;
	int reactToBomb;
	int rightOk;
	int leftOk;
	int relativeTime;
	SDL_Surface* objectSurface;
}KaamObject;


typedef struct{		//structure gérant les polices
	TTF_Font * FontName;

	SDL_Color couleurNameBleu;
	SDL_Color couleurNameRouge;
	SDL_Color couleurNameVert;
	SDL_Color couleurNameJaune;

	int nbEquipe;
	int nbWormsEquipe;

}GlobalVariable;

GlobalVariable globalVar;
FILE* logFile;

SDL_Surface* spriteDeplacement;
SDL_Surface* spriteGrenadeExplosion;
SDL_Surface* spriteNukeExplosion;
#endif