#include "Libraries.h" //Inclus toutes les librairies

int mainFenetre2()
{
	int closeWindow = 0, i = 0, x1 = 0, y1 = 0;
	int click = 0;
	SDL_Event event;
	SDL_Renderer* renderer = NULL; //d�claration du renderer
	SDL_Window* pWindow = NULL;
	SDL_Rect rect1 = { 0, 0, 50, 50 };

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		printf("�chec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	/* Cr�ation de la fen�tre */
	pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (pWindow == NULL)
	{
		fprintf(stderr, "Erreur de cr�ation de la fen�tre: %s\n", SDL_GetError());
		return NULL;
	}
	else return pWindow;
	}
	
int afficheImage(SDL_Window *pWindow, SDL_Surface * image)
	{
	SDL_Rect dest = { 1080 / 2 - image->w / 2, 600 / 2 - image->h / 2, 50, 50 };
	SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(pWindow), &dest);
}

//Fonction de cr�ation de surface
SDL_Surface * loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
				{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		return NULL;
	};
	return image;
				}

void AfficherPoint(SDL_Renderer * r)
				{
	int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_RenderDrawPoint(r, x, y);
	SDL_RenderPresent(r);
				}

void deplacementRectangle(SDL_Renderer * rend, SDL_Rect * rect, int x2, int y2)
				{
	int x1, y1;
	SDL_GetMouseState(&x1, &y1);
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderFillRect(rend, rect);
	rect->x = rect->x + (x1 - x2);
	rect->y = rect->y + (y1 - y2);
	SDL_SetRenderDrawColor(rend, 210, 50, 60, 255);
	SDL_RenderFillRect(rend, rect);
	SDL_RenderPresent(rend);
		}

int sandboxRenderer()
{
	int closeWindow = 0;
	int click = 0;
	SDL_Event event;
	SDL_Renderer* renderer = NULL; //d�claration du renderer
	SDL_Window* pWindow = NULL;

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		printf("�chec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	/* Cr�ation de la fen�tre */
	pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (pWindow == NULL)
	{
		SDL_Quit();
		return -1;
	}
	/* Cr�ation du renderer */
	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)//gestion des erreurs
	{
		printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return -1;
	}

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	while (!closeWindow)
	{

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				closeWindow = 1;
				break;

			case SDL_MOUSEBUTTONDOWN:
				click = 1;/*Booleen de m�morisation du click*/
				if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
				{SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{SDL_SetRenderDrawColor(renderer, 50, 210, 60, 255);}
				else if (event.button.button == SDL_BUTTON_MIDDLE)
				{SDL_SetRenderDrawColor(renderer, 50, 60, 210, 255);}
				SDL_GetMouseState(&x1, &y1);
				SDL_RenderDrawPoint(renderer, x1, y1);
				SDL_RenderPresent(renderer);
				break;

			case SDL_MOUSEBUTTONUP:
				click = 0;/*Booleen de d�m�morisation du click*/
				break;

			case SDL_MOUSEMOTION:
				if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
				{
					SDL_GetMouseState(&x1, &y1);
					SDL_RenderDrawPoint(renderer, x1, y1);
					SDL_RenderPresent(renderer);
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_c) /*Clear de la fen�tre*/
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				break;

			default:
				break;
			}
		}

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}

