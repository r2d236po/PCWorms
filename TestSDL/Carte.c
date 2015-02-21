#include "carte.h"
#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

int initialisionTerrain(Terrain** map, SDL_Renderer * pRenderer, const char * file, const char * file2)
{
	Terrain * mapTemp = NULL;
	SDL_Rect back = { 0, 0, 0, 0 };
	int i = 0;
	
	//Creation du pointeur de Terrain
	mapTemp = (Terrain*)malloc(sizeof(Terrain));
	if (mapTemp == NULL)
	{
		printf("Probleme de lors de l'allocation memoire du terrain");
		return -1;
	}
	mapTemp->imageBackground = NULL;
	mapTemp->imageMap = NULL;
	mapTemp->imageMapSurface = NULL;
	mapTemp->mapCollision = NULL;

	//Creation de la texture de l'image de fond
	mapTemp->imageBackground = loadTexture(pRenderer, file);
	if (mapTemp->imageBackground == NULL)
	{
		printf("Probleme de lors de la creation de la texture Background");
		destroyMap(&mapTemp);
		return -1;
	}

	//Creation de la surface de la map qui servira pour la collision
	mapTemp->imageMapSurface = loadImage(file2);
	if (mapTemp->imageMapSurface == NULL) //Verification qu'il n'y a pas eu d'erreur lors de l'allocation en mémoire
	{
		printf("Probleme de lors de la creation de la surface de la map");
		destroyMap(&mapTemp);
		return -1;
	}

	//Creation de la texture de la map qui servira pour l'affichage
	mapTemp->imageMap = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, mapTemp->imageMapSurface->w, mapTemp->imageMapSurface->h);
	if (mapTemp->imageMap == NULL)
	{
		printf("Probleme de lors de la creation de la texture de la map");
		destroyMap(&mapTemp);
		return -1;
	}
	SDL_SetTextureBlendMode(mapTemp->imageMap, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(mapTemp->imageMap, NULL, mapTemp->imageMapSurface->pixels, mapTemp->imageMapSurface->pitch);
	
	//Creation de la deuxieme surface de collision qui pourra être redimensionné sans affecter la surface de base
	mapTemp->mapCollision = SDL_CreateRGBSurface(mapTemp->imageMapSurface->flags,
		mapTemp->imageMapSurface->w,
		mapTemp->imageMapSurface->h,
		mapTemp->imageMapSurface->format->BitsPerPixel,
		mapTemp->imageMapSurface->format->Rmask,
		mapTemp->imageMapSurface->format->Gmask,
		mapTemp->imageMapSurface->format->Bmask,
		mapTemp->imageMapSurface->format->Amask);
	if (mapTemp->mapCollision == NULL)
	{
		printf("Probleme de lors de la creation de la surface de collision de la map");
		destroyMap(&mapTemp);
		return -1;
	}

	SDL_QueryTexture(mapTemp->imageBackground, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, mapTemp->imageBackground, NULL, &back);
	SDL_RenderCopy(pRenderer, mapTemp->imageMap, NULL, &back);
	SDL_RenderPresent(pRenderer);
	(*map) = mapTemp; //récupération du pointeur du terrain
	mapTemp = NULL;
	return 1;
}


//Récupère un pixel
Uint32 ReadPixel(SDL_Surface *surface, int x, int y)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;
	//Get the requested pixel
	return pixels[(y * surface->w) + x];
}

//Ecris un pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;

	//Set the pixel
	pixels[(y * surface->w) + x] = pixel;
}

//détection de collision V1.0
int detectionCollisionRect(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Rect* pRect)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	Uint32 p = ReadPixel(pSurface, 0, 0);
	Uint8 r = 0, g = 0, b = 0, a = 0;
	int x = 0, y = 0, w = 0, h = 0;
	rect = *pRect;
	int collision = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	if ((w - rect.x <= rect.w) || (rect.x <= 0) || (rect.y <= 0) || (h - rect.y) <= rect.h)
	{
		return 1;
	}
	for (y = rect.y; (y <= rect.h + rect.y) && (collision == 0); y++)
	{
		if (y == rect.y || y == (rect.h + rect.y))
		{
			for (x = rect.x; (x <= rect.x + rect.w) && (collision == 0); x++)
			{
				p = ReadPixel(pSurface, x, y);
				SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
				if (a < 100) //transparence
				{
					collision = 0;
				}
				else
				{
					*xE = x;
					*yE = y;
					collision = 1;
				}
			}
		}
		else
		{
			p = ReadPixel(pSurface, rect.x, y);
			SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
			if (a < 100) //transparence
			{
				collision = 0;
			}
			else
			{
				*xE = rect.x + rect.w;
				*yE = y;
				collision = 1;
			}
			p = ReadPixel(pSurface, rect.x + rect.w, y);
			SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
			if (a < 100) //transparence
			{
				collision = 0;
			}
			else
			{
				*xE = rect.x + rect.w;
				*yE = y;
				collision = 1;
			}
		}
	}
	return collision;
}

//Détection collision V2.0
int detectionCollisionSurface(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Surface* pSurface2)
{
	Uint32 p = ReadPixel(pSurface, 0, 0);
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 p2 = ReadPixel(pSurface, 0, 0);
	Uint8 r2 = 0, g2 = 0, b2 = 0, a2 = 0;
	int x = 0, y = 0, w = 0, h = 0;
	int collision = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	if ((w - pSurface2->clip_rect.x < pSurface2->clip_rect.w) || (pSurface2->clip_rect.x < 0) || (pSurface2->clip_rect.y < 0) || (h - pSurface2->clip_rect.y) < pSurface2->clip_rect.h)
	{
	return 1;
	}
	for (y = pSurface2->clip_rect.y; (y <= pSurface2->clip_rect.h + pSurface2->clip_rect.y) && (collision == 0); y++)
	{
			for (x = pSurface2->clip_rect.x; (x <= pSurface2->clip_rect.x + pSurface2->clip_rect.w) && (collision == 0); x++)
			{
				p = ReadPixel(pSurface, x, y);
				p2 = ReadPixel(pSurface2, x - pSurface2->clip_rect.x, y - pSurface2->clip_rect.y);
				SDL_GetRGBA(p, pSurface->format, &r, &g, &b, &a);
				SDL_GetRGBA(p2, pSurface2->format, &r2, &g2, &b2, &a2);
				if (a != 255 || a2 != 255) //transparence
				{
					collision = 0;
				}
				else
				{
					*xE = x;
					*yE = y;
					collision = 1;
				}
			}
	}
	return collision;
}

/*int gestionCollision(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE)
{
int success = 0;

while (detectionCollision(pRenderer, pSurface, xE, yE))
{

}
return success;
}*/

SDL_Surface* ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
	Sint32 y = 0, x = 0, o_y = 0, o_x = 0;
	if (!Surface || !Width || !Height)
		return 0;
	SDL_Surface * ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
		Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
	SDL_Surface* surface = NULL;

	double  stretch_factor_x = ((double)(Width) / (double)(Surface->w));
	double	stretch_factor_y = ((double)(Height) / (double)(Surface->h));

	for (y = 0; y < Surface->h; y++)
	{
		for (x = 0; x < Surface->w; x++)
		{
			for (o_y = 0; o_y < stretch_factor_y; ++o_y)
			{
				for (o_x = 0; o_x < stretch_factor_x; ++o_x)
				{
					DrawPixel(ret, (Sint32)(stretch_factor_x * x) + o_x, (Sint32)(stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));
				}
			}
		}
	}
	surface = ret;
	SDL_FreeSurface(ret);
	return surface;
}