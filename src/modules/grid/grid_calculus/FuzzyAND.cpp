/**********************************************************
 * Version $Id: FuzzyAND.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     Grid_Calculus                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     FuzzyAND.cpp                      //
//                                                       //
//                 Copyright (C) 2004 by                 //
//           Antonio Boggia and Gianluca Massei          //
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
//    e-mail:     boggia@unipg.it						 //
//				  g_massa@libero.it				     	 //
//                                                       //
//    contact:    Antonio Boggia                         //
//                Gianluca Massei                        //
//                Department of Economics and Appraisal  //
//                University of Perugia - Italy			 //
//                www.unipg.it                           //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "FuzzyAND.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CFuzzyAND::CFuzzyAND(void)
{
	Set_Name		(_TL("Fuzzy Intersection (AND)"));

	Set_Author		(SG_T("Antonio Boggia and Gianluca Massei (c) 2004"));

	Set_Description	(_TW(
		"Calculates the intersection (min operator) for each grid cell of the selected grids.\n "
		"e-mail Gianluca Massei: g_massa@libero.it \n"
		"e-mail Antonio Boggia: boggia@unipg.it \n"
	));

	Parameters.Add_Grid_List(
		NULL, "GRIDS"	, _TL("Grids"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL, "AND"		, _TL("Intersection"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice(
		NULL, "TYPE"	, _TL("Operator Type"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("min(a, b) (non-interactive)"),
			_TL("a * b"),
			_TL("max(0, a + b - 1)")
		), 0
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CFuzzyAND::On_Execute(void)
{
	int						Type;
	CSG_Grid				*pAND;
	CSG_Parameter_Grid_List	*pGrids;

	//-----------------------------------------------------
	pGrids	= Parameters("GRIDS")	->asGridList();
	pAND	= Parameters("AND")		->asGrid();
	Type	= Parameters("TYPE")	->asInt();

	//-----------------------------------------------------
	if( pGrids->Get_Count() < 1 )
	{
		return( false );
	}

	//-----------------------------------------------------
	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=0; x<Get_NX(); x++)
		{
			bool	bNoData	= pGrids->asGrid(0)->is_NoData(x, y);
			double	AND		= pGrids->asGrid(0)->asDouble (x, y);

			for(int i=1; i<pGrids->Get_Count() && !bNoData; i++)
			{
				if( !(bNoData = pGrids->asGrid(i)->is_NoData(x, y)) )
				{
					double	iz	= pGrids->asGrid(i)->asDouble(x, y);

					switch( Type )
					{
					case 0:
						if( AND > iz )
						{
							AND	= iz;
						}
						break;

					case 1:
						AND	*= iz;
						break;

					case 2:
						if( (AND = AND + iz - 1.0) < 0.0 )
						{
							AND	= 0.0;
						}
						break;
					}
				}
			}

			if( bNoData )
			{
				pAND->Set_NoData(x, y);
			}
			else
			{
				pAND->Set_Value(x, y, AND);
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
