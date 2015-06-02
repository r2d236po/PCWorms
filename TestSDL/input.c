#include "input.h"
#include "armes.h"
#include "AffichageGeneral.h"
#include "KaamEngine.h"
#include "worms.h"
#include "Sounds.h"
#include "my_stdrFct.h"
#include "display.h"
#include "MainMenu.h"
#include "HUD.h"
#include "memory.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Acquisition des Inputs                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void getInput()
* \brief Récupère les inputs.
*
*/
void getInput()
{
	SDL_Event event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	Uint32 flags = (SDL_GetWindowFlags(globalWindow) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			globalInput->quit = 1;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				globalInput->windowResized = 1;
				globalInput->raffraichissement = 1;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtonDown(event);
			globalInput->raffraichissement = 1;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtonUp(event);
			globalInput->raffraichissement = 1;
			break;

		case SDL_MOUSEMOTION:
			globalInput->cursor.motion = 1;
			if (globalInput->rclick == 1 || globalInput->lclick == 1)
				globalInput->raffraichissement = 1;
			break;

		case SDL_MOUSEWHEEL:
			mouseWheel(event);
			globalInput->raffraichissement = 1;
			break;

		case SDL_KEYDOWN:
			INPUT_keydownInputs(event);
			if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				strcpy(globalInput->textInput, SDL_GetClipboardText());
				globalInput->textCounter += (char)strlen(SDL_GetClipboardText());
			}
			globalInput->raffraichissement = 1;
			break;
		case SDL_KEYUP:
			INPUT_keyupInputs(event);
			break;
		case SDL_TEXTINPUT:
			globalInput->textInput[globalInput->textCounter] = event.text.text[0];
			globalInput->textCounter++;
			if (globalInput->textCounter == 100)
				globalInput->textCounter = 0;
			secuTextInput(globalInput);
			globalInput->textInput[globalInput->textCounter] = '\0';
			globalInput->raffraichissement = 1;
			if (event.text.text[0] == 'q' || event.text.text[0] == 'Q')
				globalInput->quit = 0;
			break;
		default:
			break;
		}
	}
	//Gestion du plein écran
	if (keystate[SDL_SCANCODE_RETURN] && SDL_GetModState() & KMOD_CTRL)
	{
		if (SDL_SetWindowFullscreen(globalWindow, flags) < 0)
			printf("ERROR lors du passage en mode fenetre : %s", SDL_GetError());
		SDL_Delay(50);
		globalInput->raffraichissement = 1;
	}
	if (globalInput->cursor.motion){
		globalInput->cursor.before = globalInput->cursor.now;
		SDL_GetMouseState(&globalInput->cursor.now.x, &globalInput->cursor.now.y);
		globalInput->cursor.motion = 0;
	}
}

/**
* \fn void INPUT_keydownInputs(SDL_Event event)
* \brief Handles the button down event of the keyboard.
*
* \param[in] event, event of the window.
* \returns void
*/
void INPUT_keydownInputs(SDL_Event event)
{
	static Uint32 timeStartPause = 0;
	switch (event.key.keysym.sym)
	{
	case SDLK_LEFT:
		if (!globalInput->arme){
			globalInput->direction = LEFT;
			SDL_Delay(10);
		}
		break;
	case SDLK_RIGHT:
		if (!globalInput->arme){
			globalInput->direction = RIGHT;
			SDL_Delay(10);
		}
		break;
	case SDLK_UP:
		if (!globalInput->jumpOnGoing && !globalInput->arme && !globalInput->jump)
			globalInput->direction = UP;
		break;
	case SDLK_DOWN:
		if (!globalInput->arme){
			globalInput->direction = DOWN;
		}
		break;
	case SDLK_SPACE:
		if (!globalInput->jumpOnGoing && !globalInput->arme)
		{
			globalInput->jump = 1;
		}
		break;
	case SDLK_LCTRL:
		globalInput->bend = 1;
		break;
	case SDLK_ESCAPE:
		if (!globalInput->menu) {
			globalInput->menu = 1;
			timeStartPause = SDL_GetTicks();
		}
		else {
			globalInput->menu = 0;
			globalVar.timePause += SDL_GetTicks() - timeStartPause;
			timeStartPause = 0;
		}
		break;
	case SDLK_TAB:
		if (!globalInput->tabLock)
			globalInput->weaponTab = 1;
		globalInput->tabLock = 1;
		break;
	case SDLK_b:
		globalInput->bombe = 1;
		break;
	case SDLK_r:
		if (globalInput->camCentrer){
			globalInput->camCentrer = 0;
		}
		else globalInput->camCentrer = 1;
		break;
	case SDLK_c:
		if (!globalInput->arme && !globalInput->menu && !globalInput->grenade)
		{
			globalInput->changeWorms = 1;
		}
		break;
	case SDLK_PRINTSCREEN:
		if (!globalInput->screenshot)
			screenshot();
		globalInput->screenshot = 1;
		break;
	case SDLK_k:
		if (globalInput->cursor.currentCursor == 0)
		{
			SDL_SetCursor(globalInput->cursor.cursor2);
			globalInput->cursor.currentCursor = 1;
		}
		else
		{
			SDL_SetCursor(globalInput->cursor.cursor1);
			globalInput->cursor.currentCursor = 0;
		}
		break;
	case SDLK_a:
		if (!globalInput->menu && !globalInput->grenade)
		{
			if (globalInput->arme == 0)
			{
				globalInput->arme = 1;
			}
			else globalInput->arme = 0;
		}
		break;
	case SDLK_g:
		if (!globalInput->menu && !globalInput->arme)
		{
			globalInput->grenade = 1;
		}
		break;
	case SDLK_BACKSPACE:
		globalInput->textCounter--;
		secuTextInput(globalInput);
		globalInput->textInput[globalInput->textCounter] = '\0';
		break;
	}
}

