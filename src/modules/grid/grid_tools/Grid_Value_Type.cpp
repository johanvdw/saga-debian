/**********************************************************
 * Version $Id: Grid_Value_Type.cpp 1497 2012-10-22 15:25:48Z manfred-e $
 *********************************************************/

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
	//-----------------------------------------------------
	Set_Name(_TL("Convert Data Storage Type"));

	Set_Author		(SG_T("O.Conrad (c) 2003"));

	Set_Description	(_TW(
		"Changes the storage data type of a grid "
		"(e.g. from 4 byte floating point to 2 byte signed integer). "
		"If the target is not set, the original grid's storage type will be changed. ")
	);

	//-----------------------------------------------------
	Parameters.Add_Grid(
		NULL	, "INPUT"	, _TL("Grid"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "OUTPUT"	, _TL("Converted Grid"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Choice(
		NULL	, "TYPE"	, _TL("Data storage type"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|%s|%s|"),
			SG_Data_Type_Get_Name(SG_DATATYPE_Bit   ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Byte  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Char  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Word  ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Short ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_DWord ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Int   ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Float ).c_str(),
			SG_Data_Type_Get_Name(SG_DATATYPE_Double).c_str()
		)
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Value_Type::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Grid	*pOutput	= Parameters("OUTPUT")	->asGrid();
	CSG_Grid	*pInput		= Parameters("INPUT")	->asGrid();

	if( pOutput == NULL || pOutput == pInput )
	{
		pOutput	= pInput;
		pInput	= new CSG_Grid(*pOutput);
	}

	//-----------------------------------------------------
	switch( Parameters("TYPE")->asInt() )
	{
	default:
	case 0:	pOutput->Create(*Get_System(), SG_DATATYPE_Bit   );	break;
	case 1:	pOutput->Create(*Get_System(), SG_DATATYPE_Byte  );	break;
	case 2:	pOutput->Create(*Get_System(), SG_DATATYPE_Char  );	break;
	case 3:	pOutput->Create(*Get_System(), SG_DATATYPE_Word  );	break;
	case 4:	pOutput->Create(*Get_System(), SG_DATATYPE_Short );	break;
	case 5:	pOutput->Create(*Get_System(), SG_DATATYPE_DWord );	break;
	case 6:	pOutput->Create(*Get_System(), SG_DATATYPE_Int   );	break;
	case 7:	pOutput->Create(*Get_System(), SG_DATATYPE_Float );	break;
	case 8:	pOutput->Create(*Get_System(), SG_DATATYPE_Double);	break;
	}

	//-----------------------------------------------------
	
	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		#pragma omp parallel for
		for(int x=0; x<Get_NX(); x++)
		{
			if( pInput->is_NoData(x, y) )
			{
			  pOutput->Set_NoData(x, y);
			}
			else
			{
			  pOutput->Set_Value(x, y, pInput->asDouble(x, y));	    
			}
		}
	}

	//-----------------------------------------------------
	if( pInput != Parameters("INPUT")->asGrid() )
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
