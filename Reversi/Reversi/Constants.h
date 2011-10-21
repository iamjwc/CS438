#ifndef CONSTANTS_H
#define CONSTANTS_H

#define for_each_l(i,l) for( std::list<ReversiBoard*>::iterator i = l.begin(); i != l.end(); ++i )

const int INFINITY = 0xFFFFFFF;

enum Piece     { BLACK=-1, EMPTY=0, WHITE=1 };
enum Direction { NW,NE,SW,SE,N,S,W,E };

#endif
