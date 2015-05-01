#include "MainMenu.h"
#include "input.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"

int mainMenu(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, char mapName[100], int *pNbTeam, int *pNbWorms)
{
	SDL_Texture *menuTexture[NBTEXTURE];
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	enum MENU menuIn = MAIN;
	int quitMenu = 0, indexTeam = 1;
	enum CHOICE nextPrev = NEITHER;
	enum CHOICE resizable = YES, windowSize = DEFAULT, music = YES, sound = YES, savePath = DEFAULT;
	if (initTabTextureMenu(pRenderer, menuTexture, MAINMENU, MAINMENUVERSUS, MAINMENUMAP, MAINMENUOPTION,
		VERSUSMENU, VERSUSMENUNEXT, VERSUSMENUMAIN, VERSUSMENUNAMEPLAYER, VERSUSMENUNAMEPLAYERN, VERSUSMENUNAMEPLAYERM, VERSUSMENUSTART, VERSUSMENUSTARTS,
		MAPMENU, MAPMENUMAIN, MAPMENUCHOOSE, MAPMENUREPERTORY,
		OPTIONMENU, OPTIONMENUMAIN) < 0)
		return -1;
	int i;
	for (i = 0; i < 4; i++)
		strcpy(globalVar.teamNames[i], "");
	for (i = 0; i < 16; i++)
		strcpy(globalVar.wormsNames[i], "");

	while (!quitMenu)
	{
		getInput(pInput, pWindow);

		switch (menuIn)
		{
		case MAINversus:
		case MAINmap:
		case MAINoption:
		case MAIN:
			menuIn = menu(pRenderer, pInput);
			break;
		case MAPmain:
		case MAPchoose:
		case MAPrepertory:
		case MAP:
			menuIn = mapMenu(pRenderer, pInput, &nextPrev);
			break;
		case OPTIONSm:
		case OPTIONS:
			menuIn = optionMenu(pRenderer, pInput);
			break;
		case VERSUSn:
		case VERSUSm:
		case VERSUSname:
		case VERSUSnameN:
		case VERSUSnameM:
		case VERSUSstart:
		case VERSUSstartS:
		case VERSUS:
			menuIn = versusMenu(pRenderer, pInput, &quitMenu, menuIn, &indexTeam);
			break;
		}
		if (pInput->raffraichissement == 1)
		{
			SDL_RenderCopy(pRenderer, menuTexture[indiceTexture(menuIn)], NULL, NULL);
			if (menuIn == MAP || menuIn == MAPmain || menuIn == MAPchoose || menuIn == MAPrepertory)
			{
				strcpy(mapName, mapSketch(pRenderer, nextPrev));
				nextPrev = NEITHER;
			}
			if (menuIn == OPTIONS || menuIn == OPTIONSm)
			{
				setResizableOption(pWindow, pRenderer, pInput, &resizable);
				setMusicOption(pRenderer, pInput, &music);
				setSoundOption(pRenderer, pInput, &sound);
				setSizeOption(pWindow, pRenderer, pInput, &windowSize);
				setSavePathOption(pRenderer, pInput, &savePath, windowSize);
			}
			if (menuIn == VERSUS || menuIn == VERSUSn || menuIn == VERSUSm)
			{
				setColorTeam(pRenderer, pInput);
				setTeamName(pRenderer, pInput);
			}
			if (menuIn == VERSUSname || menuIn == VERSUSnameN || menuIn == VERSUSnameM
				|| menuIn == VERSUSstart || menuIn == VERSUSstartS)
			{
				setWormsName(pRenderer, pInput, indexTeam);
			}
			SDL_RenderPresent(pRenderer);
			pInput->raffraichissement = 0;
		}
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;
		if (quitMenu == 0)
			quitMenu = pInput->quit;
	}

	/*Calcul du nombre d'équipe et du nombre de worms par équipe*/
	*pNbWorms = 0;
	*pNbTeam = 0;
	for (i = 0; i < 4; i++)
	{
		if (strcmp(globalVar.teamNames[i], "") != 0)
			*pNbTeam += 1;
	}
	for (i = 0; i < 16; i++)
	{
		if (strcmp(globalVar.wormsNames[i], "") != 0)
			*pNbWorms += 1;
	}
	if ((*pNbTeam) == 0 || (*pNbWorms) == 0)
		pInput->quit = 1;
	if (*pNbWorms != 0)
		*pNbWorms = (*pNbWorms) / (*pNbTeam);
	SDL_StopTextInput();
	resetStructInput(pInput);
	destroyTextureTab(menuTexture);
	setColorForGame();
	return 0;
}

/**
* \fn enum MENU menu(SDL_Renderer* pRenderer, Input* pInput)
* \brief Manages the main menu.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns which menu to display.
*/
enum MENU menu(SDL_Renderer* pRenderer, Input* pInput)
{
	SDL_Rect versusRect, optionRect, mapRect;
	int wRender = 0, hRender = 0, xVersus = 33, xMap = 758, xOption = 1479;
	int y = 893, wBox = 470, hBox = 158, testChange = 0;
	static int alreadyChange = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
	hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
	y = (int)((float)(y / hightMenuTexture) * hRender);

