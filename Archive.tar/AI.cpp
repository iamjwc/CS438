// Programmed by Justin "Scheme Program Failure" Camerer on 2/2/2007
//


#include <iostream>
#include <list>
#include <set>

#include <string>

#define BOARD_INIT    "0000000000000000000000000000000000000000000000000"
#define BOARD_SIZE    49
#define ONE_DIMENSION 7

using namespace std;

#define for_each( index, l ) for( list<Board*>::iterator index = l.begin(); index != l.end(); ++index )

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Position
{
  Position( char row = 0, char col = 0 )
  {
    this->row = row;
    this->col = col;
  }

  Position x_to_the( char x, Direction d ) const
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

  char row, col;
};

class Board
{
public:
  Board( string board = BOARD_INIT )
  {
    this->board = board;
    this->count_pegs();
    this->parent = NULL;
  }

  bool is_solved()
  {
    return this->peg_count == 1; //
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
      ((p.row >= 0 && p.row <= 1) && (p.col >= 2 && p.col <= 4))
      || ((p.row >= 2 && p.row <= 4) && (p.col >= 0 && p.col <= 6))
      || ((p.row >= 5 && p.row <= 6) && (p.col >= 2 && p.col <= 4))
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
       jump( p, UP );
       jump( p, DOWN );
       jump( p, LEFT );
       jump( p, RIGHT );
     }
  }

  void jump( const Position& p, Direction d )
  {
    if( !this->can_jump( p, d ) )
      return;

    Position p1 = p.x_to_the( 1, d );
    Position p2 = p.x_to_the( 2, d );

    Board* new_board = new Board();
    this->copy(*new_board);

    new_board->board[p.to_one_dimension()] = '0';
    new_board->board[p.x_to_the( 1, d ).to_one_dimension()] = '0';
    new_board->board[p.x_to_the( 2, d ).to_one_dimension()] = '1';
    new_board->peg_count = this->peg_count - 1;
    new_board->parent = this;

    children.push_front( new_board );
  }

  ~Board()
  {
    for_each( i, this->children )
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
  Board* parent;
  string board;
  char   peg_count;
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

#include <fstream>

int node_count;

Board* breadth_first_search( Board& start )
{
  node_count = 0;

  set<string>  closed_set;
  list<Board*> open_list;
  Board* current;
  
  start.count_pegs();

  open_list.push_front( &start );
  
  while( open_list.size() )
  {
    current = open_list.front();
    open_list.pop_front();

    if( current->is_solved() )
      return current;
   
    current->find_children();
    while( current->children.size() )
    {
      if( closed_set.find( current->children.front()->board ) == closed_set.end() )
      {
        closed_set.insert( current->children.front()->board );
        open_list.push_back( current->children.front() );
        ++node_count;
      }
      current->children.pop_front();
    }
  }

  return NULL;
}

Board* depth_first_search( Board& start )
{
  node_count = 0;

  set<string>  closed_set;
  list<Board*> open_list;
  Board* current;
  
  start.count_pegs();

  open_list.push_front( &start );
  
  while( open_list.size() )
  {
    current = open_list.front();
    open_list.pop_front();

    if( current->is_solved() )
      return current;
   
    current->find_children();
    while( current->children.size() )
    {
      if( closed_set.find( current->children.front()->board ) == closed_set.end() )
      {
        closed_set.insert( current->children.front()->board );
        open_list.push_front( current->children.front() );
        ++node_count;
      }
      current->children.pop_front();
    }
  }

  return NULL;
}


char read_char( ifstream& f )
{
  char a;
  f >> a;
  return a;
}

string read_file( string filename )
{
  ifstream fin;
  fin.open( filename.c_str() );

  string board = "";

  for(int i = 0; i < 2; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += read_char(fin);
  for(int i = 0; i < 4; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += read_char(fin);
  for(int i = 0; i < 2; ++i) board += ' ';

  for(int i = 0; i < 7; ++i) board += read_char(fin);
  for(int i = 0; i < 7; ++i) board += read_char(fin);
  for(int i = 0; i < 7; ++i) board += read_char(fin);

  for(int i = 0; i < 2; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += read_char(fin);
  for(int i = 0; i < 4; ++i) board += ' ';
  for(int i = 0; i < 3; ++i) board += read_char(fin);
  for(int i = 0; i < 2; ++i) board += ' ';

  fin.close();

  return board;
}

void print_path( Board* b )
{
  if( b == NULL )
    return;

  print_path( b->parent );
  cout << *b << endl;
}

int main()
{
  bool f = true;
  while(1)
  {
    string input;
    int choice = 0;

    cout << "Please enter the name of the shape to be solved." << endl
         << "Enter Q or Quit to terminatethe program." << endl;
    if( !f ) cin.ignore();
    getline( cin, input );

    f = false;
    if( input == "Quit" || input == "Q" )
      return 0;

    Board b;
    try
    {
      b = Board( read_file( input + ".in" ) );
    }
    catch( string e )
    {
      cout << "Could not open file \"" << input << ".in\"" << endl << endl;
    }

    while( choice != 1 && choice != 2 )
    {
      cout << "\tSelect a search method:" << endl
           << "\t1. Depth-first Search" << endl
           << "\t2. Breadth-first Search" << endl;
      cin >> choice;
    }

    Board* answer;
    if( choice == 1 )
      answer = depth_first_search( b );
    else
      answer = breadth_first_search( b );

    if( !answer )
    {
      cout << "No path found." << endl;
      continue;
    }

    print_path( answer );
    cout << "Total nodes generated: " << node_count << endl;
  }

  system("PAUSE");

  return 0;
}
