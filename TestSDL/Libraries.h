#ifndef LIBRARIES_H
#define LIBRARIES_H

/*Bibliothèques de base*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Bibliothèques supplémentaires*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>


//structures globales
#define MY_ABS(a) ((a) < 0 ? (-a) : (a))
#define SWAP(x,y) ((x ^= y), (y ^= x), (x ^= y))
#define pi 3.1415

/*Chemins de map*/
#define cMAP		"../assets/pictures/map.png"
#define cMAP_TEST	"../assets/pictures/maptest.png"
#define cMAP_TEST2	"../assets/pictures/maptest2.png"
#define cMAP_TEST3	"../assets/pictures/maptest3.png"

typedef struct{		//structure Point
	int x;
	int y;
}Point;

typedef struct{ //structure Curseur
	Point before;
	Point now;
	int motion;
}Cursor;

enum DIRECTION { RIGHT = 0, LEFT = 1, DOWN = 2, UP = 3, UPLEFT = 4, UPRIGHT = 5, DLEFT = 6, DRIGHT = 7, NONE = 8 };

typedef struct{		//structure gérant les différents inputs
	char jump;	//touche de saut
	char jumpOnGoing;	//indique qu'un saut est en cours et désactive les touches de direction
	char bend;	//touche pour s'accroupir
	char menu;	//touche de menu principal
	char rclick;		//click droite de la souris
	enum DIRECTION direction;	//stocke la direction du déplacement du worms
	char lclick;	//click gauche de la souris
	char quit;	//quitte le programme
	char weaponTab;
	char wheelUp; //zoomIn
	char wheelDown; //zoomOut
	char raffraichissement;
	char windowResized;
	char acceleration;
	char bombe;
	Cursor cursor; //cursor avec position actuelle et précédente
}Input;

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

#endif