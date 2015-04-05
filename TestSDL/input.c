#include "input.h"
#include "AffichageGeneral.h"
#include "physique.h"
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
				if (globalVar.nbWormsEquipe > 1 && !pInput->jumpOnGoing)
				{
					if (pInput->wormsPlaying != globalVar.nbWormsEquipe - 1)
						pInput->wormsPlaying += 1;
					else pInput->wormsPlaying = 0;
				}
				pInput->raffraichissement = 1;
				break;
			case SDLK_PRINTSCREEN :
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
	inputsCamera(pInput, pTextureDisplay, pCamera, pRenderer);	//appel de la fonction de gestion des Inputs de la camera
	if (pInput->windowResized){
		initCameras(pRenderer, pMapTerrain, pCamera, NULL);
		pInput->windowResized = 0;
	}
	inputsWeapons(pInput, pTextureDisplay, pCamera, pMapTerrain, pRenderer);	//appel de la fonction de gestion des Inputs des armes
	if (wormsTab[pInput->wormsPlaying]->vie > 0)	//Si le worms en cours de jeu est vivant
	{
		inputsJumpWorms(pInput, wormsTab[pInput->wormsPlaying]);	//appel de la fonction de gestion des Inputs de saut de worms
		gestionPhysique(pMapTerrain, pTextureDisplay, pInput, 0, wormsTab[pInput->wormsPlaying]);	//appel de la fonction de gestion de physique globale
		if (deathByLimitMap(wormsTab[pInput->wormsPlaying], pMapTerrain->imageMapSurface))	//Si le worms tombe en bas de la map
		{
			pInput->jumpOnGoing = 0;	//Remise à 0 du booleen indiquant un saut en cours
			pInput->jump = 0;	//Remise à 0 du booleen de la touche espace
		}
		if (pInput->deplacement)	//Si le worms s'est déplacé
		{
			updateWorms(wormsTab, pMapTerrain->imageMapSurface, pInput, pTextureDisplay);	//appel de la fonction gerant l'update de l'affichage et d'overlay si nécessaire
		}
	}
	if (pInput->screenshot)
	{
		screenshot(pRenderer);
		pInput->screenshot = 0;
	}
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
void inputsCamera(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, SDL_Renderer * pRenderer)
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
}


/**
* \fn void inputsJumpWorms(Input* pInput, Worms* worms)
* \brief Gere les inputs relatives au saut du worms.
*
* \param[in] pInput, pointeur pInput vers la structure qui stocke l'état des inputs.
* \param[in] pWorms, pointeur vers la structure du worms en cours
* \returns void
*/
void inputsJumpWorms(Input* pInput, Worms* pWorms)
{
	/*On verifie qu'on est pas deja dans un saut*/
	if (!pInput->jumpOnGoing)
	{
		//Recuperation des informations relatives à la direction du worms pour un deplacement au sol
		if (pInput->direction == RIGHT || pInput->direction == LEFT)
		{
			pWorms->dir = pInput->direction;	//direction du worms = direction de la fleche du clavier appuyee
			pInput->raffraichissement = 1;	//Variable de raffraichissement
		}
		//Gestion du saut en arriere
		if (pInput->jump && (pInput->direction != pWorms->dirSurface) && pInput->direction != NONE)
		{
			if (pWorms->dirSurface == RIGHT)	//Si le worms est oriente vers la droite
			{
				pWorms->dir = UPLEFT;	//Direction du saut en diagonale haut gauche
				pWorms->vitx = -(float)(cos(pi / 3)* 0.75);	//valeur du vecteur de vitesse horizontale negative pour aller a gauche, legerement inferieur au saut en avant
			}
			else	//Si le worms est oriente vers la gauche
			{
				pWorms->dir = UPRIGHT;	//Direction du saut en diagonale haut droite
				pWorms->vitx = (float)(cos(pi / 3)*0.75);	//valeur du vecteur de vitesse horizontale positive pour aller a droite, legerement inferieur au saut en avant
			}
			pInput->jumpOnGoing = 1;	//mise à 1 du booleen indiquant qu'un saut est en cours
			pWorms->vity = (float)(sin(pi / 3)*1.6);	//valeur du vecteur de vitesse vertical, legerement superieure au saut en avant
		}
		//Gestion du saut classique
		else if (pInput->jump && pWorms->dir == DOWN)
		{
			if (pWorms->dirSurface == RIGHT)	//Si le worms est oriente vers la droite
			{
				pWorms->dir = UPRIGHT;	//Direction du saut en diagonale haut droite
				pWorms->vitx = (float)(cos(pi / 3)* 0.95);	//valeur du vecteur de vitesse horizontale positive pour aller a droite
			}
			else	//Si le worms est oriente vers la gauche
			{
				pWorms->dir = UPLEFT;	//Direction du saut en diagonale haut gauche
				pWorms->vitx = -(float)(cos(pi / 3)*0.95);	//valeur du vecteur de vitesse horizontale negative pour aller a gauche
			}
			pInput->jumpOnGoing = 1;	//mise à 1 du booleen indiquant qu'un saut est en cours
			pWorms->vity = (float)(sin(pi / 3)*1.3);	//valeur du vecteur de vitesse vertical
		}
		//Gestion du saut sur place
		else if (pInput->direction == UP)
		{
			pWorms->dir = UP;	//direction du worms vers le haut
			pWorms->vitx = 0;	//mise a 0 de la valeur du vecteur de vitesse horizontale
			pInput->jumpOnGoing = 1;	//mise à 1 du booleen indiquant qu'un saut est en cours
			pWorms->vity = (float)(sin(pi / 3)*1.3);	//valeur du vecteur de vitesse vertical
		}
	}
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
void inputsWeapons(Input* pInput, SDL_Texture* pTextureDisplay, SDL_Rect* pCamera, Terrain* pMapTerrain, SDL_Renderer * pRenderer)
{
	if (pInput->bombe)
	{
		static int rW, rH;
		Mix_PlayChannel(2, sndFx, 0);

		SDL_GetRendererOutputSize(pRenderer, &rW, &rH);
		explosion((int)(pInput->cursor.now.x * ((float)pCamera->w / (float)rW) + pCamera->x), (int)(pInput->cursor.now.y * ((float)pCamera->h / (float)rH) + pCamera->y), 50, pMapTerrain->imageMapSurface, pTextureDisplay);
		pInput->bombe = 0;
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
	pInput->wormsPlaying = 0;
	pInput->deplacement = 0;
	pInput->screenshot = 0;
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
