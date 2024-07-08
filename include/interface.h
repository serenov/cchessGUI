#pragma once

#include <cchess/engine.h>

void ccInterfaceInit (); 
bool ccInterfacePlayMove(int from, int to, char promotionToPiece); 
Bitboard ccInterfaceListMoves(int position); 