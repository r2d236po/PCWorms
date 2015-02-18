#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies

int mainFenetre()
{
	int closeWindow = 0, x1 = 0, y1 = 0, fullscreen = 0;
	int click = 0;
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Renderer* renderer = NULL; //déclaration du renderer
	SDL_Window* pWindow = NULL;
	Terrain * mainMap = malloc(sizeof(Terrain));
	SDL_Rect rect1 = { 0, 0, 50, 50 };
	SDL_Rect camera;

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

	clearRenderer(renderer);

	initialisionTerrain(mainMap, renderer, "../assets/pictures/fond2.png", "../assets/pictures/map.png");
	SDL_SetRenderDrawColor(renderer, 210, 50, 60, 255);
	SDL_RenderDrawRect(renderer, &rect1);
	SDL_RenderPresent(renderer);

	while (!closeWindow)
	{

		click = 0;/*Booleen de démémorisation du click*/
		SDL_GetMouseState(&x1, &y1);
		if (((x1 >= rect1.x) && x1 <= (rect1.x + rect1.w)) && ((y1 >= rect1.y) && y1 <= (rect1.y + rect1.h)))
		{
			click = 1;/*Booleen de mémorisation du click*/
		}
		if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
		{
			deplacementRectangle(renderer, &rect1, x1, y1);
			SDL_GetMouseState(&x1, &y1);
		}
		updateScreen(renderer, 3, 0, mainMap, 2, 0xD2323CFF, &rect1);
		frameRate(frame_max);
		frame_max = SDL_GetTicks() + FRAME_RATE;
	}
	SDL_DestroyTexture(mainMap->imageBackground);
	SDL_DestroyTexture(mainMap->imageMap);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	free(mainMap);
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
void clearRenderer(SDL_Renderer * r)
{
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderClear(r);
	SDL_RenderPresent(r);
}

//Gestion des input
void getInput(Input * input)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			break;

		case SDL_MOUSEBUTTONDOWN:

			break;

		case SDL_MOUSEBUTTONUP:
			break;

		case SDL_MOUSEMOTION:
			break;

		case SDL_KEYUP:
			break;

		default:
			break;
		}
	}
}

/*affichage de la frame actuelle */
void updateScreen(SDL_Renderer * pRenderer, int nb, ...)
{
	SDL_Rect back = { 0, 0, 0, 0 };
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
			SDL_QueryTexture(map->imageBackground, NULL, NULL, &back.w, &back.h);
			SDL_RenderCopy(pRenderer, map->imageBackground, NULL, &back);
			SDL_RenderSetViewport(pRenderer, &back);
			SDL_QueryTexture(map->imageMap, NULL, NULL, &back.w, &back.h);
			SDL_RenderCopy(pRenderer, map->imageMap, NULL, &back);
			break;
		case 1:
			text = va_arg(list, SDL_Texture*);
			SDL_QueryTexture(text, NULL, NULL, &back.w, &back.h);
			SDL_RenderCopy(pRenderer, text, NULL, &back);
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
void cameras(const SDL_Window * pWindow,SDL_Rect * camera){
	/*int x = 0, y = 0;
	camera.x = SDL_GetWindowSize(pWindow, x, y);*/
}
