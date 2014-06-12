/**********************************************************
 * Version $Id: QueryBuilder.cpp 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    QueryBuilder.cpp
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
#include "QueryBuilder.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSelect_Numeric::CSelect_Numeric(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name		(_TL("Select by Attributes... (Numerical Expression)"));

	Set_Author		(SG_T("V.Olaya (c) 2004, O.Conrad (c) 2011"));

	Set_Description	(_TW(
		"Selects records for which the expression is true."
	));

	//-----------------------------------------------------
	pNode	= Parameters.Add_Shapes(
		NULL	, "SHAPES"		, _TL("Shapes"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Table_Field(
		pNode	, "FIELD"		, _TL("Attribute"),
		_TL("attribute to be searched; if not set all attributes will be searched"),
		true
	);

	Parameters.Add_String(
		NULL	, "EXPRESSION"	, _TL("Expression"),
		_TL(""),
		SG_T("a > 0")
	);

	Parameters.Add_Choice(
		NULL	, "METHOD"		, _TL("Method"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|"),
			_TL("new selection"),
			_TL("add to current selection"),
			_TL("select from current selection"),
			_TL("remove from current selection")
		), 0
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSelect_Numeric::On_Execute(void)
{
	int				Method, Field;
	CSG_String		Expression;
	CSG_Shapes		*pShapes;
	CSG_Formula		Formula;

	//-----------------------------------------------------
	pShapes		= Parameters("SHAPES")		->asShapes();
	Field		= Parameters("FIELD")		->asInt();
	Expression	= Parameters("EXPRESSION")	->asString();
	Method		= Parameters("METHOD")		->asInt();

	//-----------------------------------------------------
	if( !Formula.Set_Formula(Expression) )
	{
		CSG_String	Message;

		if( Formula.Get_Error(Message) )
		{
			Error_Set(Message);
		}

		return( false );
	}

	//-----------------------------------------------------
	double	*Values	= new double[pShapes->Get_Field_Count()];

	for(int i=0; i<pShapes->Get_Count() && Set_Progress(i, pShapes->Get_Count()); i++)
	{
		CSG_Shape	*pShape	= pShapes->Get_Shape(i);

		if( Field >= pShapes->Get_Field_Count() )
		{
			for(int j=0; j<pShapes->Get_Field_Count(); j++)
			{
				Values[j]	= pShape->asDouble(j);
			}
		}
		else
		{
			Values[0]	= pShape->asDouble(Field);
		}

		switch( Method )
		{
		case 0:	// New selection
			if( ( pShape->is_Selected() && !Formula.Get_Value(Values, pShapes->Get_Field_Count()))
			||	(!pShape->is_Selected() &&  Formula.Get_Value(Values, pShapes->Get_Field_Count())) )
			{
				pShapes->Select(i, true);
			}
			break;

		case 1:	// Add to current selection
			if(  !pShape->is_Selected() &&  Formula.Get_Value(Values, pShapes->Get_Field_Count()) )
			{
				pShapes->Select(i, true);
			}
			break;

		case 2:	// Select from current selection
			if(   pShape->is_Selected() && !Formula.Get_Value(Values, pShapes->Get_Field_Count()) )
			{
				pShapes->Select(i, true);
			}
			break;

		case 3:	// Remove from current selection
			if(   pShape->is_Selected() &&  Formula.Get_Value(Values, pShapes->Get_Field_Count()) )
			{
				pShapes->Select(i, true);
			}
			break;
		}
	}

	delete[](Values);

	//-----------------------------------------------------
	Message_Add(CSG_String::Format(SG_T("%s: %d"), _TL("selected shapes"), pShapes->Get_Selection_Count()));

	DataObject_Update(pShapes);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
