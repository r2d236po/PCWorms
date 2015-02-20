#include "carte.h"
#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

void initialisionTerrain(Terrain * map, SDL_Renderer * pRenderer, const char * file, const char * file2)
{
	SDL_Rect back = { 0, 0, 0, 0 };
	int i = 0;

	map->imageBackground = loadTexture(pRenderer, file);
	SDL_QueryTexture(map->imageBackground, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageBackground, NULL, &back);
	/*map->imageMap = loadTexture(pRenderer, file2);
	SDL_QueryTexture(map->imageMap, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageMap, NULL, &back);*/
	map->imageMapSurface = loadImage(file2);
	map->imageMap = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, map->imageMapSurface->w, map->imageMapSurface->h);
	SDL_SetTextureBlendMode(map->imageMap, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(map->imageMap, NULL, map->imageMapSurface->pixels, map->imageMapSurface->pitch);
	SDL_RenderCopy(pRenderer, map->imageMap, NULL, NULL);
	//SDL_RenderPresent(pRenderer);
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
/*int detectionCollisionSurface(SDL_Renderer* pRenderer, SDL_Surface* pSurface, int* xE, int* yE, SDL_Surface* pSurface2)
{
	Uint32 p = ReadPixel(pSurface, 0, 0);
	Uint8 r = 0, g = 0, b = 0, a = 0;
	int x = 0, y = 0, w = 0, h = 0;
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
}*/

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