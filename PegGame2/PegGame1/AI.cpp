// Programmed by Justin Camerer
// +--------------+---------+----------------+
// | Puzzle       | A* Time | Nodes Expanded |
// +--------------+---------+----------------+
// | BigPlus      | 0.02    | 98             |
// | BigFireplace | 0.05    | 483            |
// | Diamond      | 0.10    | 1332           |
// | Big Diamond  | 0.10    | 1332           |
// | Solitaire    | 13.63   | 75999          |
// | BigSolitaire | 0.57    | 4932           |
// | UpArrow      | 0.04    | 403            |
// | BigUpArrow   | 0.59    | 5946           |
// +--------------+---------+----------------+

#include <iostream>
#include <list>
#include <set>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>
using namespace std;

#include "iamjwc/IO.h"
using namespace iamjwc;

#define BOARD_INIT    "000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define BOARD_WEIGHTS "000545000000434000000323000543212345432101234543212345000323000000434000000545000"
#define BOARD_SIZE    81
#define ONE_DIMENSION 9

#define for_each_l( index, l ) for( list<Board*>::iterator index = l.begin(); index != l.end(); ++index )

#include "Position.h"
#include "Direction.h"
#include "Board.h"
#include "BoardPtr.h"

int node_count;

class MyQueue : public list<BoardPtr>
{
public:
  BoardPtr at_front() { return this->front(); }
  void pop_it() { this->pop_front(); }
  void push_it( BoardPtr bp ) { this->push_back( bp ); }
};

class MyStack : public list<BoardPtr>
{
public:
  BoardPtr at_front() { return this->front(); }
  void pop_it() { this->pop_front(); }
  void push_it( BoardPtr bp ) { this->push_front( bp ); }
};

class MyHeap : public priority_queue<BoardPtr>
{
public:
  BoardPtr at_front() { return this->top(); }
  void pop_it() { this->pop(); }
  void push_it( BoardPtr bp ) { this->push( bp ); }
};

template<class DataStructure>
BoardPtr search( BoardPtr start )
{
  node_count = 0;

  set<string> closed_set;
  DataStructure open_list;
  BoardPtr current;
  
  start->count_pegs();

  open_list.push_it( start );
  
  while( open_list.size() )
  {
    current = open_list.at_front();
    open_list.pop_it();

    if( current->is_solved() )
      return current;
   
    current->find_children();
    while( current->children.size() )
    {
      if( closed_set.find( current->children.front()->board ) == closed_set.end() )
      {
        closed_set.insert( current->children.front()->board );
        open_list.push_it( BoardPtr( current->children.front() ) );
        ++node_count;
      }
      current->children.pop_front();
    }
  }

  return NULL;
}

string read_file( string filename )
{
  string board = "";

  ifstream* fin = new ifstream( filename.c_str() );
  IO fio( fin, NULL );

  for(int i = 0; i < 3; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 6; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 6; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 3; ++i) board += ' ';

  for(int i = 0; i < 9; ++i) board += fio.read<char>();
  for(int i = 0; i < 9; ++i) board += fio.read<char>();
  for(int i = 0; i < 9; ++i) board += fio.read<char>();

  for(int i = 0; i < 3; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 6; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 6; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += fio.read<char>();
  for(int i = 0; i < 3; ++i) board += ' ';

  fin->close();

  delete fin;
  fin = NULL;

  return board;
}

void print_path( BoardPtr b )
{
  if( b == NULL )
    return;

  print_path( b->parent );
  stdio << b << nl;
}


int main()
{
  while(1)
  {
    string input;
    int choice = 0;

    stdio << "Please enter the name of the shape to be solved." << nl
          << "Enter Q or Quit to terminatethe program." << nl;
    stdio >> input;

    if( input.size() > 0 && ( input[0] == 'q' || input[0] == 'Q' ) )
      return 0;

    Board b;
    try
    {
      b = Board( read_file( input + ".in" ) );
    }
    catch( string e )
    {
      stdio << "Could not open file \"" << input << ".in\"" << nl << nl;
    }

    while( choice != 1 && choice != 2  && choice != 3 )
    {
      stdio << tab << "Select a search method:" << nl
            << tab << "1. Depth-first Search"   << nl
            << tab << "2. Breadth-first Search" << nl
            << tab << "3. Heuristic Search"     << nl;
      stdio >> choice;
    }

    BoardPtr answer;
    if( choice == 1 )
      answer = search<MyStack>( BoardPtr( &b ) );
    else if( choice == 2 )
      answer = search<MyQueue>( BoardPtr( &b ) );
    else
      answer = search<MyHeap>( BoardPtr( &b ) );

    if( answer == NULL )
    {
      stdio << "No path found." << nl;
      continue;
    }

    print_path( answer );
    stdio << "Total nodes generated: " << node_count << nl;
  }

  return 0;
}