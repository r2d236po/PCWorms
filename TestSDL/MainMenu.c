#include "MainMenu.h"
#include "input.h"
#include "my_stdrFct.h"
#include "AffichageGeneral.h"
#include "Sounds.h"
#include "HUD.h"
#include "partie.h"
#include "memory.h"

int NBMAP = 0;

int mainMenu(char mapName[100])
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
	char **menuTab = initMAPstrings();
	if (menuTab == NULL)
		return -1;
	playMusique(globalInput->musicAllowed, MusiqueMenu);


	while (!quitMenu)
	{
		getInput();

		switch (menuIn)
		{
		case MAINversus:
		case MAINmap:
		case MAINoption:
		case MAIN:
			menuIn = menu();
			break;
		case MAPmain:
		case MAPchoose:
		case MAPrepertory:
		case MAP:
			menuIn = mapMenu(&nextPrev);
			break;
		case OPTIONSm:
		case OPTIONS:
			menuIn = optionMenu();
			break;
		case VERSUSn:
		case VERSUSm:
		case VERSUSname:
		case VERSUSnameN:
		case VERSUSnameM:
		case VERSUSstart:
		case VERSUSstartS:
		case VERSUS:
			menuIn = versusMenu(&quitMenu, menuIn, &indexTeam);
			break;
		}
		if (menuIn == MAIN)
			indexTeam = 1;
		if (globalInput->raffraichissement == 1)
		{
			SDL_RenderCopy(globalRenderer, menuTexture[indiceTexture(menuIn)], NULL, NULL);
			if (menuIn == MAP || menuIn == MAPmain || menuIn == MAPchoose || menuIn == MAPrepertory
				|| menuIn == MAIN && nextPrev == CLICK)
			{
				mapSketch(nextPrev, mapName, menuTab);
				nextPrev = NEITHER;
			}
			if (menuIn == OPTIONS || menuIn == OPTIONSm)
			{
				setResizableOption(&resizable);
				setMusicOption(&music);
				setSoundOption(&sound);
				setSizeOption(&windowSize);
				setSavePathOption(&savePath, windowSize);
			}
			if (menuIn == VERSUS || menuIn == VERSUSn || menuIn == VERSUSm)
			{
				setColorTeam();
				setTeamName();
			}
			if (menuIn == VERSUSname || menuIn == VERSUSnameN || menuIn == VERSUSnameM
				|| menuIn == VERSUSstart || menuIn == VERSUSstartS)
			{
				setWormsName(indexTeam);
			}
			SDL_RenderPresent(globalRenderer);
			globalInput->raffraichissement = 0;
		}
		menuPrec = menuIn;
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;
		if (quitMenu == 0)
			quitMenu = globalInput->quit;
		if (quitMenu == 1 && !globalInput->quit)
		{
			/*Calcul du nombre d'équipe et du nombre de worms par équipe*/
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
	if (menuTab != NULL)
		destroyMenuTab(menuTab, NBMAP);
	SDL_StopTextInput();
	resetStructInput();
	destroyTextureTab(menuTexture);
	setColorForGame();
	return 0;
}

/**
* \fn enum MENU menu()
* \brief Manages the main menu.
*
* \returns which menu to display.
*/
enum MENU menu()
{
	SDL_Rect versusRect, optionRect, mapRect;
	int testChange = 0;
	static int alreadyChange = 0;

	versusRect = initButtonBox(33, 893, 470, 158);
	mapRect = initButtonBox(758, 893, 470, 158);
	optionRect = initButtonBox(1479, 893, 470, 158);

	if (collisionPointWithRect(globalInput->cursor.now, &versusRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			alreadyChange = 0;
			strcpy(globalInput->textInput, "");
			return VERSUS;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAINversus;
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &mapRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			alreadyChange = 0;
			return MAP;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAINmap;
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &optionRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			alreadyChange = 0;
			return OPTIONS;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAINoption;
	}
	else
	{
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			globalInput->raffraichissement = 1;
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
* \fn enum MENU versusMenu(int* quit, enum MENU menuPrec, int *pindexTeam)
* \brief Manages the versus menu.
*
* \param[in] quit, pointer to the quit variable to start the game.
* \param[in] menuPrec, precedent menu.
* \returns which menu to display.
*/
enum MENU versusMenu(int* quit, enum MENU menuPrec, int *pindexTeam)
{
	SDL_Rect nextRect, mainRect, startRect;
	int testChange = 0, start = 0, next = 0;
	static int alreadyChange = 0;

	start = !strcmp(globalVar.teamNames[*pindexTeam - 1], "");
	next = strcmp(globalVar.teamNames[*pindexTeam], "");

	if (menuPrec != VERSUSstart && menuPrec != VERSUSstartS)
	{
		mainRect = initButtonBox(48, 965, 324, 121);
		nextRect = initButtonBox(1625, 929, 328, 198);
		if (collisionPointWithRect(globalInput->cursor.now, &nextRect))
		{
			if (globalInput->lclick)
			{
				globalInput->lclick = 0;
				globalInput->raffraichissement = 1;
				alreadyChange = 0;
				strcpy(globalInput->textInput, "");
				if (!start && next && (menuPrec == VERSUSn || menuPrec == VERSUS))
				{
					return VERSUSname;
				}
				else if (next && (menuPrec == VERSUSnameN || menuPrec == VERSUSname) && (*pindexTeam) < 3)
				{
					*pindexTeam += 1;
					next = strcmp(globalVar.teamNames[*pindexTeam], "");
					if (next)
						return VERSUSname;
					else return VERSUSstart;
				}
				else if (menuPrec == VERSUSnameN || menuPrec == VERSUSname || (menuPrec == VERSUSn || menuPrec == VERSUS) && !start)
				{
					if (next)
						*pindexTeam += 1;
					return VERSUSstart;
				}
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				globalInput->raffraichissement = 1;
			}
			if (menuPrec == VERSUS || menuPrec == VERSUSn)
				return VERSUSn;
			else return VERSUSnameN;
		}
		else if (collisionPointWithRect(globalInput->cursor.now, &mainRect))
		{
			if (globalInput->lclick)
			{
				globalInput->lclick = 0;
				globalInput->raffraichissement = 1;
				alreadyChange = 0;
				strcpy(globalInput->textInput, "");
				*pindexTeam = 1;
				return MAIN;
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				globalInput->raffraichissement = 1;
			}
			if (menuPrec == VERSUS || menuPrec == VERSUSm)
				return VERSUSm;
			else return VERSUSnameM;
		}
	}
	else
	{
		startRect = initButtonBox(1450, 917, 470, 158);
		if (collisionPointWithRect(globalInput->cursor.now, &startRect))
		{
			if (globalInput->lclick)
			{
				globalInput->lclick = 0;
				globalInput->raffraichissement = 1;
				alreadyChange = 0;
				*quit = 1;
			}
			else if (!alreadyChange)
			{
				alreadyChange = 1;
				globalInput->raffraichissement = 1;
			}
			return VERSUSstartS;
		}
	}
	*quit = 0;
	testChange = alreadyChange;
	alreadyChange = 0;
	if (testChange != alreadyChange)
		globalInput->raffraichissement = 1;

	if (menuPrec == VERSUSstartS || menuPrec == VERSUSstart)
		return VERSUSstart;
	else if (menuPrec == VERSUSn || menuPrec == VERSUSm || menuPrec == VERSUS)
		return VERSUS;
	else if (menuPrec == VERSUSnameN || menuPrec == VERSUSnameM || menuPrec == VERSUSname)
		return VERSUSname;
	return VERSUS;
}

/**
* \fn void setColorTeam()
* \brief Manages the color of a team.
*
* \returns void.
*/
void setColorTeam()
{
	int indexColor = 0, indexTeam = 0, indexGlobal = 0;
	static SDL_Color colorTab[6];
	static int init = 0;

	if (!init)
	{
		init = 1;
		initColorTab(colorTab);
	}

	indexGlobal = getIndexColor();
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
* \fn int getIndexColor()
* \brief Determinate the color and the to apply the color.
*
* \returns index to the color and the team corresponding.
*/
int getIndexColor()
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
			if (collisionPointWithRect(globalInput->cursor.now, &colorRect))
			{
				if (globalInput->lclick)
				{
					globalInput->lclick = 0;
					colorPrec = indexColor + (indexTeam - 1) * 6;
					return indexColor + (indexTeam - 1) * 6;
				}
			}
		}
	}
	return colorPrec;
}

/**
* \fn void setTeamName()
* \brief Writes the team names.
*
* \returns void.
*/
void setTeamName()
{
	int indexPrec = 0, i, y = 0;
	static int indexTeam = 0;

	indexPrec = indexTeam;
	indexTeam = getTeamIndexText();
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
		setTextInput(globalVar.teamNames[indexTeam - 1], indexPrec, indexTeam);
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(globalVar.teamNames[i], 647, y, 16, globalVar.colorTab[i]);
	}
}

/**
* \fn void setWormsName(int indexTeam)
* \brief Writes the team names.
*
* \param[in] indexTeam, index of the team.
* \returns void.
*/
void setWormsName(int indexTeam)
{
	int indexPrec = 0, i, y = 0, init = 0, indeXTEST;
	static int indexWorms = 0, team = 1;
	char strTitre[45];
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	sprintf(strTitre, "Choix des noms des joueurs de l'équipe %d :", indexTeam);
	renderText(strTitre, 470, 32, 32, color);
	if (team != indexTeam)
		init = 1;
	else init = 0;
	indexPrec = indexWorms;
	indexWorms = getWormsIndexText(init);
	if (indexWorms > 1)
	{
		indeXTEST = indexWorms + (indexTeam - 1) * 4;
		for (i = (indexTeam - 1) * 4; i < indeXTEST - 1; i++)
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
		setTextInput(globalVar.wormsNames[indexWorms - 1], indexPrec, indexWorms);
	}
	for (i = 0; i < 4; i++)
	{
		y = 225 + i * 163;
		renderText(globalVar.wormsNames[i + (indexTeam - 1) * 4], 920, y, 16, globalVar.colorTab[(indexTeam - 1)]);
	}
	team = indexTeam;
}

/**
* \fn void setTextInput(char* str, int indexPrec, int indexNow)
* \brief Text input function. Manages the precdent writing, deleting, etc.
*
* \param[in] str, the string to write in.
* \param[in] indexPrec, precedent index.
* \param[in] indexNow, index now.
* \returns void.
*/
void setTextInput(char* str, int indexPrec, int indexNow)
{
	if (indexPrec == indexNow && strcmp(globalInput->textInput, "") == 0 && strlen(str) == 1)
	{
		strcpy(str, globalInput->textInput);
		globalInput->textCounter = 0;
	}
	else if (indexPrec != indexNow || strcmp(globalInput->textInput, "") == 0)
	{
		strcpy(globalInput->textInput, str);
		globalInput->textCounter = (char)strlen(str);
	}
	secuTextInput();
	strcpy(str, globalInput->textInput);
}

/**
* \fn int getTeamIndexText()
* \brief Determine what team to write to.
*
* \returns index of the team.
*/
int getTeamIndexText()
{
	return getIndexText(647, 0);
}

/**
* \fn int getWormsIndexText(int init)
* \brief Determine what team to write to.
*
* \returns index of the worms.
*/
int getWormsIndexText(int init)
{
	return getIndexText(920, init);
}

/**
* \fn int getIndexText(int xBox, int init)
* \brief Determine what team to write to.
*
* \param[in] xBox, origin of the text.
* \returns index of the box text.
*/
int getIndexText(int xBox, int init)
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
		if (collisionPointWithRect(globalInput->cursor.now, &textRect))
		{
			if (globalInput->lclick)
			{
				globalInput->lclick = 0;
				indexPrec = index;
				return index;
			}
		}
		yText = yNom + (index)* 163;
	}
	if (globalInput->lclick)
	{
		globalInput->lclick = 0;
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
* \fn enum MENU mapMenu(enum CHOICE *nextPrev)
* \brief Manages the map menu.
*.
* \param[in] nextPrev, pointer to a choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns which menu to display.
*/
enum MENU mapMenu(enum CHOICE *nextPrev)
{
	SDL_Rect mainMenuRect, nextRect, previousRect, chooseRect, repertoryRect;
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(48, 966, 324, 121);
	repertoryRect = initButtonBox(1629, 966, 324, 121);
	chooseRect = initButtonBox(845, 927, 324, 121);
	nextRect = initButtonBox(1245, 892, 211, 194);
	previousRect = initButtonBox(545, 892, 211, 194);

	if (collisionPointWithRect(globalInput->cursor.now, &mainMenuRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			alreadyChange = 0;
			globalInput->raffraichissement = 1;
			return MAIN;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAPmain;
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &nextRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			*nextPrev = NEXT;
			globalInput->raffraichissement = 1;
			return MAP;
		}
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &previousRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			*nextPrev = PREVIOUS;
			globalInput->raffraichissement = 1;
			return MAP;
		}
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &chooseRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			alreadyChange = 0;
			globalInput->raffraichissement = 1;
			*nextPrev = CLICK;
			return MAIN;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAPchoose;
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &repertoryRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			globalInput->raffraichissement = 1;
			return MAP;
		}
		else if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return MAPrepertory;
	}
	else
	{
		*nextPrev = NEITHER;
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			globalInput->raffraichissement = 1;
	}
	return MAP;
}

