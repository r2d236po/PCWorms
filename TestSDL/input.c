#include "input.h"
#include "AffichageGeneral.h"
#include "Libraries.h"


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
			if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
			{
				pInput->lclick = 1;
				pInput->cursor.before = pInput->cursor.now;
				pInput->raffraichissement = 1;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				pInput->rclick = 1;
				pInput->cursor.before = pInput->cursor.now;
				pInput->raffraichissement = 1;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
			{
				pInput->raffraichissement = 1;
				pInput->lclick = 0;
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				pInput->raffraichissement = 1;
				pInput->rclick = 0;
			}
			break;

		case SDL_MOUSEMOTION:
			pInput->cursor.before = pInput->cursor.now;
			SDL_GetMouseState(&pInput->cursor.now.x, &pInput->cursor.now.y);
			break;

		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0){
				pInput->wheelDown = 1;
				pInput->raffraichissement = 1;
			}
			else
			{
				pInput->wheelUp = 1;
				pInput->raffraichissement = 1;
			}
			break;

		case SDL_KEYDOWN:
			//a optimiser avec une enum DIRECTION dispo dans libraries.h
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				pInput->left = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				pInput->right = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				pInput->up = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				pInput->down = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				pInput->jump = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_LCTRL)
			{
				pInput->bend = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				pInput->menu = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				pInput->quit = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_TAB)
			{
				pInput->weaponTab = 1;
				pInput->raffraichissement = 1;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				if (pInput->acceleration == 1)
				{
					pInput->acceleration = 10;
				}
				else pInput->acceleration = 1;

			}
			else if (event.key.keysym.sym == SDLK_b)
			{
				pInput->bombe = 1;
				pInput->raffraichissement = 1;
			}
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


/**
* \fn int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms)
* \brief G�re les inputs.
* G�n�re les actions correspondant aux inputs.
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'�tat des inputs.
* \param[in] pRenderer pointeur pWindow pour r�cup�rer les informations relative � la fen�tre.
* \param[in] map pointeur Terrain vers la structure du terrain en cours.
* \param[in] pTexture pointeur vers la texture sur laquelle est appliqu� la cam�ra.
* \param[in] cam�ra pointeur vers la structure SDL_Rect de la cam�ra pour modifier ses valeurs.
* \param[in] worms pointeur vers la structure du worms en cours de jeu pour modifier ses param�tres de position.
* \returns int, indicateur si la fonction a bien fonctionn�e (1 = succes, -1 = echec)
*/
int gestInput(Input* pInput, SDL_Renderer * pRenderer, Terrain* map, SDL_Texture* pTexture, SDL_Rect* camera, Worms* worms, SDL_Surface * surfaceTab[])
{
	/*if (pInput->right) //Exemple de gestion d'input V1.0, test du booleen
	{
	...
	Code � �x�cuter
	...

	pInput->right = 0;	//remise � z�ro du bool�en (si n�cessaire)
	}*/
	if (pInput->rclick)
	{
		moveCam(pTexture, camera, pInput); //gestion du scrolling de cam�ra
		pInput->cursor.before = pInput->cursor.now;
	}
	if (pInput->wheelUp){
		zoomIn(pRenderer, camera);
		pInput->wheelUp = 0;
	}
	if (pInput->wheelDown){
		zoomOut(pRenderer, pTexture, camera);
		pInput->wheelDown = 0;
	}
	if (pInput->windowResized){
		initCameras(pRenderer, map, camera);
		pInput->windowResized = 0;
	}
	if (pInput->bombe){
		bombExplo(500, 400, 100, surfaceTab, pTexture);
		pInput->bombe = 0;
	}
	gestionPhysique(map, pInput, 0, worms);
	return 1;	//flag de gestion d'erreur, -1 il y a eu un probl�me, 1 c'est okay
}

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
	pInput->bend = 0;
	pInput->jump = 0;
	pInput->left = 0;
	pInput->right = 0;
	pInput->up = 0;
	pInput->down = 0;
	pInput->quit = 0;
	pInput->lclick = 0;
	pInput->rclick = 0;
	pInput->menu = 0;
	pInput->cursor.before.x = 0;
	pInput->cursor.before.y = 0;
	pInput->cursor.now.x = 0;
	pInput->cursor.now.y = 0;
	pInput->weaponTab = 0;
	pInput->wheelUp = 0;
	pInput->wheelDown = 0;
	pInput->raffraichissement = 1;
	pInput->acceleration = 1;
	pInput->bombe = 0;
	//pInput->direction = NONE;
	return pInput;
}