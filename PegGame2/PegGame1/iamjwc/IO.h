#ifndef IAMJWC_IO_H
#define IAMJWC_IO_H

#include <iostream>
#include <string>

namespace iamjwc
{
  class IO
  {
  public:
    IO( std::istream* i = &std::cin, std::ostream* o = &std::cout )
    {
      this->i = i;
      this->o = o;
    }

    template<class T>
    void write( T v )
    {
      (*this->o) << v;
    }

    template<class T>
    T read()
    {
      T v;
      (*this->i) >> v;
      return v;
    }

    std::string read( bool ignore_spaces = false )
    {
      if( !ignore_spaces )
        return this->read<std::string>();

      this->i->ignore();

      std::string s;
      std::getline( *this->i, s );
      return s;
    }

    template<class T>
    IO& operator<<( const T& v )
    {
      this->write( v );
      return *this;
    }

    template<class T>
    IO& operator>>( T& v )
    {
      v = this->read<T>();
      return *this;
    }

  private:
    std::istream* i;
    std::ostream* o;
  };

  static const char nl  = '\n';
  static const char tab = '\t';

    /*template<class T>
    IO& operator<<( IO& io, const T& v )
    {
      io.write( v );
      return io;
    }

    template<class T>
    IO& operator>>( IO& io, T& v )
    {
      v = io.read<T>();
      return io;
    }*/

  static IO stdio;
}

#endif