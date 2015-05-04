#include "HUD.h"
#include "my_stdrFct.h"
#include "MainMenu.h"
/**
* \fn void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
* \brief Initializes an SDL_Color structure.
*
* \param[in] color, pointer to the SDL_Color structure.
* \param[in] r, value of the red.
* \param[in] g, value of the green.
* \param[in] b, value of the blue.
* \returns void
*/
void setSDLColor(SDL_Color * color, Uint8 r, Uint8 g, Uint8 b)
{
	color->r = r;
	color->g = g;
	color->b = b;
}

/**
* \fn void updateTextSurfaceWorms(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
* \returns void
*/
void updateTextSurfaceWorms(Worms** wormsTab)
{
	char str[10];
	int i;
	SDL_Surface *txtLifeSurface = NULL;

	for (i = 0; i < globalVar.nbWormsTotal ; i++)
	{
		sprintf(str, " %d ", wormsTab[i]->vie);
		txtLifeSurface = TTF_RenderText_Blended(globalVar.FontName, str, *(wormsTab[i]->color));
		copySurfacePixels(txtLifeSurface, NULL, wormsTab[i]->texteLifeSurface, NULL);
		SDL_FreeSurface(txtLifeSurface);
		txtLifeSurface = NULL;
	}

	updateTextSurfacePosition(wormsTab);
}

/**
* \fn void updateTextSurfacePosition(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
*/
void updateTextSurfacePosition(Worms** wormsTab)
{
	int i, t_x, t_y;

	for (i = 0; i < globalVar.nbWormsTotal; i++)
	{
		//Affichage du Nom
		t_x = wormsTab[i]->wormsObject->objectSurface->clip_rect.x + 15 - (wormsTab[i]->texteNameSurface->clip_rect.w / 2);
		t_y = wormsTab[i]->wormsObject->objectSurface->clip_rect.y - 40;

		if (t_x >= 0){ wormsTab[i]->texteNameSurface->clip_rect.x = t_x; }
		else { wormsTab[i]->texteNameSurface->clip_rect.x = wormsTab[i]->wormsObject->objectSurface->clip_rect.x; }

		if (t_y >= 0) { wormsTab[i]->texteNameSurface->clip_rect.y = t_y; }
		else { wormsTab[i]->texteNameSurface->clip_rect.y = wormsTab[i]->wormsObject->objectSurface->clip_rect.y + 40; }

		//Affichage de la Vie
		wormsTab[i]->texteLifeSurface->clip_rect.x = wormsTab[i]->texteNameSurface->clip_rect.x + (wormsTab[i]->texteNameSurface->clip_rect.w / 2) - (wormsTab[i]->texteLifeSurface->clip_rect.w /2);
		wormsTab[i]->texteLifeSurface->clip_rect.y = wormsTab[i]->texteNameSurface->clip_rect.y + wormsTab[i]->texteNameSurface->clip_rect.h;
	}
}

/**
* \fn void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
* \brief Gestion du menu InGame
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns int, indicateur si la fonction a bien fonctionnée (1 = succes, -1 = echec)
*/


void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
{
	SDL_Texture* textureMenu = loadTexture(INGAMEMENU);
	SDL_Texture* textureMenuMainMenu = loadTexture(INGAMEMENUMAINMENU);
	SDL_Texture* textureMenuOption = loadTexture(INGAMEMENUOPTIONS);
	SDL_Texture* textureMenuQuit = loadTexture(INGAMEMENUQUIT);

	SDL_Rect rectMenu = initButtonBox(-1, -1, 565, 717);
	renderScreen(3, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, textureMenu, NULL, &rectMenu);
	SDL_DestroyTexture(textureMenu);
}