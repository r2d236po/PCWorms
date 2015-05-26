#ifndef HUD_H
#define HUD_H

#include "Libraries.h" //Inclus toutes les librairies

#define INGAMEMENU "../assets/pictures/Interface/ingameMenu.png"
#define INGAMEMENUMAINMENU "../assets/pictures/Interface/ingameMenuMainMenu.png"
#define INGAMEMENUOPTIONS "../assets/pictures/Interface/ingameMenuOptions.png"
#define INGAMEMENUQUIT "../assets/pictures/Interface/ingameMenuQuit.png"
#define DEFAULTHUDMAINW 565.0
#define DEFAULTHUDMAINH 717.0

#define INGAMEOPTION "../assets/pictures/Interface/optionsMenu.png"
#define INGAMEOPTIONCONFIG "../assets/pictures/Interface/optionsMenuConfig.png"
#define INGAMEOPTIONBACKTOGAME "../assets/pictures/Interface/optionsMenuBack.png"
#define INGAMEOPTIONHOWTOPLAY "../assets/pictures/Interface/optionsMenuHow.png"
#define DEFAULTHUDOPTIONW 565.0
#define DEFAULTHUDOPTIONH 842.0

#define INGAMEHOW "../assets/pictures/Interface/howToPlayMenu.png"
#define INGAMEHOWOPTION "../assets/pictures/Interface/howToPlayMenuOptions.png"
#define DEFAULTHUDHOWNW 1452.0

#define INGAMECONFIG "../assets/pictures/Interface/configMenu.png"
#define INGAMECONFIGOPTION "../assets/pictures/Interface/configMenuOption.png"
#define DEFAULTHUDCONFIGNW 1452.0

#define WEAPONTABPATH "../assets/pictures/Interface/weaponTab.png"
#define WEAPONTABW 1502.0
#define WEAPONTABH 842.0
#define SELECTTEXTURE "../assets/pictures/Interface/weaponSelect.png"
#define NBWEAPON 3

#define NBFONTS 1
#define FONTSIZENAME 12
#define TEMPSPARTIE 900
#define TEMPSPARTOUR 30
#define TEMPSAPRESDERNIERTIR 5

enum HUDMENU{HUDMAIN,HUDOPTION,HUDCONFIG,HUDHOW};

int mainInit();					// Initialise les polices et les variables globales
void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b);
void updateTextSurfaceWormsTab(Worms** wormsTab);
int updateTextSurfaceWorms(Worms* pWorms);
void updateTextSurfacePosition(Worms* pWorms);
void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, int reset);
void HUD_weaponsMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, int reset);
int HUD_selectWeapon(SDL_Rect rect, SDL_Rect *rectReturn);
SDL_Rect getRectMenu(enum HUDMENU menu);
SDL_Texture* getTextureMenu(enum HUDMENU menu, int subMenu);
enum HUDMENU mainHUD(int *subMenu, SDL_Rect mainRect);
enum HUDMENU optionHUD(int *subMenu, SDL_Rect mainRect);
enum HUDMENU configHUD(int *subMenu, SDL_Rect mainRect);
enum HUDMENU howHUD(int *subMenu, SDL_Rect mainRect);
SDL_Rect initHUDRect(int x, int y, int w, int h, SDL_Rect mainRect, double defW, double defH);
void updateHUD(Worms** wormsTab);
void updateRectTimerPosition();
void EngGameScreen(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera);
int setFonts();
void destroyFonts();

#endif