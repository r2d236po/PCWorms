#include "HUD.h"
#include "my_stdrFct.h"
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
* \fn void updateTextSurfaceWormsTab(Worms** wormsTab)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, array of worms.
* \returns void
*/
void updateTextSurfaceWormsTab(Worms** wormsTab)
{
	int i;
	for (i = 0; i < globalVar.nbWormsTotal ; i++)
	{
		updateTextSurfaceWorms(wormsTab[i]);
	}
}


/**
* \fn void updateTextSurfaceWorms(Worms** wormsTab)
* \brief Updates the life display of one worms in game.
*
* \param[in] pWorms,  pointer of the worms.
* \returns -1 if error, 0 else
*/
int updateTextSurfaceWorms(Worms* pWorms)
{
	char str[10];
	SDL_Surface *txtLifeSurface = NULL;
	sprintf(str, " %d ", pWorms->vie);
	txtLifeSurface = TTF_RenderText_Blended(globalVar.FontName, str, *(pWorms->color));
	if (txtLifeSurface == NULL)
		return -1;
	copySurfacePixels(txtLifeSurface, NULL, pWorms->texteLifeSurface, NULL);
	SDL_FreeSurface(txtLifeSurface);
	txtLifeSurface = NULL;
	updateTextSurfacePosition(pWorms);
	return 0;
}


/**
* \fn void updateTextSurfacePosition(Worms* pWorms)
* \brief Updates the life display of the worms in game.
*
* \param[in] wormsTab, pointer of the worms.
*/
void updateTextSurfacePosition(Worms* pWorms)
{
	int i, t_x, t_y;

	for (i = 0; i < globalVar.nbWormsTotal; i++)
	{
		//Affichage du Nom
		t_x = pWorms->wormsObject->objectSurface->clip_rect.x + 15 - (pWorms->texteNameSurface->clip_rect.w / 2);
		t_y = pWorms->wormsObject->objectSurface->clip_rect.y - 40;

		if (t_x >= 0)
		{ 
			pWorms->texteNameSurface->clip_rect.x = t_x; 
		}
		else pWorms->texteNameSurface->clip_rect.x = pWorms->wormsObject->objectSurface->clip_rect.x;

		if (t_y >= 0) 
		{ 
			pWorms->texteNameSurface->clip_rect.y = t_y; 
		}
		else pWorms->texteNameSurface->clip_rect.y = pWorms->wormsObject->objectSurface->clip_rect.y + 40;

		//Affichage de la Vie
		pWorms->texteLifeSurface->clip_rect.x = pWorms->texteNameSurface->clip_rect.x + (pWorms->texteNameSurface->clip_rect.w / 2) - (pWorms->texteLifeSurface->clip_rect.w / 2);
		pWorms->texteLifeSurface->clip_rect.y = pWorms->texteNameSurface->clip_rect.y + pWorms->texteNameSurface->clip_rect.h;
	}
}