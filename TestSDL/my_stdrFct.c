#include "Libraries.h"
#include "my_stdrFct.h"

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////              Création d'une fenêtre                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn SDL_Window * creerFenetre(const int windowWidth, const int windowHight, const char * windowName)
* \brief Create and initialize an SDL window.
*
* \param[in] windowWidth, width of the window.
* \param[in] windowHight, hight of the window.
* \param[in] windowName, name of the window.
* \returns pointer to the created window, NULL if error.
*/
SDL_Window* creerFenetre(const int windowWidth, const int windowHight, const char * windowName){
	SDL_Window * pWindow = NULL;
	pWindow = SDL_CreateWindow(windowName,	//nom de la fenêtre
		SDL_WINDOWPOS_CENTERED, //position en x de la fenêtre
		SDL_WINDOWPOS_CENTERED,	//position en y de la fenêtre
		windowWidth,	//largeur de la fenêtre
		windowHight,	//hauteur de la fenêtre
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);	//propriétés supplémentaires de la fenêtre
	if (pWindow == NULL)
	{
		fprintf(logFile, "creerFenetre : FAILURE, erreur de création de la fenêtre: %s.\n", SDL_GetError());
		return NULL;
	}
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
* \brief Create and load an image into a surface.
*
* \param[in] file, name of the image to load (with the entire path if necessary).
* \returns pointer to the created surface, NULL if error.
*/
SDL_Surface* loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
	{
		fprintf(logFile, "loadImage : FAILURE, probleme lors du chargement de la surface : %s.\n\tnom du fichier : %s.", IMG_GetError(), file);
		return NULL;
	}
	fprintf(logFile, "loadImage : SUCCESS.\n\tnom du fichier : %s.\n\n", file);
	return image;
}


/**
* \fn SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file)
* \brief Create and load an image into a texture for the current renderer.
*
* \param[in] pRenderer, pointer to the window's renderer.
* \param[in] file, name of the image to load (with the entire path if necessary).
* \returns pointer to the created texture, NULL if error
*/
SDL_Texture* loadTexture(SDL_Renderer * pRenderer, const char * file)
{

	SDL_Texture * texture = IMG_LoadTexture(pRenderer, file);
	if (texture == NULL)
	{
		fprintf(logFile, "loadTexture : FAILURE, probleme chargement texture : %s.\n\tnom du fichier : %s.\n", IMG_GetError(), file);
		return NULL;
	}
	fprintf(logFile, "loadTexture : SUCCESS.\n\tnom du fichier : %s.\n", file);
	return texture;
}

/**
* \fn SDL_Texture* my_createTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer)
* \brief Create a texture from a surface.
*
* \param[in] pSurface, pointer to the source surface.
* \param[in] pRenderer, pointer to the current renderer.
* \returns pointer to the created texture, NULL if error.
* \remarks It is like createTextureFromSurface with error handling and access to pixel enable.
*/
SDL_Texture* my_createTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer)
{

	SDL_Texture* textureTemp = NULL;
	textureTemp = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, pSurface->w, pSurface->h);
	if (textureTemp == NULL)
	{
		fprintf(logFile, "my_createTextureFromSurface : FAILURE, creation textureTemp : %s.\n\n", SDL_GetError());
		return NULL;
	}
	SDL_SetTextureBlendMode(textureTemp, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(textureTemp, NULL, pSurface->pixels, pSurface->pitch);
	fprintf(logFile, "my_createTextureFromSurface : SUCCESS.\n\n");
	return textureTemp;
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////               Manipulation des pixels                  /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
* \brief Read a pixel from a surface at (x,y) coordinates.
*
* \param[in] pSurface, pointer to the surface to read from.
* \param[in] x, X coordinate of the pixel to read in the surface.
* \param[in] y, Y coordinate of the pixel to read in the surface.
* \returns Uint32, pixel read on 32 bits with the surface's format.
* \remarks This function does not handle false coordinates, be careful.
*/

Uint32 ReadPixel(SDL_Surface* pSurface, int x, int y)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixels de la surface
	//Get the requested pixel
	return pixels[(y * pSurface->w) + x];	//Return le pixel aux coordonnées x et y passées en paramètre
}


