#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies

#define INGAMEMENU "../assets/pictures/Interface/ingameMenu.png"
#define INGAMEMENUMAINMENU "../assets/pictures/Interface/ingameMenuMainMenu.png"
#define INGAMEMENUOPTIONS "../assets/pictures/Interface/ingameMenuOptions.png"
#define INGAMEMENUQUIT "../assets/pictures/Interface/ingameMenuQuit.png"

#define NBFONTS 1
#define FONTSIZENAME 12
#define TEMPSPARTOUR 30


int mainInit();					// Initialise les polices et les variables globales
void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);
void updateTextSurfaceWormsTab(Worms** wormsTab);
int updateTextSurfaceWorms(Worms* pWorms);
void updateTextSurfacePosition(Worms* pWorms);
void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);
void updateHUD(Worms** wormsTab, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);
int setFonts();
void destroyFonts();

#endif