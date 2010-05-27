/*
 * =====================================================================================
 *
 *       Filename:  csp++.h
 *
 *    Description:  
 *
 *        Version:  0.1.1
 *        Created:  23/05/2010 23:27:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  BlackLight (http://0x00.ath.cx), <blacklight@autistici.org>
 *        Licence:  GNU GPL v.3
 *        Company:  lulz
 *
 * =====================================================================================
 */

// Yes, I know it looks stupid, but ask C++ standard developers about this.
// C++ can't expand template class code until a type is explicitely specified.
// This is nearly like not using templates at all, but join me in asking C++
// compilers developers to massively allow the export keyword and change the
// way templates are managed. Until that moment, you must always include the
// .cpp file of the library inside your source code as well, and specify later
// the types you're going to use this template class for

#include  "csp++.cpp"