/**
* \fn void WritePixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
* \brief Write a pixel to the (x,y) coordinates passed in parameters.
*
* \param[in] pSurface, pointer to the surface to write to.
* \param[in] x, X coordinate of the pixel to write in the surface.
* \param[in] y, Y coordinate of the pixel to write in the surface.
* \param[in] pixelToWrite, the pixel to write.
* \remarks Make sure that the formats of the pixel to write and the surface are the same.
*/
void WritePixel(SDL_Surface* pSurface, int x, int y, Uint32 pixelToWrite)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//Récupère le tableau de pixel de la surface

	//Set the pixel
	pixels[(y * pSurface->w) + x] = pixelToWrite;	//Ecrit le pixel aux coordonnées passées en paramètre
}


/**
* \fn int pixelTransparent(Uint32 pixelToRead,SDL_PixelFormat* format)
* \brief Test the transparency of a pixel.
*
* \param[in] pixelToRead, the pixel to test.
* \param[in] format, format of the pixel.
* \returns 1 = pixel transparent, 0 = pixel not transparent
*/
int pixelTransparent(Uint32 pixelToRead, SDL_PixelFormat* format)
{
	Uint8 a, b, g, r;
	SDL_GetRGBA(pixelToRead, format, &r, &g, &b, &a);
	return a < 150;
}

/**
* \fn int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest)
* \brief Make a copy from an area of a surface to an another area of a second surface.
*
* \param[in] pSurfaceSrc, pointer to the source surface.
* \param[in] pRectSrc, area to copy in the source surface, NULL for the entire surface.
* \param[in] pSurfaceDest, pointer to the destination surface.
* \param[in] pRectDest, area to paste the pixels, NULL will start the paste from the origin of the destination surface.
* \returns 1 = copy OK, 0 = size problem between source and destination
* \remarks This function does handle size issue between source and destination.
*		   The destination SDL_Rect is used only for location, the width and hight are based on the source SDL_Rect.
*/
int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest)
{
	Uint32* pixelSrc = (Uint32*)pSurfaceSrc->pixels;
	Uint32* pixelDest = (Uint32*)pSurfaceDest->pixels;
	int ySrc = 0, xSrc = 0, xDest = 0, yDest = 0, wRectSrc = pSurfaceSrc->w, hRectSrc = pSurfaceSrc->h, wDest = pSurfaceDest->w, wSrc = pSurfaceSrc->w;
	int indexDest = 0, indexSrc = 0, index = 0, nbPixels = 0;
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
	for (index = 0; index < nbPixels; index += wRectSrc)
	{
		indexDest = xDest + yDest*wDest;
		indexSrc = xSrc + ySrc*wSrc;
		memcpy((pixelDest + indexDest), (pixelSrc + indexSrc), wRectSrc*sizeof(Uint32));
		ySrc++;
		yDest++;
	}
	return 1;
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    Update Texture                      /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRect)
* \brief Update texture associated to a surface with a second surface.
*
* \param[in] pTexture, pointer to the texture
* \param[in] pSurfaceMain, pointer to the surface associated with the texture (the map).
* \param[in] pSurfaceSecond, pointer to the second surface
* \param[in] pRect, area to erase before display the second surface (usually the precedent location of the surface).
* \returns 0 = OK, -1 = problem allocating memory
*/
int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRect)
{
	if (pRect->x != pSurfaceSecond->clip_rect.x || pRect->y != pSurfaceSecond->clip_rect.y)
		updateTextureFromSurface(pTexture, pSurfaceMain, pRect);
	if (pSurfaceMain != pSurfaceSecond)
	{
		Uint32* pixelWrite = NULL;
		Uint32 nombrePixelToUpdate, indexSurfaceMain = 0, index;
		int x, y, offsety;
		Uint32* pixelSurfaceMain = (Uint32*)pSurfaceMain->pixels;
		Uint32* pixelSurfaceSecond = (Uint32*)pSurfaceSecond->pixels;
		reajustSurfaceWithMapLimits(pSurfaceMain, pSurfaceSecond);
		nombrePixelToUpdate = pRect->h * pRect->w;
		pixelWrite = (Uint32*)malloc(nombrePixelToUpdate * sizeof(Uint32));
		if (pixelWrite == NULL)
		{
			fprintf(logFile, "updateTextureFromMultipleSurface : FAILURE, allocating memory to pixelWrite.\n\n");
			return -1;
		}
		x = pSurfaceSecond->clip_rect.x;
		y = pSurfaceSecond->clip_rect.y;
		offsety = y * pSurfaceMain->w;
		for (index = 0; index < nombrePixelToUpdate; index++)
		{
			if (pixelTransparent(pixelSurfaceSecond[index], pSurfaceSecond->format))
			{
				indexSurfaceMain = (index%pSurfaceSecond->w + x) + (index / pSurfaceSecond->w)*pSurfaceMain->w + offsety;
				pixelWrite[index] = pixelSurfaceMain[indexSurfaceMain];
			}
			else pixelWrite[index] = pixelSurfaceSecond[index];
		}
		pRect->x = x;
		pRect->y = y;
		for (index = 0; index < nombrePixelToUpdate; index += pSurfaceSecond->w)
		{
			int indexDest = x + y*pSurfaceMain->w;
			memcpy((pixelSurfaceMain + indexDest), (pixelWrite + index), pRect->w*sizeof(Uint32));
			y++;
		}
		SDL_UpdateTexture(pTexture, pRect, pixelWrite, 4 * pRect->w);
		free(pixelWrite);
		pixelWrite = NULL;
	}
	return 0;
}

