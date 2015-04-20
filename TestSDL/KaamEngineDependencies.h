#ifndef KAAMENGINEDEP_H
#define KAAMENGINEDEP_H

#include "Libraries.h"
#include "carte.h"
#include "worms.h"
#include "my_stdrFct.h"

#define groundSpeed 1

/*Set functions*/
void setSurfaceRelativeCoordinates(SDL_Surface* pSurface, int relativeTime, float Xspeed, float Yspeed);
void setSurfaceAbsoluteCoordinates(SDL_Surface* pSurface, int absoluteX, int absoluteY);
void setSpeed(float* Xspeed, float* Yspeed, float valueX, float valueY);
void setSideMotionPossibility(KaamObject* pObject, SDL_Surface* pSurfaceMap);

/*Reset functions*/
void resetInputs(Input* pInput);
void resetSpeed(float* Xspeed, float* Yspeed);
void resetMotionVariables(Input* pInput, KaamObject* pObject);
void resetAbsoluteCoordinates(SDL_Surface* pSurface, int* absoluteX, int* absoluteY);
void resetReboundVariables(KaamObject* pObject, float signX, float signY, float coeffX, float coeffY);

/*Test functions*/
int getStartMotion(Input* pInput, KaamObject* pObject, SDL_Surface* pSurfaceMap);
int jumpDoability(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION jumpDirection);
int checkDirection(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int offsetX, int offsetY, enum DIRECTION checkDirection);
int processCheck(int checkLeft, int checkRight, int checkUp, int checkUpLeft, int checkUpRight, enum DIRECTION jumpDirection);
int testGround(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, int testValue);
int sideInitialized(KaamObject* pObject);

/*Collision functions*/
int collisionSurfaceWithMapLimits(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);
int collisionSurfaceWithMapBasic(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion);
int collisionSurfaceWithMap(SDL_Surface* pSurfaceMap, SDL_Surface* pSurfaceMotion, enum DIRECTION* pDirection, int checkMode);
void calculXYBalayage(SDL_Surface* pSurfaceMotion, int* xStart, int* xEnd, int* yStart, int* yEnd, int zone);
void calculOrdreBalayage(enum DIRECTION direction, int ordre[4]);

/*Motion functions*/
int dyProcess(KaamObject* pObject);
int dxProcess(KaamObject* pObject);
enum DIRECTION motionDirectionProcess(int dx, int dy);


#endif // !KAAMENGINEDEP_H
