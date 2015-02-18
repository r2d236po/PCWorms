#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

int mainFenetre()
{
	int closeWindow = 0, x1 = 0, y1 = 0, fullscreen = 0;
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Renderer* pRenderer = NULL; //déclaration du renderer
	SDL_Window* pWindow = NULL;
	Input * pInput = malloc(sizeof(Input)); //structure contenant les informations relatives aux inputs clavier
	Terrain * mainMap = malloc(sizeof(Terrain));
	SDL_Rect camera = { 0, 0, 0, 0 };
	SDL_Rect rect1 = { 0, 0, 50, 50 };

	//init SDL + fenetre + renderer
	if (initSWR(&pWindow, &pRenderer))
	{
		//Initialisation de la caméra
		initCameras(pWindow, &camera);

		//Initialisation des inputs
		initInput(pInput);

		//Initialisation du terrain
		clearRenderer(pRenderer);
		initialisionTerrain(mainMap, pRenderer, "../assets/pictures/fond2.png", "../assets/pictures/map.png");

		while (!(pInput->quit))
		{
			//Récupération des inputs
			getInput(pInput);

			//Gestion des inputs
			if (!gestInput(pInput))
			{
				printf("Erreur lors du traitement de l'entree");
			}

			//Update de l'écran
			updateScreen(pRenderer, 3, 0, mainMap, 2, 0xD2323CFF, &rect1);

			//Gestion du frame Rate
			frameRate(frame_max);
			frame_max = SDL_GetTicks() + FRAME_RATE;
		}
		SDL_DestroyTexture(mainMap->imageBackground);
		SDL_DestroyTexture(mainMap->imageMap);
		SDL_DestroyRenderer(pRenderer);
		SDL_DestroyWindow(pWindow);
	}
	SDL_Quit();
	free(mainMap);
	free(pInput);
	return 0;
}



int sandboxRenderer()
{
	int closeWindow = 0;
	int click = 0;
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Event event;
	SDL_Renderer* renderer = NULL; //déclaration du renderer
	SDL_Window* pWindow = NULL;

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		printf("Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}

	/* Création de la fenêtre */
	pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (pWindow == NULL)
	{
		SDL_Quit();
		return -1;
	}

	/* Création du renderer */
	renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)//gestion des erreurs
	{
		printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return -1;
	}

	SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);
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
				click = 1;/*Booleen de mémorisation du click*/
				if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
					SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);
				else if (event.button.button == SDL_BUTTON_RIGHT)
					SDL_SetRenderDrawColor(renderer, 50, 210, 60, 255);
				else if (event.button.button == SDL_BUTTON_MIDDLE)
					SDL_SetRenderDrawColor(renderer, 50, 60, 210, 255);

				afficherPoint(renderer);
				break;

			case SDL_MOUSEBUTTONUP:
				click = 0;/*Booleen de démémorisation du click*/
				break;

			case SDL_MOUSEMOTION:
				if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
				{
					afficherPoint(renderer);
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_c) /*Clear de la fenêtre*/
				{
					clearRenderer(renderer);
				}
				break;

			default:
				break;
			}
		}
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}

//Fonction de création de fenêtre
SDL_Window * creerFenetre(const int w, const int h, const char * nom){
	SDL_Window * pWindow = NULL;
	pWindow = SDL_CreateWindow(nom,	//nom de la fenêtre
		SDL_WINDOWPOS_CENTERED, //position en x de la fenêtre
		SDL_WINDOWPOS_CENTERED,	//position en y de la fenêtre
		w,	//largeur de la fenêtre
		h,	//hauteur de la fenêtre
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);	//propriétés supplémentaires de la fenêtre
	if (pWindow == NULL)
	{
		fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
		return NULL;
	}
	else return pWindow;
}

//Chargement image SDL_image
int afficheImage(SDL_Window *pWindow, SDL_Surface * image)
{
	SDL_Rect dest = { 1080 / 2 - image->w / 2, 600 / 2 - image->h / 2, 50, 50 };
	SDL_BlitSurface(image, NULL, SDL_GetWindowSurface(pWindow), &dest);
}

//Fonction de chargement d'image dans une surface
SDL_Surface * loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
	{
		printf("Unable to load image: %s\n", SDL_GetError());
		return NULL;
	}
	return image;
}

//Fonction de chargement d'image dans une texture à  partir d'une surface
SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file)
{

	SDL_Texture * texture = IMG_LoadTexture(pRenderer, file);
	if (texture == NULL)
	{
		printf("Unable to load texture: %s\n", SDL_GetError());
		return NULL;
	}
	return texture;
}

//Déplace un rectangle entre les coordonnées précédentes (x2 et y2) de la souris et celles actuelles
void deplacementRectangle(SDL_Renderer * pRenderer, SDL_Rect * rect, int x2, int y2)
{
	int x1, y1;
	SDL_GetMouseState(&x1, &y1);
	rect->x = rect->x + (x1 - x2);
	rect->y = rect->y + (y1 - y2);
}

