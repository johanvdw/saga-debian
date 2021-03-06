/**********************************************************
 * Version $Id: Grid_Pattern.h 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    Grid_Pattern.h
    Copyright (C) Victor Olaya
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, USA
*******************************************************************************/ 

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Grid_Pattern_H
#define HEADER_INCLUDED__Grid_Pattern_H

//---------------------------------------------------------
#include "MLB_Interface.h"

//---------------------------------------------------------
class CGrid_Pattern : public CSG_Module_Grid {

public:
	CGrid_Pattern(void);
	virtual ~CGrid_Pattern(void);

private:
	CSG_Grid* m_pInput;
	int m_iWinSize;
	int m_iNumClasses;
	
	double getDiversity(int, int);
	int getCVN(int, int);
	int getNumberOfClasses(int, int);

protected:
	virtual bool On_Execute(void);

};

#endif // #ifndef HEADER_INCLUDED__Grid_Pattern_H
