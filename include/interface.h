#pragma once

#include <cchess/engine.h>

void ccInterfaceInit (); 
bool ccInterfacePlayMove(int from, int to, char promotionToPiece); 
bool ccInterfaceIsLegal(int from, int to); 
MoveType ccInterfaceLatestMoveType(); 
Color ccGetColor(bool isFriendly);