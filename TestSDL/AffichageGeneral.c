#include <stdio.h>
#include "AffichageGeneral.h" /* Inclus la SDL.h */

int afficheImage(SDL_Window *pWindow)
{
	//int *x = NULL, *y = NULL;

	SDL_Surface* imageTest = SDL_LoadBMP("../assets/hello.bmp");
	if (imageTest == NULL)   /*test loadBmp image*/
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return -1;
	};

	//SDL_GetWindowSize(pWindow, x, y);
	SDL_Rect dest = { 1080 / 2 - imageTest->w / 2, 600 / 2 - imageTest->h / 2, 0, 0 };
	SDL_BlitSurface(imageTest, NULL, SDL_GetWindowSurface(pWindow), &dest);
	SDL_UpdateWindowSurface(pWindow);
}

void mainFenetre(){

	int quit = 0, fullscreen = 0;
	SDL_Event event;
	SDL_Window* pWindow = NULL;

	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	{
		/* Création de la fenêtre */
		pWindow = creerFenetre(1080, 600, "KaamWorms");

		if (pWindow != NULL){
			afficheImage(pWindow);
			while (quit != 1){ /* continue tant qu'on ne clique pas sur la croix !! */
				SDL_UpdateWindowSurface(pWindow);
				while (SDL_PollEvent(&event)) /* Récupération des actions de l'utilisateur */
				{
					switch (event.type)
					{
					case SDL_QUIT: /* Clic sur la croix */
						quit = 1;
						break;
					case SDL_KEYUP: /* Relâchement d'une touche */
						if (event.key.keysym.sym == SDLK_ESCAPE){
							quit = 1;
							break;
						}
						if (event.key.keysym.sym == SDLK_KP_ENTER) /* Touche enter pour passer en plein écran ! */
						{
							/* Alterne du mode plein écran au mode fenêtré */
							if (fullscreen == 0)
							{
								fullscreen = 1;
								SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN);
							}
							else if (fullscreen == 1)
							{
								fullscreen = 0;
								SDL_SetWindowFullscreen(pWindow, 0);
							}
						}
						break;
					}
				}
			}
			SDL_DestroyWindow(pWindow);
		}
	}

	SDL_Quit();
}

SDL_Window * creerFenetre(const int x, const int y, const char * nom){
	SDL_Window * pWindow = NULL;
	pWindow = SDL_CreateWindow(nom, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		x,
		y,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (pWindow == NULL)
	{
		fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
		return NULL;
	}
	else return pWindow;
}