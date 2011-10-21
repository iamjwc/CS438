#include <iostream>
#include <string>
#include <list>
#include <ctime>

#include "Constants.h"
#include "ReversiBoard.h"
#include "Position.h"
#include "gamecomm.h"

inline double max( double a, double b ) { return a>b?a:b; }
inline double min( double a, double b ) { return a<b?a:b; }

double minimax( ReversiBoard* r, int depth, int max_depth )
{
  //std::cout << r->to_s() << std::endl;

  if( depth == max_depth )
    return r->calculate_heuristic();

  r->get_successors();
  if( !(depth % 2) )
  {
    double alpha = -INFINITY;
    for_each_l( i, r->successors )
      alpha = max( alpha, minimax( *i, depth+1, max_depth ) );
    return alpha;
  }

  double beta = INFINITY;
  for_each_l( i, r->successors )
    beta = min( beta, minimax( *i, depth+1, max_depth ) );
  return beta;
}

double alpha_beta( ReversiBoard* r, int depth, int max_depth, double a, double b )
{
  std::cout << r->to_s() << std::endl;

  if( depth == max_depth )
    r->calculate_heuristic();

  r->get_successors();
  for_each_l( i, r->successors )
  {
    a = max( a, -alpha_beta( *i, depth+1, max_depth, -b, -a ) );
    if( a >= b )
      return a;
  }
  return a;
}

int main()
{
  srand( std::time(0) );

  int n[8][8];
	getGameBoard(n);

  ReversiBoard r;
  for( int i = 0; i < 64; ++i )
    r.board[i] = (Piece)(*n)[i];

  minimax( &r, 0, 3 );

  if( r.successors.size() )
    putMove( r.successors.front()->last_move.row, r.successors.front()->last_move.col );


//	state_t s=new board(n), succ=NULL;
	// alphabeta(s, 3, 0, VS, VL, succ);	
	// if (succ)
	    // putMove(succ->r,succ->c);

  /*ReversiBoard r;
  minimax( &r, 0, 4 );

  ReversiBoard b;
  alpha_beta( &b, 4, INFINITY, -INFINITY );*/

  return 0;
}