/**
* \fn int updateTextureFromSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Rect* pRect)
* \brief Update texture associated to a surface on a selected area.
*
* \param[in] pTexture, pointer to the texture.
* \param[in] pSurfaceMain, pointer to the surface.
* \param[in] pRect, area to update, NULL for the entire surface/texture.
* \returns 0 = OK, -1 = problem allocating memory
*/
int updateTextureFromSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Rect* pRect)
{
	Uint32* pixelWrite = NULL;
	Uint32 nombrePixelToUpdate, indexSurfaceMain = 0, index;
	int x, y, offsety;
	if (pRect != NULL)
	{
		nombrePixelToUpdate = pRect->h * pRect->w;
		pixelWrite = (Uint32*)malloc(nombrePixelToUpdate * sizeof(Uint32));
		if (pixelWrite == NULL)
		{
			fprintf(logFile, "updateTextureFromSurface : FAILURE, allocating memory to pixelWrite.\n\n");
			return -1;
		}
		x = pRect->x;
		y = pRect->y;
		offsety = y *pSurfaceMain->w;
		for (index = 0; index < nombrePixelToUpdate; index += pRect->w)
		{
			indexSurfaceMain = x + y*pSurfaceMain->w;
			memmove((pixelWrite + index), ((Uint32*)pSurfaceMain->pixels + indexSurfaceMain), pRect->w * sizeof(Uint32));
			y++;
		}
		SDL_UpdateTexture(pTexture, pRect, pixelWrite, 4 * pRect->w);
		free(pixelWrite);
		pixelWrite = NULL;
	}
	else SDL_UpdateTexture(pTexture, pRect, pSurfaceMain->pixels, pSurfaceMain->pitch);
	return 0;
}

