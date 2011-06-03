/**********************************************************
 * Version $Id: MLB_Interface.cpp 955 2011-03-22 17:05:16Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                    shapes_polygons                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   MLB_Interface.cpp                   //
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
//			The Module Link Library Interface			 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// 1. Include the appropriate SAGA-API header...

#include "MLB_Interface.h"


//---------------------------------------------------------
// 2. Place general module library informations here...

const SG_Char * Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Shapes - Polygons") );

	case MLB_INFO_Author:
		return( SG_T("O. Conrad, V. Olaya (c) 2002-5") );

	case MLB_INFO_Description:
		return( _TL("Tools for polygons.") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Shapes|Polygons") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "Polygon_Intersection.h"
#include "Polygon_Centroids.h"
#include "Polygon_Geometrics.h"
#include "Polygons_From_Lines.h"
#include "Polygon_StatisticsFromPoints.h"
#include "Polygon_Union.h"
#include "polygon_to_points.h"
#include "shape_index.h"
#include "polygon_line_intersection.h"
#include "polygon_to_edges_nodes.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case 0:		return( new CPolygon_Intersection );
	case 1:		return( new CPolygon_Centroids );
	case 2:		return( new CPolygon_Geometrics );
	case 3:		return( new CPolygons_From_Lines );
	case 4:		return( new CPolygonStatisticsFromPoints );
	case 5:		return( new CPolygon_Dissolve );
	case 6:		return( new CPolygon_To_Points );
	case 7:		return( new CShape_Index );
	case 8:		return( new CPolygon_Line_Intersection );
	case 9:		return( new CPolygon_to_Edges_Nodes );
	}

	return( NULL );
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
