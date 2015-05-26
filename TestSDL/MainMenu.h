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

int mainMenu(char mapName[100]);
int initTabTextureMenu(SDL_Texture* menuTexture[NBTEXTURE], ...);
void destroyTextureTab(SDL_Texture* textureTab[NBTEXTURE]);
char* stringTexture(enum MENU menu);
char** initMAPstrings();
char* readString(FILE* file);
int getTaille(FILE *fichier);
int lireEntete(FILE* file);
void destroyMenuTab(char **tab, int size);

enum MENU menu();


enum MENU versusMenu(int* quit, enum MENU menuPrec, int *pIndexTeam);
void setTeamName();
void setWormsName( int indexTeam);
void setTextInput(char* str, int indexPrec, int indexNow);
int getTeamIndexText();
int getWormsIndexText(int init);
int getIndexText(int xBox, int init);
void setColorTeam();
int getIndexColor();
void initColorTab(SDL_Color colorArray[6]);

enum MENU mapMenu(enum CHOICE *nextPrev);
void  mapSketch(enum CHOICE nextPrev, char* mapName, char **mapString);

enum MENU optionMenu();
void setResizableOption(enum CHOICE *pChoice);
void setMusicOption(enum CHOICE *pChoice);
void setSizeOption(enum CHOICE *pChoice);
void setSavePathOption(enum CHOICE *pChoice, enum CHOICE windowSize);
void setSoundOption(enum CHOICE *pChoice);
void toggleOptions(enum CHOICE *pChoice, int x, int y);
void SDL_SetWindowResizable(SDL_bool resizable);
SDL_Texture* loadFromRenderedText(char* textureText, SDL_Color textColor, int *w, int *h, int size);
int testEndInput(char* text, int size);
void getSizeWindow(int *w, int *h, char* str);

int indiceTexture(enum MENU menu);
void setColorForGame();
void renderText(char* str, int x, int y, int sizeFont, SDL_Color color);
SDL_Rect initButtonBox(int x, int y, int w, int h);
#endif // !MAINMENU_H
