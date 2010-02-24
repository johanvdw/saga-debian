
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Grid_Shapes                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Grid_To_Points.cpp                   //
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
#include "Grid_To_Points.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_To_Points::CGrid_To_Points(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Grid Values to Points"));

	Set_Author		(SG_T("(c) 2001 by O.Conrad"));

	Set_Description	(_TW(
		"This module saves grid values to a point shape. Optionally only points "
		"can be saved, which are contained by polygons of the specified shapes layer. "
		"In addition, it is possible to exclude all cells that are coded NoData in the "
		"first grid of the grid list."
	));


	//-----------------------------------------------------
	Parameters.Add_Grid_List(
		NULL	, "GRIDS"		, _TL("Grids"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Shapes(
		NULL	, "POLYGONS"	, _TL("Polygons"),
		_TL(""),
		PARAMETER_INPUT_OPTIONAL, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Shapes(
		NULL	, "POINTS"		, _TL("Points"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Value(
		NULL	, "NODATA"		, _TL("Exclude NoData Cells"),
		_TL(""),
		PARAMETER_TYPE_Bool		, 0.0
	);
}

//---------------------------------------------------------
CGrid_To_Points::~CGrid_To_Points(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_To_Points::On_Execute(void)
{
	bool					bZFactor, bNoNoData;
	int						x, y, iGrid, iPoint;
	double					xPos, yPos;
	CSG_Grid				*pGrid;
	CSG_Parameter_Grid_List	*pGrids;
	CSG_Shape				*pPoint;
	CSG_Shapes				*pPoints, *pPolygons;

	//-----------------------------------------------------
	pGrids		= Parameters("GRIDS")	->asGridList();
	pPolygons	= Parameters("POLYGONS")->asShapes();
	pPoints		= Parameters("POINTS")	->asShapes();
	bNoNoData	= Parameters("NODATA")	->asBool();

	bZFactor	= true;

	//-----------------------------------------------------
	if( pGrids->Get_Count() > 0 )
	{
		pPoints->Create(SHAPE_TYPE_Point, _TL("Points from Grid(s)"));

		pPoints->Add_Field("ID"	, SG_DATATYPE_Int);
		pPoints->Add_Field("X"	, SG_DATATYPE_Double);
		pPoints->Add_Field("Y"	, SG_DATATYPE_Double);

		for(iGrid=0; iGrid<pGrids->Get_Count(); iGrid++)
		{
			pPoints->Add_Field(CSG_String::Format(SG_T("%s"),pGrids->asGrid(iGrid)->Get_Name()).BeforeFirst(SG_Char('.')).c_str(), SG_DATATYPE_Double);
		}

		//-------------------------------------------------
		for(y=0, yPos=Get_YMin(), iPoint=0; y<Get_NY() && Set_Progress(y); y++, yPos+=Get_Cellsize())
		{
			for(x=0, xPos=Get_XMin(); x<Get_NX(); x++, xPos+=Get_Cellsize())
			{
				if( (!bNoNoData || (bNoNoData && !pGrids->asGrid(0)->is_NoData(x, y)))
				&&	(!pPolygons || is_Contained(xPos, yPos, pPolygons)) )
				{
					pPoint	= pPoints->Add_Shape();
					pPoint->Add_Point(xPos, yPos);

					pPoint->Set_Value(0, ++iPoint);
					pPoint->Set_Value(1, xPos);
					pPoint->Set_Value(2, yPos);

					for(iGrid=0; iGrid<pGrids->Get_Count(); iGrid++)
					{
						pGrid	= pGrids->asGrid(iGrid);

						pPoint->Set_Value(iGrid + 3, pGrid->is_NoData(x, y) ? -99999 : pGrid->asDouble(x, y, bZFactor));
					}
				}
			}
		}

		return( pPoints->Get_Count() > 0 );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CGrid_To_Points::is_Contained(double x, double y, CSG_Shapes *pPolygons)
{
	if( pPolygons && pPolygons->Get_Type() == SHAPE_TYPE_Polygon )
	{
		for(int iPolygon=0; iPolygon<pPolygons->Get_Count(); iPolygon++)
		{
			CSG_Shape_Polygon *pPolygon	= (CSG_Shape_Polygon *)pPolygons->Get_Shape(iPolygon);

			if( pPolygon->is_Containing(x, y) )
			{
				return( true );
			}
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
