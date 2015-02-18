#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h" //Inclus toutes les librairies
#include "carte.h"

#define FRAME_RATE (1000 / 70)

typedef struct{		//structure gérant les différents inputs
	int x;
	int y;
}Point;

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
	Point cursor;
}Input;

int mainFenetre();
int sandboxRenderer();
int afficheImage(SDL_Window *pWindow, SDL_Surface * image);
int initSWR(SDL_Window** pWindow, SDL_Renderer **pRenderer);

void afficherPoint(SDL_Renderer * r);
void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2);
void clearRenderer(SDL_Renderer * pRenderer);
void deplacementRectangle(SDL_Renderer * rend, SDL_Rect * rect, int x2, int y2);
void frameRate(int fM);
void getInput(Input* pInput);
int gestInput(Input* pInput, SDL_Renderer* pRenderer);
void initInput(Input* pInput);
void updateScreen(SDL_Renderer * pRenderer, SDL_Rect * camera, int nb, ...);
void initCameras(const SDL_Window * pWindow, SDL_Rect * camera);
void moveCam(Terrain * map, SDL_Rect * camera, Point pt);


SDL_Surface * loadImage(const char * file);
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);



#endif
