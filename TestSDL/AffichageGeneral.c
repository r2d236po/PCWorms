#include "AffichageGeneral.h"
#include "input.h"



int mainFenetre()
{
	unsigned int frame_max = SDL_GetTicks() + FRAME_RATE;
	SDL_Renderer * pRenderer = NULL; //déclaration du renderer
	SDL_Window * pWindow = NULL;	//déclaration de la window
	Input * pInput = NULL; //structure contenant les informations relatives aux inputs clavier
	Terrain * mainMap = NULL;
	SDL_Texture * display = NULL;	//Texture globale
	Worms * worms1 = NULL; //worms
	SDL_Rect camera = { 0, 0, 0, 0 }; // rect(x,y,w,h)


	//init SDL + fenetre + renderer
	if (initSWR(&pWindow, &pRenderer))
	{
		//Initialisation des inputs
		pInput = initInput();
		if (pInput == NULL)
		{
			printf("Erreur lors de l'allocation de pInput");
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}
		//Initialisation du terrain
		if (initialisionTerrain(&mainMap, pRenderer, "../assets/pictures/fond2.png", "../assets/pictures/maptest3.png") < 0)
		{
			printf("Probleme lors de la creation du terrain");
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}
		   
		initialisationPolice(); /*Ouvre des polices pour le HUD*/


		//Initialisation worms
		worms1 = createWorms("../assets/pictures/wormsg.png", "../assets/pictures/wormsd.png");
		if (worms1 == NULL)
		{
			printf("Erreur creation worms");
			destroyMap(&mainMap);
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;
		}
		//Initialisation des caméras
		initCameras(pRenderer, mainMap, &camera, worms1);

		//Initialisation de l'affichage
		if (createGlobalTexture(mainMap->imageMapSurface, &display, pRenderer, &camera) < 0)
		{
			printf("Erreur creation de la texture globale");
			destroyWorms(&worms1);
			destroyMap(&mainMap);
			cleanUp(&pWindow, &pRenderer, &pInput);
			return -1;

		}
		updateGlobaleTexture(mainMap->imageMapSurface, worms1->wormsSurface, display, &worms1->wormsRect);
		updateGlobaleTexture(mainMap->imageMapSurface, worms1->texteSurface, display, &worms1->texteSurface->clip_rect);
		updateScreen(pRenderer, 2, 0, mainMap, 1, display, &camera, NULL);
		while (!(pInput->quit))
		{
			//Récupération des inputs
			getInput(pInput, pWindow);

			//Gestion des inputs
			if (!gestInput(pInput, pRenderer, mainMap, display, &camera, worms1))
			{
				printf("Erreur lors du traitement de l'entree");
			}

			//Update de l'écran
			if (pInput->raffraichissement)
			{
				updateScreen(pRenderer, 2, 0, mainMap, 1, display, &camera, NULL);
			}

			//Gestion du frame Rate
			frameRate(frame_max);
			frame_max = SDL_GetTicks() + FRAME_RATE;
		}
		destroyMap(&mainMap);
		destroyWorms(&worms1);
		destroyPolice();
		SDL_DestroyTexture(display);
		display = NULL;
		cleanUp(&pWindow, &pRenderer, &pInput);
	}
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
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
	} //comment

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
	return 0;
}

/**
* \fn int initSWR(SDL_Window** pWindow, SDL_Renderer **pRenderer)
* \brief Initialisation de la fenêtre.
*
*La fonction initSWR initialise la SDL, la SDL_Image, la SDL_TTF et les pointeurs
*pWindow ainsi que pRenderer. Elle créé la fenêtre et le renderer associé.
*Elle retourne 1 si tout c'est bien passé, -1 sinon.
*
* \param[in] pWindow, adresse du pointeur pWindow pour l'initialiser
* \param[in] pRenderer, adresse du pointeur pRenderer pour l'initialiser
* \returns int, indicateur si la fonction a bien fonctionnée.
* \returns les pointeurs pWindow et pRenderer
*/
int initSWR(SDL_Window ** pWindow, SDL_Renderer ** pRenderer)
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
		SDL_DestroyWindow(*pWindow);
		(*pWindow) = NULL;
		SDL_Quit();
		return -1;
	}
	/*Initialisation SDL_Image*/
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		printf("Erreur Img_init : %s", SDL_GetError());
		cleanUp(pWindow, pRenderer, NULL);
		SDL_Quit();
		return -1;
	}
	/*Initialisation SDL_TTF*/
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		return -1;
	}
	return 1;
}