/**
* \fn void updateSurfaceFromSurface(SDL_Surface* pSurfaceDest, SDL_Surface* pSurfaceSrc, SDL_Rect* pRect)
* \brief Update a surface from a same size surface.
*
* \param[in] pSurfaceDest, pointer to the destination surface.
* \param[in] pSurfaceSrc, pointer to the source surface.
* \param[in] pRect, area to update.
* \returns void
* \remarks Note that this function is supposed to work as a calc, which means the two surfaces should be based on the same image.
*          It will only take the transparent pixel of the source surface to copy them in the destination surface.
*          IMPORTANT : This function does not handle different size of surface, they have to be the same size.
*/
void updateSurfaceFromSurface(SDL_Surface* pSurfaceDest, SDL_Surface* pSurfaceSrc, SDL_Rect* pRect)
{
	int x = 0, y = 0;
	Uint32 pixel;
	reajustRect(pRect, pSurfaceDest);
	for (y = pRect->y; y < (pRect->y + pRect->h); y++)
	{
		for (x = pRect->x; x < (pRect->x + pRect->w); x++)
		{
			pixel = ReadPixel(pSurfaceSrc, x, y);
			if (pixelTransparent(pixel, pSurfaceSrc->format))
				WritePixel(pSurfaceDest, x, y, pixel);
		}
	}
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Basics collisions                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/**
* \fn int collisionRectWithRect(SDL_Rect* pRect, SDL_Rect* pRect2)
* \brief Test a collision between two rectangles.
*
* \param[in] pRect, pointer to the first rectangle.
* \param[in] pRect2, pointer to the second rectangle.
* \returns 1 = collision, 0 = no collision.
*/
int collisionRectWithRect(SDL_Rect* pRect, SDL_Rect* pRect2)
{
	int widthRel1 = pRect->w + pRect->x, widthRel2 = pRect2->w + pRect2->x;
	int hightRel1 = pRect->h + pRect->y, hightRel2 = pRect2->h + pRect2->y;
	if (pRect2->x >= widthRel1
		|| widthRel2 <= pRect->x
		|| pRect2->y >= hightRel1
		|| hightRel2 <= pRect->y)
		return 0;
	else return 1;
}

/**
* \fn int collisionPointWithCercle(Point P, int centerX, int centerY, int radius)
* \brief Test if a point is inside a cercle.
*
* \param[in] P, point to test
* \param[in] centerX, X coordinate of the center of the cercle.
* \param[in] centerY, Y coordinate of the center of the cercle.
* \param[in] radius, radius of the cercle.
* \returns 1 = point is in the cercle, 0 = point is out of the cercle
*/
int collisionPointWithCercle(Point P, int centerX, int centerY, int radius)
{
	int d2 = CARRE((P.x - centerX)) + CARRE((P.y - centerY));
	if (d2 > CARRE(radius))
		return 0;
	else
		return 1;
}

/**
* \fn int collisionPointWithRect(Point P, SDL_Rect* box)
* \brief Test if a point is inside a rect.
*
* \param[in] P, point to test.
* \param[in] box, the box to test the collision with
* \returns 1 = point is in the box, 0 = point is out of the box
*/
int collisionPointWithRect(Point P, SDL_Rect* box)
{
	if (P.x >= box->x
		&& P.x < box->x + box->w
		&& P.y >= box->y
		&& P.y < box->y + box->h)
		return 1;
	else
		return 0;
}

/**
* \fn int pointProjectionOnSegment(Point C, int Ax, int Ay, int Bx, int By)
* \brief Test if a point is inside a rect.
*
* \param[in] C, point to test.
* \param[in] Ax, X coordinate of first edge of the segment.
* \param[in] Ay, Y coordinate of first edge of the segment.
* \param[in] Bx, X coordinate of second edge of the segment.
* \param[in] By, Y coordinate of second edge of the segment.
* \returns 1 = the projection of the point is on the segment, 0 = the projection of the point is not on the segment
*/
int pointProjectionOnSegment(Point C, int Ax, int Ay, int Bx, int By)
{
	int ACx = C.x - Ax;
	int ACy = C.y - Ay;
	int ABx = Bx - Ax;
	int ABy = By - Ay;
	int BCx = C.x - Bx;
	int BCy = C.y - By;
	int s1 = (ACx*ABx) + (ACy*ABy);
	int s2 = (BCx*ABx) + (BCy*ABy);
	if (s1*s2 > 0)
		return 0;
	return 1;
}






//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                  Fonctions Diverses                    /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn SDL_Rect initRect(int x, int y, int w, int h)
* \brief Create and initializes a SDL_Rect structure.
*
* \param[in] x, top left X coordinate.
* \param[in] y, top left Y coordinate.
* \param[in] w, width of the rectangle.
* \param[in] h, hight of the rectangle
* \return SDL_Rect structure initialized.
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


/**
* \fn int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect)
* \brief Test if a SDL_Rect can fit into a surface.
*
* \param[in] pSurface, pointer to the source surface.
* \param[in] pRect, destination area in the source surface.
* \returns 1 = dimensions OK, 0 = size problem between the SDL_Rect and the surface.
* \remarks This function only returns if either the Rect is too big or not within the surface limits,
* it does not say which issue it is.
*/
int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect)
{
	if (pRect->w > pSurface->w || (pRect->x + pRect->w) > (pSurface->clip_rect.x + pSurface->w))
		return 0;
	else if (pRect->x < 0 || pRect->x >(pSurface->clip_rect.x + pSurface->w))
		return 0;
	else if (pRect->h > pSurface->h || (pRect->y + pRect->h) > (pSurface->clip_rect.y + pSurface->h))
		return 0;
	else if (pRect->y < 0 || pRect->y >(pSurface->clip_rect.y + pSurface->h))
		return 0;
	return 1;
}

