#ifndef MY_STDRFCT_H
#define MY_STDRFCT_H

/*Création d'une fenêtre*/
SDL_Window * creerFenetre(const int windowWidth, const int windowHight, const char * windowName);	//Créé une fenêtre de largeur w, hauteur h et de nom nom

/*Chargement d'image dans une surface ou une texture*/
SDL_Surface * loadImage(const char * file);	//Charge une image dans une surface
SDL_Texture * loadTexture(const char * file);	//Charge une image dans une texture
SDL_Texture* my_createTextureFromSurface(SDL_Surface* pSurface);

/*Manipulation des pixels*/
Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y);
void WritePixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite);
int pixelTransparent(Uint32 pixelToRead, SDL_PixelFormat* format);
int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest);
void updateSurfaceFromSurface(SDL_Surface* pSurfaceDest, SDL_Surface* pSurfaceSrc, SDL_Rect* pRect, char mode);

/*Update Texture*/
int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRectPrec);
int updateTextureFromSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Rect* pRect);


/*Basics collisions*/
int collisionRectWithRect(SDL_Rect* pRect, SDL_Rect* pRect2);
int collisionRectWithMap(SDL_Surface* pSurfaceMap, SDL_Rect* pRect, int* xE, int* yE);
int collisionPointWithCercle(Point P, int centerX, int centerY, int radius);
int collisionPointWithRect(Point P, SDL_Rect* box);
int pointProjectionOnSegment(Point C, int Ax, int Ay, int Bx, int By);

/*Fonctions diverse*/
SDL_Rect initRect(int x, int y, int w, int h);	//Initialise un SDL_Rect
SDL_Rect createGlobalRect(int nbRect, SDL_Rect** rectTab);
void reajustSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);
int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap);	//réajuste un rectangle pour pas dépasser de la map
int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect);
int rand_a_b(int a, int b);
void centerRectToPoint(SDL_Rect* pRect, int x, int y);

/*Manipulation de formes*/
int detectShape(SDL_Surface* pSurface, SDL_Point* shapeTab);
void drawShape(SDL_Point* shapeTab, int nbPoint);
void moveShape(SDL_Point* shapeTab, int dx, int dy, int nbPoint);
int simplifiedShape(SDL_Point* shapeTab, int nbShapeOriginal, int coeff);
#endif