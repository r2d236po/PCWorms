#include "carte.h"
#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

void initialisionTerrain(Terrain * map, SDL_Renderer * pRenderer, const char * file,const char * file2)
{
	SDL_Rect back = { 0, 0, 0, 0 };
	Uint32 i;

	map->imageBackground = loadTexture(pRenderer,file);
	SDL_QueryTexture(map->imageBackground, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageBackground, NULL, &back);
	map->imageMap = loadTexture(pRenderer, file2);
	SDL_QueryTexture(map->imageMap, NULL, &i, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageMap, NULL, &back);
	SDL_RenderPresent(pRenderer);
}

/*int gestionCollision(SDL_Renderer* pRenderer, SDL_Texture* pTexture)
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	void* pixel;
	int pitch;
	SDL_RenderReadPixels(pRenderer, &rect, NULL, pixel, pitch);
	return SDL_TRUE;
}*/



