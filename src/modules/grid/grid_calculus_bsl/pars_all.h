/**********************************************************
 * Version $Id: pars_all.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/
#ifndef __PARS_ALL_H 
#define __PARS_ALL_H 

#include "foreach.h"
#include "bedingung.h"

extern T_AnweisungList AnweisungList;

void pars_ausdruck_string(const std::string& s, T_AnweisungList & al);
void pars_ausdruck(int& zeile, int& pos);


#endif
