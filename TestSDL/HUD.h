#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies

#define INGAMEMENU "../assets/pictures/Interface/ingameMenu.png"
#define INGAMEMENUMAINMENU "../assets/pictures/Interface/ingameMenuMainMenu.png"
#define INGAMEMENUOPTIONS "../assets/pictures/Interface/ingameMenuOptions.png"
#define INGAMEMENUQUIT "../assets/pictures/Interface/ingameMenuQuit.png"

void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);
void updateTextSurfaceWormsTab(Worms** wormsTab);
int updateTextSurfaceWorms(Worms* pWorms);
void updateTextSurfacePosition(Worms* pWorms);
void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);

#endif