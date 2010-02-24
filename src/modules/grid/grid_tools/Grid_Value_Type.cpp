
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Grid_Tools                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Grid_Value_Type.cpp                  //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                      Olaf Conrad                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Grid_Value_Type.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_Value_Type::CGrid_Value_Type(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name(_TL("Convert Data Storage Type"));

	Set_Author		(SG_T("(c) 2003 by O.Conrad"));

	Set_Description	(_TW(
		"Changes the storage data type of a grid "
		"(e.g. from 4 byte floating point to 2 byte signed integer). "
		"If the target is not set, the original grid's storage type will be changed. ")
	);

	//-----------------------------------------------------
	pNode	= Parameters.Add_Grid(
		NULL	, "INPUT"	, _TL("Grid"),
		_TL(""),
		PARAMETER_INPUT
	);

	pNode	= Parameters.Add_Grid(
		NULL	, "OUTPUT"	, _TL("Converted Grid"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	pNode	= Parameters.Add_Choice(
		NULL	, "TYPE"	, _TL("Data storage type"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|%s|%s|"),
			_TL("1 bit"),
			_TL("1 byte unsigned"),
			_TL("1 byte signed"),
			_TL("2 byte unsigned"),
			_TL("2 byte signed"),
			_TL("4 byte unsigned"),
			_TL("4 byte signed"),
			_TL("4 byte floating point"),
			_TL("8 byte floating point")
		)
	);
}

//---------------------------------------------------------
CGrid_Value_Type::~CGrid_Value_Type(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Value_Type::On_Execute(void)
{
	bool			bCopy;
	TSG_Data_Type	Type;
	CSG_Grid		*pInput, *pOutput;

	//-----------------------------------------------------
	pOutput	= Parameters("OUTPUT")->asGrid();

	if( pOutput == NULL || pOutput == Parameters("INPUT")->asGrid() )
	{
		bCopy	= true;

		pInput	= new CSG_Grid(Parameters("INPUT")->asGrid());
		pInput->Assign(Parameters("INPUT")->asGrid());

		if( pOutput == NULL )
		{
			pOutput	= Parameters("INPUT")->asGrid();
		}
	}
	else
	{
		bCopy	= false;

		pInput	= Parameters("INPUT")->asGrid();
	}

	//-----------------------------------------------------
	switch( Parameters("TYPE")->asInt() )
	{
	default:
	case 0:	Type	= SG_DATATYPE_Bit;		break;
	case 1:	Type	= SG_DATATYPE_Byte;		break;
	case 2:	Type	= SG_DATATYPE_Char;		break;
	case 3:	Type	= SG_DATATYPE_Word;		break;
	case 4:	Type	= SG_DATATYPE_Short;	break;
	case 5:	Type	= SG_DATATYPE_DWord;	break;
	case 6:	Type	= SG_DATATYPE_Int;		break;
	case 7:	Type	= SG_DATATYPE_Float;	break;
	case 8:	Type	= SG_DATATYPE_Double;	break;
	}

	//-----------------------------------------------------
	pOutput->Create(pInput, Type);
	pOutput->Assign(pInput);

	//-----------------------------------------------------
	if( bCopy )
	{
		delete(pInput);
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
