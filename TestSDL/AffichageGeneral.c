#include "AffichageGeneral.h"
#include "Libraries.h" //Inclus toutes les librairies



int mainFenetre()
{
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Renderer* pRenderer = NULL; //déclaration du renderer
	SDL_Window* pWindow = NULL;
	Input * pInput = malloc(sizeof(Input)); //structure contenant les informations relatives aux inputs clavier
	Terrain * mainMap = malloc(sizeof(Terrain));
	SDL_Rect camera = { 0, 0, 0, 0 }; // rect(x,y,w,h)
	SDL_Rect rect = { 0, 0, 25, 25 };
	SDL_Surface * surfaceCollision = NULL;
	int x = 0, y = 0;
	int XE = 0, YE = 0;
	SDL_Surface * Worms = loadImage("../assets/pictures/worms.png");
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
		surfaceCollision = SDL_CreateRGBSurface(mainMap->imageMapSurface->flags,
			mainMap->imageMapSurface->w,
			mainMap->imageMapSurface->h,
			mainMap->imageMapSurface->format->BitsPerPixel,
			mainMap->imageMapSurface->format->Rmask,
			mainMap->imageMapSurface->format->Gmask,
			mainMap->imageMapSurface->format->Bmask,
			mainMap->imageMapSurface->format->Amask);
		SDL_Texture* Worms_texture = SDL_CreateTextureFromSurface(pRenderer, Worms);

		rect.w = Worms->clip_rect.w;
		rect.h = Worms->clip_rect.h;
		updateScreen(pRenderer, &camera, &surfaceCollision, 2, 0, mainMap, 1, Worms_texture, &rect, NULL);
		while (!(pInput->quit))
		{
			//Récupération des inputs
			getInput(pInput, pWindow);

			//Gestion des inputs
			if (!gestInput(pInput, pRenderer, mainMap, &camera))
			{
				printf("Erreur lors du traitement de l'entree");
			}
			if (pInput->right)
			{
				rect.x += 1;
				Worms->clip_rect.x = rect.x;
				Worms->clip_rect.y = rect.y;
				if (detectionCollisionSurface(pRenderer, surfaceCollision, &XE, &YE, Worms))
				{
					rect.x -= 1;
					Worms->clip_rect.x = rect.x;
					Worms->clip_rect.y = rect.y;
				}
				pInput->right = 0;
			}
			if (pInput->left)
			{
				rect.x -= 1;
				Worms->clip_rect.x = rect.x;
				Worms->clip_rect.y = rect.y;
				if (detectionCollisionSurface(pRenderer, surfaceCollision, &XE, &YE, Worms))
				{
					rect.x += 1;
					Worms->clip_rect.x = rect.x;
					Worms->clip_rect.y = rect.y;
				}
				pInput->left = 0;
			}
			if (pInput->down)
			{
				rect.y += 1;
				Worms->clip_rect.x = rect.x;
				Worms->clip_rect.y = rect.y;
				if (detectionCollisionSurface(pRenderer, surfaceCollision, &XE, &YE, Worms))
				{
					rect.y -= 1;
					Worms->clip_rect.x = rect.x;
					Worms->clip_rect.y = rect.y;
				}
				pInput->down = 0;
			}
			if (pInput->up)
			{
				rect.y -= 1;
				Worms->clip_rect.x = rect.x;
				Worms->clip_rect.y = rect.y;
				if (detectionCollisionSurface(pRenderer, surfaceCollision, &XE, &YE, Worms))
				{
					rect.y += 1;
					Worms->clip_rect.x = rect.x;
					Worms->clip_rect.y = rect.y;
				}
				pInput->up = 0;
			}
			//Update de l'écran
			if (pInput->raffraichissement)
			{
				updateScreen(pRenderer, &camera, &surfaceCollision, 2, 0, mainMap, 1, Worms_texture, &rect, &camera);
			}

			//Gestion du frame Rate
			frameRate(frame_max);
			frame_max = SDL_GetTicks() + FRAME_RATE;
		}
		SDL_DestroyTexture(mainMap->imageBackground);
		SDL_DestroyTexture(mainMap->imageMap);
		SDL_FreeSurface(mainMap->imageMapSurface);
		SDL_FreeSurface(Worms);
		SDL_DestroyRenderer(pRenderer);
		SDL_DestroyWindow(pWindow);
	}
	IMG_Quit();
	SDL_Quit();
	free(mainMap);
	free(pInput);
	return 0;
}




