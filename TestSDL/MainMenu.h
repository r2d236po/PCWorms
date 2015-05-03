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
#define WIDTHMENUTEXTURE 2000.0
#define HIGHTMENUTEXTURE 1125.0
enum MENU { MAIN, MAINversus, MAINmap, MAINoption, VERSUS,VERSUSn, VERSUSm,VERSUSname, VERSUSnameN, VERSUSnameM, VERSUSstart, VERSUSstartS, MAP, MAPmain, MAPchoose, MAPrepertory, OPTIONS,OPTIONSm };
enum CHOICE{ NEXT, PREVIOUS, NEITHER, YES, NO, DEFAULT, SET, TEXT, CLICK};

int mainMenu(Input* pInput, char mapName[100]);
int initTabTextureMenu(SDL_Texture* menuTexture[NBTEXTURE], ...);
void destroyTextureTab(SDL_Texture* textureTab[NBTEXTURE]);
char* stringTexture(enum MENU menu);

enum MENU menu(Input* pInput);


enum MENU versusMenu(Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam);
void setTeamName(Input* pInput);
void setWormsName(Input* pInput, int indexTeam);
void setTextInput(Input* pInput, char* str, int indexPrec, int indexNow);
int getTeamIndexText(Input* pInput);
int getWormsIndexText(Input* pInput, int init);
int getIndexText(Input* pInput, int xBox, int init);
void setColorTeam(Input* pInput);
int getIndexColor(Input* pInput);
void initColorTab(SDL_Color colorArray[6]);

enum MENU mapMenu(Input* pInput, enum CHOICE *nextPrev);
void  mapSketch(enum CHOICE nextPrev, char* mapName);

enum MENU optionMenu(Input* pInput);
void setResizableOption(Input* pInput, enum CHOICE *pChoice);
void setMusicOption(Input* pInput, enum CHOICE *pChoice);
void setSizeOption(Input* pInput, enum CHOICE *pChoice);
void setSavePathOption(Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize);
void setSoundOption(Input* pInput, enum CHOICE *pChoice);
void toggleOptions(Input* pInput, enum CHOICE *pChoice, int x, int y);
void SDL_SetWindowResizable(SDL_bool resizable);
SDL_Texture* loadFromRenderedText(char* textureText, SDL_Color textColor, int *w, int *h, int size);
int testEndInput(char* text, int size);
void getSizeWindow(int *w, int *h, char* str);

int indiceTexture(enum MENU menu);
void setColorForGame();
void renderText(char* str, int x, int y, int sizeFont, SDL_Color color);
SDL_Rect initButtonBox(int x, int y, int w, int h);
#endif // !MAINMENU_H
