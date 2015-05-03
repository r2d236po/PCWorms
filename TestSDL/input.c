#include "input.h"
#include "AffichageGeneral.h"
#include "KaamEngine.h"
#include "worms.h"
#include "Sounds.h"
#include "my_stdrFct.h"
#include "display.h"
#include "MainMenu.h"


//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Acquisition des Inputs                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void getInput(Input * pInput, SDL_Window* pWindow)
* \brief Récupère les inputs.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pWindow, pointeur pWindow pour récupérer les informations relative à la fenêtre.
*/
void getInput(Input * pInput, SDL_Window* pWindow)
{
	SDL_Event event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	Uint32 flags = (SDL_GetWindowFlags(pWindow) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	static int wormsCounter = 0;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			pInput->quit = 1;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				pInput->windowResized = 1;
				pInput->raffraichissement = 1;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				pInput->lclick = 1;
				pInput->cursor.before = pInput->cursor.now;
				break;
			case SDL_BUTTON_RIGHT:
				pInput->rclick = 1;
				pInput->cursor.before = pInput->cursor.now;
				break;
			}
			pInput->raffraichissement = 1;
			break;

		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				if (pInput->lclick)
					pInput->lclick = 0;
				break;
			case SDL_BUTTON_RIGHT:
				if (pInput->rclick)
					pInput->rclick = 0;
				break;
			}
			pInput->raffraichissement = 1;
			break;

		case SDL_MOUSEMOTION:
			pInput->cursor.motion = 1;
			if (pInput->rclick == 1 || pInput->lclick == 1)
				pInput->raffraichissement = 1;
			break;

		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0)
				pInput->wheelDown = 1;
			else pInput->wheelUp = 1;
			pInput->raffraichissement = 1;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (!pInput->arme){
					pInput->direction = LEFT;
					SDL_Delay(10);
				}
				break;
			case SDLK_RIGHT:
				if (!pInput->arme){
					pInput->direction = RIGHT;
					SDL_Delay(10);
				}
				break;
			case SDLK_UP:
				if (!pInput->jumpOnGoing && !pInput->arme)
					pInput->direction = UP;
				break;
			case SDLK_DOWN:
				if (!pInput->arme){
					pInput->direction = DOWN;
				}
				break;
			case SDLK_SPACE:
				if (!pInput->jumpOnGoing && !pInput->arme)
					pInput->jump = 1;
				break;
			case SDLK_LCTRL:
				pInput->bend = 1;
				break;
			case SDLK_ESCAPE:
				if (!pInput->menu)
					pInput->menu = 1;
				else pInput->menu = 0;
				break;
			case SDLK_q:
				pInput->quit = 1;
				break;
			case SDLK_TAB:
				pInput->weaponTab = 1;
				break;
			case SDLK_b:
				pInput->bombe = 1;
				break;
			case SDLK_r:
				if (pInput->camCentrer){
					pInput->camCentrer = 0;
				}
				else pInput->camCentrer = 1;
				break;
			case SDLK_c:
				if (!pInput->arme){
					pInput->changeWorms = 1;
				}
				break;
			case SDLK_PRINTSCREEN:
				pInput->screenshot = 1;
				break;
			case SDLK_k:
				if (pInput->cursor.currentCursor == 0)
				{
					SDL_SetCursor(pInput->cursor.cursor2);
					pInput->cursor.currentCursor = 1;
				}
				else
				{
					SDL_SetCursor(pInput->cursor.cursor1);
					pInput->cursor.currentCursor = 0;
				}
				break;
			case SDLK_a:
				if (pInput->arme == 0) { pInput->arme = 1; }
				else pInput->arme = 0;
				break;
			case SDLK_BACKSPACE:
				pInput->textCounter--;
				secuTextInput(pInput);
				pInput->textInput[pInput->textCounter] = '\0';
				break;

			}
			if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			{
				strcpy(pInput->textInput, SDL_GetClipboardText());
				pInput->textCounter += (char)strlen(SDL_GetClipboardText());
			}
			pInput->raffraichissement = 1;
			break;
		case SDL_TEXTINPUT:
			pInput->textInput[pInput->textCounter] = event.text.text[0];
			pInput->textCounter++;
			if (pInput->textCounter == 100)
				pInput->textCounter = 0;
			secuTextInput(pInput);
			pInput->textInput[pInput->textCounter] = '\0';
			pInput->raffraichissement = 1;
			if (event.text.text[0] == 'q')
				pInput->quit = 0;
			break;
		default:
			break;
		}
	}
	//Gestion du plein écran
	if (keystate[SDL_SCANCODE_RETURN] && SDL_GetModState() & KMOD_CTRL)
	{
		if (SDL_SetWindowFullscreen(pWindow, flags) < 0)
			printf("ERROR lors du passage en mode fenetre : %s", SDL_GetError());
		SDL_Delay(50);
		pInput->raffraichissement = 1;
	}
	if (pInput->cursor.motion){
		pInput->cursor.before = pInput->cursor.now;
		SDL_GetMouseState(&pInput->cursor.now.x, &pInput->cursor.now.y);
		pInput->cursor.motion = 0;
	}
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Gestion des Inputs                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab)
* \brief Gere les inputs.
* Genere les actions correspondant aux inputs.
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pRenderer, pointeur pWindow pour récupérer les informations relative à la fenêtre.
* \param[in] pMapTerrain, pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \param[in] wormsTab, pointeur vers la structure du worms en cours de jeu pour modifier ses paramètres de position.
* \returns int, indicateur si la fonction a bien fonctionnée (1 = succes, -1 = echec)
*/
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* pMapTerrain, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Worms** wormsTab)
{
	/*if (pInput->right) //Exemple de gestion d'input V1.0, test du booleen
	{
	...
	Code à éxécuter
	...

	pInput->right = 0;	//remise à zéro du booléen (si nécessaire)
	}*/
	static KaamObject* test = NULL;
	inputsCamera(pInput, pTextureDisplay, pCamera, pRenderer, wormsTab[globalVar.indexWormsTab]);	//appel de la fonction de gestion des Inputs de la camera
	if (pInput->windowResized){
		initCameras(pRenderer, pMapTerrain, pCamera, NULL);
		pInput->windowResized = 0;
	}
	inputsWeapons(pInput, pTextureDisplay, pCamera, pMapTerrain, pRenderer, wormsTab);	//appel de la fonction de gestion des Inputs des armes
	if (pInput->screenshot)
	{
		screenshot(pRenderer);
		pInput->screenshot = 0;
	}
	if (pInput->changeWorms)
	{
		if (!pInput->jumpOnGoing && !globalVar.gameEnd){
			callNextWorms(wormsTab);
		}
		pInput->changeWorms = 0;
	}
	if (pInput->menu)
	{
		SDL_Texture* textureMenu = loadTexture(pRenderer, INGAMEMENU);
		SDL_Rect rectMenu = initButtonBox(pRenderer, -1, -1, 565, 717);
		renderScreen(pRenderer, 3, 0, pMapTerrain, 1, pTextureDisplay, pCamera, NULL, 1, textureMenu, NULL, &rectMenu);
		pInput->raffraichissement = 0;
		SDL_DestroyTexture(textureMenu);
	}
	inputsJumpWorms(pInput, wormsTab[globalVar.indexWormsTab], pMapTerrain->collisionMapSurface);
	if (pInput->direction == DOWN)
	{
		teleportWorms(pInput, wormsTab[globalVar.indexWormsTab], pMapTerrain->collisionMapSurface, pCamera);
	}
	updateGameWorms(pInput, wormsTab, pMapTerrain->collisionMapSurface, pMapTerrain, pTextureDisplay, pRenderer);

	return 1;	//flag de gestion d'erreur, -1 il y a eu un problème, 1 c'est okay
}