int sandboxRenderer()
{
	int closeWindow = 0;
	Point p1, p2;
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
				SDL_GetMouseState(&p2.x, &p2.y); //Initialisationn pour affichage ligne
				afficherPoint(renderer);
				break;

			case SDL_MOUSEBUTTONUP:
				click = 0;/*Booleen de démémorisation du click*/
				break;

			case SDL_MOUSEMOTION:
				if (click)/*Trace les points en suivant la souris, ne pas aller trop vite*/
				{
					afficherLigne(renderer, &p1, &p2);
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
void deplacementRectangle(SDL_Renderer * pRenderer, SDL_Rect * rect, int x2, int y2, int dir)
{
	int x1 = 0, y1 = 0;
	SDL_GetMouseState(&x1, &y1);
	if (dir == 1)
	{
		rect->x = rect->x + (x1 - x2);
		rect->y = rect->y + (y1 - y2);
	}
	else if (dir == -1)
	{
		rect->x = rect->x - (x1 - x2);
		rect->y = rect->y - (y1 - y2);
	}
}

//Affiche un point aux coordonnées de la souris
void afficherPoint(SDL_Renderer * r)
{
	static int x, y;
	SDL_GetMouseState(&x, &y);
	SDL_RenderDrawPoint(r, x, y);
	SDL_RenderPresent(r);
}

void afficherLigne(SDL_Renderer * r, Point * p1, Point * p2)
{
	p1->x = p2->x;
	p1->y = p2->y;
	SDL_GetMouseState(&p2->x, &p2->y);
	SDL_RenderDrawLine(r, p1->x, p1->y, p2->x, p2->y);
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
			break;
		default:
			pInput->raffraichissement = 0;
			break;
		}
		//Gestion du plein écran
		if (keystate[SDL_SCANCODE_RETURN] && (keystate[SDL_SCANCODE_RCTRL] || keystate[SDL_SCANCODE_LCTRL]))
		{
			if (SDL_SetWindowFullscreen(pWindow, flags) < 0)
				printf("ERROR lors du passage en mode fenetre : %s", SDL_GetError());
			SDL_Delay(10);
		}
	}
}

//Gestion des input
int gestInput(Input* pInput, SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect* camera)
{
	/*if (pInput->right) //Exemple de gestion d'input V1.0, test du booleen
	{
	...
	Code à éxécuter
	...

	pInput->right = 0;	//remise à zéro du booléen (si nécessaire)
	}*/
	if (pInput->rclick)
	{
		moveCam(pTexture, camera, pInput); //gestion du scrolling de caméra
	}
	if (pInput->wheelUp){
		zoomIn(pRenderer, camera);
		pInput->wheelUp = 0;
	}
	if (pInput->wheelDown){
		zoomOut(pRenderer, pTexture, camera);
		pInput->wheelDown = 0;
	}

	return 1;	//flag de gestion d'erreur, 0 il y a eu un problème, 1 c'est okay
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
}

/*affichage de la frame actuelle */
void updateScreen(SDL_Renderer * pRenderer, SDL_Rect * camera, SDL_Surface** pSurface, int nb, ...)
{
	SDL_Rect* rect = NULL;
	SDL_Rect* rect2 = NULL;
	SDL_Rect temp = { 0, 0, 0, 0 };
	Terrain * map = NULL;
	SDL_Texture* text = NULL;
	void * pixel;
	va_list list;
	Uint32 rgb = 0;
	int i = 0, w = 0, h = 0;
	va_start(list, nb);
	for (i = 0; i < nb; i++)
	{
		switch (va_arg(list, int))
		{
		case 0:
			map = va_arg(list, Terrain*);
			SDL_GetRendererOutputSize(pRenderer, &w, &h);
			temp.w = w;
			temp.h = h;
			SDL_RenderCopy(pRenderer, map->imageBackground, NULL, &temp);
			SDL_RenderCopy(pRenderer, map->imageMap, camera, &temp);
			if (pSurface != NULL)
			{
				*pSurface = crop_surface(map->imageMapSurface, camera->x, camera->y, camera->w, camera->h);
			}
			text  = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, w, h);
			SDL_RenderReadPixels(pRenderer, NULL, SDL_PIXELFORMAT_ABGR8888, &pixel, 4 * w);
			Uint32 * pixels = (Uint32*)pixel;
			SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);
			SDL_UpdateTexture(text, NULL,pixels, 4 * camera->w);
			SDL_RenderCopy(pRenderer, text, NULL, NULL);
			break;
		case 1:
			text = va_arg(list, SDL_Texture*);
			rect = va_arg(list, SDL_Rect*);
			rect2 = va_arg(list, SDL_Rect*); 			
			SDL_RenderCopy(pRenderer, text, rect2, rect);
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
	rect = NULL;
	SDL_RenderPresent(pRenderer);
	SDL_DestroyTexture(text);
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

//init de la cameras sur le 0/0
void initCameras(const SDL_Window * pWindow, SDL_Rect * camera){
	int w = 0, h = 0;
	SDL_GetWindowSize(pWindow, &w, &h);
	camera->x = 0;
	camera->y = 0;
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
	/*Initialisation SDL_Image*/
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		printf("Erreur Img_init : %s", SDL_GetError());
		return -1;
	}
}

