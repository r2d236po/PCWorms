#include "carte.h"
#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

void initialisionTerrain(Terrain * map, SDL_Renderer * pRenderer, const char * file,const char * file2)
{
	SDL_Rect back = { 0, 0, 0, 0 };

	map->imageBackground = loadTexture(pRenderer,file);
	SDL_QueryTexture(map->imageBackground, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageBackground, NULL, &back);
	map->imageMap = loadTexture(pRenderer, file2);
	SDL_QueryTexture(map->imageMap, NULL, NULL, &back.w, &back.h);
	SDL_RenderCopy(pRenderer, map->imageMap, NULL, &back);
	SDL_RenderPresent(pRenderer);
}



