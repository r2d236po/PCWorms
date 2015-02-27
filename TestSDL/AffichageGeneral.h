

#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h" //Inclus toutes les librairies
#include "worms.h"
#include "armes.h"
#include "carte.h"

#define FRAME_RATE (1000 / 70)



int mainFenetre();
int sandboxRenderer();


//Fonctions d'initialisation
int initSWR(SDL_Window** pWindow, SDL_Renderer **pRenderer);	//Initialise la SDL, la SDL_Image, le pWindow et le pRenderer
Input* initInput();	//Initialise les Input
Worms* createWorms(const char *file1, const char* file2);	//Créé un worms à partir d'une image
SDL_Surface * loadImage(const char * file);	//Charge une image dans une surface
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file);	//Charge une image dans une texture
SDL_Window * creerFenetre(const int w, const int h, const char * nom);	//Créé une fenêtre de largeur w, hauteur h et de nom nom
void initCameras(SDL_Renderer * pRenderer, Terrain * map, SDL_Rect * camera);	//Initialise la position de la caméra
int createGlobalTexture(SDL_Surface* pSurfaceTab[], int nbSurface, SDL_Texture** pTexture, SDL_Renderer* pRenderer,SDL_Rect* camera);

//Fonctions diverses
void afficherPoint(SDL_Renderer * r);	//affiche un point 
void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2);	//affiche une ligne
void deplacementRectangle(SDL_Rect * rect, int x2, int y2, int dir);	//déplace un rectangle en fonction de la souris
void deplacementWorms(Input* pInput, Worms* worms, SDL_Surface* surfaceCollision);	//déplace un worms au clavier
void frameRate(unsigned int fM);	//gère le framerate
void getInput(Input* pInput, SDL_Window* pWindow);	//récupère les inputs
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms, SDL_Surface * surfaceTab[]);
//gestion des inputs
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms, SDL_Surface * surfaceTab[]);
void updateScreen(SDL_Renderer * pRenderer, int nb, ...);	//actualise l'affichage
void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput);	//déplace la caméra dans l'espace
void zoomIn(SDL_Renderer * pRenderer, SDL_Rect * camera);	//Zoom In, grossis
void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera);	//Zoom out, rétrécis
int updateGlobaleTexture(SDL_Surface* pSurfaceTab[], SDL_Texture* pTexture, int surface, SDL_Rect* pRect);	//Met à jour la texture globale
SDL_Surface* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height);	//Met à l'échelle la surface de collision

//Fonctions de nettoyage
void destroyWorms(Worms** worms);	//détruit un worms
void destroyMap(Terrain** map);	//détruit un terrain
void cleanUp(SDL_Window** pWindow, SDL_Renderer** pRenderer, Input** pInput);	//nettoie et quit la SDL
void clearRenderer(SDL_Renderer * pRenderer);	//Clear de l'écran

void animationWorms(SDL_Surface * display);

#endif
