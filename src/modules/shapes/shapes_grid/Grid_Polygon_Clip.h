/**********************************************************
 * Version $Id: Grid_Polygon_Clip.h 1025 2011-04-28 16:16:37Z oconrad $
 *********************************************************/

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
//                  Grid_Polygon_Clip.h                  //
//                                                       //
//                 Copyright (C) 2006 by                 //
//                     Stefan Liersch                    //
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
//    e-mail:     stefan.liersch@ufz.de                  //
//                stliersch@freenet.de                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Grid_Polygon_Clip_H
#define HEADER_INCLUDED__Grid_Polygon_Clip_H

//---------------------------------------------------------

#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGrid_Polygon_Clip : public CSG_Module_Grid
{
public:
	// constructor
	CGrid_Polygon_Clip(void);

	virtual const SG_Char *	Get_MenuPath	(void)	{	return( _TL("R:Spatial Extent") );	}

protected:
	// execute module
	virtual bool			On_Execute		(void);

private:

	// estimates the Extent of the new grid
	bool					Get_Extent		(int &xMin, int &xCount,
											 int &yMin, int &yCount, CSG_Grid *pMask);

	// This function has been copied from Module: 'Grid_Statistics_AddTo_Polygon'
	// Function: Get_ShapeIDs
	// copyright by Olaf Conrad
	bool					Get_Mask		(CSG_Shapes *pShapes, CSG_Grid *pMask);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Grid_Polygon_Clip_H
