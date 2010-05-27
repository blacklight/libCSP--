/*
 * =====================================================================================
 *
 *       Filename:  csp++-def.h
 *
 *    Description:  Header file containing the CSP class definition. Never include this
 *                  file directly in your sources. Include csp++.h instead.
 *
 *        Version:  0.1.1
 *        Created:  16/05/2010 23:16:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  lulz
 *
 * =====================================================================================
 */

#ifndef __CSPPP_H
#define __CSPPP_H

#ifndef __CSPPP_CPP
#error  "csp++-def.h must not be included directly - include csp++.h instead"
#endif

#define  __CSPPP_VERSION 	"0.1.1"

#include	<vector>
#include	<exception>

/**
 * \struct CSPvariable csp++.h
 * \brief Struct used for describing a variable in the CSP
 */
template<class T>
struct CSPvariable  {
	//! Index of the variable
	int index;
	
	//! If the value of the variable is set explicitely, this value is true
	bool fixed;
	
	//! The value of the variable
	T value;
	
 	//! Domain of the variable, declared as a vector
	std::vector<T> domain;
};

/**
 * \class CSPexception csp++.h
 * \brief Class for managing exception in CSP
 */
class CSPexception : public std::exception  {
	const char* message;

public:
	/**
	 * \brief Constructor
	 * \param m Message describing the exception
	 */
	CSPexception (const char *m)  {
		message = m;
	}

	/**
	 * \brief Return the description of the exception
	 */
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
	 * \brief Empty constructor - just do nothing, used for declaring an object and
	 *        initialize it later
	 */
	CSP()  {}
	
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
	 * \brief  Set the domain for the i-th variable
	 * \param  index Variable for which we're setting the domain
	 * \param  domain Vector containing the possible values for that variable
	 */
	void setDomain ( size_t index, std::vector<T> domain );

	/**
	 * \brief  Set the domain for the i-th variable
	 * \param  index Variable for which we're setting the domain
	 * \param  domain Array containing the possible values for that variable
	 * \param  size Size of "domain" array
	 */
	void setDomain ( size_t index, T domain[], int size );

	/**
	 * \brief  Apply the constraint to the CSP as a boolean function
	 * \param  c Boolean function representing the constraint of the CSP
	 */
	void setConstraint ( bool (*c)(std::vector< CSPvariable<T> >) );

	/**
	 * \brief  Apply the constraints to the CSP as vector of boolean functions
	 * \param  c Vector containing pointers to boolean functions representing
	 *           the constraints of the CSP
	 */
	void setConstraint ( std::vector< bool(*)(std::vector< CSPvariable<T> >) > c );

	/**
	 * \brief  Drops a constraint from the CSP
	 * \param  index Index of the constraint to be dropped
	 */
	void dropConstraint ( size_t index );

	/**
	 * \brief  Append a constraint to the list of the constraint of the CSP
	 * \param  c A function pointer returning a boolean value representing the new constraint
	 */
	void appendConstraint ( bool (*c)(std::vector< CSPvariable<T> >) );

	/**
	 * \brief  Updates the domains of the variables. Any constraint or node fixed value is applied
	 * \param  max_iterations Maximum number of iterations, after which the function will quit. This
	 *         can be used to break endless loops in domains assignations.
	 */
	void refreshDomains ( void );

	/**
	 * \brief Get, if it exists, the solution of the CSP, calling refreshDomains until a fixed point
	 *        is reached
	 */
	void solve ( size_t maxIterations = 0 );

	/**
	 * \brief  Get the domain of the i-th variable
	 * \param  index Variable for which we're going to get the domain
	 * \return The domain of the i-th variable as a vector of T
	 */
	std::vector<T> domain ( size_t index );

	/**
	 * \brief  Get the number of variables in the current CSP
	 * \return Size of the CSP
	 */
	size_t size ( void );
	
	/**
	 * \brief  Set the value of a variable as a constraint
	 * \param  index Index of the parameter to be set
	 * \param  value Value to be set
	 */
	void setValue ( size_t index, T value );

	/**
	 * \brief  Marks a variable as not set, and if a default value was assigned
	 *         in the CSP constructor, this value will be set. By default, unless
	 *         specified in the constructor, all the variables are considered as
	 *         not set
	 * \param  index Index of the variable to be unset
	 */
	void unsetValue ( size_t index );

	/**
	 * \brief  Check if the current CSP, with the applied constraints, is satisfiable
	 * \return true if the CSP has at least a possible solution, false otherwise
	 */
	bool isSatisfiable ( void );

	/**
	 * \brief  Check if the CSP, with the given variables, domains and constraints,
	 *         admits a unique solution, i.e. each domain of each variable contains
	 *         an only element
	 * \return true if the CSP has an only possible solution, false otherwise
	 */
	bool hasUniqueSolution ( void );

	/**
	 * \brief  Check if any of the variables in the CSP has a domain containing an only
	 *         item. If so, it automatically assignes that item as value of the variable
	 */

	void assignUniqueDomains ( void );
	/**
	 * \brief  Check if the i-th variable in the CSP has a fixed value
	 * \param  i Index of the variable to be checked
	 * \return true if the i-th variable has a fixed value, false otherwise
	 */
	bool isSet ( size_t i );

	/**
	 * \brief  Get the value of the i-th variable of the CSP. Be careful: before
	 *         using this function you should check whether the variable you're
	 *         going to check is actually set, using the isSet() method, unless
	 *         you know what you're doing, in order to avoid reading inconsistent
	 *         data
	 * \param  i Index of the variable to get
	 * \return The value of the variable, if the variable exists
	 */
	T value ( size_t i );
};

#endif

