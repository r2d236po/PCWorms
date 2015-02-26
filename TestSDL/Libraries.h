#ifndef LIBRARIES_H
#define LIBRARIES_H

/*Bibliothèques de base*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

/*Bibliothèques supplémentaires*/
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>


//structures globales

typedef struct{		//structure Point
	int x;
	int y;
}Point;

typedef struct{ //structure Curseur
	Point before;
	Point now;
}Cursor;

typedef struct{		//structure gérant les différents inputs
	char jump;	//touche de saut
	char bend;	//touche pour s'accroupir
	char menu;	//touche de menu principal
	char rclick;		//click droite de la souris
	char left;	//touche de déplacement à gauche
	char right;	//touche de déplacement à droite
	char up;	//touche de déplacement haut
	char down;	//touche de déplacement bas
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



#endif