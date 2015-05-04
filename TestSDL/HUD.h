#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies
#include "Worms.h"

#define INGAMEMENU "../assets/pictures/Interface/ingameMenu.png"
#define INGAMEMENUMAINMENU "../assets/pictures/Interface/ingameMenuMainMenu.png"
#define INGAMEMENUOPTIONS "../assets/pictures/Interface/ingameMenuOptions.png"
#define INGAMEMENUQUIT "../assets/pictures/Interface/ingameMenuQuit.png"

void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);
void updateTextSurfaceWorms(Worms** wormsTab);
void updateTextSurfacePosition(Worms** wormsTab);
void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);

#endif