/**
* \fn void reajustSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
* \brief Reajust a surface with edges of the map.
*
* \param[in] pSurfaceMap, pointer to the map's surface
* \param[in] pSurfaceMotion, pointer to the motion object's surface.
* \return rect, une structure SDL_Rect initialisee aux dimensions et coordonnées combinees des deux rect.
*/
void reajustSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
{
	if (pSurfaceMotion->clip_rect.x < 0)
		pSurfaceMotion->clip_rect.x = 0;
	if (pSurfaceMotion->clip_rect.y < 0)
		pSurfaceMotion->clip_rect.y = 0;
	if (pSurfaceMotion->clip_rect.x + pSurfaceMotion->w > pSurfaceMap->w)
		pSurfaceMotion->clip_rect.x = pSurfaceMap->w - pSurfaceMotion->w;
	if (pSurfaceMotion->clip_rect.y + pSurfaceMotion->h > pSurfaceMap->h)
		pSurfaceMotion->clip_rect.y = pSurfaceMap->h - pSurfaceMotion->h;
}

/**
* \fn int rand_a_b(int a, int b)
* \brief Retourne un nombre aleatoire appartenant a [a;b[.
*
* \param[in] a, borne inferieur, inclus.
* \param[in] b, borne superieure, exclus.
* \returns rand, valeur aleatoire
*/
int rand_a_b(int a, int b)
{
	if (b != a)
		return rand() % (b - a) + a;
	else return a;
}


/**
* \fn int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap)
* \brief Réajuste les dimensions d'un rectangle qui depasserai de la map
*
* \param[in] pRect, rectangle a evaluer, peut etre modifie par la fonction
* \param[in] pSurfaceMap, surface de la map
* \return 1 si il y a eu modification, 0 sinon
*/
int reajustRect(SDL_Rect* pRect, SDL_Surface* pSurfaceMap)
{
	int modif = 0;
	if (pRect->x < 0) {
		pRect->w -= pRect->x;
		pRect->x = 0;
		modif = 1;
	}
	if (pRect->y < 0) {
		pRect->h -= pRect->y;
		pRect->y = 0;
		modif = 1;
	}
	if (pRect->x + pRect->w >= pSurfaceMap->clip_rect.w){
		pRect->w -= pRect->x + pRect->w - pSurfaceMap->clip_rect.w;
		modif = 1;
	}
	if (pRect->y + pRect->h >= pSurfaceMap->clip_rect.h){
		pRect->h -= pRect->y + pRect->h - pSurfaceMap->clip_rect.h;
		modif = 1;
	}
	return modif;
}


