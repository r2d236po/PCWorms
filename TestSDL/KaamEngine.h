#ifndef KAAMENGINE_H
#define KAAMENGINE_H

/*Dependencies*/
#include "KaamEngineDependencies.h"

/*KaamEngine V1.0*/
void KaamEngine();

/*Inits*/
void KaamInit();
KaamObject* KaamInitObject(SDL_Rect rectSurface, float initSpeedX, float initSpeedY, enum DIRECTION initDirection, int weapon);
void KaamInitSurfaceObject(KaamObject* pObject, Uint32* pixels, Uint32 nbPixels);

/*Inputs*/
void KaamInputs(Input* pInput);

/*Physic Management*/
void KaamPhysicManagement(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap);
void KaamGravityManagement(SDL_Surface* pSurfaceMap, KaamObject* pObject);
void KaamMotionManagement(Input* pInput, KaamObject* object);
void KaamWormsMotionManagement(Input* pInput, Worms* pWorms, SDL_Surface* pSurfaceMap);

/*Motion Management*/
void groundMotionReset(Input* pInput, KaamObject* pObject);
void groundCollisionProcess(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap, int deplacement);
void groundMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap);
void nonLinearMotion(Input* pInput, SDL_Surface* pSurfaceMap, KaamObject* pObject, int allowRebound);

/*Collision Management*/
int KaamCollisionManagement(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection);

#endif // !KAAMENGINE_H
