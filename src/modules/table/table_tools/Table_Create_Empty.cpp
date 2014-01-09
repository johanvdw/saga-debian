/**********************************************************
 * Version $Id: Table_Create_Empty.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Table_Tools                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 Table_Create_Empty.cpp                //
//                                                       //
//                 Copyright (C) 2005 by                 //
//              SAGA User Group Associaton               //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@gwdg.de                        //
//                                                       //
//    contact:    SAGA User Group Associaton             //
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
#include "Table_Create_Empty.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_NODE(i)	CSG_String::Format(SG_T("NODE%03d"), i)
#define GET_NAME(i)	CSG_String::Format(SG_T("NAME%03d"), i)
#define GET_TYPE(i)	CSG_String::Format(SG_T("TYPE%03d"), i)


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CTable_Create_Empty::CTable_Create_Empty(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Create New Table"));

	Set_Author		(SG_T("O. Conrad (c) 2005"));

	Set_Description	(_TW(
		"Creates a new empty table. "
		"Possible field types are:\n"
		" - character string\n"
		" - 1 byte integer\n"
		" - 2 byte integer\n"
		" - 4 byte integer\n"
		" - 4 byte floating point\n"
		" - 8 byte floating point\n"
		" - 32 bit true color (RGB)\n"
	));


	//-----------------------------------------------------
	Parameters.Add_Table_Output(
		NULL	, "TABLE"	, _TL("Table"),
		_TL("")
	);

	Parameters.Add_String(
		NULL	, "NAME"	, _TL("Name"),
		_TL(""),
		_TL("New Table")
	);

	Parameters.Add_Value(
		NULL	, "NFIELDS"		, _TL("Number of Attributes"),
		_TL(""),
		PARAMETER_TYPE_Int		, 2, 1, true
	);

	Parameters.Add_Parameters(
		NULL	, "FIELDS"		, _TL("Attributes"),
		_TL("")
	);

	//-----------------------------------------------------
	CSG_Parameters	*pAttributes	= Parameters("FIELDS")->asParameters();

	_Set_Field_Count(pAttributes, Parameters("NFIELDS")->asInt());

	pAttributes->Get_Parameter(GET_NAME(0))->Set_Value(_TL("ID"));
	pAttributes->Get_Parameter(GET_TYPE(0))->Set_Value(3);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CTable_Create_Empty::_Set_Field_Count(CSG_Parameters *pAttributes, int nAttributes)
{
	//-----------------------------------------------------
	CSG_String	Types;

	Types.Printf(SG_T("%s|%s|%s|%s|%s|%s|%s|"),
		_TL("character string"),
		_TL("1 byte integer"),
		_TL("2 byte integer"),
		_TL("4 byte integer"),
		_TL("4 byte floating point"),
		_TL("8 byte floating point"),
		_TL("color (rgb)")
	);

	//-----------------------------------------------------
	if( pAttributes && nAttributes > 0 )
	{
		int		nCurrent	= pAttributes->Get_Count() / 3;

		if( nCurrent < nAttributes )
		{
			for(int i=nCurrent; i<nAttributes; i++)
			{
				CSG_Parameter	*pNode	= pAttributes->Add_Node(
					NULL	, GET_NODE(i), CSG_String::Format(SG_T("%i. %s"), i + 1, _TL("Attribute")), _TL("")
				);

				pAttributes->Add_String(
					pNode	, GET_NAME(i), _TL("Name"), _TL(""), _TL("Name")
				);

				pAttributes->Add_Choice(
					pNode	, GET_TYPE(i), _TL("Type"), _TL(""), Types
				);
			}
		}
		else if( nCurrent > nAttributes )
		{
			CSG_Parameters	Tmp;
			Tmp.Assign(pAttributes);
			pAttributes->Destroy();
			pAttributes->Set_Name(Tmp.Get_Name());

			for(int i=0; i<nAttributes; i++)
			{
				CSG_Parameter	*pNode	= pAttributes->Add_Node(
					NULL	, GET_NODE(i), CSG_String::Format(SG_T("%i. %s"), i + 1, _TL("Attribute")), _TL("")
				);

				pAttributes->Add_String(
					pNode	, GET_NAME(i), _TL("Name"), _TL(""), Tmp(GET_NAME(i)) ? Tmp(GET_NAME(i))->asString() : _TL("Name")
				);

				pAttributes->Add_Choice(
					pNode	, GET_TYPE(i), _TL("Type"), _TL(""), Types, Tmp(GET_TYPE(i)) ? Tmp(GET_TYPE(i))->asInt() : 0
				);
			}
		}
	}
}

//---------------------------------------------------------
int CTable_Create_Empty::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("NFIELDS")) )
	{
		_Set_Field_Count(pParameters->Get_Parameter("FIELDS")->asParameters(), pParameter->asInt());

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CTable_Create_Empty::On_Execute(void)
{
	CSG_Table	*pTable	= SG_Create_Table();

	pTable->Set_Name(Parameters("NAME")->asString());

	//-----------------------------------------------------
	int				i, n;
	TSG_Data_Type	Type;
	CSG_Parameters	*pAttributes;

	pAttributes	= Parameters("FIELDS")->asParameters();
	n			= pAttributes->Get_Count() / 3;

	for(i=0; i<n; i++)
	{
		switch( pAttributes->Get_Parameter(GET_TYPE(i))->asInt() )
		{
		default:
		case 0:	Type	= SG_DATATYPE_String;	break;
		case 1:	Type	= SG_DATATYPE_Char  ;	break;
		case 2:	Type	= SG_DATATYPE_Short ;	break;
		case 3:	Type	= SG_DATATYPE_Int   ;	break;
		case 4:	Type	= SG_DATATYPE_Float ;	break;
		case 5:	Type	= SG_DATATYPE_Double;	break;
		case 6:	Type	= SG_DATATYPE_Color ;	break;
		}

		pTable->Add_Field(pAttributes->Get_Parameter(GET_NAME(i))->asString(), Type);
	}

	Parameters("TABLE")->Set_Value(pTable);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
