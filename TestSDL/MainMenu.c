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

	SDL_StartTextInput();

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
			if (menuIn == MAP || menuIn == MAPmain || menuIn == MAPchoose || menuIn == MAPrepertory
				|| menuIn == MAIN && nextPrev == CLICK)
			{
				mapSketch(pRenderer, nextPrev, mapName);
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
		if (quitMenu == 1 && !pInput->quit)
		{
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
			{
				quitMenu = 0;
				menuIn = MAIN;
				indexTeam = 1;
			}
			if (*pNbWorms != 0)
				*pNbWorms = (*pNbWorms) / (*pNbTeam);
				
		}
	}

	
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
	int testChange = 0;
	static int alreadyChange = 0;

	versusRect = initButtonBox(pRenderer, 33, 893, 470, 158);
	mapRect = initButtonBox(pRenderer, 758, 893, 470, 158);
	optionRect = initButtonBox(pRenderer, 1479, 893, 470, 158);

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
	int testChange = 0;
	static int alreadyChange = 0;

	if (menuPrec != VERSUSstart && menuPrec != VERSUSstartS)
	{
		mainRect = initButtonBox(pRenderer, 48, 965, 324, 121);
		nextRect = initButtonBox(pRenderer, 1625, 929, 328, 198);
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
		startRect = initButtonBox(pRenderer, 1450, 917, 470, 158);
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
	int indexColor = 0, indexTeam = 0, indexGlobal = 0;
	static SDL_Color colorTab[6];
	static int init = 0;

	if (!init)
	{
		init = 1;
		initColorTab(colorTab);
	}

	indexGlobal = getIndexColor(pRenderer, pInput);
	indexTeam = indexGlobal / 6;
	indexColor = indexGlobal % 6;
	globalVar.colorTab[indexTeam] = colorTab[indexColor];
}

/**
* \fn void initColorTab(SDL_Color colorArray[6])
* \brief Init the array of color.
*
* \param[in] colorArray, array of color.
* \returns void.
*/
void initColorTab(SDL_Color colorArray[6])
{
	setSDLColor(&colorArray[0], 134, 0, 0);
	colorArray[0].a = 255;
	setSDLColor(&colorArray[1], 0, 176, 80);
	colorArray[1].a = 255;
	setSDLColor(&colorArray[2], 255, 192, 0);
	colorArray[2].a = 255;
	setSDLColor(&colorArray[3], 112, 48, 160);
	colorArray[3].a = 255;
	setSDLColor(&colorArray[4], 0, 32, 96);
	colorArray[4].a = 255;
	setSDLColor(&colorArray[5], 237, 125, 49);
	colorArray[5].a = 255;
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
	wBox = (int)((float)(wBox / WIDTHMENUTEXTURE) * wRender);
	hBox = (int)((float)(hBox / HIGHTMENUTEXTURE) * hRender);

	for (indexTeam = 1; indexTeam <= 4; indexTeam++)
	{
		yBox = yNom + (indexTeam - 1) * 164;
		yBox = (int)((float)(yBox / HIGHTMENUTEXTURE) * hRender);
		for (indexColor = 0; indexColor < 6; indexColor++)
		{
			xBox = xNom + indexColor * 130;
			xBox = (int)((float)(xBox / WIDTHMENUTEXTURE) * wRender);
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
	int indexPrec = 0, i, y = 0;
	static int indexTeam = 0;

	indexPrec = indexTeam;
	indexTeam = getTeamIndexText(pRenderer, pInput);
	if (indexTeam != 0)
		setTextInput(pInput, globalVar.teamNames[indexTeam - 1], indexPrec, indexTeam);
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(pRenderer, globalVar.teamNames[i], 647, y, 16, globalVar.colorTab[i]);
	}
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
	int indexPrec = 0, i, y = 0;
	static int indexWorms = 0;
	char strTitre[45];
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	sprintf(strTitre, "Choix des noms des joueurs de l'équipe %d :", indexTeam);
	renderText(pRenderer, strTitre, 470, 32, 32, color);

	indexPrec = indexWorms;
	indexWorms = getWormsIndexText(pRenderer, pInput);
	if (indexWorms != 0)
	{
		indexWorms = indexWorms + (indexTeam - 1) * 4;
		setTextInput(pInput, globalVar.wormsNames[indexWorms - 1], indexPrec, indexWorms);
	}
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(pRenderer, globalVar.wormsNames[i + (indexTeam - 1) * 4], 920, y, 16, globalVar.colorTab[(indexTeam - 1)]);
	}
}

