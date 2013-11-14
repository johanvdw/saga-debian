/**********************************************************
 * Version $Id: wksp_shapes_manager.h 1646 2013-04-10 16:29:00Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                WKSP_Shapes_Manager.h                  //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Manager_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Manager_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_base_manager.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Shapes_Manager : public CWKSP_Base_Manager
{
public:
	CWKSP_Shapes_Manager(void);

	virtual TWKSP_Item			Get_Type			(void)	{	return( WKSP_ITEM_Shapes_Manager );	}

	virtual wxString			Get_Name			(void);
	virtual wxString			Get_Description		(void);

	virtual wxMenu *			Get_Menu			(void);

	class CWKSP_Shapes_Type *	Get_Shapes_Type		(int i)	{	return( (class CWKSP_Shapes_Type *)Get_Item(i) );	}
	class CWKSP_Shapes_Type *	Get_Shapes_Type		(TSG_Shape_Type Type);

	class CWKSP_Shapes *		Get_Data			(class CSG_Shapes *pShapes);
	class CWKSP_Shapes *		Add_Data			(class CSG_Shapes *pShapes);


private:

	class CWKSP_Shapes_Type *	_Get_Shapes_Type	(TSG_Shape_Type Type);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Manager_H
