/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
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
#include  <string>
#include	<vector>
#include	<cstdlib>
#include  <cstring>
#include	<csp++/csp++.h>

using namespace std;

typedef enum  {
	I, CH, DK, D, A, F, E, P, B, L, NL
} Country;

typedef enum  {
	red, green, blue, yellow
} Colour;

#define 	COUNTRIES 	11
#define 	COLOURS 		4

static const char* countries[COUNTRIES] = {
	"Italy", "Switz.", "Denmark", "Germany", "Austria", "France", "Spain", "Portugal", "Belgium", "Luxemb.", "Holland"
};

static const char* colours[COLOURS] = {
	"red", "green", "blue", "yellow"
};

/**
 * FUNCTION: c
 *
 * Constraint function, it expresses the logic of the constraint. In our case,
 * the adjoining countries must have different colours. Each colours condition
 * is verified only if both the values have been set ("fixed" field in CSPvariable
 * structure), in our case by the user, in order to avoid an inconstistent CSP
 * from random values found inside of the variables before the initialization
 */
bool
c ( std::vector< CSPvariable<Colour> > variables)  {
	return (
			( !(variables[I] .fixed || variables[CH].fixed) || (variables[I] .value != variables[CH].value) ) &&
			( !(variables[I] .fixed || variables[A] .fixed) || (variables[I] .value != variables[A] .value) ) &&
			( !(variables[I] .fixed || variables[F] .fixed) || (variables[I] .value != variables[F] .value) ) &&
			( !(variables[A] .fixed || variables[CH].fixed) || (variables[A] .value != variables[CH].value) ) &&
			( !(variables[A] .fixed || variables[D] .fixed) || (variables[A] .value != variables[D] .value) ) &&
			( !(variables[CH].fixed || variables[D] .fixed) || (variables[CH].value != variables[D] .value) ) &&
			( !(variables[CH].fixed || variables[F] .fixed) || (variables[CH].value != variables[F] .value) ) &&
			( !(variables[D] .fixed || variables[F] .fixed) || (variables[D] .value != variables[F] .value) ) &&
			( !(variables[E] .fixed || variables[F] .fixed) || (variables[E] .value != variables[F] .value) ) &&
			( !(variables[E] .fixed || variables[P] .fixed) || (variables[E] .value != variables[P] .value) ) &&
			( !(variables[B] .fixed || variables[F] .fixed) || (variables[B] .value != variables[F] .value) ) &&
			( !(variables[B] .fixed || variables[D] .fixed) || (variables[B] .value != variables[D] .value) ) &&
			( !(variables[B] .fixed || variables[NL].fixed) || (variables[B] .value != variables[NL].value) ) &&
			( !(variables[B] .fixed || variables[L] .fixed) || (variables[B] .value != variables[L] .value) ) &&
			( !(variables[L] .fixed || variables[F] .fixed) || (variables[L] .value != variables[F] .value) ) &&
			( !(variables[L] .fixed || variables[D] .fixed) || (variables[L] .value != variables[D] .value) ) &&
			( !(variables[NL].fixed || variables[D] .fixed) || (variables[NL].value != variables[D] .value) ) &&
			( !(variables[DK].fixed || variables[D] .fixed) || (variables[DK].value != variables[D] .value) )
		);
}

/**
 * FUNCTION: printDomain
 *
 * Given the CSP and the index of the variable, prints its allowed domain
 */
void
printDomain (CSP<Colour> csp, int variable)
{
	cout << "[ ";

	for ( size_t i=0; i < csp.getDomain(variable).size(); i++ ) {
		cout << colours[csp.getDomain(variable)[i]];

		if ( i < csp.getDomain(variable).size() - 1 )
			cout << ", ";
	}

	cout << " ]";
}

/**
 * FUNCTION: printDomains
 *
 * Given the CSP, prints the domains of all the variables
 */
void
printDomains (CSP<Colour> csp)
{
	for ( size_t i=0; i < csp.getSize(); i++)  {
		cout << "Domain for variable '" << countries[i] << "':\t";
		printDomain(csp, i);
		cout << endl;
	}

	cout << endl;

	if (csp.isSatisfiable())  {
		cout << "The CSP has a solution\n";

		if (csp.hasUniqueSolution())
			cout << "The solution is unique\n";
	} else
		cout << "The CSP does not have a solution\n";
}

/**
 * FUNCTION: getIndexByColour
 *
 * Given a colour as string, it picks up the associated index,
 * if the colour exists in the list, otherwise it throws an
 * evil exception
 */
size_t
getIndexByColour ( const char* colour )
{
	for ( size_t i=0; i < COLOURS; i++ ) {
		if (!strcmp(colours[i], colour))
			return i;
	}

	throw 666;
}

/**
 * FUNCTION: valueOK
 *
 * Given the CSP, the index of a variable and a value, it checks
 * if the given value is consistent to the domain of that variable
 */
bool
valueOK ( CSP<Colour> csp, size_t variable, Colour value )  {
	for ( size_t i=0; i < csp.getDomain(variable).size(); i++ ) {
		if (csp.getDomain(variable)[i] == value)
			return true;
	}

	return false;
}

int
main ( int argc, char *argv[] )
{
	vector<Colour> domain;

	// Create the domain containing all the allowed colours
	for ( int i=0; i < COLOURS; i++)
		domain.push_back((Colour) i);

	// The CSP will contain as many variables as the number of countries,
	// applying the logical constraint specified in "c" function
	CSP<Colour> csp(COUNTRIES, c);

	// Set the domain for the variables
	for ( size_t i=0; i < COUNTRIES; i++ )
		csp.setDomain(i, domain);

	// Repeat until we don't find a unique solution for the CSP
	while (!csp.hasUniqueSolution())  {
		for ( size_t i=0; i < COUNTRIES && !csp.hasUniqueSolution(); i++ )  {
			bool satisfiable = true;

			// If a variable has an empty domain, something went wrong
			// if (csp.getDomain(i).size() == 0)  {
			// 	cout << "No values left for country " << countries[i]
			// 		<< ", the domain was probably too small (few colours) or the problem is unsatisfiable\n";
			// 	return EXIT_FAILURE;
			// }

			// If a variable has a domain containing an only element,
			// just choose it without annoying the user
			if (csp.getDomain(i).size() == 1)  {
				cout << "Setting colour " << colours[csp.getDomain(i)[0]] << " for " << countries[i] << endl;
				csp.setValue( i, csp.getDomain(i)[0] );
				csp.refreshDomains();
				continue;
			}

			// Repeat the loop until the user inserts a valid colour for the CSP
			do  {
				bool found;
				size_t col_index;

				// Repeat the loop until the users inserts a colour in the list
				do  {
					string colour;

					cout << "Insert a colour for country '" << countries[i] << "' between " << endl << "\t";
					printDomain(csp, i);
					cout << ": ";
					getline (cin, colour);

					try  {
						col_index = getIndexByColour(colour.c_str());
						found = true;
					}

					catch (int e)  {
						cout << "Invalid input, please try again\n";
						found = false;
					}
				} while (!found);

				// Check if the inserted colour is valid for the domain of the variable
				satisfiable = valueOK(csp, i, (Colour) col_index);

				if (!satisfiable)  {
					cout << "You made an invalid choice according to the current constraints, please try again\n";
				} else {
					csp.setValue(i, (Colour) col_index);
					csp.refreshDomains();
				}
			} while (!satisfiable);
		}
	}

	cout << endl;
	printDomains(csp);
	return EXIT_SUCCESS;
}

