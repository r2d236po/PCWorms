#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h"
#include "carte.h"

SDL_Texture* pGlobalTexture;
Terrain* pMainTerrain;

#define NBSURFACEMAX 50

/*Init display*/
int initDisplay(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay);
void endDisplay();

/*Display*/
int display(SDL_Surface* pSurface, int mode);
void reindexTab(int size, int startPosition, int type, void* tab);
#endif // !DISPLAY_H
