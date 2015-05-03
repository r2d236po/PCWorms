#include "MainMenu.h"
#include "input.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"
#include "Sounds.h"

int mainMenu(Input* pInput, char mapName[100])
{
	SDL_Texture *menuTexture[NBTEXTURE];
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	enum MENU menuIn = MAIN, menuPrec = MAIN;
	int quitMenu = 0, indexTeam = 1;
	enum CHOICE nextPrev = NEITHER;
	enum CHOICE resizable = YES, windowSize = DEFAULT, music = YES, sound = YES, savePath = DEFAULT;
	if (initTabTextureMenu(menuTexture, MAINMENU, MAINMENUVERSUS, MAINMENUMAP, MAINMENUOPTION,
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

	if (pInput->musicAllowed)
	{
		if (loadSounds(ExploSourde, 1))
		{
			/*if (Mix_PlayMusic(ptrMusic, -1) == -1)
						{
						fprintf(logFile, "Bug playsound\n");
						}*/
		}

	}
	while (!quitMenu)
	{
		getInput(pInput);

		switch (menuIn)
		{
		case MAINversus:
		case MAINmap:
		case MAINoption:
		case MAIN:
			menuIn = menu(pInput);
			break;
		case MAPmain:
		case MAPchoose:
		case MAPrepertory:
		case MAP:
			menuIn = mapMenu(pInput, &nextPrev);
			break;
		case OPTIONSm:
		case OPTIONS:
			menuIn = optionMenu(pInput);
			break;
		case VERSUSn:
		case VERSUSm:
		case VERSUSname:
		case VERSUSnameN:
		case VERSUSnameM:
		case VERSUSstart:
		case VERSUSstartS:
		case VERSUS:
			menuIn = versusMenu(pInput, &quitMenu, menuIn, &indexTeam);
			break;
		}
		if (menuIn == MAIN)
			indexTeam = 1;
		if (pInput->raffraichissement == 1)
		{
			SDL_RenderCopy(globalRenderer, menuTexture[indiceTexture(menuIn)], NULL, NULL);
			if (menuIn == MAP || menuIn == MAPmain || menuIn == MAPchoose || menuIn == MAPrepertory
				|| menuIn == MAIN && nextPrev == CLICK)
			{
				mapSketch(nextPrev, mapName);
				nextPrev = NEITHER;
			}
			if (menuIn == OPTIONS || menuIn == OPTIONSm)
			{
				setResizableOption(pInput, &resizable);
				setMusicOption(pInput, &music);
				setSoundOption(pInput, &sound);
				setSizeOption(pInput, &windowSize);
				setSavePathOption(pInput, &savePath, windowSize);
			}
			if (menuIn == VERSUS || menuIn == VERSUSn || menuIn == VERSUSm)
			{
				setColorTeam(pInput);
				setTeamName(pInput);
			}
			if (menuIn == VERSUSname || menuIn == VERSUSnameN || menuIn == VERSUSnameM
				|| menuIn == VERSUSstart || menuIn == VERSUSstartS)
			{
				setWormsName(pInput, indexTeam);
			}
			SDL_RenderPresent(globalRenderer);
			pInput->raffraichissement = 0;
		}
		menuPrec = menuIn;
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;
		if (quitMenu == 0)
			quitMenu = pInput->quit;
		if (quitMenu == 1 && !pInput->quit)
		{
			/*Calcul du nombre d'�quipe et du nombre de worms par �quipe*/
			for (i = 0; i < 4; i++)
			{
				globalVar.nbWormsEquipe[i] = 0;
			}
			globalVar.nbEquipe = 0;
			globalVar.nbWormsTotal = 0;

			for (i = 0; i < 4; i++)
			{
				if (strcmp(globalVar.teamNames[i], "") != 0)
					globalVar.nbEquipe += 1;
			}
			for (i = 0; i < 16; i++)
			{
				if (strcmp(globalVar.wormsNames[i], "") != 0 && strcmp(globalVar.teamNames[(int)(i / 4)], "") != 0)
					globalVar.nbWormsEquipe[(int)(i / 4)] += 1;
			}
			for (i = 0; i < 4; i++)
			{
				globalVar.nbWormsTotal += globalVar.nbWormsEquipe[i];
			}
			if (globalVar.nbWormsTotal == 0 || globalVar.nbEquipe == 0)
			{
				quitMenu = 0;
				menuIn = MAIN;
				indexTeam = 1;
			}
		}
	}


	SDL_StopTextInput();
	resetStructInput(pInput);
	destroyTextureTab(menuTexture);
	setColorForGame();
	return 0;
}

/**
* \fn enum MENU menu(Input* pInput)
* \brief Manages the main menu.
*
* \param[in] pInput, pointer to the input structure.
* \returns which menu to display.
*/
enum MENU menu(Input* pInput)
{
	SDL_Rect versusRect, optionRect, mapRect;
	int testChange = 0;
	static int alreadyChange = 0;

	versusRect = initButtonBox(33, 893, 470, 158);
	mapRect = initButtonBox(758, 893, 470, 158);
	optionRect = initButtonBox(1479, 893, 470, 158);

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
* \fn enum MENU versusMenu(Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam)
* \brief Manages the versus menu.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] quit, pointer to the quit variable to start the game.
* \param[in] menuPrec, precedent menu.
* \returns which menu to display.
*/
enum MENU versusMenu(Input* pInput, int* quit, enum MENU menuPrec, int *pIndexTeam)
{
	SDL_Rect nextRect, mainRect, startRect;
	int testChange = 0, start = 0, next = 0;
	static int alreadyChange = 0;

	start = !strcmp(globalVar.teamNames[*pIndexTeam - 1], "");
	next = strcmp(globalVar.teamNames[*pIndexTeam], "");

	if (menuPrec != VERSUSstart && menuPrec != VERSUSstartS)
	{
		mainRect = initButtonBox(48, 965, 324, 121);
		nextRect = initButtonBox(1625, 929, 328, 198);
		if (collisionPointWithRect(pInput->cursor.now, &nextRect))
		{
			if (pInput->lclick)
			{
				pInput->lclick = 0;
				pInput->raffraichissement = 1;
				alreadyChange = 0;
				strcpy(pInput->textInput, "");
				if (!start && next && (menuPrec == VERSUSn || menuPrec == VERSUS))
				{
					return VERSUSname;
				}
				else if (next && (menuPrec == VERSUSnameN || menuPrec == VERSUSname) && (*pIndexTeam) < 3)
				{
					*pIndexTeam += 1;
					next = strcmp(globalVar.teamNames[*pIndexTeam], "");
					if (next)
						return VERSUSname;
					else return VERSUSstart;
				}
				else if (menuPrec == VERSUSnameN || menuPrec == VERSUSname || (menuPrec == VERSUSn || menuPrec == VERSUS) && !start)
				{
					if (next)
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
		startRect = initButtonBox(1450, 917, 470, 158);
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
* \fn void setColorTeam(Input* pInput)
* \brief Manages the color of a team.
*
* \param[in] pInput, pointer to the input structure.
* \returns void.
*/
void setColorTeam(Input* pInput)
{
	int indexColor = 0, indexTeam = 0, indexGlobal = 0;
	static SDL_Color colorTab[6];
	static int init = 0;

	if (!init)
	{
		init = 1;
		initColorTab(colorTab);
	}

	indexGlobal = getIndexColor(pInput);
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
* \fn int getIndexColor(Input* pInput)
* \brief Determinate the color and the to apply the color.
*
* \param[in] pInput, pointer to the input structure.
* \returns index to the color and the team corresponding.
*/
int getIndexColor(Input* pInput)
{
	int xBox = 1243, yBox = 237, wBox = 63, hBox = 60;
	int xNom = xBox, yNom = yBox, wRender, hRender;
	int indexColor, indexTeam;
	static int colorPrec = 0;
	SDL_Rect colorRect;

	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
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
* \fn void setTeamName(Input* pInput)
* \brief Writes the team names.
*
* \param[in] pInput, pointer to the input structure.
* \returns void.
*/
void setTeamName(Input* pInput)
{
	int indexPrec = 0, i, y = 0;
	static int indexTeam = 0;

	indexPrec = indexTeam;
	indexTeam = getTeamIndexText(pInput);
	if (indexTeam > 1)
	{
		for (i = 1; i < indexTeam; i++)
		{
			if (strcmp(globalVar.teamNames[i - 1], "") == 0)
			{
				indexTeam = 0;
				break;
			}
		}
	}
	if (indexTeam != 0)
		setTextInput(pInput, globalVar.teamNames[indexTeam - 1], indexPrec, indexTeam);
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(globalVar.teamNames[i], 647, y, 16, globalVar.colorTab[i]);
	}
}

/**
* \fn void setWormsName(Input* pInput, int indexTeam)
* \brief Writes the team names.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] indexTeam, index of the team.
* \returns void.
*/
void setWormsName(Input* pInput, int indexTeam)
{
	int indexPrec = 0, i, y = 0, init = 0, indexTest;
	static int indexWorms = 0, team = 1;
	char strTitre[45];
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	sprintf(strTitre, "Choix des noms des joueurs de l'�quipe %d :", indexTeam);
	renderText(strTitre, 470, 32, 32, color);
	if (team != indexTeam)
		init = 1;
	else init = 0;
	indexPrec = indexWorms;
	indexWorms = getWormsIndexText(pInput, init);
	if (indexWorms > 1)
	{
		indexTest = indexWorms + (indexTeam - 1) * 4;
		for (i = (indexTeam - 1) * 4; i < indexTest - 1; i++)
		{
			if (strcmp(globalVar.wormsNames[i], "") == 0)
			{
				indexWorms = 0;
				break;
			}
		}
	}
	if (indexWorms != 0 && strcmp(globalVar.teamNames[indexTeam - 1], "") != 0)
	{
		indexWorms = indexWorms + (indexTeam - 1) * 4;
		setTextInput(pInput, globalVar.wormsNames[indexWorms - 1], indexPrec, indexWorms);
	}
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(globalVar.wormsNames[i + (indexTeam - 1) * 4], 920, y, 16, globalVar.colorTab[(indexTeam - 1)]);
	}
	team = indexTeam;
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
* \fn int getTeamIndexText(Input* pInput)
* \brief Determine what team to write to.
*
* \param[in] pInput, pointer to the input structure.
* \returns index of the team.
*/
int getTeamIndexText(Input* pInput)
{
	return getIndexText(pInput, 647, 0);
}

/**
* \fn int getWormsIndexText(Input* pInput, int init)
* \brief Determine what team to write to.
*
* \param[in] pInput, pointer to the input structure.
* \returns index of the worms.
*/
int getWormsIndexText(Input* pInput, int init)
{
	return getIndexText(pInput, 920, init);
}

/**
* \fn int getIndexText(Input* pInput, int xBox, int init)
* \brief Determine what team to write to.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] xBox, origin of the text.
* \returns index of the box text.
*/
int getIndexText(Input* pInput, int xBox, int init)
{
	int xText = xBox, yText = 225, wText = 533, hText = 87, yNom = 225;
	int wRender, hRender, index;
	static int indexPrec = 0;
	SDL_Rect textRect;
	if (init)
		indexPrec = 0;
	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
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
* \fn enum MENU mapMenu(Input* pInput, enum CHOICE *nextPrev)
* \brief Manages the map menu.
*.
* \param[in] pInput, pointer to the input structure.
* \param[in] nextPrev, pointer to a choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns which menu to display.
*/
enum MENU mapMenu(Input* pInput, enum CHOICE *nextPrev)
{
	SDL_Rect mainMenuRect, nextRect, previousRect, chooseRect, repertoryRect;
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(48, 966, 324, 121);
	repertoryRect = initButtonBox(1629, 966, 324, 121);
	chooseRect = initButtonBox(845, 927, 324, 121);
	nextRect = initButtonBox(1245, 892, 211, 194);
	previousRect = initButtonBox(545, 892, 211, 194);

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
* \fn void  mapSketch(enum CHOICE nextPrev, char* mapName)
* \brief Displays a sketch of the selected map.
*
* \param[in] nextPrev, choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns string of the selected map path.
*/
void mapSketch(enum CHOICE nextPrev, char* mapName)
{
	SDL_Rect sketchRect;
	SDL_Texture* sketchTexture = NULL;
	static int counter = 0;
	int x = 182, y = 57, w = 1638, h = 740, wRender = 0, hRender = 0;
	float coeffX = (float)(x / WIDTHMENUTEXTURE), coeffY = (float)(y / HIGHTMENUTEXTURE), coeffW = (float)(w / WIDTHMENUTEXTURE), coeffH = (float)(h / HIGHTMENUTEXTURE);
	static char *mapString[NUMBERMAP] = { cMAP, cMAP_HD, cMAP_TEST, cMAP_TEST2, cMAP_TEST3, cMAP_TEST4, cMAP_CAM };

	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
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
	sketchTexture = loadTexture(mapString[counter]);
	if (sketchTexture == NULL)
	{
		fprintf(logFile, "mapSketch : FAILURE, loadTexture sketchTexture.\n\n");
		nextPrev = NEITHER;
		return;
	}
	SDL_RenderCopy(globalRenderer, sketchTexture, NULL, &sketchRect);
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
* \fn char* stringTexture(enum MENU menu)
* \brief Convert the menu name into an int.
*
* \param[in] menu, menu enumeration.
* \returns the string of the texture to display.
*/
char* stringTexture(enum MENU menu)
{
	switch (menu)
	{
	case MAIN:
		return MAINMENU;
	case MAINversus:
		return MAINMENUVERSUS;
	case MAINmap:
		return MAINMENUMAP;
	case MAINoption:
		return MAINMENUOPTION;
	case VERSUS:
		return VERSUSMENU;
	case VERSUSn:
		return VERSUSMENUNEXT;
	case VERSUSm:
		return VERSUSMENUMAIN;
	case VERSUSname:
		return VERSUSMENUNAMEPLAYER;
	case VERSUSnameN:
		return VERSUSMENUNAMEPLAYERN;
	case VERSUSnameM:
		return VERSUSMENUNAMEPLAYERM;
	case VERSUSstart:
		return VERSUSMENUSTART;
	case VERSUSstartS:
		return VERSUSMENUSTARTS;
	case MAP:
		return MAPMENU;
	case MAPmain:
		return MAPMENUMAIN;
	case MAPchoose:
		return MAPMENUCHOOSE;
	case MAPrepertory:
		return MAPMENUREPERTORY;
	case OPTIONS:
		return OPTIONMENU;
	case OPTIONSm:
		return OPTIONMENUMAIN;
	}
	return MAINMENU;
}

/**
* \fn int initTabTextureMenu(SDL_Texture* menuTexture[NBTEXTURE], ...)
* \brief Initialiaze the array of texture for the menu.
*
* \param[in] menuTexture, array of texture.
* \returns 0 = SUCCESS, -1 = ERROR.
* \remarks the addition parameters are the strings of the image to load.
*/
int initTabTextureMenu(SDL_Texture* menuTexture[NBTEXTURE], ...)
{
	int indexTexture = 0;
	va_list list;
	va_start(list, menuTexture);
	for (indexTexture = 0; indexTexture < NBTEXTURE; indexTexture++)
	{
		menuTexture[indexTexture] = loadTexture(va_arg(list, char*));
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
* \fn enum MENU optionMenu(Input* pInput)
* \brief Manages the option menu.
*
* \param[in] pInput, pointer to the input structure.
* \returns which menu to display.
*/
enum MENU optionMenu(Input* pInput)
{
	SDL_Rect mainMenuRect;
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(44, 965, 324, 121);

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
* \fn void setResizableOption(Input* pInput, enum CHOICE *pChoice)
* \brief Set the resizable option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setResizableOption(Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1197, y = 149;
	toggleOptions(pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		SDL_SetWindowResizable(SDL_TRUE);
	else SDL_SetWindowResizable(SDL_FALSE);
}

/**
* \fn void setMusicOption(Input* pInput, enum CHOICE *pChoice)
* \brief Set the music option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setMusicOption(Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1197, y = 434;
	toggleOptions(pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		pInput->musicAllowed = 1;
	else pInput->musicAllowed = 0;
}

/**
* \fn void setSoundOption(Input* pInput, enum CHOICE *pChoice)
* \brief Set the sound option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSoundOption(Input* pInput, enum CHOICE *pChoice)
{
	int xYes = 1199, y = 576;
	toggleOptions(pInput, pChoice, xYes, y);
	if (*pChoice == YES)
		pInput->soundAllowed = 1;
	else pInput->soundAllowed = 0;
}

/**
* \fn void setSizeOption(Input* pInput, enum CHOICE *pChoice)
* \brief Set the size option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSizeOption(Input* pInput, enum CHOICE *pChoice)
{
	static int wWindow, hWindow;
	static char strSize[21] = "1080*600";
	int xYes = 1197, y = 292;
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	toggleOptions(pInput, pChoice, xYes, y);
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
			SDL_SetWindowSize(globalWindow, wWindow, hWindow);
			pInput->textCounter = 0;
		}
	}
	else if (*pChoice == DEFAULT && testChange != *pChoice)
	{
		wWindow = 1080;
		hWindow = 600;
		strcpy(strSize, "1080*600");
		SDL_SetWindowSize(globalWindow, wWindow, hWindow);
	}
	else
	{
		SDL_GetWindowSize(globalWindow, &wWindow, &hWindow);
		sprintf(strSize, "%d*%d", wWindow, hWindow);
	}
	renderText(strSize, 1493, 292, 16, color);
}

/**
* \fn void setSavePathOption(Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize)
* \brief Set the save path option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSavePathOption(Input* pInput, enum CHOICE *pChoice, enum CHOICE windowSize)
{
	int xYes = 1197, y = 719;
	static char strPath[100] = "";
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	toggleOptions(pInput, pChoice, xYes, y);
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
	renderText(strPath, 1493, 719, 16, color);
}

/**
* \fn void toggleOptions(Input* pInput, enum CHOICE *pChoice, int x, int y)
* \brief Toggle an option.
*
* \param[in] pInput, pointer to the input structure.
* \param[in] pChoice, pointer to a choice variable.
* \param[in] x, x coordinnate of the first option box.
* \param[in] y, y coordinnate of the first option box.
* \returns void.
*/
void toggleOptions(Input* pInput, enum CHOICE *pChoice, int x, int y)
{
	SDL_Texture* textureChoice = NULL;
	SDL_Rect yesRect, noRect, textRect, setRect;

	yesRect = initButtonBox(x, y, 172, 88);
	noRect = initButtonBox(1493, y, 172, 88);
	textRect = initButtonBox(1493, y, 386, 88);
	setRect = initButtonBox(1846, y, 107, 88);

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
		textureChoice = loadTexture(YESTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(globalRenderer, textureChoice, NULL, &yesRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice == DEFAULT)
	{
		textureChoice = loadTexture(DEFAULTTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(globalRenderer, textureChoice, NULL, &yesRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice == SET)
	{
		textureChoice = loadTexture(SETTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(globalRenderer, textureChoice, NULL, &setRect);
		pInput->raffraichissement = 1;
	}
	else if (*pChoice != NEITHER && *pChoice != TEXT)
	{
		textureChoice = loadTexture(NOTEXTURE);
		if (textureChoice == NULL)
		{
			fprintf(logFile, "setMusicOption : FAILURE, loadTexture.\n\n");
			return;
		}
		else SDL_RenderCopy(globalRenderer, textureChoice, NULL, &noRect);
		pInput->raffraichissement = 1;
	}
	SDL_DestroyTexture(textureChoice);
}

/**
* \fn void SDL_SetWindowResizable(SDL_bool resizable)
* \brief Set the resizable flag of the window.
*
* \param[in] resizable, activation of the resizable possibility, TRUE = resizable.
* \returns void
*/
void SDL_SetWindowResizable(SDL_bool resizable)
{
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(globalWindow, &info);

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
* \fn SDL_Texture* loadFromRenderedText(char* textureText, SDL_Color textColor, int *w, int *h, int size)
* \brief Create a texture filled with a text.
*
* \param[in] textureText, string.
* \param[in] textColor, color to apply to the text.
* \param[in] w, pointer to the width to be filled with the width of the text.
* \param[in] h, pointer to the hight to be filled with the hight of the text.
* \param[in] size, size of the font.
* \returns void.
*/
SDL_Texture* loadFromRenderedText(char* textureText, SDL_Color textColor, int *w, int *h, int size)
{
	SDL_Texture* mTexture = NULL;
	//Render text surface
	int sizeFont = size, hRender;
	SDL_GetRendererOutputSize(globalRenderer, NULL, &hRender);
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
		mTexture = SDL_CreateTextureFromSurface(globalRenderer, textSurface);
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
* \fn void renderText(char* str, int x, int y, int sizeFont, SDL_Color color)
* \brief Render a text from a screen and paste it to the renderer.
*
* \param[in] str, string.
* \param[in] x, x position of the text in the renderer.
* \param[in] y, y position of the text in the renderer.
* \param[in] sizeFont, size of the font.
* \param[in] color, color to apply to the text.
* \returns void.
*/
void renderText(char* str, int x, int y, int sizeFont, SDL_Color color)
{
	int hText = 87, wRender, hRender, wText = 0;
	SDL_Rect rect;

	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
	x = (int)((float)(x / WIDTHMENUTEXTURE) * wRender);
	y = (int)((float)(y / HIGHTMENUTEXTURE) * hRender) + (int)((float)(0.3 * hText / HIGHTMENUTEXTURE) * hRender);
	SDL_Texture* text = loadFromRenderedText(str, color, &wText, &hText, sizeFont);
	rect = initRect(x, y, wText, hText);
	SDL_RenderCopy(globalRenderer, text, NULL, &rect);
	SDL_DestroyTexture(text);
}

/**
* \fn SDL_Rect initButtonBox(int x, int y, int w, int h)
* \brief Initialize a button box from the standard dimensions in the image to the relative dimensions in the renderer.
*
* \param[in] x, x position of the original box.
* \param[in] y, y position of the original box.
* \param[in] w, width of the original box.
* \param[in] h, hight of the original box.
* \returns void.
*/
SDL_Rect initButtonBox(int x, int y, int w, int h)
{
	int wRender, hRender;

	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
	w = (int)((float)(w / WIDTHMENUTEXTURE) * wRender);
	h = (int)((float)(h / HIGHTMENUTEXTURE) * hRender);
	if (x >= 0)
		x = (int)((float)(x / WIDTHMENUTEXTURE) * wRender);
	else x = wRender / 2 - w / 2;
	if (y >= 0)
		y = (int)((float)(y / HIGHTMENUTEXTURE) * hRender);
	else y = hRender / 2 - h / 2;
	return initRect(x, y, w, h);
}