/**
* \fn void INPUT_keyupInputs(SDL_Event event)
* \brief Handles the button up event of the keyboard.
*
* \param[in] event, event of the window.
* \returns void
*/
void INPUT_keyupInputs(SDL_Event event)
{
	switch (event.key.keysym.sym)
	{
	case SDLK_TAB:
		globalInput->tabLock = 0;
		globalInput->weaponTab = 0;
		break;
	case SDLK_PRINTSCREEN:
		globalInput->screenshot = 0;
		break;
	}
}

/**
* \fn void mouseButtonDown(SDL_Event event)
* \brief Handles the button down event of the mouse.
*
* \param[in] event, event of the window.
* \returns void
*/
void mouseButtonDown(SDL_Event event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		globalInput->lclick = 1;
		globalInput->cursor.before = globalInput->cursor.now;
		break;
	case SDL_BUTTON_RIGHT:
		globalInput->rclick = 1;
		globalInput->cursor.before = globalInput->cursor.now;
		break;
	}
}

/**
* \fn void mouseButtonUp(SDL_Event event)
* \brief Handles the button up event of the mouse.
*
* \param[in] event, event of the window.
* \returns void
*/
void mouseButtonUp(SDL_Event event)
{
	switch (event.button.button)
	{
	case SDL_BUTTON_LEFT:
		if (globalInput->lclick)
			globalInput->lclick = 0;
		break;
	case SDL_BUTTON_RIGHT:
		if (globalInput->rclick)
			globalInput->rclick = 0;
		break;
	}
}

