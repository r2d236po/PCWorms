#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h" //Inclus toutes les librairies
#include "carte.h"

#define FRAME_RATE (1000 / 70)

typedef struct{		//structure g�rant les diff�rents inputs
	int x;
	int y;
}Point;

typedef struct{
	Point before;
	Point now;
}Cursor;

typedef struct{		//structure g�rant les diff�rents inputs
	char jump;	//touche de saut
	char bend;	//touche pour s'accroupir
	char menu;	//touche de menu principal
	char rclick;		//click droite de la souris
	char left;	//touche de d�placement � gauche
	char right;	//touche de d�placement � droite
	char up;	//touche de d�placement haut
	char down;	//touche de d�placement bas
	char lclick;	//click gauche de la souris
	char quit;	//quitte le programme
	char weaponTab;
	char wheelUp; //zoomIn
	char wheelDown; //zoomOut
	char windowResized;
	Cursor cursor; //cursor avec position actuelle et pr�c�dente
}Input;

int mainFenetre();
int sandboxRenderer();
int afficheImage(SDL_Window *pWindow, SDL_Surface * image);
int initSWR(SDL_Window** pWindow, SDL_Renderer **pRenderer);
int gestInput(Input* pInput, SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect* camera);

void afficherPoint(SDL_Renderer * r);
void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2);
void clearRenderer(SDL_Renderer * pRenderer);
void deplacementRectangle(SDL_Renderer * pRenderer, SDL_Rect * rect, int x2, int y2,int dir);
void frameRate(int fM);
void getInput(Input* pInput, SDL_Window* pWindow);
void initInput(Input* pInput);
void updateScreen(SDL_Renderer * pRenderer, SDL_Rect * camera, SDL_Surface* pSurface, int nb, ...);
void initCameras(const SDL_Renderer * pRenderer, Terrain * map, SDL_Rect * camera);
void moveCam(Terrain * map, SDL_Rect * camera, Input * pInput);
void zoomIn(SDL_Renderer * fenetre, SDL_Rect * camera);
void zoomOut(SDL_Renderer * fenetre, Terrain * map, SDL_Rect * camera);
SDL_Surface* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height);


SDL_Surface * loadImage(const char * file);
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file);
SDL_Window * creerFenetre(const int w, const int h, const char * nom);



#endif
