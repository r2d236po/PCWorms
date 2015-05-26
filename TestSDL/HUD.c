#include "HUD.h"
#include "my_stdrFct.h"
#include "MainMenu.h"
#include "AffichageGeneral.h"
#include "input.h"
#include "memory.h"

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
	for (i = 0; i < NBFONTS; i++)
	{
		if (globalVar.FontName[i] != NULL)
		{
			TTF_CloseFont(globalVar.FontName[i]);
			globalVar.FontName[i] = NULL;
		}
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
	char str[20];
	SDL_Surface *txtLifeSurface = NULL;
	if (pWorms->vie > 0)
		sprintf(str, " %d ", pWorms->vie);
	else sprintf(str, " ");
	txtLifeSurface = my_RenderText_Blended(globalVar.FontName[0], str, *(pWorms->color));
	if (txtLifeSurface == NULL)
		return -1;
	cleanSurface(pWorms->texteLifeSurface);
	if (!copySurfacePixels(txtLifeSurface, NULL, pWorms->texteLifeSurface, NULL))
	{
		fprintf(logFile, "updateTextSurfaceWorms : FAILURE, copySurfacePixels.\n\n");
	}
	my_freeSurface(txtLifeSurface);
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
* \fn void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, int reset)
* \brief Gestion du menu InGame
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns int, indicateur si la fonction a bien fonctionnée (1 = succes, -1 = echec)
*/
void inGameMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, int reset)
{
	SDL_Texture* textureMenu = NULL;
	SDL_Rect rectMenu;
	static enum HUDMENU menuIn = HUDMAIN, menuPrec = HUDMAIN;
	static int alreadyRender = 0, subMenuPrec = 0;
	int subMenu = 0;
	Point mousePoint;
	getMousePosition(pCamera, &mousePoint.x, &mousePoint.y);

	if (reset)
	{
		alreadyRender = 0;
		//playsoundOpenMenu
	}

	switch (menuIn)
	{
	case HUDMAIN:
		menuIn = mainHUD(&subMenu, getRectMenu(menuIn));
		break;
	case HUDCONFIG:
		menuIn = configHUD(&subMenu, getRectMenu(menuIn));
		break;
	case HUDOPTION:
		menuIn = optionHUD(&subMenu, getRectMenu(menuIn));
		break;
	case HUDHOW:
		menuIn = howHUD(&subMenu, getRectMenu(menuIn));
		break;
	}
	if (menuIn != menuPrec || subMenu != subMenuPrec)
	{
		alreadyRender = 0;
		//if (subMenu > 0)
		//playsound
	}
	if (!alreadyRender)
	{
		alreadyRender = 1;
		textureMenu = getTextureMenu(menuIn, subMenu);
		rectMenu = getRectMenu(menuIn);
		if (textureMenu != NULL)
		{
			renderScreen(3, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, textureMenu, NULL, &rectMenu);
			my_freeTexture(textureMenu);
			textureMenu = NULL;
		}
	}
	if (!globalInput->menu)
	{
		menuIn = HUDMAIN;
		subMenu = 0;
		alreadyRender = 0;
	}
	menuPrec = menuIn;
	subMenuPrec = subMenu;
}


void HUD_weaponsMenu(Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, int reset)
{
	SDL_Texture* weaponTexture = NULL;
	SDL_Texture* selectTexture = NULL;
	SDL_Rect weaponRect = initButtonBox(-1, -1, (int)WEAPONTABW, (int)WEAPONTABH);
	SDL_Rect rectSelect;
	static int alreadyRender = 0;
	int selected = 0;
	if (reset)
		alreadyRender = 0;
	if (HUD_selectWeapon(weaponRect, &rectSelect))
	{
		selectTexture = loadTexture(SELECTTEXTURE);
		if (selectTexture != NULL)
			alreadyRender = 0;
		selected = 1;
	}
	if (!alreadyRender)
	{
		weaponTexture = loadTexture(WEAPONTABPATH);
		if (weaponTexture != NULL)
		{
			if (!selected)
				renderScreen(3, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, weaponTexture, NULL, &weaponRect);
			else
			{
				renderScreen(4, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, weaponTexture, NULL, &weaponRect, 1, selectTexture, NULL, &rectSelect);
				my_freeTexture(selectTexture);
			}
		}
		my_freeTexture(weaponTexture);
		alreadyRender = 1;
	}
}

