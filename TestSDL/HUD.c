#include "HUD.h"
#include "my_stdrFct.h"
#include "MainMenu.h"
#include "AffichageGeneral.h"
#include "input.h"

/**
* \fn int setFonts()
* \brief set les polices globales.
*
* \returns 0 si OK, -1 si erreur
*/
int setFonts()
{
	globalVar.FontName[0] = NULL;
	globalVar.FontName[0] = TTF_OpenFont("../assets/fonts/Worms_3D_Font.ttf", FONTSIZENAME);
	if (globalVar.FontName[0] == NULL)
	{
		fprintf(logFile, "setFonts : FAILURE loading fonts.\n");
		fprintf(logFile, "error : %s\n\n", TTF_GetError());
		return -1;
	}
	return 0;
}

/**
* \fn void destroyFonts()
* \brief detruit les polices globales.
*
* \returns void
*/
void destroyFonts()
{
	int i;
	for (i = 0; i < NBFONTS; i++){
		TTF_CloseFont(globalVar.FontName[i]);
	}
}

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
	for (i = 0; i < globalVar.nbWormsTotal; i++)
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
	if (pWorms->vie > 0) 
		sprintf(str, " %d ", pWorms->vie);
	else sprintf(str, " ");
	txtLifeSurface = TTF_RenderText_Blended(globalVar.FontName[0], str, *(pWorms->color));
	if (txtLifeSurface == NULL)
		return -1;
	cleanSurface(pWorms->texteLifeSurface);
	if (!copySurfacePixels(txtLifeSurface, NULL, pWorms->texteLifeSurface, NULL))
	{
		fprintf(logFile, "updateTextSurfaceWorms : FAILURE, copySurfacePixels.\n\n");
	}
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
	int t_x, t_y;
	//Affichage du Nom
	t_x = pWorms->wormsObject->objectSurface->clip_rect.x + pWorms->wormsObject->objectSurface->w / 2 - (pWorms->texteNameSurface->clip_rect.w / 2);
	t_y = pWorms->wormsObject->objectSurface->clip_rect.y - pWorms->texteLifeSurface->h - pWorms->texteNameSurface->h;

	if (t_x >= 0)
	{
		pWorms->texteNameSurface->clip_rect.x = t_x;
	}
	else pWorms->texteNameSurface->clip_rect.x = pWorms->wormsObject->objectSurface->clip_rect.x;

	if (t_y >= 0)
	{
		pWorms->texteNameSurface->clip_rect.y = t_y;
	}
	else pWorms->texteNameSurface->clip_rect.y = pWorms->wormsObject->objectSurface->clip_rect.y + pWorms->texteLifeSurface->h + pWorms->texteNameSurface->h;

	//Affichage de la Vie
	pWorms->texteLifeSurface->clip_rect.x = pWorms->texteNameSurface->clip_rect.x + (pWorms->texteNameSurface->w / 2) - (pWorms->texteLifeSurface->w / 2);
	pWorms->texteLifeSurface->clip_rect.y = pWorms->texteNameSurface->clip_rect.y + pWorms->texteNameSurface->h;
}

/**
* \fn void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
* \brief Gestion du menu InGame
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqu� la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns int, indicateur si la fonction a bien fonctionn�e (1 = succes, -1 = echec)
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
	SDL_DestroyTexture(textureMenuMainMenu);
	SDL_DestroyTexture(textureMenuOption);
	SDL_DestroyTexture(textureMenuQuit);
}

/**
* \fn void updateHUD(Worms** wormsTab)
* \brief MAJ de toutes les textures de l'Interfaces
* \param[in] wormsTab, tableau de worms
*/
void updateHUD(Worms** wormsTab)
{
	char str[20];
	static int lastTimeTeam = 0, lastTimeGeneral = 0;

	int timeToPrintTeam = TEMPSPARTOUR + (int)(globalVar.timeLastWormsChange + globalVar.timePause - SDL_GetTicks()) / 1000;
	int timeToPrintGeneral = TEMPSPARTIE + (int)(globalVar.timePause + globalVar.timeStartGame - SDL_GetTicks()) / 1000;

	if (timeToPrintGeneral <= 0)
	{
		globalInput->changeWorms = 1;
	}
	else if (timeToPrintGeneral != lastTimeGeneral)
	{
		SDL_DestroyTexture(timerGeneralTexture);
		sprintf(str, "%.2d : %.2d", timeToPrintGeneral/60, timeToPrintGeneral%60);
		timerGeneralTexture = loadFromRenderedText(str, globalVar.colorTab[0], &rectTimerGeneral.w, &rectTimerGeneral.h, 98);
		globalInput->raffraichissement = 1;
	}

	if (timeToPrintTeam <= 0)
	{
		globalInput->changeWorms = 1;
	}
	else if (timeToPrintTeam != lastTimeTeam)
	{
		SDL_DestroyTexture(timerTeamTexture);
		sprintf(str, "%d", timeToPrintTeam);
		timerTeamTexture = loadFromRenderedText(str, *wormsTab[calculIndex()]->color, &rectTimerTeam.w, &rectTimerTeam.h, 48);
		globalInput->raffraichissement = 1;
	}
	lastTimeTeam = timeToPrintTeam;
	lastTimeGeneral = timeToPrintGeneral;

	updateRectTimerPosition();
}


/**
* \fn void updateRectPosition()
* \brief MAJ la position des timers sur l'�cran
*/
void updateRectTimerPosition()
{
	int rW, rH;
	SDL_GetRendererOutputSize(globalRenderer, &rW, &rH);
	rectTimerGeneral.x = (rW - rectTimerGeneral.w) / 2;
}

/**
* \fn void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
* \brief Gestion du menu InGame
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqu� la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns int, indicateur si la fonction a bien fonctionn�e (1 = succes, -1 = echec)
*/
void EngGameScreen(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
{
	SDL_Texture* score = loadTexture(INGAMEMENU);
	SDL_Rect rectScore = initButtonBox(-1, -1, 565, 717);

	renderScreen(3, 0, jeu->pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, score, NULL, &rectScore);

	SDL_DestroyTexture(score);

}