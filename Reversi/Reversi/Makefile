
ReversiGame: main.o
	g++ -o ReversiGame main.o

main.o: main.cpp ReversiBoard.h Position.h Constants.h
	g++ -c main.cpp -o main.o

clean:
	rm -f ReversiGame *.o *.~