int HUD_selectWeapon(SDL_Rect rect, SDL_Rect *rectReturn)
{
	SDL_Rect rectWeapon;
	int i, x = 161, y = 154;
	Point mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	for (i = 0; i < NBWEAPON; i++)
	{
		if (i == 5)
		{
			y += 236;
			x = 160;
		}
		else if (i == 10)
		{
			y += 226;
			x = 160;
		}
		rectWeapon = initHUDRect(x, y, 186, 166, rect, WEAPONTABW, WEAPONTABH);
		if (collisionPointWithRect(mouse, &rectWeapon))
		{
			if (globalInput->lclick)
			{
				globalInput->lclick = 0;
				if (i != 1)
				{
					globalInput->weaponIndex = (char)(i- (i>0));
					globalInput->arme = 1;
				}
				else globalInput->grenade = 1;
				globalInput->weaponTab = 0;
			}
			*rectReturn = rectWeapon;
			return 1;
		}
		x += 249;
	}
	return 0;
}

/**
* \fn SDL_Rect getRectMenu(enum HUDMENU menu)
* \brief Get the rect of the menu.
* \param[in] menu, type of menu.
* \returns the rect with the position to display the menu.
*/
SDL_Rect getRectMenu(enum HUDMENU menu)
{
	if (menu == HUDCONFIG)
		return initButtonBox(-1, -1, (int)DEFAULTHUDCONFIGNW, (int)DEFAULTHUDOPTIONH);
	if (menu == HUDOPTION)
		return initButtonBox(-1, -1, (int)DEFAULTHUDOPTIONW, (int)DEFAULTHUDOPTIONH);
	if (menu == HUDHOW)
		return initButtonBox(-1, -1, (int)DEFAULTHUDHOWNW, (int)DEFAULTHUDOPTIONH);
	return initButtonBox(-1, -1, (int)DEFAULTHUDMAINW, (int)DEFAULTHUDMAINH);
}

/**
* \fn SDL_Texture* getTextureMenu(enum HUDMENU menu,int subMenu)
* \brief Get the right texture to display.
* \param[in] menu, type of menu..
* \param[in] subMenu, index of the sub menu to display.
* \returns loadTexture(path to the right picture)
*/
SDL_Texture* getTextureMenu(enum HUDMENU menu, int subMenu)
{
	if (menu == HUDMAIN)
	{
		switch (subMenu)
		{
		case 1:
			return loadTexture(INGAMEMENUMAINMENU);
		case 2:
			return loadTexture(INGAMEMENUOPTIONS);
		case 3:
			return loadTexture(INGAMEMENUQUIT);
		}
	}
	else if (menu == HUDOPTION)
	{
		switch (subMenu)
		{
		case 1:
			return loadTexture(INGAMEOPTIONCONFIG);
		case 2:
			return loadTexture(INGAMEOPTIONHOWTOPLAY);
		case 3:
			return loadTexture(INGAMEOPTIONBACKTOGAME);
		default:
			return loadTexture(INGAMEOPTION);
		}
	}
	else if (menu == HUDCONFIG)
	{
		switch (subMenu)
		{
		case 1:
			return loadTexture(INGAMECONFIGOPTION);
		default:
			return loadTexture(INGAMECONFIG);
		}
	}
	else if (menu == HUDHOW)
	{
		switch (subMenu)
		{
		case 1:
			return loadTexture(INGAMEHOWOPTION);
		default:
			return loadTexture(INGAMEHOW);
		}
	}
	return loadTexture(INGAMEMENU);
}

