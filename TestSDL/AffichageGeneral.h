

#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H

#include "worms.h"
#include "HUD.h"
#include "carte.h"
#include "armes.h"
#include "partie.h"

#define FRAME_RATE (1000 / 80)



int mainFenetre();


//Fonctions d'initialisation
int initSWR();	//Initialise la SDL, la SDL_Image, le pWindow et le pRenderer
void initCameras(Terrain * pMapTerrain, SDL_Rect * pCamera, Worms  * pWorms);	//Initialise la position de la caméra
int initSprites(void);
Worms** initWormsTab(Equipe** equipes);

//Fonctions diverses
void afficherPoint();	//affiche un point 
void afficherLigne(Point * p1, Point * p2);	//affiche une ligne
void deplacementRectangle(SDL_Rect * rect, int x2, int y2, int dir);	//déplace un rectangle en fonction de la souris
void frameRate(unsigned int fM);	//gère le framerate
void screenshot();
void getMousePosition(SDL_Rect * camera, int * x, int *y);

//gestion de l'affichage
void renderScreen(int nb, ...);	//actualise l'affichage
void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput);	//déplace la caméra dans l'espace
void zoomIn(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput);	//Zoom In, grossis
void zoomOut(SDL_Texture* pTexture, SDL_Rect * camera);	//Zoom out, rétrécis
int centerCam(SDL_Rect * camera, SDL_Surface * surfaceWhereCenter, SDL_Texture* pTexture);

//Fonctions de nettoyage
void cleanUp(Input** p_pInput, SDL_Texture** p_pTextureDisplay);	//nettoie et quit la SDL
void clearRenderer();	//Clear de l'écran
void cleanSprites(void);



#endif
