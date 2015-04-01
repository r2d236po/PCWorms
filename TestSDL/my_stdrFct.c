#include "Libraries.h"
#include "my_stdrFct.h"

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////              Création d'une fenêtre                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn SDL_Window * creerFenetre(const int windowWidth, const int windowHight, const char * windowName)
* \brief Cree une fenetre SDL.
*
* \param[in] windowWidth, largeur de la fenetre
* \param[in] windowHight, hauteur de la fenetre
* \param[in] windowName, nom de la fenetre
* \returns pointeur vers la fenetre creee, NULL si echec
*/
SDL_Window * creerFenetre(const int windowWidth, const int windowHight, const char * windowName){
	SDL_Window * pWindow = NULL;
	pWindow = SDL_CreateWindow(windowName,	//nom de la fenêtre
		SDL_WINDOWPOS_CENTERED, //position en x de la fenêtre
		SDL_WINDOWPOS_CENTERED,	//position en y de la fenêtre
		windowWidth,	//largeur de la fenêtre
		windowHight,	//hauteur de la fenêtre
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);	//propriétés supplémentaires de la fenêtre
	if (pWindow == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "creerFenetre : FAILURE, erreur de création de la fenêtre: %s.\n", SDL_GetError());
		return NULL;
	}
	if (logFile != NULL)
		fprintf(logFile, "creerFenetre : SUCCESS.\n");
	return pWindow;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////               Chargement d'images                      /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn SDL_Surface * loadImage(const char * file)
* \brief Cree une surface a partir d'une image exterieur.
*
* \param[in] file, nom du fichier image a ouvrir
* \returns pointeur vers la surface creee, NULL si echec
*/
SDL_Surface * loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "loadImage : FAILURE, probleme lors du chargement de la surface : %s.\n\tnom du fichier : %s.", IMG_GetError(), file);
		return NULL;
	}
	if (logFile != NULL)
		fprintf(logFile, "loadImage : SUCCESS.\n\tnom du fichier : %s.\n", file);
	return image;
}


/**
* \fn SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file)
* \brief Cree une texture a partir d'une image exterieur dans le renderer.
*
* \param[in] pRenderer, pointeur vers le renderer de la fenetre
* \param[in] file, nom du fichier image a ouvrir
* \returns pointeur vers la texture creee, NULL si echec
*/
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file)
{

	SDL_Texture * texture = IMG_LoadTexture(pRenderer, file);
	if (texture == NULL)
	{
		if (logFile != NULL)
			fprintf(logFile, "loadTexture : FAILURE, probleme chargement texture : %s.\n\tnom du fichier : %s.\n", IMG_GetError(), file);
		return NULL;
	}
	if (logFile != NULL)
		fprintf(logFile, "loadTexture : SUCCESS.\n\tnom du fichier : %s.\n", file);
	return texture;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////               Manipulation des pixels                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
* \brief Lit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] pSurface, surface dans laquelle on lit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \returns Uint32, pixel lu aux coordonnees contenant les 4 canaux (RGBA)
* \remarks Faire attention que x et y soient bien compris dans la surface.
*/

Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixels de la surface
	//Get the requested pixel
	return pixels[(y * pSurface->w) + x];	//Return le pixel aux coordonnées x et y passées en paramètre
}


/**
* \fn void DrawPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
* \brief Ecrit un pixel aux coordonnees x et y d'une surface.
*
* \param[in] pSurface, surface dans laquelle on ecrit le pixel
* \param[in] x, abscisse dans la surface
* \param[in] y, ordonne dans la surface
* \param[in] pixelToWrite, pixel a ecrire
*/
void DrawPixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixel de la surface

	//Set the pixel
	pixels[(y * pSurface->w) + x] = pixelToWrite;	//Ecrit le pixel aux coordonnées passées en paramètre
}

