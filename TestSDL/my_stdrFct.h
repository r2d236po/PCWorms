#ifndef MY_STDRFCT_H
#define MY_STDRFCT_H

/*Création d'une fenêtre*/
SDL_Window * creerFenetre(const int windowWidth, const int windowHight, const char * windowName);	//Créé une fenêtre de largeur w, hauteur h et de nom nom

/*Chargement d'image dans une surface ou une texture*/
SDL_Surface * loadImage(const char * file);	//Charge une image dans une surface
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file);	//Charge une image dans une texture
SDL_Texture* my_createTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer);
int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRectPrec);
int updateTextureFromSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Rect* pRect);

/*Manipulation des pixels*/
Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y);
void DrawPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite);
int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest);
int pixelTransparent(Uint32 pixelToRead, SDL_PixelFormat* format);

/*Fonctions diverse*/
SDL_Rect initRect(int x, int y, int w, int h);	//Initialise un SDL_Rect
int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect);
int checkRectOverlay(SDL_Rect* pRect, SDL_Rect* pRect2);
int my_malloc(void** var, size_t size, const char* fctName);
#endif