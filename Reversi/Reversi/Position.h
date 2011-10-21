#ifndef POSITION_H
#define POSITION_H

#include <iostream>

#include "Constants.h"

class Position
{
public:
  Position( int r, int c ) : row(r), col(c) {}
  Position( int p ){ this->row = p / 8; this->col = p % 8; }

  bool is_on_board()
  {
    return 0 <= this->row && 7 >= this->row && 0 <= this->col && 7 >= this->col;
  }

  int to_index()
  {
    return 8 * this->row + this->col;
  }
  
  Position move( Direction d )
  {
    Position p( this->row, this->col );

    switch( d )
    {
      case N:
        --p.row;
        break;

      case S:
        ++p.row;
        break;

      case E:
        ++p.col;
        break;

      case W:
        --p.col;
        break;

      case NW:
        --p.row;
        --p.col;
        break;

      case NE:
        --p.row;
        ++p.col;
        break;

      case SW:
        ++p.row;
        --p.col;
        break;

      case SE:
        ++p.row;
        ++p.col;
        break;
    }

    if( !p.is_on_board() )
      throw("ERROR!");

    return p;
  }


  int row, col;
};

std::ostream& operator<<( std::ostream& o, Position p )
{
  o << "[" << p.row << ", " << p.col << "]";
  return o;
}

#endif