/**
* \fn void mouseWheel(SDL_Event event)
* \brief Handles the wheel event of the mouse.
*
* \param[in] event, event of the window.
* \returns void
*/
void mouseWheel(SDL_Event event)
{
	if (event.wheel.y < 0)
		globalInput->wheelDown = 1;
	else globalInput->wheelUp = 1;
}


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Gestion des Inputs                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int gestInput(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab)
* \brief Gere les inputs.
* Genere les actions correspondant aux inputs.
* \param[in] jeu, pointeur vers le jeu en cours
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \param[in] wormsTab, pointeur vers la structure du worms en cours de jeu pour modifier ses paramètres de position.
* \returns int, indicateur si la fonction a bien fonctionnée (1 = succes, -1 = echec)
*/
int gestInput(Jeu* jeu, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab)
{
	/*if (globalInput->right) //Exemple de gestion d'input V1.0, test du booleen
	{
	...
	Code à éxécuter
	...

	globalInput->right = 0;	//remise à zéro du booléen (si nécessaire)
	}*/
	static char menuPrec = 0, weaponPrec = 0;
	if (globalInput->windowResized)
	{
		initCameras(jeu->pMapTerrain, pCamera, NULL);
		globalInput->windowResized = 0;
		globalInput->raffraichissement = 1;
		return 1;
	}
	if (!globalVar.gameEnd && globalInput->menu)
	{
		if (menuPrec != globalInput->menu)
			inGameMenu(jeu->pMapTerrain, pTextureDisplay, pCamera, 1);
		else inGameMenu(jeu->pMapTerrain, pTextureDisplay, pCamera, 0);
		globalInput->raffraichissement = 0;
	}
	if (globalVar.gameEnd)
	{
		EngGameScreen(jeu, pTextureDisplay, pCamera);
		globalInput->raffraichissement = 0;
	}
	if (globalInput->weaponTab)
	{
		if (globalInput->arme || globalInput->grenade)
		{
			globalInput->arme = 0;
			globalInput->grenade = 0;
			eraseRectFromMap(jeu->pMapTerrain, pTextureDisplay, NULL);
			displayWormsTab(wormsTab, 1);
		}
		if (weaponPrec != globalInput->weaponTab)
			HUD_weaponsMenu(jeu->pMapTerrain, pTextureDisplay, pCamera, 1);
		else HUD_weaponsMenu(jeu->pMapTerrain, pTextureDisplay, pCamera, 0);
		globalInput->raffraichissement = 0;
	}
	inputsCamera(pTextureDisplay, pCamera, wormsTab[globalVar.indexWormsTab]);	//appel de la fonction de gestion des Inputs de la camera
	inputsWeapons(pTextureDisplay, pCamera, jeu->pMapTerrain, wormsTab);	//appel de la fonction de gestion des Inputs des armes

	if (globalInput->changeWorms)
	{
		if (!globalInput->jumpOnGoing){
			callNextWorms(wormsTab);
		}
		globalInput->changeWorms = 0;
	}
	inputsJumpWorms(wormsTab[globalVar.indexWormsTab], jeu->pMapTerrain->collisionMapSurface);
	if (globalInput->direction == DOWN &&wormsTab[globalVar.indexWormsTab]->vie > 0)
	{
		teleportWorms(wormsTab[globalVar.indexWormsTab], jeu->pMapTerrain->collisionMapSurface, pCamera);
	}

	updateGameWorms(jeu, wormsTab, pTextureDisplay, pCamera);

	menuPrec = globalInput->menu;
	weaponPrec = globalInput->weaponTab;


	return 1;	//flag de gestion d'erreur, -1 il y a eu un problème, 1 c'est okay
}


/**
* \fn void inputsCamera(SDL_Texture* pTexture, SDL_Rect* camera)
* \brief Gere les inputs relatives a la camera.
*
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la caméra.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la caméra pour modifier ses valeurs.
* \returns void
*/
void inputsCamera(SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms * pWorms)
{
	if (globalInput->menu || globalVar.gameEnd || globalInput->weaponTab)
		return;
	if (globalInput->rclick && !globalInput->grenade)
	{
		moveCam(pTextureDisplay, pCamera); //gestion du scrolling de caméra
		globalInput->cursor.before = globalInput->cursor.now;
	}
	if (globalInput->camCentrer)
	{
		globalInput->raffraichissement = (char)centerCam(pCamera, pWorms->wormsObject->objectSurface, pTextureDisplay);
	}
	if (globalInput->wheelUp){
		zoomIn(pTextureDisplay, pCamera);
		globalInput->wheelUp = 0;
		globalInput->raffraichissement = 1;
	}
	if (globalInput->wheelDown){
		zoomOut(pTextureDisplay, pCamera);
		globalInput->wheelDown = 0;
		globalInput->raffraichissement = 1;
	}


}


/**
* \fn void inputsJumpWorms(Worms* worms)
* \brief Handles the inputs for a worms's jump.
*
* \param[in] pWorms, pointeur vers la structure du worms en cours
* \returns void
*/
void inputsJumpWorms(Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int onGround = testGround(pSurfaceMap, pWorms->wormsObject->objectSurface, 1);
	if (onGround)
	{
		if (!globalInput->jumpOnGoing)
		{
			if (globalInput->jump)
			{
				setWormsSpeed(pWorms, pWorms->dirSurface);
				globalInput->jumpOnGoing = 1;
			}
			else if (globalInput->direction == UP)
			{
				setWormsSpeed(pWorms, UP);
				globalInput->jumpOnGoing = 1;
			}
			if (globalInput->jumpOnGoing)
			{
				pWorms->indexAnim = 0;
				animationWorms(pWorms, pWorms->indexAnim, pWorms->dirSurface, 0);
			}
		}
	}
}

