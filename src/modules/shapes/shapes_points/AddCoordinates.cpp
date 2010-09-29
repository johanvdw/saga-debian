/*******************************************************************************
    AddCoordinates.cpp
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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/ 

#include "AddCoordinates.h"

//---------------------------------------------------------
CAddCoordinates::CAddCoordinates(void)
{	
	Set_Name		(_TL("Add Coordinates to points"));

	Set_Author		(SG_T("Victor Olaya (c) 2004"));

	Set_Description	(_TW(
		"(c) 2004 by Victor Olaya."
	));

	Parameters.Add_Shapes(
		NULL, "INPUT"	, _TL("Points"),
		_TL(""),
		PARAMETER_INPUT
	);	

	Parameters.Add_Shapes(
		NULL, "OUTPUT"	, _TL("Output"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);	
}

//---------------------------------------------------------
bool CAddCoordinates::On_Execute(void)
{
	CSG_Shapes	*pShapes	= Parameters("OUTPUT")->asShapes();

	if( pShapes )
	{
		pShapes->Assign(Parameters("INPUT")->asShapes());
	}
	else
	{
		pShapes	= Parameters("INPUT")->asShapes();
	}

	//-----------------------------------------------------
	int	xField	= pShapes->Get_Field_Count();
	pShapes->Add_Field("X", SG_DATATYPE_Double);

	int	yField	= pShapes->Get_Field_Count();
	pShapes->Add_Field("Y", SG_DATATYPE_Double);

	//-----------------------------------------------------
	for(int i=0; i<pShapes->Get_Count(); i++)
	{
		CSG_Shape	*pShape	= pShapes->Get_Shape(i);

		pShape->Set_Value(xField, pShape->Get_Point(0).x);
		pShape->Set_Value(yField, pShape->Get_Point(0).y);
	}

	return( true );
}
