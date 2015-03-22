#include "input.h"
#include "AffichageGeneral.h"
#include "physique.h"



//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                 Acquisition des Inputs                 /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn void getInput(Input * pInput, SDL_Window* pWindow)
* \brief R�cup�re les inputs.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] pWindow, pointeur pWindow pour r�cup�rer les informations relative � la fen�tre.
*/
void getInput(Input * pInput, SDL_Window* pWindow)
{
	SDL_Event event;
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	Uint32 flags = (SDL_GetWindowFlags(pWindow) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);

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
			}
			pInput->raffraichissement = 1;
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
				break;
			case SDLK_RIGHT:
				pInput->direction = RIGHT;
				break;
			case SDLK_UP:
				pInput->direction = UP;
				break;
			case SDLK_DOWN:
				pInput->direction = DOWN;
				break;
			case SDLK_SPACE:
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
			case SDLK_c:
				if (globalVar.nbWormsEquipe > 1)
				{
					if (pInput->wormsPlaying == 0)
						pInput->wormsPlaying = 1;
					else pInput->wormsPlaying = 0;
				}
				break;
			}
			pInput->raffraichissement = 1;
			break;
		default:
			break;
		}
	}
	//Gestion du plein �cran
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
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] pRenderer pointeur pWindow pour r�cup�rer les informations relative � la fen�tre.
* \param[in] map pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTexture pointeur vers la texture sur laquelle est appliqu� la camera.
* \param[in] camera pointeur vers la structure SDL_Rect de la camera pour modifier ses valeurs.
* \param[in] worms pointeur vers la structure du worms en cours de jeu pour modifier ses param�tres de position.
* \returns int, indicateur si la fonction a bien fonctionn�e (1 = succes, -1 = echec)
*/
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms** worms)
{
	/*if (pInput->right) //Exemple de gestion d'input V1.0, test du booleen
	{
	...
	Code � �x�cuter
	...

	pInput->right = 0;	//remise � z�ro du bool�en (si n�cessaire)
	}*/
	inputsCamera(pInput, pTexture, camera, pRenderer);
	if (pInput->windowResized){
		initCameras(pRenderer, map, camera, NULL);
		pInput->windowResized = 0;
	}
	if (pInput->bombe){
		static int rW, rH;
		SDL_GetRendererOutputSize(pRenderer, &rW, &rH);
		explosion((int)(pInput->cursor.now.x * ((float)camera->w / (float)rW) + camera->x), (int)(pInput->cursor.now.y * ((float)camera->h / (float)rH) + camera->y), 50, map->imageMapSurface, pTexture);
		pInput->bombe = 0;
	}
	if (worms[pInput->wormsPlaying]->vie > 0)
	{
		inputsJumpWorms(pInput, worms[pInput->wormsPlaying]);
		gestionPhysique(map, pTexture, pInput, 0, worms[pInput->wormsPlaying]);
	}
	if (worms[pInput->wormsPlaying]->wormsSurface->clip_rect.y + worms[pInput->wormsPlaying]->wormsSurface->h > (14 * map->imageMapSurface->h / 15) && checkDeplacement(map->imageMapSurface, worms[pInput->wormsPlaying]->wormsSurface, DOWN))
	{
		worms[pInput->wormsPlaying]->wormsSurface->pixels = worms[pInput->wormsPlaying]->wormsSurfaceTomb->pixels;
		updateGlobaleTexture(map->imageMapSurface, worms[pInput->wormsPlaying]->wormsSurface, pTexture, &worms[pInput->wormsPlaying]->wormsRect);
		worms[pInput->wormsPlaying]->vie = 0;
	}
	return 1;	//flag de gestion d'erreur, -1 il y a eu un probl�me, 1 c'est okay
}


/**
* \fn void inputsCamera(Input* pInput, SDL_Texture* pTexture, SDL_Rect* camera, SDL_Renderer * pRenderer)
* \brief Gere les inputs relatives a la camera.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] pTexture pointeur vers la texture sur laquelle est appliqu� la cam�ra.
* \param[in] camera pointeur vers la structure SDL_Rect de la cam�ra pour modifier ses valeurs.
* \param[in] pRenderer pointeur pWindow pour r�cup�rer les informations relative � la fen�tre.
* \returns void
*/
void inputsCamera(Input* pInput, SDL_Texture* pTexture, SDL_Rect* camera, SDL_Renderer * pRenderer)
{
	if (pInput->rclick)
	{
		moveCam(pTexture, camera, pInput); //gestion du scrolling de cam�ra
		pInput->cursor.before = pInput->cursor.now;
	}
	if (pInput->cursor.motion){
		pInput->cursor.before = pInput->cursor.now;
		SDL_GetMouseState(&pInput->cursor.now.x, &pInput->cursor.now.y);
		pInput->cursor.motion = 0;
	}
	if (pInput->wheelUp){
		zoomIn(pRenderer, pTexture, camera, pInput);
		pInput->wheelUp = 0;
	}
	if (pInput->wheelDown){
		zoomOut(pRenderer, pTexture, camera);
		pInput->wheelDown = 0;
	}
}


/**
* \fn void inputsJumpWorms(Input* pInput, Worms* worms)
* \brief Gere les inputs relatives au saut du worms.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] worms, pointeur vers la structure du worms en cours
* \returns void
*/
void inputsJumpWorms(Input* pInput, Worms* worms)
{
	if (!pInput->jumpOnGoing)
	{
		if (pInput->direction == RIGHT || pInput->direction == LEFT)
		{
			worms->dir = pInput->direction;
		}
		if (pInput->jump && (pInput->direction != worms->dirSurface) && pInput->direction != NONE)
		{
			if (worms->dirSurface == RIGHT)
			{
				worms->dir = UPLEFT;
				worms->vitx = -(float)(cos(pi / 3)* 0.95); //saut vers la droite
			}
			else
			{
				worms->dir = UPRIGHT;
				worms->vitx = (float)(cos(pi / 3)*0.95); //saut vers la gauche
			}
			pInput->jumpOnGoing = 1;
			worms->vity = (float)(sin(pi / 3)*1.5);
		}
		else if (pInput->jump && worms->dir == DOWN)
		{
			if (worms->dirSurface == RIGHT)
			{
				worms->dir = UPRIGHT;
				worms->vitx = (float)(cos(pi / 3)* 0.95); //saut vers la droite
			}
			else
			{
				worms->dir = UPLEFT;
				worms->vitx = -(float)(cos(pi / 3)*0.95); //saut vers la gauche
			}
			pInput->jumpOnGoing = 1;
			worms->vity = (float)(sin(pi / 3)*1.3);
		}
		else if (pInput->direction == UP)
		{
			worms->dir = UP;
			worms->vitx = 0;
			pInput->jumpOnGoing = 1;
			worms->vity = (float)(sin(pi / 3)*1.3);
		}
	}
}









//////////////////////////////////////////////////////////////////////////////////////////
/////////////////                                                        /////////////////
/////////////////                    Initialisations                     /////////////////
/////////////////                                                        /////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
* \fn Input* initInput()
* \brief Cr�� et initialise une structure Input.
*
* \returns Input*, pointeur vers la structure cr��e, NULL si echec
*/
Input* initInput()
{
	Input* pInput = NULL;

	pInput = (Input*)malloc(sizeof(Input));
	if (pInput == NULL)
	{
		printf("Erreur lors de l'allocation de memoire de pInput");
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
	pInput->cursor = initCursor();
	pInput->wormsPlaying = 0;
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
