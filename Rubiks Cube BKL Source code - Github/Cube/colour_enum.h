//-----------------------------------------------------------------------------
// RUBIKS CUBE AUTHOR: BRIAN LEMAN, SID 16024894 D.O.B. 22/01/1985 START DATE: 30/09/2009

#pragma once
enum Colour {Red=1, Green, Blue, Yellow, White, Orange};
enum SIDE_SELECTED { FRONT,BACK,TOP,BOTTOM,LEFT,RIGHT,VERTICAL_MIDDLE,HORIZONTAL_MIDDLE, NONE };
#define NBCUBES 	27 // A CENTRE ONE FOR EASE OF DECTING SIDE CUBES
#define WIDTH 		1
#define HALFWIDTH 	0.5