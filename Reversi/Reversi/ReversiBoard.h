#ifndef REVERSI_BOARD_H
#define REVERSI_BOARD_H

#include <iostream>
#include <list>
#include <string>

#include "Constants.h"
#include "Position.h"

class ReversiBoard
{
public:
  ReversiBoard( Piece turn = WHITE, Piece* board = NULL )
    : turn(turn), parent( NULL ), last_move( Position( 0, 0 ) )
  {
    if( NULL != board )
      for( int i = 0; i < 64; ++i )
        this->board[i] = board[i];
    else
      this->default_board_setup();
  }

  ~ReversiBoard()
  {
    for_each_l( i, this->successors )
    {
      delete *i;
      *i = NULL;
    }
  }

  double calculate_heuristic()
  {
    this->heuristic_value = rand();
    return this->heuristic_value;
  }

  void default_board_setup()
  {
    for( int i = 0; i < 64; ++i )
      this->board[i] = EMPTY;

    this->board[ Position(3,3).to_index() ] = BLACK;
    this->board[ Position(4,4).to_index() ] = BLACK;
    this->board[ Position(3,4).to_index() ] = WHITE;
    this->board[ Position(4,3).to_index() ] = WHITE;
  }

  // Gets the piece adjacent to Position p at Direction d
  Piece adjacent_piece( Position p, Direction d )
  {
    return this->board[ p.move(d).to_index() ];
  }
  
  // Finds successors from every piece on the board
  void get_successors()
  {
    for( int i = 0; i < 64; ++i )
      this->do_jump( Position( i ) );
  }

  void do_jump( Position p )
  {

    if( p.row == 7 && p.col == 6 )
      p.col = 6;

    if( EMPTY != this->board[ p.to_index() ] )
      return;
   
    // Dynamically creates a new Reversi Board
    ReversiBoard* r = new ReversiBoard( (Piece)-this->turn, this->board );
    r->parent       = this;
    r->last_move    = p;

    // Loops through all of the directions and makes jumps for each of them on
    // the same game board
    bool changed = false;
    for( int i = 0; i < 8; ++i )
    {
      changed |= this->do_jump( p, (Direction)i, r, false );
    }

    // If no jumps were made, garbage collect r
    if( !changed )
    {
      delete r;
      r = NULL;
    }
    // Else, adds the successor state r to a list of successors to be garbage
    // collected later
    else
      this->successors.push_front( r );
  }

  bool do_jump( Position p, Direction d, ReversiBoard* r, bool visited_one )
  {
    try
    {
    if( !p.is_on_board() )
      return false;

    if( visited_one && EMPTY == this->adjacent_piece( p, d ) )
      return false;

    // If the adjacent piece to the direction D is an opponent
    if( this->adjacent_piece( p, d ) == -this->turn )
    {
      // If do_jump for the adjacent piece to the direction D
      if( this->do_jump( p.move(d), d, r, true ) )
      {

        // Replace the opponents piece with your own
        r->board[ p.to_index() ] = this->turn;
        return true;
      }
    }
    // Else if the adjacent piece to the direction D is your own piece
    else if( this->adjacent_piece( p, d ) == this->turn )
    {
      // Replace the opponents piece with your own
      r->board[ p.to_index() ] = this->turn;

      // Only return true if this has been recursively called at least one
      // time, because that means that there is at least one opponent piece
      // between the position selected and your other piece that is in line
      // with it
      return visited_one;
    }
    }
    catch(...)
    {return false;}

    return false;
  }

  std::string to_s()
  {
    std::string s = "";
    for( int i = 0; i < 64; ++i )
    {
      if( EMPTY == this->board[i] )
        s += "0";
      else if( BLACK == this->board[i] )
        s += "-1";
      else
        s += "1";
    }

    return s;
  }

  Piece turn;
  Piece board[64];

  double heuristic_value;

  Position last_move;

  ReversiBoard* parent;
  std::list<ReversiBoard*> successors;
};

std::ostream& operator<<( std::ostream& o, ReversiBoard r )
{
  for( int i = 0; i < 64; ++i )
  {
    o << " " << ((r.board[i] > -1) ? " " : "") << r.board[i];

    if( (i+1) % 8 == 0 )
      o << std::endl;
  }

  return o;
}

#endif
