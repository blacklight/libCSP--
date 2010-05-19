/*
 * =====================================================================================
 *
 *       Filename:  csp++.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  17/05/2010 09:17:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Company:  lulz
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<algorithm>
#include	"csp++.h"
using namespace std;
// using std::vector;


template<class T>
void
CSP<T>::__init (int n, bool (*c)(vector< CSPvariable<T> >))
{
	variables = vector< CSPvariable<T> >(n);
	fixed = vector<bool>(n);
	__default_domains = vector< vector<T> >(n);

	for (size_t i=0; i < variables.size(); i++)  {
		variables[i].index = i;
		variables[i].fixed = false;
		// TODO Remove this line
		fixed[i] = false;
	}

	constraints = vector< bool (*)(std::vector< CSPvariable<T> >) >(1);
	constraint = c;
	__has_default_value = false;
}

template<class T>
CSP<T>::CSP (int n, bool (*c)(vector< CSPvariable<T> >))
{
	__init (n, c);
}

template<class T>
CSP<T>::CSP ( int n, T default_value, bool set_value, bool (*c)(std::vector< CSPvariable<T> >) )
{
	__init (n, c);

	for ( size_t i=0; i < variables.size(); i++ )  {
		variables[i].value = default_value;
		variables[i].fixed = set_value;
		// TODO Remove this line
		fixed[i] = set_value;
	}

	__default_value = default_value;
	__has_default_value = true;
}

template<class T>
void
CSP<T>::setDomain (size_t index, vector<T> domain)
{
	if (index >= variables.size())
		throw CSPexception("Index out of range");

	variables[index].domain = domain;
	__default_domains[index] = domain;
}

template<class T>
void
CSP<T>::setDomain (size_t index, T domain[], int size)
{
	if (index >= variables.size())
		throw CSPexception("Index out of range");

	if (size < 0)
		throw CSPexception("Invalid domain size");

	variables[index].domain = vector<T>(size);

	for (int i=0; i < size; i++)  {
		variables[index].domain[i] = domain[i];
		__default_domains[index].push_back(domain[i]);
	}
}

template<class T>
void
CSP<T>::setConstraint ( bool (*c)(vector< CSPvariable<T> >))
{
	constraints = vector< bool(*)(vector< CSPvariable<T > >) >(1);
	constraint = c;
}

template<class T>
void
CSP<T>::setConstraint ( std::vector< bool(*)(std::vector< CSPvariable<T> >) > c )
{
	constraints = c;
}

template<class T>
void
CSP<T>::appendConstraint ( bool (*c)(vector< CSPvariable<T> >))
{
	constraints.push_back(c);
}

template<class T>
void
CSP<T>::dropConstraint ( size_t index )
{
	if (index >= constraints.size())
		throw CSPexception("Index out of range");

	constraints.erase( constraints.begin() + index );
}

template<class T>
void
CSP<T>::restoreDomains ( void )
{
	for (int i=0; i < variables.size(); i++)
		variables[i].domain = __default_domains[i];
}

template<class T>
void
CSP<T>::refreshDomains ( void )
{
	vector< arc<T> > arcs;
	restoreDomains();

	for (typename vector< CSPvariable<T> >::iterator x = variables.begin(); x != variables.end(); x++)  {
		for (typename vector< CSPvariable<T> >::iterator y = variables.begin(); y != variables.end(); y++)  {
			for (size_t i=0; i < x->domain.size(); i++)  {
				if ( x->fixed )  {
				// TODO Remove this line
				// if ( fixed[ x - variables.begin() ] )  {
					if ( x->domain[i] != x->value )
						continue;
				}

				x->value = x->domain[i];

				for (size_t j=0; j < y->domain.size(); j++)  {
					if ( y->fixed )  {
					// TODO Remove this line
					// if ( fixed[ y - variables.begin() ] )  {
						if ( y->domain[j] != y->value )
							continue;
					}

					y->value = y->domain[j];
					bool isConfigurationValid = true;

					for ( typename std::vector< bool (*)(std::vector< CSPvariable<T> >) >::iterator c = constraints.begin();
							c != constraints.end() && isConfigurationValid;
							c++ ) {
						if (!(*c)(variables))
							isConfigurationValid = false;
					}

					if (isConfigurationValid)  {
						arc<T> a;
						a.var[0] = *x; a.var[1] = *y;
						a.value[0] = x->value; a.value[1] = y->value;
						arcs.push_back(a);
					}
				}
			}
		}

		vector<T> domain = vector<T>();

		for (size_t i=0; i < arcs.size(); i++)  {
			if (arcs[i].var[0].index == x->index ||
					arcs[i].var[1].index == x->index)  {
				T value = (arcs[i].var[0].index == x->index) ? arcs[i].value[0] : arcs[i].value[1];
				bool found = false;

				for (size_t j=0; j < domain.size() && !found; j++)  {
					if (domain[j] == value)
						found = true;
				}

				if (!found)  {
					domain.push_back(value);
				}
			}
		}

		sort(domain.begin(), domain.end());
		x->domain = domain;
	}
}

template<class T>
std::vector<T>
CSP<T>::getDomain ( size_t index )
{
	if (index >= variables.size())
		throw CSPexception("Index out of range");

	return variables[index].domain;
}

template<class T>
size_t
CSP<T>::getSize( void )
{
	return variables.size();
}

template<class T>
void
CSP<T>::setValue ( size_t index, T value )
{
	if (index >= variables.size())
		throw CSPexception("Index out of range");

	variables[index].value = value;
	variables[index].fixed = true;
	// TODO Remove this line
	// fixed[index] = true;
}

template<class T>
void
CSP<T>::unsetValue ( size_t index )
{
	if (index >= variables.size())
		throw CSPexception("Index out of range");

	if (__has_default_value)
		variables[index].value = __default_value;
	variables[index].fixed = false;
	// TODO Remove this line
	// fixed[index] = false;
}

template<class T>
bool
CSP<T>::isSatisfiable ( void )
{
	for ( size_t i=0; i < variables.size(); i++ ) {
		if ( variables[i].domain.empty() )
			return false;
	}

	return true;
}

template<class T>
bool
CSP<T>::hasUniqueSolution ( void )
{
	for ( size_t i=0; i < variables.size(); i++ ) {
		if (variables[i].domain.size() != 1)
			return false;
	}

	return true;
}

