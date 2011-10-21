#ifndef BOARD_PTR_H
#define BOARD_PTR_H

#include <iostream>
using namespace std;

#include "Board.h"

struct BoardPtr
{
  BoardPtr( Board* bptr = NULL ) : b(bptr) {}
  Board* b;

  Board* operator->()
  {
    return this->b;
  }
};

bool operator<( const BoardPtr& lhs, const BoardPtr& rhs )
{
  return lhs.b->f < rhs.b->f;
}

bool operator>( const BoardPtr& lhs, const BoardPtr& rhs )
{
  return lhs.b->f > rhs.b->f;
}

bool operator==( const BoardPtr& lhs, void* rhs )
{
  return lhs.b == rhs;
}

bool operator!=( const BoardPtr& lhs, void* rhs )
{
  return lhs.b != rhs;
}


ostream& operator<<( ostream& o, const BoardPtr& b )
{
  o << *b.b;
  return o;
}

#endif