/**
* \fn void inputsCamera(Input* pInput, SDL_Texture* pTexture, SDL_Rect* camera, SDL_Renderer * pRenderer)
* \brief Gere les inputs relatives a la camera.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pTextureDisplay, pointeur vers la texture sur laquelle est appliqué la caméra.
* \param[in] pCamera, pointeur vers la structure SDL_Rect de la caméra pour modifier ses valeurs.
* \param[in] pRenderer pointeur pWindow pour récupérer les informations relative à la fenêtre.
* \returns void
*/
void inputsCamera(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, SDL_Renderer * pRenderer, Worms * pWorms)
{

	if (pInput->rclick)
	{
		moveCam(pTextureDisplay, pCamera, pInput); //gestion du scrolling de caméra
		pInput->cursor.before = pInput->cursor.now;
	}
	if (pInput->camCentrer)
	{
		pInput->raffraichissement = (char)centerCam(pCamera, pWorms->wormsObject->objectSurface, pTextureDisplay);
	}
	if (pInput->wheelUp){
		zoomIn(pRenderer, pTextureDisplay, pCamera, pInput);
		pInput->wheelUp = 0;
		pInput->raffraichissement = 1;
	}
	if (pInput->wheelDown){
		zoomOut(pRenderer, pTextureDisplay, pCamera);
		pInput->wheelDown = 0;
		pInput->raffraichissement = 1;
	}


}


