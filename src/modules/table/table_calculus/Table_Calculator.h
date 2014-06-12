/**********************************************************
 * Version $Id: Table_Calculator.h 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    TableCalculator.h
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

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CTable_Calculator_Base : public CSG_Module  
{
public:
	CTable_Calculator_Base(void);


protected:

	virtual bool			On_Execute			(void);

	CSG_String				Get_Formula			(CSG_String sFormula, CSG_Table *pTable, int *Fields, int &nFields);

};

//---------------------------------------------------------
class CTable_Calculator : public CTable_Calculator_Base  
{
public:
	CTable_Calculator(void);

};

//---------------------------------------------------------
class CTable_Calculator_Shapes : public CTable_Calculator_Base  
{
public:
	CTable_Calculator_Shapes(void);

	virtual CSG_String		Get_MenuPath		(void)	{	return( _TL("A:Shapes|Table") );	}

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
