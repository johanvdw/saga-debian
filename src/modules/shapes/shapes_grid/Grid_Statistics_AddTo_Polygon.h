/**********************************************************
 * Version $Id: Grid_Statistics_AddTo_Polygon.h 1491 2012-10-17 13:55:27Z oconrad $
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
//             Grid_Statistics_AddTo_Polygon.h           //
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
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Grid_Statistics_AddTo_Polygon_H
#define HEADER_INCLUDED__Grid_Statistics_AddTo_Polygon_H


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGrid_Statistics_AddTo_Polygon : public CSG_Module_Grid  
{
public:
	CGrid_Statistics_AddTo_Polygon(void);

	virtual CSG_String		Get_MenuPath		(void)	{	return( _TL("R:Grid Values") );	}


protected:

	virtual bool			On_Execute			(void);


private:

	CSG_Grid				m_ShapeIDs;


	bool					Get_Statistics		(CSG_Grid *pGrid, CSG_Shapes *pShapes, CSG_Simple_Statistics *Statistics, CSG_Table *Values);
	bool					Get_Statistics_Alt	(CSG_Grid *pGrid, CSG_Shapes *pShapes, CSG_Simple_Statistics *Statistics, CSG_Table *Values);

	bool					Get_ShapeIDs		(CSG_Shapes *pShapes);

};


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Grid_Statistics_AddTo_Polygon_H