/**
* \fn int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest)
* \brief Ecrit des pixels d'une surface dans une autre surface (comme BlitSurface).
*
* \param[in] pSurfaceSrc, surface source
* \param[in] pRectSrc, rectangle de destination dans la surface source, NULL pour toute la surface
* \param[in] pSurfaceDest, surface de destination
* \param[in] pRectDest, rectangle de destination dans la surface de destination, NULL démarre à zéro
* \returns 1 = copie OK, 0 = probleme de taille entre la source et la destination
*/
int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest)
{
	Uint32* pixelSrc = (Uint32*)pSurfaceSrc->pixels;
	Uint32* pixelDest = (Uint32*)pSurfaceDest->pixels;
	int ySrc = 0, xSrc = 0, xDest = 0, yDest = 0, wRectSrc = pSurfaceSrc->w, hRectSrc = pSurfaceSrc->h, wDest = pSurfaceDest->w, wSrc = pSurfaceSrc->w;
	int indexDest = 0, indexSrc = 0, index = 0, nbPixels = 0, offsetx, offsety;
	if (pRectSrc != NULL)
	{
		if (!checkRectSurfaceDimension(pSurfaceSrc, pRectSrc))
			return 0;
		ySrc = pRectSrc->y;
		xSrc = pRectSrc->x;
		wRectSrc = pRectSrc->w;
		hRectSrc = pRectSrc->h;
	}
	if (pRectDest == NULL && wRectSrc * hRectSrc > (pSurfaceDest->w *pSurfaceDest->h))
		return 0;
	else if (pRectDest != NULL && (wRectSrc * hRectSrc > (pRectDest->w * pRectDest->h)))
		return 0;
	else if (pRectDest != NULL)
	{
		if (!checkRectSurfaceDimension(pSurfaceDest, pRectDest))
			return 0;
		yDest = pRectDest->y;
		xDest = pRectDest->x;
	}
	nbPixels = wRectSrc * hRectSrc;
	indexDest = xDest + yDest*wDest;
	indexSrc = xSrc + ySrc*wSrc;
	for (index = 0; index < nbPixels; index++)
	{
		offsetx = index % wRectSrc;
		offsety = index / wRectSrc;
		pixelDest[indexDest + offsetx + offsety*wDest] = pixelSrc[indexSrc + offsetx + offsety*wSrc]; //pixelDest[(xDest + index % wRectSrc) + (index / wRectSrc + yDest)*wDest] = ...xSrc...ySrc...wSrc
	}
	return 1;
}

/**
* \fn int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect)
* \brief Test si un rect est compris dans une surface.
*
* \param[in] pSurface, surface source
* \param[in] pRect, rectangle de destination dans la surface source
* \returns 1 = dimensions OK, 0 = probleme de taille entre le rect et la surface
*/
int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect)
{
	if (pRect->w > pSurface->w || (pRect->x + pRect->w) > pSurface->w)
		return 0;
	else if (pRect->x < 0 || pRect->x >(pSurface->clip_rect.x + pSurface->w))
		return 0;
	else if (pRect->h > pSurface->h || (pRect->y + pRect->h) > pSurface->h)
		return 0;
	else if (pRect->y < 0 || pRect->y >(pSurface->clip_rect.y + pSurface->h))
		return 0;
	return 1;
}

/**
* \fn int checkRectOverlay(SDL_Rect* pRect, SDL_Rect* pRect2)
* \brief Test deux rect se superpose.
*
* \param[in] pRect, rectangle 1
* \param[in] pRect2, rectangle 2
* \returns 1 = overlay, 0 = pas d'overlay
*/
int checkRectOverlay(SDL_Rect* pRect, SDL_Rect* pRect2)
{
	int widthRel1 = pRect->w + pRect->x, widthRel2 = pRect2->w + pRect2->x;
	int hightRel1 = pRect->h + pRect->y, hightRel2 = pRect2->h + pRect2->y;
	if (widthRel1 >= pRect2->x && widthRel1 <= widthRel2 && hightRel1 >= pRect2->y && hightRel1 <= hightRel2)
		return 1;
	else if (widthRel2 >= pRect->x && widthRel2 <= widthRel1 && hightRel2 >= pRect->y && hightRel2 <= hightRel1)
		return 1;
	return 0;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Fonctions Diverses                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn SDL_Rect initRect(int x, int y, int w, int h)
* \brief Cree et nitialise un rectangle.
*
* \param[in] x, position en x du rectangle
* \param[in] y, position en y du rectangle
* \param[in] w, largeur du rectangle
* \param[in] h, hauteur du rectangle
* \return rect, une structure SDL_Rect initialisee
*/
SDL_Rect initRect(int x, int y, int w, int h)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}