/**
* \fn void setTextInput(Input* pInput, char* str, int indexPrec, int indexNow)
* \brief Text input function. Manages the precdent writing, deleting, etc.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] str, the string to write in.
* \param[in] indexPrec, precedent index.
* \param[in] indexNow, index now.
* \returns void.
*/
void setTextInput(Input* pInput, char* str, int indexPrec, int indexNow)
{
	if (indexPrec == indexNow && strcmp(pInput->textInput, "") == 0 && strlen(str) == 1)
	{
		strcpy(str, pInput->textInput);
		pInput->textCounter = 0;
	}
	else if (indexPrec != indexNow || strcmp(pInput->textInput, "") == 0)
	{
		strcpy(pInput->textInput, str);
		pInput->textCounter = (char)strlen(str);
	}
	secuTextInput(pInput);
	strcpy(str, pInput->textInput);
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
	wText = (int)((float)(wText / WIDTHMENUTEXTURE) * wRender);
	hText = (int)((float)(hText / HIGHTMENUTEXTURE) * hRender);
	xText = (int)((float)(xText / WIDTHMENUTEXTURE) * wRender);

	for (index = 1; index <= 4; index++)
	{
		yText = (int)((float)(yText / HIGHTMENUTEXTURE) * hRender);
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
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(pRenderer, 48, 966, 324, 121);
	repertoryRect = initButtonBox(pRenderer, 1629, 966, 324, 121);
	chooseRect = initButtonBox(pRenderer, 845, 927, 324, 121);
	nextRect = initButtonBox(pRenderer, 1245, 892, 211, 194);
	previousRect = initButtonBox(pRenderer, 545, 892, 211, 194);

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
			*nextPrev = CLICK;
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
* \fn void  mapSketch(SDL_Renderer* pRenderer, enum CHOICE nextPrev, char* mapName)
* \brief Displays a sketch of the selected map.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] nextPrev, choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns string of the selected map path.
*/
void mapSketch(SDL_Renderer* pRenderer, enum CHOICE nextPrev, char* mapName)
{
	SDL_Rect sketchRect;
	SDL_Texture* sketchTexture = NULL;
	static int counter = 0;
	int x = 182, y = 57, w = 1638, h = 740, wRender = 0, hRender = 0;
	float coeffX = (float)(x / WIDTHMENUTEXTURE), coeffY = (float)(y / HIGHTMENUTEXTURE), coeffW = (float)(w / WIDTHMENUTEXTURE), coeffH = (float)(h / HIGHTMENUTEXTURE);
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
		return;
	}
	SDL_RenderCopy(pRenderer, sketchTexture, NULL, &sketchRect);
	SDL_DestroyTexture(sketchTexture);
	if (nextPrev == CLICK)
		strcpy(mapName, mapString[counter]);
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
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(pRenderer, 44, 965, 324, 121);

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
	static int wWindow, hWindow;
	static char strSize[21] = "1080*600";
	int xYes = 1197, y = 292;
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	toggleOptions(pRenderer, pInput, pChoice, xYes, y);
	if (testChange != *pChoice)
	{
		strcpy(pInput->textInput, "");
		pInput->textCounter = 0;
	}
	if (*pChoice == TEXT)
	{
		if (strcmp(pInput->textInput, "") == 0 && strlen(strSize) > 1)
		{
			strcpy(pInput->textInput, strSize);
			pInput->textCounter = (char)strlen(strSize);
		}
		strcpy(strSize, pInput->textInput);
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
	else
	{
		SDL_GetWindowSize(pWindow, &wWindow, &hWindow);
		sprintf(strSize, "%d*%d", wWindow, hWindow);
	}
	renderText(pRenderer, strSize, 1493, 292, 16, color);
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
	int xYes = 1197, y = 719;
	static char strPath[100] = "";
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

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
		if (strcmp(pInput->textInput, "") == 0 && strlen(strPath) > 1)
		{
			strcpy(pInput->textInput, strPath);
			pInput->textCounter = (char)strlen(strPath);
		}
		strcpy(strPath, pInput->textInput);
		strcpy(globalVar.savePath, pInput->textInput);
	}
	renderText(pRenderer, strPath, 1493, 719, 16, color);
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

	yesRect = initButtonBox(pRenderer, x, y, 172, 88);
	noRect = initButtonBox(pRenderer, 1493, y, 172, 88);
	textRect = initButtonBox(pRenderer, 1493, y, 386, 88);
	setRect = initButtonBox(pRenderer, 1846, y, 107, 88);

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
* \param[in] size, size of the font.
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

/**
* \fn void setColorForGame()
* \brief Swap blue and red component for the game.
*
* \returns void.
*/
void setColorForGame()
{
	int i;
	for (i = 0; i < 4; i++)
		SWAP(globalVar.colorTab[i].b, globalVar.colorTab[i].r);
}

/**
* \fn void renderText(SDL_Renderer* pRenderer, char* str, int x, int y, int sizeFont, SDL_Color color)
* \brief Render a text from a screen and paste it to the renderer.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] str, string.
* \param[in] x, x position of the text in the renderer.
* \param[in] y, y position of the text in the renderer.
* \param[in] sizeFont, size of the font.
* \param[in] color, color to apply to the text.
* \returns void.
*/
void renderText(SDL_Renderer* pRenderer, char* str, int x, int y, int sizeFont, SDL_Color color)
{
	int hText = 87, wRender, hRender, wText = 0;
	SDL_Rect rect;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	x = (int)((float)(x / WIDTHMENUTEXTURE) * wRender);
	y = (int)((float)(y / HIGHTMENUTEXTURE) * hRender) + (int)((float)(0.3 * hText / HIGHTMENUTEXTURE) * hRender);
	SDL_Texture* text = loadFromRenderedText(pRenderer, str, color, &wText, &hText, sizeFont);
	rect = initRect(x, y, wText, hText);
	SDL_RenderCopy(pRenderer, text, NULL, &rect);
	SDL_DestroyTexture(text);
}

/**
* \fn SDL_Rect initButtonBox(SDL_Renderer* pRenderer, int x, int y, int w, int h)
* \brief Initialize a button box from the standard dimensions in the image to the relative dimensions in the renderer.
*
* \param[in] pRenderer, pointer to the renderer of the window.
* \param[in] x, x position of the original box.
* \param[in] y, y position of the original box.
* \param[in] w, width of the original box.
* \param[in] h, hight of the original box.
* \returns void.
*/
SDL_Rect initButtonBox(SDL_Renderer* pRenderer, int x, int y, int w, int h)
{
	int wRender, hRender;

	SDL_GetRendererOutputSize(pRenderer, &wRender, &hRender);
	y = (int)((float)(y / HIGHTMENUTEXTURE) * hRender);
	w = (int)((float)(w / WIDTHMENUTEXTURE) * wRender);
	h = (int)((float)(h / HIGHTMENUTEXTURE) * hRender);
	x = (int)((float)(x / WIDTHMENUTEXTURE) * wRender);
	return initRect(x, y, w, h);
}