/**
* \fn void cleanUp(SDL_Window** pWindow, SDL_Renderer** pRenderer, Input** pInput)
* \brief Détruit la fenêtre et le renderer. Libère la mémoire de pInput et quitte la SDL et la SDL_Image.
*
* \param[in] pWindow, adresse du pointeur de la fenêtre.
* \param[in] pRenderer, adresse du pointeur du renderer.
* \param[in] pInput, adresse du pointeur de la structure Input.
*/
void cleanUp(SDL_Window** pWindow, SDL_Renderer** pRenderer, Input** pInput)
{
	if ((*pInput) != NULL)
	{
		free(*pInput);
		(*pInput) = NULL;
	}

	SDL_DestroyRenderer(*pRenderer);
	(*pRenderer) = NULL;
	SDL_DestroyWindow(*pWindow);
	(*pWindow) = NULL;

	IMG_Quit();
	SDL_Quit();
}

/**
* \fn SDL_Window * creerFenetre(const int w, const int h, const char * nom)
* \brief Cree une fenetre SDL.
*
* \param[in] w, largeur de la fenetre
* \param[in] h, hauteur de la fenetre
* \param[in] nom, nom de la fenetre
* \returns pointeur vers la fenetre creee, NULL si echec
*/
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

/**
* \fn SDL_Surface * loadImage(const char * file)
* \brief Cree une surface a partir d'une image exterieur.
*
* \param[in] file, nom du fichier image a ouvrir
* \returns pointeur vers la surface creee, NULL si echec
*/
SDL_Surface * loadImage(const char * file){
	SDL_Surface* image = IMG_Load(file);
	if (image == NULL)
	{
		printf("Unable to load image: %s\n", SDL_GetError());
		return NULL;
	}
	return image;
}

/**
* \fn SDL_Texture * loadTexture(SDL_Renderer * pRenderer, const char * file)
* \brief Cree une texture a partir d'une image exterieur dans le renderer.
*
* \param[in] pRenderer, pointeur vers le renderer de la fenetre
* \param[in] file, nom du fichier image a ouvrir
* \returns pointeur vers la texture creee, NULL si echec
*/
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
void deplacementRectangle(SDL_Rect * rect, int x2, int y2, int dir)
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


/**
* \fn void updateScreen(SDL_Renderer * pRenderer, int nb, ...)
* \brief Actualise l'affichage.
*
* Cette fonction est appelée à chaque changement dans l'image pour
* réactuliser l'affichage.
* Elle peut prendre plusieurs types de paramètre pour réaliser des actions
* diverses (affichage de la map, d'un (de) rectangle(s) ou d'une (de) textures).
*
* \param[in] pRenderer, pointeur vers le renderer de la fenêtre
* \param[in] nb, nombre de paramètres dans l'appel
* \remarks Le formatage des paramètres supplémentaire est le suivant :
*	pour le background de la map : 0,Terrain* map
*	pour une texture : 1,Texture* pTexture,SDL_Rect* rectSource, SDL_Rect* rectDest
*	pour remplir un rectangle : 2,Uint32 couleurRemplissage,SDL_Rect* rect à remplir
*
* ATTENTION : Penser a incrementer nb en fonction du nombre de paramètre en plus,
* par exemple : une texture et un rectangle => nb = 2
*/
void updateScreen(SDL_Renderer * pRenderer, int nb, ...)
{
	SDL_Rect* rect = NULL;
	SDL_Rect* rect2 = NULL;
	SDL_Rect temp = { 0, 0, 0, 0 };
	Terrain * map = NULL;
	SDL_Texture* text = NULL;
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
			break;
		case 1:
			text = va_arg(list, SDL_Texture*);
			rect = va_arg(list, SDL_Rect*);
			rect2 = va_arg(list, SDL_Rect*);
			SDL_RenderCopy(pRenderer, text, rect, rect2);
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
	va_end(list);
}

