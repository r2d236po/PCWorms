#ifndef DISPLAY_H
#define DISPLAY_H

#include "Libraries.h"

#define NBSURFACEMAX 50

/*Init display*/
int initDisplay(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay);
void endDisplay();

/*Display*/
int display(SDL_Surface* pSurface, int mode);
int displayWorms(Worms* pWorms, int mode);
void reindexTab(int size, int startPosition, int type, void* tab);
SDL_Rect animationSprite(SDL_Surface* pSurfaceSprite, int nbFrame, int indexAnim);
#endif // !DISPLAY_H