//Affiche un point aux coordonnées de la souris
void afficherPoint(SDL_Renderer * r)
{
	static int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_RenderDrawPoint(r, x, y);
	SDL_RenderPresent(r);
}

//Clear noir du renderer
void clearRenderer(SDL_Renderer * pRenderer)
{
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);
	SDL_RenderPresent(pRenderer);
}

//Récupération des input
void getInput(Input * pInput)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			pInput->quit = 1;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
				pInput->click = 1;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				pInput->weaponTab = 1;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)/*Test du bouton de la souris*/
				pInput->click = 0;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				pInput->weaponTab = 0;
			break;

		case SDL_MOUSEMOTION:
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				pInput->left = 1;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				pInput->right = 1;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				pInput->up = 1;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				pInput->down = 1;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				pInput->jump = 1;
			}
			else if (event.key.keysym.sym == SDLK_LCTRL)
			{
				pInput->bend = 1;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				pInput->menu = 1;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				pInput->quit = 1;
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				pInput->left = 0;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT)
			{
				pInput->right = 0;
			}
			else if (event.key.keysym.sym == SDLK_UP)
			{
				pInput->up = 0;
			}
			else if (event.key.keysym.sym == SDLK_DOWN)
			{
				pInput->down = 0;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				pInput->jump = 0;
			}
			else if (event.key.keysym.sym == SDLK_LCTRL)
			{
				pInput->bend = 0;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				pInput->menu = 0;
			}
			break;

		default:
			break;
		}
	}
}

//Gestion des input
int gestInput(Input* pInput)
{
	return 1;
}

//Init des input
void initInput(Input* pInput)
{
	pInput->bend = 0;
	pInput->jump = 0;
	pInput->left = 0;
	pInput->right = 0;
	pInput->up = 0;
	pInput->down = 0;
	pInput->quit = 0;
	pInput->click = 0;
	pInput->weaponTab = 0;
	pInput->menu = 0;
}

/*affichage de la frame actuelle */
void updateScreen(SDL_Renderer * pRenderer, SDL_Rect * camera, int nb, ...)
{
	Terrain * map = NULL;
	SDL_Texture* text = NULL;
	va_list list;
	Uint32 rgb = 0;
	int i = 0;
	va_start(list, nb);
	for (i = 0; i < nb; i++)
	{
		switch (va_arg(list, int))
		{
		case 0:
			map = va_arg(list, Terrain*);
			SDL_QueryTexture(map->imageBackground, NULL, NULL, &camera->w, &camera->h);
			SDL_RenderCopy(pRenderer, map->imageBackground, NULL, camera);
			SDL_RenderSetViewport(pRenderer, camera);
			SDL_QueryTexture(map->imageMap, NULL, NULL, &camera->w, &camera->h);
			SDL_RenderCopy(pRenderer, map->imageMap, NULL, camera);
			break;
		case 1:
			text = va_arg(list, SDL_Texture*);
			SDL_QueryTexture(text, NULL, NULL, &camera->w, &camera->h);
			SDL_RenderCopy(pRenderer, text, NULL, camera);
			break;
		case 2:
			rgb = va_arg(list, Uint32);
			SDL_SetRenderDrawColor(pRenderer, rgb >> 24, (rgb >> 16) & 0x00FF, (rgb >> 8) & 0x0000FF, (rgb & 0x000000FF));
			SDL_RenderFillRect(pRenderer, va_arg(list, SDL_Rect*));
			break;
		default:
			break;
		}
	}
	map = NULL;
	text = NULL;
	SDL_RenderPresent(pRenderer);
	va_end(list);
}

//gestion du frame rate pour pas bouffer le cpu
void frameRate(int fM)
{
	unsigned int tick = SDL_GetTicks();
	if (fM < tick)
	{
		return;
	}
	else if (fM > tick + FRAME_RATE)
	{
		SDL_Delay(FRAME_RATE);
	}
	else // (fM > tick)
	{
		SDL_Delay(fM - tick);
	}
}

//gestion cameras
void initCameras(const SDL_Window * pWindow, SDL_Rect * camera){
	int w = 0, h = 0;
	SDL_GetWindowSize(pWindow, &w, &h);
	camera->x = w / 2;
	camera->y = h / 2;
	camera->w = w;
	camera->h = h;

	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > w - camera->w)
	{
		camera->x = w - camera->w;
	}
	if (camera->y > h - camera->h)
	{
		camera->y = h - camera->h;
	}

}

//Initialisation fenetre renderer
int initSWR(SDL_Window** pWindow, SDL_Renderer **pRenderer)
{

	/* Initialisation simple */
	if (SDL_VideoInit(NULL) < 0)
	{
		printf("Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
		return -1;
	}
	/* Création de la fenêtre */
	*pWindow = creerFenetre(1080, 600, "KaamWorms");
	if (*pWindow == NULL)
	{
		SDL_Quit();
		return -1;
	}
	/* Création du renderer */
	*pRenderer = SDL_CreateRenderer(*pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (*pRenderer == NULL)//gestion des erreurs
	{
		printf("Erreur lors de la creation d'un renderer : %s", SDL_GetError());
		return -1;
	}

	return 1;
}