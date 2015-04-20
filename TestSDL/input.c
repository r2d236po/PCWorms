#include "input.h"
#include "AffichageGeneral.h"
#include "KaamEngine.h"
#include "worms.h"
#include "Sounds.h"
#include "my_stdrFct.h"


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
				pInput->direction = LEFT;
				SDL_Delay(10);
				break;
			case SDLK_RIGHT:
				pInput->direction = RIGHT;
				SDL_Delay(10);
				break;
			case SDLK_UP:
				if (!pInput->jumpOnGoing)
					pInput->direction = UP;
				break;
			case SDLK_DOWN:
				pInput->direction = DOWN;
				break;
			case SDLK_SPACE:
				if (!pInput->jumpOnGoing)
					pInput->jump = 1;
				break;
			case SDLK_LCTRL:
				pInput->bend = 1;
				break;
			case SDLK_ESCAPE:
				pInput->menu = 1;
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
				pInput->changeWorms = 1;
				break;
			case SDLK_PRINTSCREEN:
				pInput->screenshot = 1;
				break;
			}
			pInput->raffraichissement = 1;
			break;
		default:
			break;
		}
	}
	//Gestion du plein écran
	if (keystate[SDL_SCANCODE_RETURN] && (keystate[SDL_SCANCODE_RCTRL] || keystate[SDL_SCANCODE_LCTRL]))
	{
		if (SDL_SetWindowFullscreen(pWindow, flags) < 0)
			printf("ERROR lors du passage en mode fenetre : %s", SDL_GetError());
		SDL_Delay(50);
		pInput->raffraichissement = 1;
	}
}








//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                   Gestion des Inputs                   /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms)
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
			callNextWorms();
		}
		pInput->changeWorms = 0;
	}
	inputsJumpWorms(pInput, wormsTab[globalVar.indexWormsTab], pMapTerrain->collisionMapSurface);


	updateGameWorms(pInput, wormsTab, pTextureDisplay, pMapTerrain->collisionMapSurface, pMapTerrain->globalMapSurface);
	/*Pour afficher le texte*/
	//updateTexteSurfacePosition
	//updateTextureFromMultipleSurface(pTextureDisplay, pMapTerrain->globalMapSurface, wormsTab[globalVar.indexWormsTab]->texteSurface, &wormsTab[globalVar.indexWormsTab]->texteSurface->clip_rect);
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
	if (pInput->cursor.motion){
		pInput->cursor.before = pInput->cursor.now;
		SDL_GetMouseState(&pInput->cursor.now.x, &pInput->cursor.now.y);
		pInput->cursor.motion = 0;
	}
	if (pInput->wheelUp){
		zoomIn(pRenderer, pTextureDisplay, pCamera, pInput);
		pInput->wheelUp = 0;
	}
	if (pInput->wheelDown){
		zoomOut(pRenderer, pTextureDisplay, pCamera);
		pInput->wheelDown = 0;
	}
	if (pInput->camCentrer)
	{
		centerCam(pCamera, pWorms->wormsObject->objectSurface, pTextureDisplay);
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
		}
	}
}

/**
* \fn void callNextWorms()
* \brief Change the index of the worms playing if it is dead.
*
* \returns void
*/
void callNextWorms()
{
	if (globalVar.teamPlaying != globalVar.nbEquipe - 1)
	{
		globalVar.teamPlaying += 1;
	}
	else {
		globalVar.teamPlaying = 0;
		if (globalVar.wormsPlaying != globalVar.nbWormsEquipe - 1)
		{
			globalVar.wormsPlaying += 1; // tester si le worms suivant et vivant !!!!!!	
		}
		else {
			globalVar.wormsPlaying = 0;
		}
	}
	globalVar.indexWormsTab = globalVar.teamPlaying * globalVar.nbWormsEquipe + globalVar.wormsPlaying;
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
		updateSurfaceFromSurface(pMapTerrain->collisionMapSurface, pMapTerrain->globalMapSurface, &rect);
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
	pInput->cursor = initCursor();
	pInput->deplacement = 0;
	pInput->screenshot = 0;
	pInput->camCentrer = 0;
	pInput->changeWorms = 0;
	fprintf(logFile, "initInput : SUCCESS.\n\n");
	return pInput;
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
	return curseur;
}


