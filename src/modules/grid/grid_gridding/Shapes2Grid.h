
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     Grid_Gridding                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                     Shapes2Grid.h                     //
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
#ifndef HEADER_INCLUDED__Shapes2Grid_H
#define HEADER_INCLUDED__Shapes2Grid_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CShapes2Grid : public CSG_Module  
{
public:
	CShapes2Grid(void);
	virtual ~CShapes2Grid(void);


protected:

	virtual bool			On_Execute			(void);


private:

	int						m_Lock_ID, m_Method_Lines;

	double					m_Value;

	CSG_Grid				*m_pGrid, *m_pLock;

	CSG_Shapes				*m_pShapes;


	TSG_Data_Type			Get_Grid_Type		(int iType);
	CSG_Grid *				Get_Target_Grid		(CSG_Parameters *pParameters, CSG_Shapes *pShapes);

	void					Set_Value			(int x, int y);

	void					Set_Points			(CSG_Shape *pShape);

	void					Set_Line			(CSG_Shape *pShape);
	void					Set_Line_A			(TSG_Point a, TSG_Point b);
	void					Set_Line_B			(TSG_Point a, TSG_Point b);

	void					Set_Polygon			(CSG_Shape *pShape);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Shapes2Grid_H