/**
* \fn void  mapSketch(enum CHOICE nextPrev, char* mapName, char **mapString)
* \brief Displays a sketch of the selected map.
*
* \param[in] nextPrev, choice structure to be filled with NEXT or PREVIOUS or NEITHER command.
* \returns string of the selected map path.
*/
void mapSketch(enum CHOICE nextPrev, char* mapName, char **mapString)
{
	SDL_Rect sketchRect;
	SDL_Texture* sketchTexture = NULL;
	static int counter = 0;
	int x = 182, y = 57, w = 1638, h = 740, wRender = 0, hRender = 0;
	float coeffX = (float)(x / WIDTHMENUTEXTURE), coeffY = (float)(y / HIGHTMENUTEXTURE), coeffW = (float)(w / WIDTHMENUTEXTURE), coeffH = (float)(h / HIGHTMENUTEXTURE);

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
	if (counter >= NBMAP)
		counter = 0;
	else if (counter < 0)
		counter = NBMAP - 1;
	if (mapString[counter] != NULL)
		sketchTexture = loadTexture(mapString[counter]);
	if (sketchTexture == NULL)
	{
		fprintf(logFile, "mapSketch : FAILURE, loadTexture sketchTexture.\n\n");
		nextPrev = NEITHER;
		return;
	}
	SDL_RenderCopy(globalRenderer, sketchTexture, NULL, &sketchRect);
	my_freeTexture(sketchTexture);
	if (nextPrev == CLICK && mapString[counter] != NULL)
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
			my_freeTexture(textureTab[indexTexture]);
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
* \fn enum MENU optionMenu()
* \brief Manages the option menu.
*
* \returns which menu to display.
*/
enum MENU optionMenu()
{
	SDL_Rect mainMenuRect;
	int testChange = 0;
	static int alreadyChange = 0;

	mainMenuRect = initButtonBox(44, 965, 324, 121);

	if (collisionPointWithRect(globalInput->cursor.now, &mainMenuRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			globalInput->raffraichissement = 1;
			return MAIN;
		}
		if (!alreadyChange)
		{
			alreadyChange = 1;
			globalInput->raffraichissement = 1;
		}
		return OPTIONSm;
	}
	else
	{
		testChange = alreadyChange;
		alreadyChange = 0;
		if (testChange != alreadyChange)
			globalInput->raffraichissement = 1;
	}
	return OPTIONS;
}

/**
* \fn void setResizableOption(enum CHOICE *pChoice)
* \brief Set the resizable option.
*
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setResizableOption(enum CHOICE *pChoice)
{
	int xYes = 1197, y = 149;
	toggleOptions(pChoice, xYes, y);
	if (*pChoice == YES)
		SDL_SetWindowResizable(SDL_TRUE);
	else SDL_SetWindowResizable(SDL_FALSE);
}

/**
* \fn void setMusicOption(enum CHOICE *pChoice)
* \brief Set the music option.
*
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setMusicOption(enum CHOICE *pChoice)
{
	int xYes = 1197, y = 434;
	toggleOptions(pChoice, xYes, y);
	if (*pChoice == YES)
		globalInput->musicAllowed = 1;
	else globalInput->musicAllowed = 0;
	playMusique(globalInput->musicAllowed, MusiqueMenu);
}

/**
* \fn void setSoundOption(enum CHOICE *pChoice)
* \brief Set the sound option.
*
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSoundOption(enum CHOICE *pChoice)
{
	int xYes = 1199, y = 576;
	toggleOptions(pChoice, xYes, y);
	if (*pChoice == YES)
		globalInput->soundAllowed = 1;
	else globalInput->soundAllowed = 0;
}

/**
* \fn void setSizeOption(enum CHOICE *pChoice)
* \brief Set the size option.
*
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSizeOption(enum CHOICE *pChoice)
{
	static int wWindow, hWindow;
	static char strSize[21] = "1080*600";
	int xYes = 1197, y = 292;
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	toggleOptions(pChoice, xYes, y);
	if (testChange != *pChoice)
	{
		strcpy(globalInput->textInput, "");
		globalInput->textCounter = 0;
	}
	if (*pChoice == TEXT)
	{
		if (strcmp(globalInput->textInput, "") == 0 && strlen(strSize) > 1)
		{
			strcpy(globalInput->textInput, strSize);
			globalInput->textCounter = (char)strlen(strSize);
		}
		strcpy(strSize, globalInput->textInput);
	}
	else if (*pChoice == SET)
	{
		if (testChange != *pChoice)
		{
			getSizeWindow(&wWindow, &hWindow, strSize);
			SDL_SetWindowSize(globalWindow, wWindow, hWindow);
			globalInput->textCounter = 0;
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
* \fn void setSavePathOption(enum CHOICE *pChoice, enum CHOICE windowSize)
* \brief Set the save path option.
*
* \param[in] pChoice, pointer to a choice variable.
* \returns void.
*/
void setSavePathOption(enum CHOICE *pChoice, enum CHOICE windowSize)
{
	int xYes = 1197, y = 719;
	static char strPath[100] = "";
	enum CHOICE testChange = *pChoice;
	SDL_Color color;
	setSDLColor(&color, 0, 0, 0);
	color.a = 255;

	toggleOptions(pChoice, xYes, y);
	if (testChange != *pChoice)
	{
		strcpy(globalInput->textInput, "");
		globalInput->textCounter = 0;
	}
	if (*pChoice == DEFAULT)
	{
		if (strcmp(globalVar.savePath, DEFAULTPATH) != 0)
			strcpy(globalVar.savePath, DEFAULTPATH);
		strcpy(strPath, DEFAULTPATH);
	}
	else if (*pChoice == TEXT && windowSize != TEXT)
	{
		if (strcmp(globalInput->textInput, "") == 0 && strlen(strPath) > 1)
		{
			strcpy(globalInput->textInput, strPath);
			globalInput->textCounter = (char)strlen(strPath);
		}
		strcpy(strPath, globalInput->textInput);
		strcpy(globalVar.savePath, globalInput->textInput);
	}
	renderText(strPath, 1493, 719, 16, color);
}

/**
* \fn void toggleOptions(enum CHOICE *pChoice, int x, int y)
* \brief Toggle an option.
*
* \param[in] pChoice, pointer to a choice variable.
* \param[in] x, x coordinnate of the first option box.
* \param[in] y, y coordinnate of the first option box.
* \returns void.
*/
void toggleOptions(enum CHOICE *pChoice, int x, int y)
{
	SDL_Texture* textureChoice = NULL;
	SDL_Rect yesRect, noRect, textRect, setRect;

	yesRect = initButtonBox(x, y, 172, 88);
	noRect = initButtonBox(1493, y, 172, 88);
	textRect = initButtonBox(1493, y, 386, 88);
	setRect = initButtonBox(1846, y, 107, 88);

	if (collisionPointWithRect(globalInput->cursor.now, &yesRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			if (*pChoice != NEITHER && *pChoice != DEFAULT && *pChoice != SET && *pChoice != TEXT)
				*pChoice = YES;
			else *pChoice = DEFAULT;
		}
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &noRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			if (*pChoice != NEITHER && *pChoice != DEFAULT && *pChoice != SET && *pChoice != TEXT)
				*pChoice = NO;
			else *pChoice = TEXT;
		}
	}
	else if (collisionPointWithRect(globalInput->cursor.now, &setRect))
	{
		if (globalInput->lclick)
		{
			globalInput->lclick = 0;
			if (*pChoice == NEITHER || *pChoice == DEFAULT || *pChoice == TEXT)
				*pChoice = SET;
		}
	}
	else if ((*pChoice == TEXT || *pChoice == NEITHER) && globalInput->lclick)
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
		globalInput->raffraichissement = 1;
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
		globalInput->raffraichissement = 1;
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
		globalInput->raffraichissement = 1;
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
		globalInput->raffraichissement = 1;
	}
	my_freeTexture(textureChoice);
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
	SDL_Surface* textSurface = my_RenderText_Blended(font, textureText, textColor);
	if (textSurface == NULL)
	{
		fprintf(logFile, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = my_CreateTextureFromSurface(globalRenderer, textSurface);
		if (mTexture == NULL)
		{
			fprintf(logFile, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		if (w != NULL)
			*w = textSurface->w;
		if (h != NULL)
			*h = textSurface->h;
		//Get rid of old surface
		my_freeSurface(textSurface);
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

	if (strcmp(str, "") == 0)
		return;

	SDL_GetRendererOutputSize(globalRenderer, &wRender, &hRender);
	x = (int)((float)(x / WIDTHMENUTEXTURE) * wRender);
	y = (int)((float)(y / HIGHTMENUTEXTURE) * hRender) + (int)((float)(0.3 * hText / HIGHTMENUTEXTURE) * hRender);
	SDL_Texture* text = loadFromRenderedText(str, color, &wText, &hText, sizeFont);
	rect = initRect(x, y, wText, hText);
	SDL_RenderCopy(globalRenderer, text, NULL, &rect);
	my_freeTexture(text);
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

/**
* \fn char** initMAPstrings()
* \brief Initializes the array of maps names.
*
* \returns Array of strings of the maps names, NULL if error.
*/
char** initMAPstrings()
{
	FILE* file = fopen("../assets/Cartes/Cartes.txt", "r");
	char **mapTab = NULL;
	if (file != NULL)
	{
		if (lireEntete(file))
		{
			int nbInt = fscanf(file, "%d", &NBMAP);
			if (nbInt == EOF || nbInt > 1)
				return NULL;
			fseek(file, 2, SEEK_CUR);
			mapTab = (char**)my_malloc(NBMAP*sizeof(char*));
			if (mapTab != NULL)
			{
				int i = 0;
				for (i = 0; i < NBMAP; i++)
				{
					mapTab[i] = NULL;
					mapTab[i] = readString(file);
				}
			}
		}
		fclose(file);
	}
	return mapTab;
}

/**
* \fn char* readString(FILE *file)
* \brief Read a string in a file.
*
* \param[in] file, pointer to the file.
* \returns the string read, NULL if error.
*/
char* readString(FILE* file)
{
	char *str = NULL;
	long cur = ftell(file);
	int n = 1;
	char tmp = (char)fgetc(file);
	while (tmp != '\n' && tmp != '\0' && tmp >= 0)
	{
		tmp = (char)fgetc(file);
		n++;
	}
	fseek(file, cur, 0);
	str = my_malloc(n*sizeof(char));
	if (str != NULL)
	{
		fgets(str, n, file);
		fseek(file, 2, SEEK_CUR);
	}
	return str;
}

/**
* \fn int lireEntete(FILE *file)
* \brief Read the start of the map file to test if its correct.
*
* \param[in] file, pointer to the file.
* \returns 1 = no error, 0 = the map file is not write well.
*/
int lireEntete(FILE* file)
{
	char fichier[8];
	fgets(fichier, 8, file);
	if (!strcmp(fichier, "Fichier"))
	{
		fseek(file, 1, SEEK_CUR);
		fgets(fichier, 8, file);
		if (strcmp(fichier, "cartes:") != 0)
		{
			return 0;
		}
		fseek(file, 2, SEEK_CUR);
	}
	return 1;
}

/**
* \fn int getTaille(FILE *fichier)
* \brief Calculate the size of a file.
*
* \param[in] fichier, pointer to the file.
* \returns size of the file in bytes.
*/
int getTaille(FILE *fichier)
{
	int size;
	int position_actuelle = ftell(fichier);
	fseek(fichier, 0, SEEK_END); /**< Place le curseur a la fin du fichier. */
	size = (int)ftell(fichier); /**< Donne le nombre d'octets jusqu'a la position du curseur. */
	fseek(fichier, position_actuelle, SEEK_SET); /**< Replace le curseur a l'emplacement ou il se trouvait au moment de l'appelle de la fonction. */
	return size;
}

/**
* \fn void destroyMenuTab(char **tab, int size)
* \brief Destroys the array of maps names.
*
* \param[in] tab, x position of the original box.
* \param[in] size, y position of the original box.
* \returns void.
*/
void destroyMenuTab(char **tab, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (tab[i] != NULL)
			my_free(tab[i]);
	}
	my_free(tab);
}