/**
* \fn void callNextWorms()
* \brief Change the index of the worms playing if it is dead.
* \param[in] wormsTab, array of worms
* \returns void
*/
void callNextWorms(Worms** wormsTab)
{
	int nbEquipe = 0;

	if (globalVar.nbWormsTotal <= 1)
		return;
	if (globalInput->arme)
		globalInput->arme = 0;

	//Changement de worms pour l'equipe qui vient de jouer
	if (globalVar.wormsPlaying[globalVar.teamPlaying] != (globalVar.nbWormsEquipe[globalVar.teamPlaying] - 1))
	{
		globalVar.wormsPlaying[globalVar.teamPlaying] += 1;
	}
	else globalVar.wormsPlaying[globalVar.teamPlaying] = 0;

	//Determine la nouvelle equipe
	if (!globalVar.gameEnd)
	{
		do
		{
			if (globalVar.teamPlaying != (globalVar.nbEquipe - 1))
				globalVar.teamPlaying += 1;
			else { globalVar.teamPlaying = 0; }
			nbEquipe++;
		} while ((wormsTab[calculIndex()]->team->vie <= 0 || globalVar.gameEnd) && nbEquipe <= globalVar.nbEquipe);

	}
	//Determine le nouveau worms
	while (wormsTab[calculIndex()]->vie <= 0)
	{
		if (globalVar.wormsPlaying[globalVar.teamPlaying] != (globalVar.nbWormsEquipe[globalVar.teamPlaying] - 1))
			globalVar.wormsPlaying[globalVar.teamPlaying] += 1;
		else globalVar.wormsPlaying[globalVar.teamPlaying] = 0;
	}

	//Affecte la valeur à l'index global
	globalVar.indexWormsTab = calculIndex();
	globalVar.timeLastWormsChange = SDL_GetTicks();
	globalVar.timeWeapon = 0;
	globalVar.timePause = 0;
}


/**
* \fn int calculIndex()
* \brief calcul le nouvel index à partir des variables globales
* \returns int nouvel index
*/
int calculIndex()
{
	int i, index = 0;

	for (i = 0; i < globalVar.teamPlaying; i++)
	{
		index += globalVar.nbWormsEquipe[i];
	}
	index += globalVar.wormsPlaying[globalVar.teamPlaying];

	return index;
}