	xVersus = (int)((float)(xVersus / widthMenuTexture) * wRender);
	xMap = (int)((float)(xMap / widthMenuTexture) * wRender);
	xOption = (int)((float)(xOption / widthMenuTexture) * wRender);

	versusRect = initRect(xVersus, y, wBox, hBox);
	mapRect = initRect(xMap, y, wBox, hBox);
	optionRect = initRect(xOption, y, wBox, hBox);
	if (collisionPointWithRect(pInput->cursor.now, &versusRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			alreadyChange = 0;
			strcpy(pInput->textInput, "");
			return VERSUS;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAINversus;
	}
	else if (collisionPointWithRect(pInput->cursor.now, &mapRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			alreadyChange = 0;
			return MAP;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAINmap;
	}
	else if (collisionPointWithRect(pInput->cursor.now, &optionRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			alreadyChange = 0;
			return OPTIONS;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAINoption;
	}
	else
	{
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			pInput->raffraichissement = 1;
	}
	return MAIN;
}


/********************************************************************************/
/********************************************************************************/
/**************************                        ******************************/
/**************************       VERSUS MENU      ******************************/
/**************************                        ******************************/
/********************************************************************************/
/********************************************************************************/


/**
* \fn enum MENU versusMenu(SDL_Renderer* pRenderer, Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam)
* \brief Manages the versus menu.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] quit, pointer to the quit variable to start the game.
* \param[in] menuPrec, precedent menu.
* \returns which menu to display.
*/
enum MENU versusMenu(SDL_Renderer* pRenderer, Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam)
{
	SDL_Rect nextRect, mainRect, startRect;
	int wRender = 0, hRender = 0, xNext = 1625, xMain = 48, xStart = 1450, testChange = 0;
	int yNext = 929, yMain = 965, yStart = 917, wBox = 328, hBox = 198;
	static int alreadyChange = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);

	if (menuPrec != VERSUSstart && menuPrec != VERSUSstartS)
	{
		wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
		hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
		xNext = (int)((float)(xNext / widthMenuTexture) * wRender);
		yNext = (int)((float)(yNext / hightMenuTexture) * hRender);
		nextRect = initRect(xNext, yNext, wBox, hBox);

		wBox = 324;
		hBox = 121;
		wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
		hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
		xMain = (int)((float)(xMain / widthMenuTexture) * wRender);
		yMain = (int)((float)(yMain / hightMenuTexture) * hRender);
		mainRect = initRect(xMain, yMain, wBox, hBox);

		if (collisionPointWithRect(pInput->cursor.now, &nextRect))
		{
			if (pInput->lclick)
			{
				pInput->lclick = 0;
				pInput->raffraichissement = 1;
				alreadyChange = 0;
				strcpy(pInput->textInput, "");
				if (menuPrec == VERSUSn || menuPrec == VERSUS)
				{
					return VERSUSname;
				}
				else if ((menuPrec == VERSUSnameN || menuPrec == VERSUSname) && (*pIndexTeam) < 3)
				{
					*pIndexTeam += 1;
					return VERSUSname;
				}
				else if (menuPrec == VERSUSnameN || menuPrec == VERSUSname)
				{
					*pIndexTeam += 1;
					return VERSUSstart;
				}
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				pInput->raffraichissement = 1;
			}
			if (menuPrec == VERSUS || menuPrec == VERSUSn)
				return VERSUSn;
			else return VERSUSnameN;
		}
		else if (collisionPointWithRect(pInput->cursor.now, &mainRect))
		{
			if (pInput->lclick)
			{
				pInput->lclick = 0;
				pInput->raffraichissement = 1;
				alreadyChange = 0;
				strcpy(pInput->textInput, "");
				*pIndexTeam = 1;
				return MAIN;
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				pInput->raffraichissement = 1;
			}
			if (menuPrec == VERSUS || menuPrec == VERSUSm)
				return VERSUSm;
			else return VERSUSnameM;
		}
	}
	else
	{
		wBox = 470;
		hBox = 158;
		wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
		hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
		xStart = (int)((float)(xStart / widthMenuTexture) * wRender);
		yStart = (int)((float)(yStart / hightMenuTexture) * hRender);
		startRect = initRect(xStart, yStart, wBox, hBox);
		if (collisionPointWithRect(pInput->cursor.now, &startRect))
		{
			if (pInput->lclick)
			{
				pInput->lclick = 0;
				pInput->raffraichissement = 1;
				alreadyChange = 0;
				*quit = 1;
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				pInput->raffraichissement = 1;
			}
			return VERSUSstartS;
		}
	}

	*quit = 0;
	testChange = alreadyChange;
	alreadyChange = 0;
	if (testChange != alreadyChange)
		pInput->raffraichissement = 1;

	if (menuPrec == VERSUSstartS || menuPrec == VERSUSstart)
		return VERSUSstart;
	else if (menuPrec == VERSUSn || menuPrec == VERSUSm || menuPrec == VERSUS)
		return VERSUS;
	else if (menuPrec == VERSUSnameN || menuPrec == VERSUSnameM || menuPrec == VERSUSname)
		return VERSUSname;
	return VERSUS;
}

/**
* \fn void setColorTeam(SDL_Renderer* pRenderer, Input* pInput)
* \brief Manages the color of a team.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns void.
*/
void setColorTeam(SDL_Renderer* pRenderer, Input* pInput)
{
	SDL_Color rouge, vert, jaune, violet, bleu, beige;
	int indexColor = 0, indexTeam = 1;
	indexColor = getIndexColor(pRenderer, pInput);
	if (indexColor >= 0 && indexColor <= 5)
		indexTeam = 1;
	else if (indexColor >= 6 && indexColor <= 11)
	{
		indexTeam = 2;
		indexColor -= 6;
	}
	else if (indexColor >= 12 && indexColor <= 17)
	{
		indexTeam = 3;
		indexColor -= 12;
	}
	else if (indexColor >= 18 && indexColor <= 23)
	{
		indexTeam = 4;
		indexColor -= 18;
	}

	switch (indexColor)
	{
	case 0:
		setSDLColor(&rouge, 134, 0, 0);
		rouge.a = 255;
		globalVar.colorTab[indexTeam - 1] = rouge;
		break;
	case 1:
		setSDLColor(&vert, 0, 176, 80);
		vert.a = 255;
		globalVar.colorTab[indexTeam - 1] = vert;
		break;
	case 2:
		setSDLColor(&jaune, 255, 192, 0);
		jaune.a = 255;
		globalVar.colorTab[indexTeam - 1] = jaune;
		break;
	case 3:
		setSDLColor(&violet, 112, 48, 160);
		violet.a = 255;
		globalVar.colorTab[indexTeam - 1] = violet;
		break;
	case 4:
		setSDLColor(&bleu, 0, 32, 96);
		bleu.a = 255;
		globalVar.colorTab[indexTeam - 1] = bleu;
		break;
	case 5:
		setSDLColor(&beige, 237, 125, 49);
		beige.a = 255;
		globalVar.colorTab[indexTeam - 1] = beige;
		break;
	}
}

/**
* \fn int getIndexColor(SDL_Renderer* pRenderer, Input* pInput)
* \brief Determinate the color and the to apply the color.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns index to the color and the team corresponding.
*/
int getIndexColor(SDL_Renderer* pRenderer, Input* pInput)
{
	int xBox = 1243, yBox = 237, wBox = 63, hBox = 60;
	int xNom = xBox, yNom = yBox, wRender, hRender;
	int indexColor, indexTeam;
	static int colorPrec = 0;
	SDL_Rect colorRect;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
	hBox = (int)((float)(hBox / hightMenuTexture) * hRender);

	for (indexTeam = 1; indexTeam <= 4; indexTeam++)
	{
		yBox = yNom + (indexTeam - 1) * 164;
		yBox = (int)((float)(yBox / hightMenuTexture) * hRender);
		for (indexColor = 0; indexColor < 6; indexColor++)
		{
			xBox = xNom + indexColor * 130;
			xBox = (int)((float)(xBox / widthMenuTexture) * wRender);
			colorRect = initRect(xBox, yBox, wBox, hBox);
			if (collisionPointWithRect(pInput->cursor.now, &colorRect))
			{
				if (pInput->lclick)
				{
					pInput->lclick = 0;
					colorPrec = indexColor + (indexTeam - 1) * 6;
					return indexColor + (indexTeam - 1) * 6;
				}
			}
		}
	}
	return colorPrec;
}

/**
* \fn void setTeamName(SDL_Renderer* pRenderer, Input* pInput)
* \brief Writes the team names.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns void.
*/
void setTeamName(SDL_Renderer* pRenderer, Input* pInput)
{
	int indexPrec = 0;
	static int indexTeam = 0;
	indexPrec = indexTeam;
	indexTeam = getTeamIndexText(pRenderer, pInput);
	if (indexTeam == 0)
		return;
	else
	{
		if (indexPrec != indexTeam || strcmp(pInput->textInput, "") == 0)
		{
			strcpy(pInput->textInput, globalVar.teamNames[indexTeam - 1]);
			pInput->textCounter = (char)strlen(globalVar.teamNames[indexTeam - 1]);
		}
		if (pInput->textCounter >= 49)
			pInput->textCounter = 49;
		strcpy(globalVar.teamNames[indexTeam - 1], pInput->textInput);
		int xText = 647, y = 0, wRender, hRender, wText, hText = 87, i;
		SDL_Rect rect;

		SDL_StartTextInput();
		SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
		xText = (int)((float)(xText / widthMenuTexture) * wRender);
		for (i = 0; i < 4; i++)
		{
			y = 225 + i * 163;
			y = (int)((float)(y / hightMenuTexture) * hRender) + (int)((float)(0.3 * hText / hightMenuTexture) * hRender);
			SDL_Texture* text = loadFromRenderedText(pRenderer, globalVar.teamNames[i], globalVar.colorTab[i], &wText, &hText, 16);
			rect = initRect(xText, y, wText, hText);
			SDL_RenderCopy(pRenderer, text, NULL, &rect);
			SDL_DestroyTexture(text);
			hText = 87;
		}

	}
}

/**
* \fn int getTeamIndexText(SDL_Renderer* pRenderer, Input* pInput)
* \brief Determine what team to write to.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns index of the team.
*/
int getTeamIndexText(SDL_Renderer* pRenderer, Input* pInput)
{
	return getIndexText(pRenderer, pInput, 647);
}

/**
* \fn void setWormsName(SDL_Renderer* pRenderer, Input* pInput, int indexTeam)
* \brief Writes the team names.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] indexTeam, index of the team.
* \returns void.
*/
void setWormsName(SDL_Renderer* pRenderer, Input* pInput, int indexTeam)
{
	int indexPrec = 0;
	static int indexWorms = 0;
	int xTitre = 470, yTitre = 32, wTitre, hTitre, wRender, hRender;
	char strTitre[45];
	SDL_Color color;
	color.a = 255;
	color.b = 0;
	color.r = color.g = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	sprintf(strTitre, "Choix des noms des joueurs de l'équipe %d :", indexTeam);
	xTitre = (int)((float)(xTitre / widthMenuTexture) * wRender);
	yTitre = (int)((float)(yTitre / hightMenuTexture) * hRender);
	SDL_Texture* text = loadFromRenderedText(pRenderer, strTitre, color, &wTitre, &hTitre, 32);
	SDL_Rect rectTitre = initRect(xTitre, yTitre, wTitre, hTitre);
	SDL_RenderCopy(pRenderer, text, NULL, &rectTitre);


	indexPrec = indexWorms;
	indexWorms = getWormsIndexText(pRenderer, pInput);
	if (indexWorms == 0)
		return;
	else
	{
		indexWorms = indexWorms + (indexTeam - 1) * 4;
		if (indexPrec != indexWorms || strcmp(pInput->textInput, "") == 0)
		{
			strcpy(pInput->textInput, globalVar.wormsNames[indexWorms - 1]);
			pInput->textCounter = (char)strlen(globalVar.wormsNames[indexWorms - 1]);
		}
		if (pInput->textCounter >= 49)
			pInput->textCounter = 49;
		strcpy(globalVar.wormsNames[indexWorms - 1], pInput->textInput);
		int xText = 920, y = 0, wText, hText = 87, i;
		SDL_Rect rect;

		SDL_StartTextInput();
		xText = (int)((float)(xText / widthMenuTexture) * wRender);
		for (i = 0; i < 4; i++)
		{
			y = 225 + i * 163;
			y = (int)((float)(y / hightMenuTexture) * hRender) + (int)((float)(0.3 * hText / hightMenuTexture) * hRender);
			SDL_Texture* text = loadFromRenderedText(pRenderer, globalVar.wormsNames[i + (indexTeam - 1) * 4], globalVar.colorTab[(indexTeam - 1)], &wText, &hText, 16);
			rect = initRect(xText, y, wText, hText);
			SDL_RenderCopy(pRenderer, text, NULL, &rect);
			SDL_DestroyTexture(text);
			hText = 87;
		}

	}
}

/**
* \fn int getWormsIndexText(SDL_Renderer* pRenderer, Input* pInput)
* \brief Determine what team to write to.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns index of the worms.
*/
int getWormsIndexText(SDL_Renderer* pRenderer, Input* pInput)
{
	return getIndexText(pRenderer, pInput, 920);
}

/**
* \fn int getIndexText(SDL_Renderer* pRenderer, Input* pInput, int xBox)
* \brief Determine what team to write to.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] xBox, origin of the text.
* \returns index of the box text.
*/
int getIndexText(SDL_Renderer* pRenderer, Input* pInput, int xBox)
{
	int xText = xBox, yText = 225, wText = 533, hText = 87, yNom = 225;
	int wRender, hRender, index;
	static int indexPrec = 0;
	SDL_Rect textRect;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	wText = (int)((float)(wText / widthMenuTexture) * wRender);
	hText = (int)((float)(hText / hightMenuTexture) * hRender);
	xText = (int)((float)(xText / widthMenuTexture) * wRender);

	for (index = 1; index <= 4; index++)
	{
		yText = (int)((float)(yText / hightMenuTexture) * hRender);
		textRect = initRect(xText, yText, wText, hText);
		if (collisionPointWithRect(pInput->cursor.now, &textRect))
		{
			if (pInput->lclick)
			{
				pInput->lclick = 0;
				indexPrec = index;
				return index;
			}
		}
		yText = yNom + (index)* 163;
	}
	if (pInput->lclick)
	{
		pInput->lclick = 0;
		indexPrec = 0;
		return 0;
	}
	return indexPrec;
}












/*****************************************************************************/
/*****************************************************************************/
/**************************                     ******************************/
/**************************       MAP MENU      ******************************/
/**************************                     ******************************/
/*****************************************************************************/
/*****************************************************************************/



/**
* \fn enum MENU mapMenu(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *nextPrev)
* \brief Manages the map menu.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] nextPrev, pointer to a choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns which menu to display.
*/
enum MENU mapMenu(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *nextPrev)
{
	SDL_Rect mainMenuRect, nextRect, previousRect, chooseRect, repertoryRect;
	int wRender = 0, hRender = 0, xMain = 48, xNext = 1245, xPrevious = 545, xChoose = 845, xRepertory = 1629;
	int y = 966, wBox = 324, hBox = 121, testChange = 0;
	static int alreadyChange = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	y = (int)((float)(y / hightMenuTexture) * hRender);
	wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
	hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
	xMain = (int)((float)(xMain / widthMenuTexture) * wRender);
	mainMenuRect = initRect(xMain, y, wBox, hBox);

	xRepertory = (int)((float)(xRepertory / widthMenuTexture) * wRender);
	repertoryRect = initRect(xRepertory, y, wBox, hBox);

	y = 927;
	y = (int)((float)(y / hightMenuTexture) * hRender);
	xChoose = (int)((float)(xChoose / widthMenuTexture) * wRender);
	chooseRect = initRect(xChoose, y, wBox, hBox);

	y = 892;
	y = (int)((float)(y / hightMenuTexture) * hRender);
	xNext = (int)((float)(xNext / widthMenuTexture) * wRender);
	wBox = (int)((float)(211 / widthMenuTexture) * wRender);
	hBox = (int)((float)(194 / hightMenuTexture) * hRender);
	nextRect = initRect(xNext, y, wBox, hBox);

	xPrevious = (int)((float)(xPrevious / widthMenuTexture) * wRender);
	previousRect = initRect(xPrevious, y, wBox, hBox);


	if (collisionPointWithRect(pInput->cursor.now, &mainMenuRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			alreadyChange = 0;
			pInput->raffraichissement = 1;
			return MAIN;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAPmain;
	}
	else if (collisionPointWithRect(pInput->cursor.now, &nextRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			*nextPrev = NEXT;
			pInput->raffraichissement = 1;
			return MAP;
		}
	}
	else if (collisionPointWithRect(pInput->cursor.now, &previousRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			*nextPrev = PREVIOUS;
			pInput->raffraichissement = 1;
			return MAP;
		}
	}
	else if (collisionPointWithRect(pInput->cursor.now, &chooseRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			alreadyChange = 0;
			pInput->raffraichissement = 1;
			return MAIN;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAPchoose;
	}
	else if (collisionPointWithRect(pInput->cursor.now, &repertoryRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			pInput->raffraichissement = 1;
			return MAP;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return MAPrepertory;
	}
	else
	{
		*nextPrev = NEITHER;
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			pInput->raffraichissement = 1;
	}
	return MAP;
}

/**
* \fn char* mapSketch(SDL_Renderer* pRenderer, enum CHOICE nextPrev)
* \brief Displays a sketch of the selected map.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] nextPrev, choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns string of the selected map path.
*/
char* mapSketch(SDL_Renderer* pRenderer, enum CHOICE nextPrev)
{
	SDL_Rect sketchRect;
	SDL_Texture* sketchTexture = NULL;
	static int counter = 0;
	int x = 182, y = 57, w = 1638, h = 740, wRender = 0, hRender = 0;
	float coeffX = (float)(x / widthMenuTexture), coeffY = (float)(y / hightMenuTexture), coeffW = (float)(w / widthMenuTexture), coeffH = (float)(h / hightMenuTexture);
	static char *mapString[NUMBERMAP] = { cMAP, cMAP_HD, cMAP_TEST, cMAP_TEST2, cMAP_TEST3, cMAP_TEST4, cMAP_CAM };

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	x = (int)(coeffX * wRender);
	y = (int)(coeffY * hRender);
	w = (int)(coeffW * wRender);
	h = (int)(coeffH * hRender);

	sketchRect = initRect(x, y, w, h);

	if (nextPrev == NEXT)
		counter++;
	else if (nextPrev == PREVIOUS)
		counter--;
	if (counter >= NUMBERMAP)
		counter = 0;
	else if (counter < 0)
		counter = NUMBERMAP - 1;
	sketchTexture = loadTexture(pRenderer, mapString[counter]);
	if (sketchTexture == NULL)
	{
		fprintf(logFile, "mapSketch : FAILURE, loadTexture sketchTexture.\n\n");
		nextPrev = NEITHER;
		return NULL;
	}
	SDL_RenderCopy(pRenderer, sketchTexture, NULL, &sketchRect);
	SDL_DestroyTexture(sketchTexture);
	return mapString[counter];
}

/**
* \fn int indiceTexture(enum MENU menu)
* \brief Convert the menu name into an int.
*
* \param[in] menu, menu enumeration.
* \returns the index of the texture to display.
*/
int indiceTexture(enum MENU menu)
{
	switch (menu)
	{
	case MAIN:
		return 0;
	case MAINversus:
		return 1;
	case MAINmap:
		return 2;
	case MAINoption:
		return 3;
	case VERSUS:
		return 4;
	case VERSUSn:
		return 5;
	case VERSUSm:
		return 6;
	case VERSUSname:
		return 7;
	case VERSUSnameN:
		return 8;
	case VERSUSnameM:
		return 9;
	case VERSUSstart:
		return 10;
	case VERSUSstartS:
		return 11;
	case MAP:
		return 12;
	case MAPmain:
		return 13;
	case MAPchoose:
		return 14;
	case MAPrepertory:
		return 15;
	case OPTIONS:
		return 16;
	case OPTIONSm:
		return 17;
	}
	return 0;
}

/**
* \fn int initTabTextureMenu(SDL_Renderer* pRenderer, SDL_Texture* menuTexture[NBTEXTURE], ...)
* \brief Initialiaze the array of texture for the menu.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] menuTexture, array of texture.
* \returns 0 = SUCCESS, -1 = ERROR.
* \remarks the addition parameters are the strings of the image to load.
*/
int initTabTextureMenu(SDL_Renderer* pRenderer, SDL_Texture* menuTexture[NBTEXTURE], ...)
{
	int indexTexture = 0;
	va_list list;
	va_start(list, menuTexture);
	for (indexTexture = 0; indexTexture < NBTEXTURE; indexTexture++)
	{
		menuTexture[indexTexture] = loadTexture(pRenderer, va_arg(list, char*));
		if (menuTexture[indexTexture] == NULL)
		{
			fprintf(logFile, "initTabTextureMenu : FAILURE, loadTexture versusMenuTexture.\n\n");
			va_end(list);
			return -1;
		}
	}
	va_end(list);
	return 0;
}

/**
* \fn void destroyTextureTab(SDL_Texture* textureTab[NBTEXTURE])
* \brief Destroy the array of texture for the menu.
*
* \param[in] textureTab, array of texture.
* \returns void
*/
void destroyTextureTab(SDL_Texture* textureTab[NBTEXTURE])
{
	int indexTexture = 0;
	for (indexTexture = 0; indexTexture < NBTEXTURE; indexTexture++)
	{
		if (textureTab[indexTexture] != NULL)
		{
			SDL_DestroyTexture(textureTab[indexTexture]);
			textureTab[indexTexture] = NULL;
		}
	}
}








/****************************************************************************/
/****************************************************************************/
/**************************                    ******************************/
/**************************    OPTIONS MENU    ******************************/
/**************************                    ******************************/
/****************************************************************************/
/****************************************************************************/

/**
* \fn enum MENU optionMenu(SDL_Renderer* pRenderer, Input* pInput)
* \brief Manages the option menu.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \returns which menu to display.
*/
enum MENU optionMenu(SDL_Renderer* pRenderer, Input* pInput)
{
	SDL_Rect mainMenuRect;
	int wRender = 0, hRender = 0, xMain = 44;
	int y = 965, wBox = 324, hBox = 121, testChange = 0;
	static int alreadyChange = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	xMain = (int)((float)(xMain / widthMenuTexture) * wRender);
	wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
	hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
	y = (int)((float)(y / hightMenuTexture) * hRender);
	mainMenuRect = initRect(xMain, y, wBox, hBox);


	if (collisionPointWithRect(pInput->cursor.now, &mainMenuRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			pInput->raffraichissement = 1;
			return MAIN;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			pInput->raffraichissement = 1;
		}
		return OPTIONSm;
	}
	else
	{
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			pInput->raffraichissement = 1;
	}
	return OPTIONS;
}

/**
* \fn void setResizableOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
* \brief Set the resizable option.
*
* \param[in] pWindow, pointer to the window.
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setResizableOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1197, y = 149;
	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		SDL_SetWindowResizable(pWindow, SDL_TRUE);
	else SDL_SetWindowResizable(pWindow, SDL_FALSE);
}

/**
* \fn void setMusicOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
* \brief Set the music option.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setMusicOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1197, y = 434;
	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		pInput->musicAllowed = 1;
	else pInput->musicAllowed = 0;
}

/**
* \fn void setSoundOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
* \brief Set the sound option.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSoundOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1199, y = 576;
	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		pInput->soundAllowed = 1;
	else pInput->soundAllowed = 0;
}

/**
* \fn void setSizeOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
* \brief Set the size option.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSizeOption(SDL_Window* pWindow, SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1197, y = 292, xText = 1493, wText = 386, hText = 88, wRender = 0, hRender = 0;
	static int textStart = 0, wWindow, hWindow;
	SDL_Rect rect;
	enum CHOICE testChange = *pChoice;
	static char strSize[21] = "1080*600";
	SDL_Color color;
	color.a = 255;
	color.b = 0;
	color.r = color.g = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (testChange != *pChoice)
	{
		strcpy(pInput->textInput, "");
		pInput->textCounter = 0;
	}
	if (*pChoice == TEXT)
	{
		if (!textStart)
		{
			textStart = 1;
			SDL_StartTextInput();
		}
		if (textStart)
		{
			textStart = !testEndInput(pInput->textInput, 100);
			if (strcmp(pInput->textInput, "") == 0 && strlen(strSize) > 1)
			{
				strcpy(pInput->textInput, strSize);
				pInput->textCounter = (char)strlen(strSize);
			}
			strcpy(strSize, pInput->textInput);
		}
	}
	else if (*pChoice == SET)
	{
		if (testChange != *pChoice)
		{
			getSizeWindow(&wWindow, &hWindow, strSize);
			SDL_SetWindowSize(pWindow, wWindow, hWindow);
			pInput->textCounter = 0;
		}
	}
	else if (*pChoice == DEFAULT && testChange != *pChoice)
	{
		wWindow = 1080;
		hWindow = 600;
		strcpy(strSize, "1080*600");
		SDL_SetWindowSize(pWindow, wWindow, hWindow);
	}
	xText = (int)((float)(xText / widthMenuTexture) * wRender);
	y = (int)((float)(y / hightMenuTexture) * hRender) + (int)((float)(0.3 * hText / hightMenuTexture) * hRender);
	SDL_Texture* text = loadFromRenderedText(pRenderer, strSize, color, &wText, &hText, 16);
	rect = initRect(xText, y, wText, hText);
	SDL_RenderCopy(pRenderer, text, NULL, &rect);
	SDL_DestroyTexture(text);
}

/**
* \fn void setSavePathOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize)
* \brief Set the save path option.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSavePathOption(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize)
{
	int xYes = 1197, y = 719, xText = 1493, wText = 386, hText = 88, wRender = 0, hRender = 0;
	static int textStart = 0;
	SDL_Rect rect;
	static char strPath[100] = "";
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	color.a = 255;
	color.b = 0;
	color.r = color.g = 0;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (testChange != *pChoice)
	{
		strcpy(pInput->textInput, "");
		pInput->textCounter = 0;
	}
	if (*pChoice == DEFAULT)
	{
		if (strcmp(globalVar.savePath, DEFAULTPATH) != 0)
			strcpy(globalVar.savePath, DEFAULTPATH);
		strcpy(strPath, DEFAULTPATH);
	}
	else if (*pChoice == TEXT && windowSize != TEXT)
	{
		if (!textStart)
		{
			textStart = 1;
			SDL_StartTextInput();
		}
		if (textStart)
		{
			textStart = !testEndInput(pInput->textInput, 100);
			if (strcmp(pInput->textInput, "") == 0 && strlen(strPath) > 1)
			{
				strcpy(pInput->textInput, strPath);
				pInput->textCounter = (char)strlen(strPath);
			}
			strcpy(strPath, pInput->textInput);
		}
	}
	xText = (int)((float)(xText / widthMenuTexture) * wRender);
	y = (int)((float)(y / hightMenuTexture) * hRender) + (int)((float)(0.3 * hText / hightMenuTexture) * hRender);
	SDL_Texture* text = loadFromRenderedText(pRenderer, strPath, color, &wText, &hText, 16);
	rect = initRect(xText, y, wText, hText);
	SDL_RenderCopy(pRenderer, text, NULL, &rect);
	SDL_DestroyTexture(text);
}

/**
* \fn void toggleOptions(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, int x, int y)
* \brief Toggle an option.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \param[in] x, x coordinnate of the first option box.
* \param[in] y, y coordinnate of the first option box.
* \returns void.
*/
void toggleOptions(SDL_Renderer* pRenderer, Input* pInput, enum CHOICE *pChoice, int x, int y)
{
	SDL_Texture* textureChoice = NULL;
	SDL_Rect yesRect, noRect, textRect, setRect;
	int xYes = x, xNo = 1493, yBox = y, xSet = 1846;
	int wRender = 0, hRender = 0, wBox = 172, hBox = 88, wText = 386, wSet = 107;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	wBox = (int)((float)(wBox / widthMenuTexture) * wRender);
	wText = (int)((float)(wText / widthMenuTexture) * wRender);
	wSet = (int)((float)(wSet / widthMenuTexture) * wRender);
	hBox = (int)((float)(hBox / hightMenuTexture) * hRender);
	xYes = (int)((float)(xYes / widthMenuTexture) * wRender);
	xSet = (int)((float)(xSet / widthMenuTexture) * wRender);
	xNo = (int)((float)(xNo / widthMenuTexture) * wRender);
	yBox = (int)((float)(yBox / hightMenuTexture) * hRender);

	yesRect = initRect(xYes, yBox, wBox, hBox);
	noRect = initRect(xNo, yBox, wBox, hBox);
	textRect = initRect(xNo, yBox, wText, hBox);
	setRect = initRect(xSet, yBox, wSet, hBox);

	if (collisionPointWithRect(pInput->cursor.now, &yesRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			if (*pChoice != NEITHER && *pChoice != DEFAULT && *pChoice != SET && *pChoice != TEXT)
				*pChoice = YES;
			else *pChoice = DEFAULT;
		}
	}
	else if (collisionPointWithRect(pInput->cursor.now, &noRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			if (*pChoice != NEITHER && *pChoice != DEFAULT && *pChoice != SET && *pChoice != TEXT)
				*pChoice = NO;
			else *pChoice = TEXT;
		}
	}
	else if (collisionPointWithRect(pInput->cursor.now, &setRect))
	{
		if (pInput->lclick)
		{
			pInput->lclick = 0;
			if (*pChoice == NEITHER || *pChoice == DEFAULT || *pChoice == TEXT)
				*pChoice = SET;
		}
	}
	else if ((*pChoice == TEXT || *pChoice == NEITHER) && pInput->lclick)
		*pChoice = NEITHER;
	if (*pChoice == YES)
	{
		textureChoice = loadTexture(pRenderer, YESTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(pRenderer, textureChoice, NULL, &yesRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice == DEFAULT)
	{
		textureChoice = loadTexture(pRenderer, DEFAULTTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(pRenderer, textureChoice, NULL, &yesRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice == SET)
	{
		textureChoice = loadTexture(pRenderer, SETTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(pRenderer, textureChoice, NULL, &setRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice != NEITHER && *pChoice != TEXT)
	{
		textureChoice = loadTexture(pRenderer, NOTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(pRenderer, textureChoice, NULL, &noRect);
		pInput->raffraichissement = 1;
	}
	SDL_DestroyTexture(textureChoice);
}

/**
* \fn void SDL_SetWindowResizable(SDL_Window *pWindow, SDL_bool resizable)
* \brief Set the resizable flag of the window.
*
* \param[in] pWindow, pointer to the window.
* \param[in] resizable, activation of the resizable possibility, TRUE = resizable.
* \returns void
*/
void SDL_SetWindowResizable(SDL_Window *pWindow, SDL_bool resizable)
{
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(pWindow, &info);

#if WIN32
	HWND hwnd = info.info.win.window;
	DWORD style = GetWindowLong(hwnd, GWL_STYLE);
	if (resizable)
		style |= WS_THICKFRAME;
	else
		style &= ~WS_THICKFRAME;
	SetWindowLong(hwnd, GWL_STYLE, style);
#endif
}

/**
* \fn SDL_Texture* loadFromRenderedText(SDL_Renderer* pRenderer, char* textureText, SDL_Color textColor, int *w, int *h, int size)
* \brief Create a texture filled with a text.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] textureText, string.
* \param[in] textColor, color to apply to the text.
* \param[in] w, pointer to the width to be filled with the width of the text.
* \param[in] h, pointer to the hight to be filled with the hight of the text.
* \returns void.
*/
SDL_Texture* loadFromRenderedText(SDL_Renderer* pRenderer, char* textureText, SDL_Color textColor, int *w, int *h, int size)
{
	SDL_Texture* mTexture = NULL;
	//Render text surface
	int sizeFont = size, hRender;
	SDL_GetRendererOutputSize(pRenderer, NULL, &hRender);
	sizeFont = (int)((float)(sizeFont / 600.0) * hRender);
	TTF_Font* font = TTF_OpenFont("../assets/fonts/Roboto-Regular.ttf", sizeFont);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText, textColor);
	if (textSurface == NULL)
	{
		fprintf(logFile, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
		if (mTexture == NULL)
		{
			fprintf(logFile, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		if (w != NULL)
			*w = textSurface->w;
		if (h != NULL)
			*h = textSurface->h;
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	TTF_CloseFont(font);
	//Return success
	return mTexture;
}

/**
* \fn int testEndInput(char* text, int size)
* \brief Test if the non null caracter is in the string.
*
* \param[in] text, string.
* \param[in] size, size of the string.
* \returns 1 = string if ended with '\0', 0 = string is not ended.
*/
int testEndInput(char* text, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (text[i] == '\0')
			return 1;
	}
	return 0;
}

/**
* \fn void getSizeWindow(int *w, int *h, char* str)
* \brief Calculate the widht and the hight from a string.
*
* \param[in] w, pointer to the width to be filled.
* \param[in] h, pointer to the hight to be filled.
* \param[in] str, string.
* \returns void.
*/
void getSizeWindow(int *w, int *h, char* str)
{
	char wString[10];
	char hString[10];
	int i, j;
	if (strcmp(str, "") != 0)
	{
		for (j = 0; j < 20; j++)
		{
			if (str[j] == '*')
				break;
			else wString[j] = str[j];
		}
		wString[j] = '\0';
		if (str[j] != '*')
			return;
		for (i = j + 1; i < 20; i++)
		{
			if (str[i] == '\0')
				break;
			else hString[i - (j + 1)] = str[i];
		}
		hString[i - (j + 1)] = '\0';
		*w = strtol(wString, (char **)NULL, 10);
		*h = strtol(hString, (char **)NULL, 10);
	}
}


void setColorForGame()
{
	int i;
	for (i = 0; i < 4; i++)
		SWAP(globalVar.colorTab[i].b, globalVar.colorTab[i].r);
}