/**
* \fn enum HUDMENU mainHUD(int *subMenu, SDL_Rect mainRect)
* \brief Manages the main Ingame Menu.
* \param[in] subMenu, pointer to be filled with index of the sub menu to display.
* \param[in] mainRect, index of the sub menu to display.
* \returns the new menu to go.
*/
enum HUDMENU mainHUD(int *subMenu, SDL_Rect mainRect)
{
	SDL_Rect mainMenuButton = initHUDRect(116, 65, 333, 110, mainRect, DEFAULTHUDMAINW, DEFAULTHUDMAINH);
	SDL_Rect optionButton = initHUDRect(116, 292, 333, 110, mainRect, DEFAULTHUDMAINW, DEFAULTHUDMAINH);
	SDL_Rect quitButton = initHUDRect(116, 519, 333, 110, mainRect, DEFAULTHUDMAINW, DEFAULTHUDMAINH);
	Point mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	*subMenu = 0;
	if (collisionPointWithRect(mouse, &mainMenuButton))
	{
		if (globalInput->lclick)
		{
			globalInput->backToMainMenu = 1;
			globalInput->quit = 1;
			globalInput->lclick = 0;
		}
		*subMenu = 1;
	}
	else if (collisionPointWithRect(mouse, &optionButton))
	{
		*subMenu = 2;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			return HUDOPTION;
		}
	}
	else if (collisionPointWithRect(mouse, &quitButton))
	{
		if (globalInput->lclick)
		{
			globalInput->quit = 1;
			globalInput->lclick = 0;
		}
		*subMenu = 3;
	}
	return HUDMAIN;
}

/**
* \fn enum HUDMENU optionHUD(int *subMenu, SDL_Rect mainRect)
* \brief Manages the main Ingame Menu.
* \param[in] subMenu, pointer to be filled with index of the sub menu to display.
* \param[in] mainRect, index of the sub menu to display.
* \returns the new menu to go.
*/
enum HUDMENU optionHUD(int *subMenu, SDL_Rect mainRect)
{
	SDL_Rect configButton = initHUDRect(116, 190, 333, 110, mainRect, DEFAULTHUDOPTIONW, DEFAULTHUDOPTIONH);
	SDL_Rect HowButton = initHUDRect(116, 418, 333, 110, mainRect, DEFAULTHUDOPTIONW, DEFAULTHUDOPTIONH);
	SDL_Rect backToGameButton = initHUDRect(116, 645, 333, 110, mainRect, DEFAULTHUDOPTIONW, DEFAULTHUDOPTIONH);
	Point mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	*subMenu = 0;
	if (collisionPointWithRect(mouse, &configButton))
	{
		*subMenu = 1;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			return HUDCONFIG;
		}
	}
	else if (collisionPointWithRect(mouse, &HowButton))
	{
		*subMenu = 2;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			return HUDHOW;
		}
	}
	else if (collisionPointWithRect(mouse, &backToGameButton))
	{
		*subMenu = 3;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			globalInput->menu = 0;
		}
	}
	return HUDOPTION;
}

/**
* \fn enum HUDMENU configHUD(int *subMenu, SDL_Rect mainRect)
* \brief Manages the main Ingame Menu.
* \param[in] subMenu, pointer to be filled with index of the sub menu to display.
* \param[in] mainRect, index of the sub menu to display.
* \returns the new menu to go.
*/
enum HUDMENU configHUD(int *subMenu, SDL_Rect mainRect)
{
	SDL_Rect optionButton = initHUDRect(585, 30, 283, 93, mainRect, DEFAULTHUDCONFIGNW, DEFAULTHUDOPTIONH);
	Point mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	*subMenu = 0;
	if (collisionPointWithRect(mouse, &optionButton))
	{
		*subMenu = 1;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			return HUDOPTION;
		}
	}
	return HUDCONFIG;
}