/**
* \fn void frameRate(unsigned int fM)
* \brief Actualise l'affichage.
*
* Cette fonction est appelée à chaque boucle principale du programme pour
* réguler le frame rate et limiter l'usage du CPU par la fonction getInput.
* \param[in] fM, valeur du temps ecoule lors de la derniere boucle
*/
void frameRate(unsigned int fM)
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

/**
* \fn void initCameras(SDL_Renderer * pRenderer, Terrain * map, SDL_Rect * camera)
* \brief Initialise une camera.
*
* \param[in] pRenderer, le renderer de la fenêtre.
*
* \param[in] map, la texture de la map.
*
* \param[in] camera, le rect de la camera à initialiser.
*
* \returns void
*/
void initCameras(SDL_Renderer * pRenderer, Terrain * map, SDL_Rect * camera, Worms * worms){
	int w = 0, h = 0, wW = 0, hW = 0;
	SDL_GetRendererOutputSize(pRenderer, &wW, &hW);
	if (worms == NULL){
	w = map->imageMapSurface->w;
	h = map->imageMapSurface->h;
	camera->x = 0;
	camera->y = 0;
		if (h > hW || w > wW){
		camera->h = h;
		camera->w = (int)(camera->h * ((float)wW / (float)hW));
	}
	}
	else{
		camera->h = worms->wormsSurface->h * 10;
		camera->w = camera->h * ((float)wW / (float)hW);
		camera->x = worms->xAbs - (camera->w / 2);
		camera->y = worms->yAbs - (camera->h / 2);
		if (camera->x < 0)camera->x = 0;
		if (camera->y < 0)camera->y = 0;
	}
}

/**
* \fn void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput,...)
* \brief Déplace la camera suivant la souris.
*
* \param[in] pTexture, la texture de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \param[in] pInput, les inputs utilisateur.
*
* \returns void
*/
void moveCam(SDL_Texture* pTexture, SDL_Rect * camera, Input * pInput)
{
	int w = 0, h = 0;
	SDL_QueryTexture(pTexture, NULL, NULL, &w, &h);
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
}

/**
* \fn void zoomIn(SDL_Renderer * pRenderer, SDL_Rect * camera)
* \brief Réajuste la taille de la caméra pour un grossissement de l'image.
*
* \param[in] pRenderer, le renderer de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \returns void
*/
void zoomIn(SDL_Renderer * pRenderer, SDL_Texture * pTexture, SDL_Rect * camera, Input * pInput)
{
	int wW = 0, hW = 0, w = 0, h = 0;
	float x = 0, y = 0;
	SDL_GetRendererOutputSize(pRenderer, &wW, &hW);
	SDL_QueryTexture(pTexture, NULL, NULL, &w, &h);
	camera->h = camera->h - 10;
	camera->w = (int)(camera->h * ((float)wW / (float)hW));// keep the ratio depending of the size of the window!!!!!

	x = (pInput->cursor.now.x / (float)wW) - (1 / 2.0);
	y = (pInput->cursor.now.y / (float)hW) - (1 / 2.0);
	camera->x += x * 30;
	camera->y += y * 30;

	if (camera->x < 0)camera->x = 0;
	if (camera->y < 0)camera->y = 0;
	if (camera->x + camera->w > w) camera->x = w - camera->w;
	if (camera->y + camera->h > h) camera->y = h - camera->h;
}

