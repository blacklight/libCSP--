/*
 * =====================================================================================
 *
 *       Filename:  csp++.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  16/05/2010 23:16:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Company:  lulz
 *
 * =====================================================================================
 */

#ifndef __CSPPP_H
#define __CSPPP_H

#include	<vector>
#include	<exception>

template<class T>
struct CSPvariable  {
	int index;
	bool fixed;
	T value;
	std::vector<T> domain;
};

/**
 * \class CSPexception csp++.h
 * \brief Class for managing exception in CSP
 */
class CSPexception : public std::exception  {
	const char* message;

public:
	CSPexception (const char *m)  {
		message = m;
	}

	virtual const char* what()  {
		return message;
	}
};

/**
 * \class CSP csp++.h
 * \brief Main class for managing a CSP
 */
template<class T>
class CSP  {
private:
	template<class TT>
	struct arc  {
		CSPvariable<TT> var[2];
		TT value[2];
	};

	std::vector<bool> fixed;
	std::vector< CSPvariable<T> > variables;
	std::vector< bool (*)(std::vector< CSPvariable<T> >) > constraints;
	#define 	constraint 	constraints[0]

	std::vector< std::vector<T> > __default_domains;
	T __default_value;
	bool __has_default_value;
	static bool __default_constraint ( std::vector< CSPvariable<T> > v )  { return true; }
	void __init ( int n, bool (*c)(std::vector< CSPvariable<T> >) );
	void restoreDomains ( void );

public:
	/**
	 * \brief  Class constructor
	 * \param  n Number of variables in the CSP
	 * \param  c Boolean function representing the constraint of the CSP
	 *           If no constraint function is set in the constructor or
	 *           using the applyConstraint() method, a default function
	 *           always returning true will be used, that allows any
	 *           domain for any variable to be valid (no constraint)
	 */
	CSP ( int n, bool (*c)(std::vector< CSPvariable<T> >) = __default_constraint );

	/**
	 * \brief  Class constructor
	 * \param  n Number of variables in the CSP
	 * \param  default_value Default value for the variables in the CSP
	 *         when initialized
	 * \param  set_variables Decide whether mark the variables set with
	 *         default_value as "set" or "not set" (default: not set)
	 * \param  c Boolean function representing the constraint of the CSP
	 *           If no constraint function is set in the constructor or
	 *           using the applyConstraint() method, a default function
	 *           always returning true will be used, that allows any
	 *           domain for any variable to be valid (no constraint)
	 */
	CSP ( int n, T default_value, bool set_variables = false, bool (*c)(std::vector< CSPvariable<T> >) = __default_constraint );

	/**
	 * METHOD: setDomain
	 * \brief  Set the domain for the i-th variable
	 * \param  index Variable for which we're setting the domain
	 * \param  domain Vector containing the possible values for that variable
	 */
	void setDomain ( size_t index, std::vector<T> domain );

	/**
	 * METHOD: setDomain
	 * \brief  Set the domain for the i-th variable
	 * \param  index Variable for which we're setting the domain
	 * \param  domain Array containing the possible values for that variable
	 * \param  size Size of "domain" array
	 */
	void setDomain ( size_t index, T domain[], int size );

	/**
	 * METHOD: setConstraint
	 * \brief  Apply the constraint to the CSP as a boolean function
	 * \param  c Boolean function representing the constraint of the CSP
	 */
	void setConstraint ( bool (*c)(std::vector< CSPvariable<T> >) );

	/**
	 * METHOD: setConstraint
	 * \brief  Apply the constraints to the CSP as vector of boolean functions
	 * \param  c Vector containing pointers to boolean functions representing
	 *           the constraints of the CSP
	 */
	void setConstraint ( std::vector< bool(*)(std::vector< CSPvariable<T> >) > c );

	/**
	 * METHOD: dropConstraint
	 * \brief  Drops a constraint from the CSP
	 * \param  index Index of the constraint to be dropped
	 */
	void dropConstraint ( size_t index );

	/**
	 * METHOD: appendConstraint
	 * \brief  Append a constraint to the list of the constraint of the CSP
	 * \param  c A function pointer returning a boolean value representing the new constraint
	 */
	void appendConstraint ( bool (*c)(std::vector< CSPvariable<T> >) );

	/**
	 * METHOD: refreshDomains
	 * \brief  Updates the domains of the variables. Any constraint or node fixed value is applied
	 */
	void refreshDomains( void );

	/**
	 * METHOD: getDomain
	 * \brief  Get the domain of the i-th variable
	 * \param  index Variable for which we're going to get the domain
	 * \return The domain of the i-th variable as a vector of T
	 */
	std::vector<T> getDomain ( size_t index );

	/**
	 * METHOD: getSize
	 * \brief  Get the number of variables in the current CSP
	 * \return Size of the CSP
	 */
	size_t getSize ( void );
	
	/**
	 * METHOD: setValue
	 * \brief  Set the value of a variable as a constraint
	 * \param  index Index of the parameter to be set
	 * \param  value Value to be set
	 */
	void setValue ( size_t index, T value );

	/**
	 * METHOD: unsetValue
	 * \brief  Marks a variable as not set, and if a default value was assigned
	 *         in the CSP constructor, this value will be set. By default, unless
	 *         specified in the constructor, all the variables are considered as
	 *         not set
	 * \param  index Index of the variable to be unset
	 */
	void unsetValue ( size_t index );

	/**
	 * METHOD: isSatisfiable
	 * \brief  Check if the current CSP, with the applied constraints, is satisfiable
	 * \return true if the CSP has at least a possible solution, false otherwise
	 */
	bool isSatisfiable ( void );

	
	/**
	 * METHOD: hasUniqueSolution
	 * \brief  Check if the CSP, with the given variables, domains and constraints,
	 *         admits a unique solution, i.e. each domain of each variable contains
	 *         an only element
	 * \return true if the CSP has an only possible solution, false otherwise
	 */
	bool hasUniqueSolution ( void );
};

#endif