/**
* \fn enum HUDMENU howHUD(int *subMenu, SDL_Rect mainRect)
* \brief Manages the main Ingame Menu.
* \param[in] subMenu, pointer to be filled with index of the sub menu to display.
* \param[in] mainRect, index of the sub menu to display.
* \returns the new menu to go.
*/
enum HUDMENU howHUD(int *subMenu, SDL_Rect mainRect)
{
	SDL_Rect optionButton = initHUDRect(584, 30, 283, 93, mainRect, DEFAULTHUDHOWNW, DEFAULTHUDOPTIONH);
	Point mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	*subMenu = 0;
	if (collisionPointWithRect(mouse, &optionButton))
	{
		*subMenu = 1;
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			return HUDOPTION;
		}
	}
	return HUDHOW;
}
/**
* \fn SDL_Rect initHUDRect(int x, int y, int w, int h, SDL_Rect mainRect, int defW, int defH)
* \brief Initialize a button box from the standard dimensions in the image to the relative dimensions of the menu.
*
* \param[in] x, x position of the original box.
* \param[in] y, y position of the original box.
* \param[in] w, width of the original box.
* \param[in] h, hight of the original box.
* \param[in] mainRect, the rect of the main menu.
* \returns The rect with the right dimensions and positions.
*/
SDL_Rect initHUDRect(int x, int y, int w, int h, SDL_Rect mainRect, double defW, double defH)
{
	w = (int)((float)(w / defW) * mainRect.w);
	h = (int)((float)(h / defH) * mainRect.h);
	if (x >= 0)
		x = (int)((float)(x / defW) * mainRect.w) + mainRect.x;
	else x = mainRect.w / 2 - w / 2 + mainRect.x;
	if (y >= 0)
		y = (int)((float)(y / defH) * mainRect.h) + mainRect.y;
	else y = mainRect.h / 2 - h / 2 + mainRect.y;
	return initRect(x, y, w, h);
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

	int timeToPrintTeam = TEMPSPARTOUR + (int)(globalVar.timeLastWormsChange + globalVar.timePause - globalVar.timeWeapon - SDL_GetTicks()) / 1000;
	int timeToPrintGeneral = TEMPSPARTIE + (int)(globalVar.timePause + globalVar.timeStartGame - SDL_GetTicks()) / 1000;

	if (timeToPrintGeneral <= 0)
	{
		globalInput->changeWorms = 1;
	}
	else if (timeToPrintGeneral != lastTimeGeneral)
	{
		if (timerGeneralTexture != NULL)
			my_freeTexture(timerGeneralTexture);
		sprintf(str, "%.2d : %.2d", timeToPrintGeneral / 60, timeToPrintGeneral % 60);
		timerGeneralTexture = loadFromRenderedText(str, globalVar.colorTab[0], &rectTimerGeneral.w, &rectTimerGeneral.h, 72);
		globalInput->raffraichissement = 1;
	}

	if (timeToPrintTeam <= 0)
	{
		globalInput->changeWorms = 1;
	}
	else if (timeToPrintTeam != lastTimeTeam)
	{
		if (timerTeamTexture != NULL)
			my_freeTexture(timerTeamTexture);
		sprintf(str, "%d", timeToPrintTeam);
		SDL_Color color;
		color.r = wormsTab[calculIndex()]->color->b;
		color.g = wormsTab[calculIndex()]->color->g;
		color.b = wormsTab[calculIndex()]->color->r;
		color.a = 255;
		timerTeamTexture = loadFromRenderedText(str, color, &rectTimerTeam.w, &rectTimerTeam.h, 48);
		globalInput->raffraichissement = 1;
	}
	lastTimeTeam = timeToPrintTeam;
	lastTimeGeneral = timeToPrintGeneral;

	updateRectTimerPosition();
}


/**
* \fn void updateRectPosition()
* \brief MAJ la position des timers sur l'écran
*/
void updateRectTimerPosition()
{
	int rW, rH;
	SDL_GetRendererOutputSize(globalRenderer, &rW, &rH);
	rectTimerGeneral.x = (rW - rectTimerGeneral.w) / 2;
}

/**
* \fn void EngGameScreen(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
* \brief Gestion du menu InGame
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \returns int, indicateur si la fonction a bien fonctionnée (1 = succes, -1 = echec)
*/
void EngGameScreen(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera)
{
	SDL_Texture* score = loadTexture(INGAMEMENU);
	SDL_Rect rectScore = initButtonBox(-1, -1, 565, 717);

	renderScreen(3, 0, jeu->pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, score, NULL, &rectScore);

	my_freeTexture(score);

}