/**
* \fn void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera)
* \brief Réajuste la taille de la caméra pour un rétrécissement de l'image.
*
* \param[in] pRenderer, le renderer de la fenêtre courante.
*
* \param[in] pTexture, la texture de la fenêtre courante.
*
* \param[in] camera, le rect de la camera courante.
*
* \returns void
*/
void zoomOut(SDL_Renderer * pRenderer, SDL_Texture* pTexture, SDL_Rect * camera)
{
	int w = 0, h = 0, wM = 0, hM = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	SDL_QueryTexture(pTexture, NULL, NULL, &wM, &hM);
	if (camera->h < hM){
		camera->x = camera->x + (camera->w) / 2;
		camera->y = camera->y + (camera->h) / 2;
		camera->h = camera->h + 20;
		camera->w = (int)(camera->h * ((float)w / (float)h));// keep the ratio depending of the size of the window!!!!!
		if (camera->w > w){
			camera->w = w;
		}
		if (camera->h > h){
			camera->h = h;
		}
		camera->x = camera->x - ((camera->w) / 2);
		camera->y = camera->y - ((camera->h) / 2);
	}

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


/*void animationWorms(SDL_Surface * display)
{
const int largeurSprite = 632;
const int hauteurSprite = 317;
SDL_Surface *sprite = loadImage("../assets/pictures/sprite.png");
SDL_Rect offset;
offset.x = 0;
offset.y = 0;
SDL_Rect clip[15];
int i;
for (i = 0; i < 15; i++){
clip[i].x = 8 + (i + 1) * 27;
clip[i].y = 27;
clip[i].w = 27;
clip[i].h = 40;
SDL_BlitSurface(sprite, &clip[i], display, &offset);
}

}*/

/**
* \fn int createGlobalTexture(SDL_Surface* pSurfaceMap, SDL_Texture** pTexture, SDL_Renderer* pRenderer, SDL_Rect* camera)
* \brief Cree une texture globale a partir des pixels de toutes les surfaces composant le jeu.
*
* \param[in] pSurfaceTab, tableau de pointeur vers les surfaces.
* \param[in] nbSurface, nombre de surfaces dans le tableau.
* \param[in] pTexture, adresse du pointeur de la texture a initialiser.
* \param[in] pRenderer, pointeur du renderer.
* \param[in] camera, pointeur de la struture camera.
* \returns int, indicateur de reussite de la fonction : 0 = succes, -1 = echec
* \remarks si -1, le pointeur de la structure Texture sera mis a NULL sinon il pointera vers la texture globale.
*/
int createGlobalTexture(SDL_Surface* pSurfaceMap, SDL_Texture** pTexture, SDL_Renderer* pRenderer, SDL_Rect* camera)
{
	int x = 0, y = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	Uint32 pixelRead = 0;
	Uint32* pixelWrite = NULL;
	Uint32 pixelTransparent = SDL_MapRGBA(pSurfaceMap->format, 255, 255, 255, 0);
	SDL_Texture* textureTemp = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 };
	int nombrePixel = 0;
	int h = 0, w = 0;
	SDL_GetRendererOutputSize(pRenderer, &w, &h);
	rect.w = w;
	rect.h = h;

	nombrePixel = pSurfaceMap->w * pSurfaceMap->h;
	pixelWrite = malloc(nombrePixel * sizeof(Uint32));
	if (pixelWrite == NULL)
	{
		printf("Unable to allocate memory for screenshot pointeur data buffer!\n ");
		return -1;
	}
	for (y = pSurfaceMap->clip_rect.y; y < pSurfaceMap->clip_rect.y + pSurfaceMap->h; y++)
	{
		for (x = pSurfaceMap->clip_rect.x; x < pSurfaceMap->clip_rect.x + pSurfaceMap->w; x++)
		{
			pixelRead = ReadPixel(pSurfaceMap, x - pSurfaceMap->clip_rect.x, y - pSurfaceMap->clip_rect.y);
			SDL_GetRGBA(pixelRead, pSurfaceMap->format, &r, &g, &b, &a);
				if (a < 150)
				{
				pixelWrite[x + y *  pSurfaceMap->w] = pixelTransparent;
				}
				else
				{
				pixelWrite[x + y *  pSurfaceMap->w] = pixelRead;
			}
		}
	}
	textureTemp = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, pSurfaceMap->w, pSurfaceMap->h);
	if (textureTemp == NULL)
	{
		printf("Erreur lors de la creation de la texture");
		free(pixelWrite);
		pixelWrite = NULL;
		return -1;
	}
	SDL_SetTextureBlendMode(textureTemp, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(textureTemp, NULL, pixelWrite, pSurfaceMap->pitch);
	*pTexture = textureTemp;
	textureTemp = NULL;
	SDL_RenderCopy(pRenderer, *pTexture, camera, &rect);
	SDL_RenderPresent(pRenderer);
	free(pixelWrite);
	pixelWrite = NULL;
	return 0;
}

