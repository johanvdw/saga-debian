/**********************************************************
 * Version $Id: MLB_Interface.cpp 1735 2013-06-19 10:12:32Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//               geostatistics_regression                //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   MLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2010 by                 //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//			The Module Link Library Interface			 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// 1. Include the appropriate SAGA-API header...

#include "MLB_Interface.h"


//---------------------------------------------------------
// 2. Place general module library informations here...

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Spatial and Geostatistics - Regression") );

	case MLB_INFO_Author:
		return( _TL("O.Conrad (c) 2010" ));

	case MLB_INFO_Description:
		return( _TL("Tools for regression analyses.") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Spatial and Geostatistics|Regression") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "point_grid_regression.h"
#include "point_multi_grid_regression.h"
#include "point_trend_surface.h"

#include "gw_regression.h"
#include "gw_regression_grid.h"
#include "gw_multi_regression.h"
#include "gw_multi_regression_grid.h"
#include "gw_multi_regression_points.h"
#include "gwr_grid_downscaling.h"

#include "grid_multi_grid_regression.h"
#include "grids_trend_polynom.h"

#include "table_trend.h"
#include "table_regression_multiple.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case  0:	return( new CPoint_Grid_Regression );
	case  1:	return( new CPoint_Multi_Grid_Regression );
	case  2:	return( new CPoint_Trend_Surface );

	case  3:	return( new CGW_Regression );
	case  4:	return( new CGW_Regression_Grid );
	case  5:	return( new CGW_Multi_Regression );
	case  6:	return( new CGW_Multi_Regression_Grid );
	case  7:	return( new CGW_Multi_Regression_Points );
	case 14:	return( new CGWR_Grid_Downscaling );

	case  8:	return( new CGrid_Multi_Grid_Regression );
	case  9:	return( new CGrids_Trend );

	case 10:	return( new CTable_Trend );
	case 11:	return( new CTable_Trend_Shapes );

	case 12:	return( new CTable_Regression_Multiple );
	case 13:	return( new CTable_Regression_Multiple_Shapes );

	//-----------------------------------------------------
	case 19:	return( NULL );
	default:	return( MLB_INTERFACE_SKIP_MODULE );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
//{{AFX_SAGA

	MLB_INTERFACE

//}}AFX_SAGA
