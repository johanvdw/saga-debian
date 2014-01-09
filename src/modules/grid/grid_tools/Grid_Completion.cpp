/**********************************************************
 * Version $Id: Grid_Completion.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       Tutorial                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Grid_Completion.cpp                  //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
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
#include "Grid_Completion.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_Completion::CGrid_Completion(void)
{
	//-----------------------------------------------------
	// 1. Info...

	Set_Name		(_TL("Patching"));

	Set_Author		(SG_T("(c) 2003 by O.Conrad"));

	Set_Description	(_TW(
		"Fill gaps of a grid with data from another grid. "
	));


	//-----------------------------------------------------
	// 2. Standard in- and output...

	Parameters.Add_Grid(
		NULL	, "ORIGINAL"		, _TL("Grid"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "ADDITIONAL"		, _TL("Patch Grid"),
		_TL(""),
		PARAMETER_INPUT, false
	);

	Parameters.Add_Grid(
		NULL	, "COMPLETED"		, _TL("Completed Grid"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice(
		NULL	, "INTERPOLATION"	, _TL("Interpolation Method"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("Nearest Neighbor"),
			_TL("Bilinear Interpolation"),
			_TL("Inverse Distance Interpolation"),
			_TL("Bicubic Spline Interpolation"),
			_TL("B-Spline Interpolation")
		), 4
	);
}

//---------------------------------------------------------
CGrid_Completion::~CGrid_Completion(void)
{}


///////////////////////////////////////////////////////////
//														 //
//	Run													 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_Completion::On_Execute(void)
{
	int					x, y;
	double				xPos, yPos, Value;
	TSG_Grid_Interpolation	Interpolation;
	CSG_Grid				*pGrid, *pAdditional;

	pAdditional		= Parameters("ADDITIONAL")	->asGrid();
	pGrid			= Parameters("COMPLETED")	->asGrid();

	if( pGrid->is_Intersecting(pAdditional->Get_Extent()) )
	{
		if( pGrid != Parameters("ORIGINAL")->asGrid() )
		{
			Process_Set_Text(_TL("Copying original data..."));

			pGrid->Assign(Parameters("ORIGINAL")->asGrid());
		}

		Interpolation	= (TSG_Grid_Interpolation)Parameters("INTERPOLATION")->asInt();

		Process_Set_Text(_TL("Data completion..."));

		for(y=0, yPos=Get_YMin(); y<Get_NY() && Set_Progress(y, Get_NY()); y++, yPos+=Get_Cellsize())
		{
			if( yPos >= pAdditional->Get_YMin() )
			{
				for(x=0, xPos=Get_XMin(); x<Get_NX() && xPos<=pAdditional->Get_XMax(); x++, xPos+=Get_Cellsize())
				{
					if( pGrid->is_NoData(x, y) && xPos >= pAdditional->Get_XMin() )
					{
						if( !pAdditional->is_NoData_Value(Value = pAdditional->Get_Value(xPos, yPos, Interpolation)) )
						{
							pGrid->Set_Value(x, y, Value);
						}
					}
				}
			}
		}

		return( true );
	}

	Error_Set(_TL("Nothing to do: there is no intersection with additonal grid."));

	return( false );
}
