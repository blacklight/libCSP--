/*
 * =====================================================================================
 *
 *       Filename:  sudoku.cpp
 *
 *    Description:  A small program that using libCSP++ allows you to solve a sudoku.
 *                  The sudoku is passed as text file, an example of the format can
 *                  be found in the file sudoku.txt. If no parameters is passed to the
 *                  program, this default sudoku is loaded and solved, otherwise the
 *                  sudoku chosen by the user in the specified text file is solved.
 *
 *          Usage:  ./sudoku <text file containing the sudoku>
 *       Complile:  g++ -IPATH/TO/csp++.h -o sudoku sudoku.cpp
 *        Version:  1.0
 *        Created:  17/05/2010 09:22:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  lulz
 *
 * =====================================================================================
 */

#include	<iostream>
#include  <fstream>
#include  <string>
#include	<cstdlib>
#include	<cmath>
#include	<csp++/csp++.h>

#define 	NOVALUE 	-1
#define 	DEFAULT_SUDOKU_FILE 	"sudoku.txt"

using namespace std;

int WIDTH  = 0;
int HEIGHT = 0;

int
toID (int i, int j)
{
	return (i * (WIDTH*WIDTH)) + j;
}

bool
rowsConstraint ( std::vector< CSPvariable<int> > variables)
{
	for ( int i=0; i < WIDTH*WIDTH; i++ ) {
		for ( int j=0; j < HEIGHT*HEIGHT; j++ ) {
			for ( int k=0; k < j; k++ ) {
				int var1 = toID(i,j);
				int var2 = toID(i,k);

				if ( variables[var1].fixed || variables[var2].fixed )  {
					if ( variables[var1].value == variables[var2].value )  {
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool
colsConstraint ( std::vector< CSPvariable<int> > variables)
{
	for ( int i=0; i < HEIGHT*HEIGHT; i++ ) {
		for ( int j=0; j < WIDTH*WIDTH; j++ ) {
			for ( int k=0; k < j; k++ ) {
				int var1 = toID(j,i);
				int var2 = toID(k,i);

				if ( variables[var1].fixed || variables[var2].fixed )  {
					if ( variables[var1].value == variables[var2].value )  {
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool
cellsConstraint ( std::vector< CSPvariable<int> > variables)
{
	for ( int x=0; x < WIDTH*WIDTH; x += WIDTH )  {
		for ( int y=0; y < HEIGHT*HEIGHT; y += HEIGHT )  {
			for ( int i=x; i < x+WIDTH; i++ ) {
				for ( int j=y; j < y+HEIGHT; j++ ) {
					int var1 = toID(i,j);

					for ( int k=x; k < i; k++ ) {
						for ( int l=y; l < j; l++ ) {
							int var2 = toID(k,l);

							if ( variables[var1].fixed || variables[var2].fixed )  {
								if ( variables[var1].value == variables[var2].value )  {
									return false;
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

void
printSudoku (CSP<int> csp)
{
	for ( int i=0; i < WIDTH*WIDTH; i++ ) {
		if (!(i % WIDTH))
			cout << endl;

		for ( int j=0; j < HEIGHT*HEIGHT; j++ ) {
			int id = toID(j,i);

			if (!(j % HEIGHT))
				cout << "  ";

			if (csp.isSet(id))
				cout << csp.value(id) << " ";
			else
				cout << ". ";
		}

		cout << endl;
	}

	cout << endl;
}

CSP<int>
getSudoku (const char* sudokuFile)
{
	string line;
	vector< vector<int> > sudoku;
	ifstream in(sudokuFile);

	if (!in)
		throw "Unable to read input file";

	for ( HEIGHT = 0; getline(in, line); HEIGHT++ )  {
		vector<int> values;
		string value = "";
		bool hasNumbers = false;

		for ( int j=0; j < line.length(); j++)  {
			if (line[j] >= '0' && line[j] <= '9')  {
				value += line[j];
				hasNumbers = true;
			} else {
				if (value.length() > 0)  {
					values.push_back( atoi(value.c_str()) );
					value = "";
				}
			}
		}

		if (!hasNumbers)  {
			HEIGHT--;
			continue;
		}

		if (value.length() > 0)  {
			values.push_back( atoi(value.c_str()) );
			value = "";
		}

		if ( HEIGHT == 0 )
			WIDTH = values.size();
		else  {
			if (values.size() != WIDTH)  {
				in.close();
				throw "The columns have different number of elements";
			}
		}

		sudoku.push_back(values);
	}

	in.close();

	if (HEIGHT != WIDTH)
		throw "The grid is not a square";

	WIDTH  = (int) sqrt(WIDTH);
	HEIGHT = (int) sqrt(HEIGHT);
	vector<int> domain;

	for ( int i=1; i <= WIDTH*WIDTH; i++)
		domain.push_back(i);

	CSP<int> csp( WIDTH*WIDTH * HEIGHT*HEIGHT, NOVALUE, false);
	csp.appendConstraint(rowsConstraint);
	csp.appendConstraint(colsConstraint);
	csp.appendConstraint(cellsConstraint);

	for ( int i=0; i < csp.size(); i++ )
		csp.setDomain(i, domain);

	for ( int i=0; i < sudoku.size(); i++ ) {
		for ( int j=0; j < sudoku[i].size(); j++ ) {
			if (sudoku[i][j] != 0)  {
				csp.setValue( toID(j,i), sudoku[i][j] );
			}
		}
	}
	
	return csp;
}

int
main ( int argc, char *argv[] )
{
	string sudokuFile;
	CSP<int> csp;

	if (argc == 1)  {
		cout << "Loading default sudoku from " << DEFAULT_SUDOKU_FILE
			<< ", pass a different file as argument if you want to "
			<< "load a different sudoku\n\n";
		sudokuFile = DEFAULT_SUDOKU_FILE;
	} else {
		sudokuFile = argv[1];
	}

	try  {
		csp = getSudoku(sudokuFile.c_str());
		cout << "Sudoku to be solved:\n";
		printSudoku(csp);
	}

	catch (const char* msg)  {
		cerr << "Exception: " << msg << endl;
		return EXIT_FAILURE;
	}

	cout << "Solving..." << endl;
	csp.solve();
	cout << endl;

	printSudoku(csp);

	if (csp.isSatisfiable())  {
		cout << "This sudoku has a solution ";

		if (csp.hasUniqueSolution())
			cout << "and the solution is unique\n";
		else
			cout << "but it wasn't possible to determine it univocally (missing values?)\n";
	} else
		cout << "This sudoku does not have any solution\n";

	return EXIT_SUCCESS;
}

