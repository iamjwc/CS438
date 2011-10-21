#include <iostream>
#include <fstream>
#include <list>
#include <string>

using namespace std;

template<class T>
void gc( T*& v )
{
  delete v;
  v = NULL;
}

void clean_stack( list<char>* stack )
{
  while( stack->size() && stack->front() == '(' ) stack->pop_front();
}

int pop_number( list<char>* stack )
{
  string s = "";
  for(; stack->size() && isdigit( stack->front() ); s = stack->front() + s, stack->pop_front() );

  int pos_neg = 1;
  if(stack->front() == '_')
  {
    stack->pop_front();
    pos_neg = -1;
  }

  return pos_neg * atoi( s.c_str() );
}

int registers[26] = {0};
int last_registers[26] = {0};

void update_registers()
{
  for( int i = 0; i < 26; ++i )
    last_registers[i] = registers[i];
}

int& pop_variable( list<char>* stack )
{
  int& val = registers[stack->front()-'A'];
  stack->pop_front();
  return val;
}

char pop_operator( list<char>* stack )
{
  char op = stack->front();
  stack->pop_front();
  return op;
}

bool get_value( list<char>* stack, int& val, char& var )
{
  if( isdigit( stack->front() ) )
    val = pop_number( stack );
  else if( isalpha( var = stack->front() ) )
    val = pop_variable( stack );
  else
    return false;

  return true;
}

int compute( char op, int a, int b )
{
  if( op == '+' ) return a + b;
  if( op == '-' ) return a - b;
  if( op == '*' ) return a * b;
  if( op == '/' ) return a / b;
}

int compute( char op, char a, int b )
{
  return registers[a-'A'] = b;
}

int eval( list<char>* stack )
{
  int  a, b;
  char op;
  char var = 0;
  bool reuse_b = false;
  bool use_a   = true;
  
  while( stack->size() && stack->front() != '(' )
  {
    if( !reuse_b && !get_value( stack, b, var ) )
    {
      stack->pop_front();
      b = eval( stack );

      if( !stack->size() || stack->front() == ')')
        return b;
    }

    if( stack->front() == '(' )
    {
      stack->pop_front();
      return b;
    }

    if( stack->size() &&  stack->front() == '_' )
    {
      b *= -1;
      stack->pop_front();
    }

    op = pop_operator( stack );

    if( !get_value( stack, a, var ) )
    {
      stack->pop_front();
      a = eval( stack );
    }

    if( stack->size() && stack->front() == '_' )
    {
      a *= -1;
      stack->pop_front();
    }


    if( isalpha(var) && op == '=' )
      b = compute( op, var, b );
    else
      b = compute( op, a, b );

    reuse_b = true;
  }

  clean_stack( stack );

  return b;
}

void print_changes()
{
  bool changed = false;
  for( int i = 0; i < 26; ++i )
  {
    if( registers[i] != last_registers[i] )
    {
      if( changed )
        cout << ", ";
      cout << char('A'+i) << " = " << registers[i];

      changed = true;
    }
  }
  if( !changed )
    cout << "No Change";

  cout << endl;
}

int main()
{
  list<char>*   stack = new list<char>();
  list<string>* queue = new list<string>();

  string s = "";

  ifstream* fin = new ifstream();
  fin->open("172.in");

  do
  {
    getline( *fin, s );
    queue->push_back( s );
  } while ( s[0] != '#' );
  queue->pop_back();

  fin->close();
  gc(fin);

  while( queue->size() )
  {
    s = queue->front();
    queue->pop_front();

    bool padded = false;
    for( int i = 0, n = s.length(); i < n; ++i )
    {
      if( s[i] != ' ' )
      {
        stack->push_front( s[i] );
        if( s[i] == '=' && !padded )
        {
          for( int i = 0; i < 100; ++i )
          {
            stack->push_front('0');
            stack->push_front('+');
            stack->push_front('(');
          }
          padded = true;
        }
      }
    }
    for( int i = 0; i < 100; ++i )
      stack->push_front(')');

    update_registers();
    eval( stack );
    print_changes();
  }

  gc(stack);
  gc(queue);

  return 0;
}