//movCam
void moveCam(Terrain * map, SDL_Rect * camera, Input * pInput)
{
	int w = 0, h = 0;
	SDL_QueryTexture(map->imageMap, NULL, NULL, &w, &h);
	camera->x = camera->x - (pInput->cursor.now.x - pInput->cursor.before.x);
	camera->y = camera->y - (pInput->cursor.now.y - pInput->cursor.before.y);
	if (camera->x + camera->w > w){
		camera->x = w - camera->w;
	}
	if (camera->y + camera->h > h){
		camera->y = h - camera->h;
	}
	if (camera->x < 0){
		camera->x = 0;
	}
	if (camera->y < 0){
		camera->y = 0;
	}
	pInput->cursor.before = pInput->cursor.now;
}

//ZoomCamera grossissement
void zoomIn(SDL_Renderer * fenetre, SDL_Rect * camera)
{
	int w = 0, h = 0;
	SDL_GetRendererOutputSize(fenetre, &w, &h);
	camera->x = camera->x + (camera->w) / 2;
	camera->y = camera->y + (camera->h) / 2;
	camera->w = camera->w - 20 * ((float)w / (float)h);// keep the ratio depending of the size of the window!!!!!
	camera->h = camera->h - 20;
	camera->x = camera->x - (camera->w) / 2;
	camera->y = camera->y - (camera->h) / 2;
}

//ZoomCamera retrécissement
void zoomOut(SDL_Renderer * fenetre, Terrain * map, SDL_Rect * camera)
{
	int w = 0, h = 0, wM = 0, hM = 0;
	SDL_GetRendererOutputSize(fenetre, &w, &h);
	SDL_QueryTexture(map->imageMap, NULL, NULL, &wM, &hM);
	camera->x = camera->x + (camera->w) / 2;
	camera->y = camera->y + (camera->h) / 2;
	camera->w = camera->w + 20 * ((float)w / (float)h) + 1;
	camera->h = camera->h + 20;
	if (camera->w > w){
		camera->w = w;
	}
	if (camera->h > h){
		camera->h = h;
	}
	camera->x = camera->x - ((camera->w) / 2);
	camera->y = camera->y - ((camera->h) / 2);
	if (camera->x + camera->w > wM){
		camera->x = wM - camera->w;
	}
	if (camera->y + camera->h > hM){
		camera->y = hM - camera->h;
	}
	if (camera->x < 0){
		camera->x = 0;
	}
	if (camera->y < 0){
		camera->y = 0;
	}
}


SDL_Surface* crop_surface(SDL_Surface* sprite_sheet, int x, int y, int width, int height)
{
	SDL_Surface* surfaceTemp = SDL_CreateRGBSurface(sprite_sheet->flags, width, height, sprite_sheet->format->BitsPerPixel, sprite_sheet->format->Rmask, sprite_sheet->format->Gmask, sprite_sheet->format->Bmask, sprite_sheet->format->Amask);
	SDL_Surface* surface = NULL;
	SDL_Rect rect = { x, y, width, height };
	SDL_BlitSurface(sprite_sheet, &rect, surfaceTemp, 0);
	surface = surfaceTemp;
	SDL_FreeSurface(surfaceTemp);
	surfaceTemp = NULL;

	return surface;
}