int mainFenetre() {

	int quit = 0, fullscreen = 0;
	SDL_Event event;
	SDL_Window* pWindow = NULL;

	/* Initialisation simple */
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("�chec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	/* Cr�ation de la fen�tre */
	pWindow = creerFenetre(1080, 600, "KaamWorms");

	/*Chargement image*/
	SDL_Surface * wormsLeft = loadImage("../assets/pictures/worms_left.png");
	SDL_Surface * wormsRight = loadImage("../assets/pictures/worms_right.png");
	SDL_Surface * icon = loadImage("../assets/pictures/worms.bmp");
	SDL_SetWindowIcon(pWindow, icon);

	if (pWindow != NULL){
		while (quit != 1)
		{
			SDL_PumpEvents(); // On demande � la SDL de mettre � jour les �tats sur les p�riph�riques

			// Clavier
			{
				Uint8 * pKeyStates = SDL_GetKeyboardState(NULL);
				if (pKeyStates[SDL_SCANCODE_ESCAPE])
				{
					quit = 1;
				}
				SDL_Keymod mod = SDL_GetModState();
				if (mod != KMOD_NONE)
				{
					printf("Vous avez appuy� sur une touche sp�ciale : %d\n", mod);
				}
				if (pKeyStates[SDL_SCANCODE_LEFT]){
					afficheImage(pWindow, wormsLeft);//affichage du worms si arrow left
				}
				if (pKeyStates[SDL_SCANCODE_RIGHT]){
					afficheImage(pWindow, wormsRight);// affichage du worms si arrow right
				}
			}
			printf("\n");
			// Souris
			{
				int x = 0;
				int y = 0;
				Uint32 boutons = SDL_GetMouseState(&x, &y);

				printf("Position de la souris : %d;%d\n", x, y);
				printf("Bouton de la souris : %d\n", boutons);

				SDL_GetRelativeMouseState(&x, &y);
				printf("D�placement de la souris : %d;%d\n", x, y);
			}
			printf("\n");

			while (SDL_PollEvent(&event)) /* R�cup�ration des actions de l'utilisateur */
			{
				switch (event.type)
				{
				case SDL_QUIT: /* Clic sur la croix */
					quit = 1;
					break;
				case SDL_KEYUP: /* Rel�chement d'une touche */
					if (event.key.keysym.sym == SDLK_KP_ENTER) /* Touche enter pour passer en plein �cran ! */
					{
						/* Alterne du mode plein �cran au mode fen�tr� */
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
				}

				SDL_UpdateWindowSurface(pWindow);
			}
		}
		SDL_DestroyWindow(pWindow);
	}
	SDL_FreeSurface(wormsLeft);
	SDL_FreeSurface(wormsRight);
	SDL_Quit();
}


int mainFenetre2()
{
	int closeWindow = 0, x2 = 0, y2 = 0;
	int click = 0;
	SDL_Event event;
	SDL_Renderer* renderer = NULL; //d�claration du renderer
	SDL_Window* pWindow = NULL;
	SDL_Rect rect1 = { 0, 0, 50, 50 };

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		fprintf(stdout, "�chec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	/* Cr�ation de la fen�tre */
	pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (pWindow == NULL)
	{
		SDL_Quit();
		return -1;
	}
	/* Cr�ation du renderer */
	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)//gestion des erreurs
	{
		fprintf(stdout, "Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return -1;
}

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);
	SDL_RenderDrawRect(renderer, &rect1);
	SDL_RenderPresent(renderer);
	while (!closeWindow)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				closeWindow = 1;
				break;

			case SDL_MOUSEBUTTONDOWN:

				if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
{
					SDL_GetMouseState(&x2, &y2);
					if (((x2 >= (rect1.x) && x2 <= (rect1.x + rect1.w))) && (((y2 >= (rect1.y) && y2 <= (rect1.y + rect1.h)))))
					{
						click = 1;/*Booleen de m�morisation du click*/
					}
					/*else
					{
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &rect1);
					SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);
					SDL_RenderDrawRect(renderer, &rect1);
					}*/
}
				break;

			case SDL_MOUSEBUTTONUP:
				click = 0;/*Booleen de d�m�morisation du click*/
				SDL_RenderPresent(renderer);
				break;

			case SDL_MOUSEMOTION:
				if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
				{

					deplacementRectangle(renderer, &rect1, x2, y2);
					SDL_GetMouseState(&x2, &y2);
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_c) /*Clear de la fen�tre*/
	{
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				break;

			default:
				break;
			}
		}

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}