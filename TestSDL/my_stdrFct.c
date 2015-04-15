#include "Libraries.h"
#include "my_stdrFct.h"

//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////              Cr�ation d'une fen�tre                    /////////////////
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
	pWindow = SDL_CreateWindow(windowName,	//nom de la fen�tre
		SDL_WINDOWPOS_CENTERED, //position en x de la fen�tre
		SDL_WINDOWPOS_CENTERED,	//position en y de la fen�tre
		windowWidth,	//largeur de la fen�tre
		windowHight,	//hauteur de la fen�tre
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);	//propri�t�s suppl�mentaires de la fen�tre
	if (pWindow == NULL)
	{
		fprintf(logFile, "creerFenetre : FAILURE, erreur de cr�ation de la fen�tre: %s.\n", SDL_GetError());
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
* \brief Cree une surface a partir d'une image exterieur.
*
* \param[in] file, nom du fichier image a ouvrir
* \returns pointeur vers la surface creee, NULL si echec
*/
SDL_Surface * loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
	{
		fprintf(logFile, "loadImage : FAILURE, probleme lors du chargement de la surface : %s.\n\tnom du fichier : %s.", IMG_GetError(), file);
		return NULL;
	}
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
		fprintf(logFile, "loadTexture : FAILURE, probleme chargement texture : %s.\n\tnom du fichier : %s.\n", IMG_GetError(), file);
		return NULL;
	}
	fprintf(logFile, "loadTexture : SUCCESS.\n\tnom du fichier : %s.\n", file);
	return texture;
}

/**
* \fn SDL_Texture* my_createTextureFromSurface(SDL_Surface* pSurface, SDL_Renderer* pRenderer)
* \brief Cree une texture globale a partir d'une (version ressemblante � createTextureFromSurface mais nous donne acces aux pixels).
*
* \param[in] pSurface, pointeur vers la surface.
* \param[in] pRenderer, pointeur du renderer.
* \returns texture cree ou NULL si problem
* \remarks si -1, le pointeur de la structure Texture sera mis a NULL sinon il pointera vers la texture globale.
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


/**
* \fn int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRect)
* \brief Update texture associated to a surface with a second surface.
*
* \param[in] pTexture, the pointer to the texture
* \param[in] pSurfaceMain, the pointer to the first surface
* \param[in] pSurfaceSecond, the pointer to the second surface
* \param[in] pRect, area to erase before to display the second surface
* \returns 0 = OK, -1 = problem allocating memory
*/
int updateTextureFromMultipleSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Surface* pSurfaceSecond, SDL_Rect* pRect)
{
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
		SDL_UpdateTexture(pTexture, pRect, pixelWrite, 4 * pRect->w);
		free(pixelWrite);
		pixelWrite = NULL;
	}
	return 0;
}

/**
* \fn int updateTextureFromSurface(SDL_Texture* pTexture, SDL_Surface* pSurfaceMain, SDL_Rect* pRect)
* \brief Update texture associated to a surface on selected area.
*
* \param[in] pTexture, the pointer to the texture
* \param[in] pSurfaceMain, the pointer to the surface
* \param[in] pRect, area to update, NULL for the entire surface/texture
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
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//R�cup�re le tableau de pixels de la surface
	//Get the requested pixel
	return pixels[(y * pSurface->w) + x];	//Return le pixel aux coordonn�es x et y pass�es en param�tre
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
	Uint32 *pixels = (Uint32 *)pSurface->pixels;	//R�cup�re le tableau de pixel de la surface

	//Set the pixel
	pixels[(y * pSurface->w) + x] = pixelToWrite;	//Ecrit le pixel aux coordonn�es pass�es en param�tre
}


/**
* \fn int pixelTransparent(Uint32 pixelToRead,SDL_PixelFormat* format)
* \brief Test la transparence d'un pixel.
*
* \param[in] pixelToRead, pixel en question
* \param[in] format, format du pixel � lire
* \returns 1 = pixel transparent, 0 = pixel non transparent
*/
int pixelTransparent(Uint32 pixelToRead, SDL_PixelFormat* format)
{
	Uint8 a, b, g, r;
	SDL_GetRGBA(pixelToRead, format, &r, &g, &b, &a);
	return a < 150;
}

/**
* \fn int copySurfacePixels(SDL_Surface* pSurfaceSrc, SDL_Rect* pRectSrc, SDL_Surface* pSurfaceDest, SDL_Rect* pRectDest)
* \brief Ecrit des pixels d'une surface dans une autre surface (comme BlitSurface).
*
* \param[in] pSurfaceSrc, surface source
* \param[in] pRectSrc, rectangle de destination dans la surface source, NULL pour toute la surface
* \param[in] pSurfaceDest, surface de destination
* \param[in] pRectDest, rectangle de destination dans la surface de destination, NULL d�marre � z�ro
* \returns 1 = copie OK, 0 = probleme de taille entre la source et la destination
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


/**
* \fn int checkRectSurfaceDimension(SDL_Surface* pSurface, SDL_Rect* pRect)
* \brief Test si un rect peut etre compris dans une surface.
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
* \fn int collisionRectWithRect(SDL_Rect* pRect, SDL_Rect* pRect2)
* \brief Test si deux rect se superpose.
*
* \param[in] pRect, rectangle 1
* \param[in] pRect2, rectangle 2
* \returns 1 = overlay, 0 = pas d'overlay
*/
int collisionRectWithRect(SDL_Rect* pRect, SDL_Rect* pRect2)
{
	int widthRel1 = pRect->w + pRect->x, widthRel2 = pRect2->w + pRect2->x;
	int hightRel1 = pRect->h + pRect->y, hightRel2 = pRect2->h + pRect2->y;
	/*if (widthRel1 >= pRect2->x && widthRel1 <= widthRel2 && hightRel1 >= pRect2->y && hightRel1 <= hightRel2)
		return 1;
		else if (widthRel2 >= pRect->x && widthRel2 <= widthRel1 && hightRel2 >= pRect->y && hightRel2 <= hightRel1)
		return 1;
		return 0;*/
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
* \fn void reajustSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion)
* \brief Reajust a surface with edges of the map.
*
* \param[in] pSurfaceMap, pointer to the map's surface
* \param[in] pSurfaceMotion, pointer to the motion object's surface.
* \return rect, une structure SDL_Rect initialisee aux dimensions et coordonn�es combinees des deux rect.
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
	return rand() % (b - a) + a;
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
						index = index - pSurface->w  - 1;
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
				&& 	indexBalayage == 1 || index < -1)
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
	int index, h = nbShapeOriginal/2, newNbShape = 0, indexTab = 0;;
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
	memcpy(shapeTab, shapeTemp, (newNbShape) * sizeof(SDL_Point));
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