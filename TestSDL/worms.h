#ifndef WORMS_H
#define WORMS_H

#include "armes.h"
#include "Libraries.h" //Inclus toutes les librairies
#define TAILLEINV 5

typedef struct {		//Structure worms
	int vie;				//Point de vie
	char* nom;				//Nom du joueur
	char* arme;				//Arme en cours d'utilisation
	Arme* invent[TAILLEINV];//Inventaire des armes et objets disponibles
	int dx;				//Décalage en X
	int dy;				//Décalage en Y
	float vitx;
	float vity;
	int dir;				//Direction : gauche ou droite
	SDL_Rect wormsRect;
	SDL_Surface* wormsSurface;
	SDL_Surface* wormsSurfaceLeft;
	SDL_Surface* wormsSurfaceRight;

	//son?
} Worms;

#endif // !1


