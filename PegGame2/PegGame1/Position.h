#ifndef POSITION_H
#define POSITION_H

#include "Direction.h"

struct Position
{
  Position()
  {
    this->row = 0;
    this->col = 0;
  }

  Position( int i )
  {
    this->row = i / ONE_DIMENSION;
    this->col = i % ONE_DIMENSION ;
  }

  Position( int row, int col )
  {
    this->row = row;
    this->col = col;
  }

  Position x_to_the( int x, Direction d ) const
  {
    if( UP == d )
      return Position( this->row-x, this->col );

    if( DOWN == d )
      return Position( this->row+x, this->col );

    if( LEFT == d )
      return Position( this->row, this->col-x );

    if( RIGHT == d )
      return Position( this->row, this->col+x );
  }

  int to_one_dimension() const
  {
    return ( this->row * ONE_DIMENSION + this->col );
  }

  int row, col;
};

#endif