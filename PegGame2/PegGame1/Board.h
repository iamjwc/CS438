#ifndef BOARD_H
#define BOARD_H

#include "Position.h"
#include "Direction.h"

#include <iostream>
using namespace std;

const string board_distance_from_center = "000545000000434000000323000543212345432101234543212345000323000000434000000545000";
const string board_edges = "000111000000101000000101000111101111100000001111101111000101000000101000000111000";

class Board
{
public:
  Board( string board = BOARD_INIT, Board* parent = NULL )
  {
    this->board = board;
    this->count_pegs();
    this->parent = parent;
    this->level = 1;
    if(parent != NULL)
      this->level = this->parent->level + 1;

    this->calculate_f();
  }

  bool is_solved()
  {
    return this->peg_count == 1; //
  }

  void calculate_f()
  {
    float h = 0;
    float g = this->level;
   
    for( int i = 0; i < BOARD_SIZE; ++i)
    {
      // Finds distance from center and adds that to h
      h += 1.85 * ((this->board[i] == '0') ? board_distance_from_center[i]-'0': 0);

      // Finds distance from center and adds that to h
      h += 1.75 * -((this->board[i] == '1') ? board_distance_from_center[i]-'0': 0);

      // If the current peg is on the edge of the board, add 1 to h
      h += ((this->board[i] == '1') ? board_edges[i]-'0' : 0);

      // If the current peg is on the edge of the board, add 1 to h
      h += -((this->board[i] == '0') ? board_edges[i]-'0' : 0);

      // If there are two 1's right next to each other, subtract 3 from h
      if( i < BOARD_SIZE-1 )
        if( this->board[i] == '1' && this->board[i+1] == '1' && (i%ONE_DIMENSION) != 0)
          h += 4;


      if( i < BOARD_SIZE-ONE_DIMENSION )
        if( this->board[i] == '1' && this->board[i+ONE_DIMENSION] == '1' )
          h += 5;
    }

    this->f = g + h;
  }

  void copy( Board& b )
  {
    b.board = this->board;
    b.count_pegs();
  }

  bool is_on_board( const Position& p )
  {
    return
    (
      ((p.row >= 0 && p.row <= 2) && (p.col >= 3 && p.col <= 5))
      || ((p.row >= 3 && p.row <= 5) && (p.col >= 0 && p.col <= 8))
      || ((p.row >= 6 && p.row <= 8) && (p.col >= 3 && p.col <= 5))
    );
  }

  bool is_open( const Position& p )
  {
    return this->is_on_board( p ) && !('0' - this->board[p.to_one_dimension()]);
  }

  bool is_closed( const Position& p )
  {
    return this->is_on_board( p ) && ('0' - this->board[p.to_one_dimension()]);
  }

  bool can_jump( const Position& p, Direction d )
  {
    return( 
      is_closed( p ) &&
      is_closed( p.x_to_the(1,d) ) &&
      is_open( p.x_to_the(2,d) )
   );
  }

  void find_children()
  {
    for( int i = 0; i < ONE_DIMENSION; ++i )
     for( int j = 0; j < ONE_DIMENSION; ++j )
     {
       Position p = Position( i, j );
       this->jump( p, UP );
       this->jump( p, DOWN );
       this->jump( p, LEFT );
       this->jump( p, RIGHT );
     }
  }

  void jump( const Position& p, Direction d )
  {
    if( !this->can_jump( p, d ) )
      return;

    Position p1 = p.x_to_the( 1, d );
    Position p2 = p.x_to_the( 2, d );

    string b = this->board;
    b[p.to_one_dimension()] = '0';
    b[p.x_to_the( 1, d ).to_one_dimension()] = '0';
    b[p.x_to_the( 2, d ).to_one_dimension()] = '1';

    Board* new_board = new Board( b, this);

    this->children.push_front( new_board );
    this->children_pointer.push_front( new_board );
  }

  ~Board()
  {
    for_each_l( i, this->children_pointer )
    {
      delete *i;
      *i = NULL;
    }
  }
  void count_pegs()
  {
    this->peg_count = 0;

    for( int i = 0; i < BOARD_SIZE; ++i )
      if( this->board[i] == '1' )
        ++this->peg_count;
  }

  list<Board*> children;
  list<Board*> children_pointer;
  Board* parent;
  string board;
  char   peg_count;
  float  f;
  int level;
};


ostream& operator<<( ostream& o, const Board& b )
{
  for( int i = 0; i < BOARD_SIZE; ++i )
  {
    o << b.board[i] << " ";
    
    if( !( (1+i) % ONE_DIMENSION ) )
      o << endl;
  }

  return o;
}

#endif