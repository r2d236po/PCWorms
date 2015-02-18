#ifndef CARTE_H
#define CARTE_H

#include "Libraries.h" //Inclus toutes les librairies

/* Déclaration des structures et constantes relatives à la carte jouée */


typedef struct {	/* Structure terrain */
	//mapcollision?				
	SDL_Texture* imageBackground; /* background */
	SDL_Texture* imageMap; /* background */
	//mapdestruction?
} Terrain;

typedef struct{
	char* nom;			/* Nom de la caisse */
	int posx;			/* Position en X */
	int posy;			/* Position en Y */
	//image?
} Caisse;

void initialisionTerrain(Terrain * map, SDL_Renderer * pRenderer, const char * file, const char * file2);

#endif