/**
* \fn SDL_Rect createGlobalRect(int nbRect, SDL_Rect* rectTab)
* \brief Create a rectangle that contains all rectangles of the array.
* ____         _______
*|   _|__     |       |
*|__|_|  | => |       |
*   |____|    |_______|
*
* \param[in] nbRect, nomber of rectangles
* \param[in] rectTab, array of pointer to the rectangles.
* \return the rectangle created
*/
SDL_Rect createGlobalRect(int nbRect, SDL_Rect** rectTab)
{
	int indexRect, xMin = 0, yMin = 0, wMax = 0, hMax = 0;
	int x2 = 0, y2 = 0, w2 = 0, h2 = 0;

	xMin = rectTab[0]->x;
	yMin = rectTab[0]->y;
	wMax = rectTab[0]->w;
	hMax = rectTab[0]->h;
	for (indexRect = 1; indexRect < nbRect; indexRect++)
	{
		x2 = rectTab[indexRect]->x;
		y2 = rectTab[indexRect]->y;
		w2 = rectTab[indexRect]->w;
		h2 = rectTab[indexRect]->h;
		if (x2 < xMin && x2 >= 0)
			xMin = x2;
		if (y2 < yMin && y2 >= 0)
			yMin = y2;
		if ((xMin + wMax) < (x2 + w2))
			wMax =x2 + w2 - xMin;
		if ((yMin + hMax) < (y2 + h2))
			hMax = y2 + h2 - yMin;
	}
	return initRect(xMin, yMin, wMax, hMax);
}




















/**
* \fn int detectShape(SDL_Surface* pSurface, SDL_Point* shapeTab)
* \brief Detect the shape of a surface and store it in an SDL_Point array.
*
* \param[in] pSurface, pointer to the surface.
* \param[in] shapeTab, array of point.
* \return nomber of points of the shape.
*/
int detectShape(SDL_Surface* pSurface, SDL_Point* shapeTab)
{
	int index, indexFin = pSurface->w * pSurface->h, indexStart = pSurface->w - 1, indexShape = 0, indexBalayage = 0;
	int xSurface = pSurface->clip_rect.x, ySurface = pSurface->clip_rect.y;
	Uint32* pixel = (Uint32*)pSurface->pixels;
	int indexLine = 0, indexRow = 0, inc = -1, pixelFound = 0;
	int lastLine = pSurface->h - 1, firstLine = 0;
	int lastRow = pSurface->w - 1, firstRow = 0;
	for (indexBalayage = 0; indexBalayage < 2; indexBalayage++)
	{
		for (index = indexStart; index <= indexFin; index += inc)
		{
			indexLine = index / pSurface->w;
			indexRow = index % pSurface->w;
			if (!pixelTransparent(pixel[index], pSurface->format))
			{
				if (indexBalayage == 0)
				{
					if (indexRow != lastRow)
					{
						if (pixelTransparent(pixel[index + 1], pSurface->format))
						{
							shapeTab[indexShape].x = indexRow + xSurface;
							shapeTab[indexShape].y = indexLine + ySurface;
							indexShape++;
							pixelFound = 1;
							index += (2 * pSurface->w - indexRow);
						}
						else pixelFound = 0;
					}
					else
					{
						shapeTab[indexShape].x = indexRow + xSurface;
						shapeTab[indexShape].y = indexLine + ySurface;
						indexShape++;
						pixelFound = 1;
						index += pSurface->w + 1;
					}
				}
				else
				{
					if (indexRow != firstRow)
					{
						if (pixelTransparent(pixel[index - 1], pSurface->format))
						{
							shapeTab[indexShape].x = indexRow + xSurface;
							shapeTab[indexShape].y = indexLine + ySurface;
							indexShape++;
							pixelFound = 1;
							index = index - pSurface->w - indexRow - 1;
						}
						else pixelFound = 0;
					}
					else
					{
						shapeTab[indexShape].x = indexRow + xSurface;
						shapeTab[indexShape].y = indexLine + ySurface;
						indexShape++;
						pixelFound = 1;
						index = index - pSurface->w - 1;
					}
				}
			}
			else pixelFound = 0;
			if ((indexRow == firstRow && indexLine == lastLine
				|| indexLine == lastLine && pixelFound)
				&& indexBalayage == 0)
			{
				inc = 1;
				indexStart = lastLine*pSurface->w;
				break;
			}
			else if ((indexRow == lastRow && indexLine == firstLine
				|| indexLine == firstLine && pixelFound)
				&& indexBalayage == 1 || index < -1)
				break;
			if (!pixelFound)
			{
				if (indexRow == firstRow && indexBalayage == 0)
				{
					index += (2 * pSurface->w - 1);
				}
				else if (indexRow == lastRow  && indexBalayage == 1)
				{
					index -= (2 * pSurface->w - 1);
				}
			}
		}
	}
	shapeTab[indexShape].x = shapeTab[0].x;
	shapeTab[indexShape].y = shapeTab[0].y;
	return indexShape + 1;
}

