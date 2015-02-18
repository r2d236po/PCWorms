#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h" //Inclus toutes les librairies
#include "carte.h"

#define FRAME_RATE (1000 / 60)


typedef struct{		//structure gérant les différents inputs
	char jump;	//touche de saut
	char bend;	//touche pour s'accroupir
	char menu;	//touche de menu principal
	char weaponTab;		//touche ouvrant le menu des armes
	char left;	//touche de déplacement à gauche
	char right;	//touche de déplacement à droite
	char up;	//touche de déplacement haut
	char down;	//touche de déplacement bas
	char click;	//click gauche de la souris
	char quit;	//quitte le programme
}Input;

int mainFenetre();
int sandboxRenderer();
int afficheImage(SDL_Window *pWindow, SDL_Surface * image);

void afficherPoint(SDL_Renderer * r);
void clearRenderer(SDL_Renderer * r);
void deplacementRectangle(SDL_Renderer * rend, SDL_Rect * rect, int x2, int y2);
void frameRate(int fM);
void getInput(Input* pInput);
int gestInput(Input* pInput);
void initInput(Input* pInput);

SDL_Surface * loadImage(const char * file);
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);



#endif
