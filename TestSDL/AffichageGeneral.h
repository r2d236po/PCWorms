

#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H

#include "worms.h"
#include "HUD.h"
#include "carte.h"
#include "armes.h"
#include "partie.h"

#define FRAME_RATE (1000 / 80)



int mainFenetre(Jeu * jeu);
int sandboxRenderer();


//Fonctions d'initialisation
int initSWR(SDL_Window** p_pWindow, SDL_Renderer **p_pRenderer);	//Initialise la SDL, la SDL_Image, le pWindow et le pRenderer
void initCameras(SDL_Renderer * pRenderer, Terrain * pMapTerrain, SDL_Rect * pCamera, Worms  * pWorms);	//Initialise la position de la caméra
int initSprites(void);
Worms** initWormsTab(Equipe** equipes);

//Fonctions diverses
void afficherPoint(SDL_Renderer * r);	//affiche un point 
void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2);	//affiche une ligne
void deplacementRectangle(SDL_Rect * rect, int x2, int y2, int dir);	//déplace un rectangle en fonction de la souris
int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap);	//réajuste un rectangle pour pas dépasser de la map
void frameRate(unsigned int fM);	//gère le framerate
void screenshot(SDL_Renderer* pRenderer);

//gestion de l'affichage
void renderScreen(SDL_Renderer * pRenderer, int nb, ...);	//actualise l'affichage
void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput);	//déplace la caméra dans l'espace
void zoomIn(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput);	//Zoom In, grossis
void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera);	//Zoom out, rétrécis
int centerCam(SDL_Rect * camera, SDL_Surface * surfaceWhereCenter, SDL_Texture* pTexture);

//Fonctions de nettoyage

void cleanUp(SDL_Window** p_pWindow, SDL_Renderer** p_pRenderer, Input** p_pInput, SDL_Texture** p_pTextureDisplay);	//nettoie et quit la SDL
void clearRenderer(SDL_Renderer * pRenderer);	//Clear de l'écran
void cleanSprites(void);



#endif