int simplifiedShape(SDL_Point* shapeTab, int nbShapeOriginal, int coeff)
{
	SDL_Point shapeTemp[100];
	int index, h = nbShapeOriginal / 2, newNbShape = 0, indexTab = 0;;
	for (index = 0; index < h; index++)
	{
		if (index == 0 || index == h - 1)
		{
			shapeTemp[newNbShape].x = shapeTab[indexTab].x;
			shapeTemp[newNbShape].y = shapeTab[indexTab].y;
			newNbShape++;
		}
		else if (index % coeff == 0)
		{
			shapeTemp[newNbShape].x = shapeTab[indexTab].x;
			shapeTemp[newNbShape].y = shapeTab[indexTab].y;
			newNbShape++;
		}
		indexTab++;
	}
	for (index = h - 1; index >= 0; index--)
	{
		if (index == 0 || index == h - 1)
		{
			shapeTemp[newNbShape].x = shapeTab[indexTab].x;
			shapeTemp[newNbShape].y = shapeTab[indexTab].y;
			newNbShape++;
		}
		else if (index % coeff == 0)
		{
			shapeTemp[newNbShape].x = shapeTab[indexTab].x;
			shapeTemp[newNbShape].y = shapeTab[indexTab].y;
			newNbShape++;
		}
		indexTab++;
	}
	memcpy(shapeTab, shapeTemp, (newNbShape)* sizeof(SDL_Point));
	shapeTab[newNbShape].x = shapeTab[0].x;
	shapeTab[newNbShape].y = shapeTab[0].y;
	return newNbShape + 1;
}

/**
* \fn void moveShape(SDL_Point* shapeTab, int dx, int dy, int nbPoint)
* \brief Move an array of points according to a delta on X and a delta on Y.
*
* \param[in] shapeTab, array of point.
* \param[in] dx, delta along X axis.
* \param[in] dy, delta along Y axis.
* \param[in] nbPoint, array of point.
* \return void
*/
void moveShape(SDL_Point* shapeTab, int dx, int dy, int nbPoint)
{
	int index;
	for (index = 0; index < nbPoint; index++)
	{
		shapeTab[index].x += dx;
		shapeTab[index].y += dy;
	}
}

/**
* \fn void drawShape(SDL_Renderer* pRenderer, SDL_Point* shapeTab, int nbPoint)
* \brief Draw an array of points.
*
* \param[in] pRenderer, pointer to the renderer.
* \param[in] shapeTab, array of point
* \param[in] nbPoint, array of point.
* \return void
*/
void drawShape(SDL_Renderer* pRenderer, SDL_Point* shapeTab, int nbPoint)
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderDrawLines(pRenderer, shapeTab, nbPoint);
}



/*
if (testLine != 0 && testLine != (pSurface->h - 1) && testRow != 0 && testRow != pSurface->w - 1)
{
if (pixelTransparent(pixel[index + 1], pSurface->format)
|| pixelTransparent(pixel[index - 1], pSurface->format)
|| pixelTransparent(pixel[index - pSurface->w], pSurface->format)
|| pixelTransparent(pixel[index + pSurface->w], pSurface->format))
{
shapeTab[indexShape].x = index%pSurface->w + xSurface;
shapeTab[indexShape].y = index / pSurface->w + ySurface;
indexShape++;
pixelFound = 1;
}
}
if (testRow == 0 || testRow == pSurface->h - 1)
{
shapeTab[indexShape].x = index%pSurface->w + xSurface;
shapeTab[indexShape].y = index / pSurface->w + ySurface;
indexShape++;
}
if (testLine == 0 || testLine == pSurface->w)
{
shapeTab[indexShape].x = index%pSurface->w + xSurface;
shapeTab[indexShape].y = index / pSurface->w + ySurface;
indexShape++;
}
*/





