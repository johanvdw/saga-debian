
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
		return( _TL("Shapes - Grid") );

	case MLB_INFO_Author:
		return( _TL("Olaf Conrad (c) 2002") );

	case MLB_INFO_Description:
		return( _TL("Tools related to gridded and vector data (conversions, combinations, etc.).") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Shapes|Grid") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "Grid_Values_AddTo_Points.h"
#include "Grid_Values_AddTo_Shapes.h"
#include "Grid_Statistics_AddTo_Polygon.h"
#include "Grid_To_Points.h"
#include "Grid_To_Points_Random.h"
#include "Grid_To_Contour.h"
#include "Grid_Classes_To_Shapes.h"
#include "Grid_Polygon_Clip.h"
#include "Grid_To_Gradient.h"
#include "grid_local_extremes_to_points.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	// Don't forget to continuously enumerate the case switches
	// when adding new modules! Also bear in mind that the
	// enumeration always has to start with [case 0:] and
	// that [default:] must return NULL!...

	CSG_Module	*pModule;

	switch( i )
	{
	case 0:
		pModule	= new CGrid_Values_AddTo_Points;
		break;

	case 1:
		pModule	= new CGrid_Values_AddTo_Shapes;
		break;

	case 2:
		pModule	= new CGrid_Statistics_AddTo_Polygon;
		break;

	case 3:
		pModule	= new CGrid_To_Points;
		break;

	case 4:
		pModule	= new CGrid_To_Points_Random;
		break;

	case 5:
		pModule	= new CGrid_To_Contour;
		break;

	case 6:
		pModule	= new CGrid_Classes_To_Shapes;
		break;

	case 7:
		pModule	= new CGrid_Polygon_Clip;
		break;

	case 8:
		pModule	= new CGrid_To_Gradient;
		break;

	case 9:
		pModule	= new CGrid_Local_Extremes_to_Points;
		break;

	default:
		pModule	= NULL;
		break;
	}

	return( pModule );
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