/**
* \fn void inputsWeapons(SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* mapTerrain)
* \brief Gere les inputs relatives aux armes.
*
* \param[in] pMapTerrain pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera pointeur vers la structure SDL_Rect de la caméra pour modifier ses valeurs.
* \returns void
*/
void inputsWeapons(SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, Worms** wormsTab)
{
	if (globalInput->bombe)
	{
		static int rW, rH;
		//Mix_PlayChannel(2, sndFx, 0);

		SDL_GetRendererOutputSize(globalRenderer, &rW, &rH);
		int x = (int)(globalInput->cursor.now.x * ((float)pCamera->w / (float)rW) + pCamera->x);
		int y = (int)(globalInput->cursor.now.y * ((float)pCamera->h / (float)rH) + pCamera->y);
		int radius = 50;
		SDL_Rect  rect = initRect(x - radius, y - radius, 2 * radius, 2 * radius);
		explosion(x, y, radius, pMapTerrain->globalMapSurface, pTextureDisplay);
		bombReactionManagement(wormsTab, &rect, x, y, radius);
		//Update of the collision surface
		updateSurfaceFromSurface(pMapTerrain->collisionMapSurface, pMapTerrain->globalMapSurface, &rect, 0);
		globalInput->raffraichissement = 1;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    Initialisations                     /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Input* initInput()
* \brief Créé et initialise une structure Input.
*
* \returns Input*, pointeur vers la structure créée, NULL si echec
*/
Input* initInput()
{
	Input* inputTemp = NULL;

	inputTemp = (Input*)my_malloc(sizeof(Input));
	if (inputTemp == NULL)
	{
		fprintf(logFile, "initInput : FAILURE, allocation memoire de globalInput.\n\n");
		decreaseMalloc();
		return NULL;
	}
	inputTemp->cursor = initCursor();
	if (inputTemp->cursor.cursor1 == NULL || inputTemp->cursor.cursor2 == NULL)
	{
		fprintf(logFile, "initInput : FAILURE, initCursor.\n\n");
		my_free(inputTemp);
		inputTemp = NULL;
		return NULL;
	}
	inputTemp->jump = 0;
	inputTemp->backJump = 0;
	inputTemp->jumpOnGoing = 0;
	inputTemp->bend = 0;
	inputTemp->menu = 0;
	inputTemp->direction = NONE;
	inputTemp->lclick = 0;
	inputTemp->rclick = 0;
	inputTemp->quit = 0;
	inputTemp->weaponTab = 0;
	inputTemp->wheelUp = 0;
	inputTemp->wheelDown = 0;
	inputTemp->raffraichissement = 1;
	inputTemp->windowResized = 1;
	inputTemp->acceleration = 1;
	inputTemp->bombe = 0;
	inputTemp->deplacement = 0;
	inputTemp->screenshot = 0;
	inputTemp->camCentrer = 1;
	inputTemp->changeWorms = 0;
	inputTemp->arme = 0;
	inputTemp->grenade = 0;
	inputTemp->soundAllowed = 1;
	inputTemp->musicAllowed = 1;
	inputTemp->backToMainMenu = 0;
	inputTemp->weaponIndex = 0;
	inputTemp->tabLock = 0;
	strcpy(inputTemp->textInput, "");
	inputTemp->textCounter = 0;
	fprintf(logFile, "initInput : SUCCESS.\n\n");
	return inputTemp;
}

/**
* \fn void resetStructInput()
* \brief Reset all inputs of a Input struct.
*
* \returns void
*/
void resetStructInput()
{
	globalInput->jump = 0;
	globalInput->jumpOnGoing = 0;
	globalInput->bend = 0;
	globalInput->menu = 0;
	globalInput->direction = NONE;
	globalInput->lclick = 0;
	globalInput->rclick = 0;
	globalInput->weaponTab = 0;
	globalInput->wheelUp = 0;
	globalInput->wheelDown = 0;
	globalInput->raffraichissement = 1;
	globalInput->windowResized = 1;
	globalInput->acceleration = 1;
	globalInput->bombe = 0;
	globalInput->deplacement = 0;
	globalInput->screenshot = 0;
	globalInput->camCentrer = 0;
	globalInput->changeWorms = 0;
	globalInput->arme = 0;
	globalInput->weaponIndex = 0;
	globalInput->backToMainMenu = 0;
	globalInput->tabLock = 0;
	globalInput->grenade = 0;
}

/**
* \fn Cursor initCursor(void)
* \brief Cree et initialise une structure cursor.
*
* \returns Cursor, structure cursor initialisee
*/
Cursor initCursor(void)
{
	Cursor curseur;
	curseur.motion = 0;
	curseur.before.x = 0;
	curseur.before.y = 0;
	curseur.now.x = 0;
	curseur.now.y = 0;
	curseur.currentCursor = 0;

	SDL_Surface * sword = loadImage("../assets/pictures/cursor.png");
	if (sword == NULL)
	{
		fprintf(logFile, "initCursor : FAILURE, loadImage.\n\n");
		curseur.cursor1 = NULL;
		return curseur;
	}
	SDL_Surface * aim = loadImage("../assets/pictures/aim.png");
	if (aim == NULL)
	{
		fprintf(logFile, "initCursor : FAILURE, loadImage.\n\n");
		curseur.cursor1 = NULL;
		my_freeSurface(sword);
		sword = NULL;
		return curseur;
	}

	curseur.cursor1 = SDL_CreateColorCursor(sword, 0, 0);
	curseur.cursor2 = SDL_CreateColorCursor(aim, aim->w / 2, aim->h / 2);

	my_freeSurface(sword);
	sword = NULL;
	my_freeSurface(aim);
	aim = NULL;

	SDL_SetCursor(curseur.cursor1);
	fprintf(logFile, "initCursor : SUCCESS.\n\n");
	return curseur;
}

/**
* \fn void secuTextInput()
* \brief Test if the textCounter is out of limit.
*
* \returns void
*/
void secuTextInput()
{
	if (globalInput->textCounter >= 49)
		globalInput->textCounter = 49;
	if (globalInput->textCounter < 0)
		globalInput->textCounter = 0;
}