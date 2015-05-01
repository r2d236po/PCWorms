#ifndef MAINMENU_H
#define MAINMENU_H

#include "Libraries.h"
#define MAINMENU "../assets/pictures/Interface/mainMenu.png"
#define MAINMENUMAP "../assets/pictures/Interface/mainMenuCartes.png"
#define MAINMENUOPTION "../assets/pictures/Interface/mainMenuOptions.png"
#define MAINMENUVERSUS "../assets/pictures/Interface/mainMenuVersus.png"

#define VERSUSMENU "../assets/pictures/Interface/versusMenu.png"
#define VERSUSMENUNEXT "../assets/pictures/Interface/versusMenuNext.png"
#define VERSUSMENUMAIN "../assets/pictures/Interface/versusMenuMain.png"
#define VERSUSMENUNAMEPLAYER "../assets/pictures/Interface/versusMenuNamePlayer.png"
#define VERSUSMENUNAMEPLAYERN "../assets/pictures/Interface/versusMenuNamePlayerNext.png"
#define VERSUSMENUNAMEPLAYERM "../assets/pictures/Interface/versusMenuNamePlayerMain.png"
#define VERSUSMENUSTART "../assets/pictures/Interface/versusMenuStart.png"
#define VERSUSMENUSTARTS "../assets/pictures/Interface/versusMenuStart2.png"

#define MAPMENU "../assets/pictures/Interface/mapMenu.png"
#define MAPMENUMAIN "../assets/pictures/Interface/mapMenuMainMenu.png"
#define MAPMENUCHOOSE "../assets/pictures/Interface/mapMenuChoose.png"
#define MAPMENUREPERTORY "../assets/pictures/Interface/mapMenuDirectory.png"

#define OPTIONMENU "../assets/pictures/Interface/mainOptionsMenu.png"
#define OPTIONMENUMAIN "../assets/pictures/Interface/mainOptionsMenuMain.png"
#define YESTEXTURE "../assets/pictures/Interface/yes.png"
#define NOTEXTURE "../assets/pictures/Interface/no.png"
#define DEFAULTTEXTURE "../assets/pictures/Interface/defaut.png"
#define SETTEXTURE "../assets/pictures/Interface/set.png"

#define NBTEXTURE 18
#define widthMenuTexture 2000.0
#define hightMenuTexture 1125.0
enum MENU { MAIN, MAINversus, MAINmap, MAINoption, VERSUS,VERSUSn, VERSUSm,VERSUSname, VERSUSnameN, VERSUSnameM, VERSUSstart, VERSUSstartS, MAP, MAPmain, MAPchoose, MAPrepertory, OPTIONS,OPTIONSm };
enum CHOICE{ NEXT, PREVIOUS, NEITHER, YES, NO, DEFAULT, SET, TEXT};

int mainMenu(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, char mapName[100], int *pNbTeam, int *pNbWorms);
int initTabTextureMenu(SDL_Renderer* pRenderer, SDL_Texture* menuTexture[NBTEXTURE], ...);
void destroyTextureTab(SDL_Texture* textureTab[NBTEXTURE]);

enum MENU menu(SDL_Renderer* pRenderer, Input* pInput);


enum MENU versusMenu(SDL_Renderer* pRenderer, Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam);
void setTeamName(SDL_Renderer* pRenderer, Input* pInput);
int getTeamIndexText(SDL_Renderer* pRenderer, Input* pInput);
void setWormsName(SDL_Renderer* pRenderer, Input* pInput, int indexTeam);
int getWormsIndexText(SDL_Renderer* pRenderer, Input* pInput);
int getIndexText(SDL_Renderer* pRenderer, Input* pInput, int xBox);
void setColorTeam(SDL_Renderer* pRenderer, Input* pInput);
int getIndexColor(SDL_Renderer* pRenderer, Input* pInput);

enum MENU mapMenu(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *nextPrev);
char* mapSketch(SDL_Renderer* pRenderer, enum CHOICE nextPrev);

enum MENU optionMenu(SDL_Renderer* pRenderer, Input* pInput);
void setResizableOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice);
void setMusicOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice);
void setSizeOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice);
void setSavePathOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize);
void setSoundOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice);
void toggleOptions(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, int x, int y);
void SDL_SetWindowResizable(SDL_Window *pWindow, SDL_bool resizable);
SDL_Texture* loadFromRenderedText(SDL_Renderer* pRenderer, char* textureText, SDL_Color textColor, int *w, int *h, int size);
int testEndInput(char* text, int size);
void getSizeWindow(int *w, int *h, char* str);

int indiceTexture(enum MENU menu);
void setColorForGame();
#endif // !MAINMENU_H
