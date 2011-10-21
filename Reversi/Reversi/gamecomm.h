#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#define ROWS 8
#define COLS 8

using namespace std;

bool getGameBoard(int iGameBoard[ROWS][COLS])
{
	int iCur;
	char cInput;
	ifstream in("board.txt");

	for(iCur=0; iCur<ROWS*COLS; iCur++)
	{
		cInput=in.get();
		switch(cInput)
		{
		case '\n':
		case '\t':
			iCur--;
			break;
		case 'X':
		case 'x':
		case '-':
			iGameBoard[iCur/COLS][iCur%COLS] = -1;
			break;
		case 'O':
		case 'o':
		case '0':
		case '+':
			iGameBoard[iCur/COLS][iCur%COLS] = 1;
			break;
		case ' ':
		case '.':
		case '_':
			iGameBoard[iCur/COLS][iCur%COLS] = 0;
			break;
		default:
			in.close();
			return false;
		}
	}
	in.close();
	return true;
}

bool putMove(int iMoveRow, int iMoveCol)
{
	ofstream out("move.txt");

	if(iMoveCol>=COLS || iMoveCol<0)
		return false;
	if(iMoveRow>=ROWS || iMoveRow<0)
		return false;
	out << iMoveRow << " " << iMoveCol << endl;
	out.close();
	return true;
}