/**
* \fn int updateGlobaleTexture(SDL_Surface* pSurfaceTab[], SDL_Texture* pTexture, int surface, SDL_Rect* pRect)
* \brief Met a jour la texture globale sur une zone bien determinee.
*
* \param[in] pSurfaceTab, tableau de pointeur vers les surfaces.
* \param[in] pTexture, pointeur vers la texture globale cree avec createGlobalTexture.
* \param[in] surface, indice de la surface a update dans le tableau.
* \param[in] pRect, pointeur vers la structure SDL_Rect associe a la surface a update, contient les coordonnees d'avant update.
* \returns int, indicateur de reussite de la fonction : 0 = succes, -1 = echec
* \remarks le SDL_Rect est mis a jour dans la fonction vers les nouvelles coordonnees de la surface
*/
int updateGlobaleTexture(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceModif, SDL_Texture* pTexture, SDL_Rect* pRectSurfaceModif)
{
	Uint32* pixelWrite = NULL;
	Uint32 pixelRead;
	int nombrePixel = 0;
	Uint8 r = 0, g = 0, b = 0, a = 0;
	int x = 0, y = 0, i = 0;
	if (pSurfaceModif->clip_rect.x < 0 || pSurfaceModif->clip_rect.y < 0 || (pSurfaceModif->clip_rect.y + pSurfaceModif->h) > pSurfaceMap->h || (pSurfaceModif->clip_rect.x + pSurfaceModif->w) > pSurfaceMap->w)
	{
		printf("La surface est sortie de l'écran");
		return -1;
	}

	nombrePixel = pSurfaceModif->w * pSurfaceModif->h;
	secureRect(pRectSurfaceModif, pSurfaceMap);
	pixelWrite = malloc(nombrePixel*sizeof(Uint32));
	for (i = 0; i < 2; i++)
	{
		for (y = pRectSurfaceModif->y; y < pRectSurfaceModif->y + pRectSurfaceModif->h; y++)
		{
			for (x = pRectSurfaceModif->x; x < pRectSurfaceModif->x + pRectSurfaceModif->w; x++)
			{
				if (i != 0)
				{
					pixelRead = ReadPixel(pSurfaceModif, x - pRectSurfaceModif->x, y - pRectSurfaceModif->y);
					SDL_GetRGBA(pixelRead, pSurfaceModif->format, &r, &g, &b, &a);
					if (a < 200)
					{
						pixelRead = ReadPixel(pSurfaceMap, x, y);
					}
				}
				else pixelRead = ReadPixel(pSurfaceMap, x, y);
				pixelWrite[x - pRectSurfaceModif->x + (y - pRectSurfaceModif->y)* pRectSurfaceModif->w] = pixelRead;
			}
		}
		SDL_UpdateTexture(pTexture, pRectSurfaceModif, pixelWrite, 4 * pRectSurfaceModif->w);
		pRectSurfaceModif->y = pSurfaceModif->clip_rect.y;
		pRectSurfaceModif->x = pSurfaceModif->clip_rect.x;
	}

	free(pixelWrite);
	pixelWrite = NULL;
	return 0;
}

/**
* \fn void secureRect(SDL_Rect* pRect, SDL_Surface* pSurface)
* \brief Réajuste les dimensions d'un rectangle qui depasserai de la map
*
* \param[in] pRect, rectangle a evaluer, peut etre modifie par la fonction
* \param[in] pSurface, surface de la map
* \return 1 si il y a eu modification, 0 sinon
*/
int secureRect(SDL_Rect* pRect, SDL_Surface* pSurface)
{
	int modif = 0;
	if (pRect->x < 0) {
		pRect->w -= pRect->x;
		pRect->x = 0;
		modif = 1;
	}
	if (pRect->y < 0) {
		pRect->h -= pRect->y;
		pRect->y = 0;
		modif = 1;
	}
	if (pRect->x + pRect->w >= pSurface->clip_rect.w){
		pRect->w -= pRect->x + pRect->w - pSurface->clip_rect.w;
		modif = 1;
	}
	if (pRect->y + pRect->h >= pSurface->clip_rect.h){
		pRect->h -= pRect->y + pRect->h - pSurface->clip_rect.h;
		modif = 1;
	}
	return modif;
}