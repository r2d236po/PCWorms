#include "HUD.h"

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
	char * str[10];
	int i;
	SDL_Surface * txtLifeSurface = NULL;

	for (i = 0; i < globalVar.nbEquipe * globalVar.nbWormsEquipe ; i++)
	{
		sprintf(str, "%d", wormsTab[i]->vie);

		txtLifeSurface = TTF_RenderText_Blended(globalVar.FontName, str, globalVar.couleurBleu);
		SDL_FreeSurface(wormsTab[i]->texteLifeSurface);
		wormsTab[i]->texteLifeSurface = txtLifeSurface;
	}
	txtLifeSurface = NULL;

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

	for (i = 0; i < globalVar.nbEquipe * globalVar.nbWormsEquipe; i++)
	{
		//Affichage du Nom
		t_x = wormsTab[i]->wormsObject->absoluteCoordinate.x + 15 - (wormsTab[i]->texteNameSurface->clip_rect.w / 2);
		t_y = wormsTab[i]->wormsObject->absoluteCoordinate.y - 40;

		if (t_x >= 0){ wormsTab[i]->texteNameSurface->clip_rect.x = t_x; }
		else { wormsTab[i]->texteNameSurface->clip_rect.x = wormsTab[i]->wormsObject->absoluteCoordinate.x; }

		if (t_y >= 0) { wormsTab[i]->texteNameSurface->clip_rect.y = t_y; }
		else { wormsTab[i]->texteNameSurface->clip_rect.y = wormsTab[i]->wormsObject->absoluteCoordinate.y + 40; }

		//Affichage de la Vie
		wormsTab[i]->texteLifeSurface->clip_rect.x = wormsTab[i]->texteNameSurface->clip_rect.x + (wormsTab[i]->texteNameSurface->clip_rect.w / 2) - (wormsTab[i]->texteLifeSurface->clip_rect.w /2);
		wormsTab[i]->texteLifeSurface->clip_rect.y = wormsTab[i]->texteNameSurface->clip_rect.y + wormsTab[i]->texteNameSurface->clip_rect.h;
	}
}