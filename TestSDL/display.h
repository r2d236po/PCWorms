#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h"

#define NBSURFACEMAX 100

/*Init display*/
int initDisplay(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay);
void endDisplay();

/*Display*/
int display(SDL_Surface* pSurface, int mode);
int displayWorms(Worms* pWorms, int mode);
void displayWormsTab(Worms** wormsTab, int mode);
void reindexTab(int size, int startPosition, int type, void* tab);
SDL_Surface* animationSprite(SDL_Surface* pSurfaceSprite, SDL_Surface* pAnimSurface, int nbFrame, int indexAnim);
void recenterSurface(SDL_Surface* oldSurface, SDL_Surface* newSurface);
#endif // !DISPLAY_H