/**
* \fn void inputsJumpWorms(Input* pInput, Worms* worms)
* \brief Handles the inputs for a worms's jump.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pWorms, pointeur vers la structure du worms en cours
* \returns void
*/
void inputsJumpWorms(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap)
{
	int onGround = testGround(pSurfaceMap, pWorms->wormsObject->objectSurface, 1);
	if (onGround)
	{
		if (!pInput->jumpOnGoing)
		{
			if (pInput->jump)
			{
				setWormsSpeed(pWorms, pWorms->dirSurface);
				pInput->jumpOnGoing = 1;
			}
			else if (pInput->direction == UP)
			{
				setWormsSpeed(pWorms, UP);
				pInput->jumpOnGoing = 1;
			}
			if (pInput->jumpOnGoing)
			{
				pWorms->indexAnim = 0;
				animationWorms(pWorms, pWorms->indexAnim, pWorms->dirSurface);
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
	//Gestion des Index
	if (globalVar.wormsPlaying[globalVar.teamPlaying] != globalVar.nbWormsEquipe[globalVar.teamPlaying] - 1)	//Changement de worms
	{
		globalVar.wormsPlaying[globalVar.teamPlaying] += 1;
	}		//tester si Worms suivant vivant
	else { globalVar.wormsPlaying[globalVar.teamPlaying] = 0; }

	if (globalVar.teamPlaying != globalVar.nbEquipe - 1) { globalVar.teamPlaying += 1; }							//Changement d'équipe
	else { globalVar.teamPlaying = 0; }

	//Calcul du IndexWormsTab
	globalVar.indexWormsTab = calculIndex();
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
* \fn void inputsWeapons(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* mapTerrain, SDL_Renderer * pRenderer)
* \brief Gere les inputs relatives aux armes.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pMapTerrain pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTextureDisplay pointeur vers la texture sur laquelle est appliqué la camera.
* \param[in] pCamera pointeur vers la structure SDL_Rect de la caméra pour modifier ses valeurs.
* \param[in] pRenderer pointeur pWindow pour récupérer les informations relative à la fenêtre.
* \returns void
*/
void inputsWeapons(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, SDL_Renderer * pRenderer, Worms** wormsTab)
{
	if (pInput->bombe)
	{
		static int rW, rH;
		//Mix_PlayChannel(2, sndFx, 0);

		SDL_GetRendererOutputSize(pRenderer, &rW, &rH);
		int x = (int)(pInput->cursor.now.x * ((float)pCamera->w / (float)rW) + pCamera->x);
		int y = (int)(pInput->cursor.now.y * ((float)pCamera->h / (float)rH) + pCamera->y);
		int radius = 50;
		SDL_Rect  rect = initRect(x - radius, y - radius, 2 * radius, 2 * radius);
		explosion(x, y, radius, pMapTerrain->globalMapSurface, pTextureDisplay);
		bombReactionManagement(pInput, wormsTab, &rect, x, y, radius);
		//Update of the collision surface
		updateSurfaceFromSurface(pMapTerrain->collisionMapSurface, pMapTerrain->globalMapSurface, &rect, 0);
		pInput->raffraichissement = 1;
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
	Input* pInput = NULL;

	pInput = (Input*)malloc(sizeof(Input));
	if (pInput == NULL)
	{
		fprintf(logFile, "initInput : FAILURE, allocation memoire de pInput.\n\n");
		return NULL;
	}
	pInput->cursor = initCursor();
	if (pInput->cursor.cursor1 == NULL || pInput->cursor.cursor2 == NULL)
	{
		fprintf(logFile, "initInput : FAILURE, initCursor.\n\n");
		free(pInput);
		pInput = NULL;
		return NULL;
	}
	pInput->jump = 0;
	pInput->jumpOnGoing = 0;
	pInput->bend = 0;
	pInput->menu = 0;
	pInput->direction = NONE;
	pInput->lclick = 0;
	pInput->rclick = 0;
	pInput->quit = 0;
	pInput->weaponTab = 0;
	pInput->wheelUp = 0;
	pInput->wheelDown = 0;
	pInput->raffraichissement = 1;
	pInput->windowResized = 1;
	pInput->acceleration = 1;
	pInput->bombe = 0;
	pInput->deplacement = 0;
	pInput->screenshot = 0;
	pInput->camCentrer = 0;
	pInput->changeWorms = 0;
	pInput->arme = 0;
	pInput->soundAllowed = 1;
	pInput->musicAllowed = 1;
	strcpy(pInput->textInput, "");
	pInput->textCounter = 0;
	fprintf(logFile, "initInput : SUCCESS.\n\n");
	return pInput;
}

/**
* \fn void resetStructInput(Input* pInput)
* \brief Reset all inputs of a Input struct.
*
* \param[in] pInput, pointer to the inputs structure.
* \returns void
*/
void resetStructInput(Input* pInput)
{
	pInput->jump = 0;
	pInput->jumpOnGoing = 0;
	pInput->bend = 0;
	pInput->menu = 0;
	pInput->direction = NONE;
	pInput->lclick = 0;
	pInput->rclick = 0;
	pInput->weaponTab = 0;
	pInput->wheelUp = 0;
	pInput->wheelDown = 0;
	pInput->raffraichissement = 0;
	pInput->windowResized = 1;
	pInput->acceleration = 1;
	pInput->bombe = 0;
	pInput->deplacement = 0;
	pInput->screenshot = 0;
	pInput->camCentrer = 0;
	pInput->changeWorms = 0;
	pInput->arme = 0;
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
		SDL_FreeSurface(sword);
		sword = NULL;
		return curseur;
	}

	curseur.cursor1 = SDL_CreateColorCursor(sword, 0, 0);
	curseur.cursor2 = SDL_CreateColorCursor(aim, 0, 0);

	SDL_FreeSurface(sword);
	sword = NULL;
	SDL_FreeSurface(aim);
	aim = NULL;

	SDL_SetCursor(curseur.cursor1);
	fprintf(logFile, "initCursor : SUCCESS.\n\n");
	return curseur;
}

/**
* \fn void secuTextInput(Input* pInput)
* \brief Test if the textCounter is out of limit.
*
* \param[in] pInput, pointer to the inputs structure.
* \returns void
*/
void secuTextInput(Input* pInput)
{
	if (pInput->textCounter >= 49)
		pInput->textCounter = 49;
	if (pInput->textCounter < 0)
		pInput->textCounter = 0;
}

