
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
//                 WKSP_Grid_Manager.h                   //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Grid_Manager_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Grid_Manager_H


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
class CWKSP_Grid_Manager : public CWKSP_Base_Manager
{
public:
	CWKSP_Grid_Manager(void);
	virtual ~CWKSP_Grid_Manager(void);

	virtual TWKSP_Item				Get_Type			(void)		{	return( WKSP_ITEM_Grid_Manager );	}

	virtual wxString				Get_Name			(void);
	virtual wxString				Get_Description		(void);

	virtual wxMenu *				Get_Menu			(void);

	virtual bool					On_Command			(int Cmd_ID);

	class CWKSP_Grid_System *		Get_System			(int i)		{	return( (class CWKSP_Grid_System *)Get_Item(i) );	}
	class CWKSP_Grid_System *		Get_System			(class CSG_Grid_System *pSystem);
	bool							Exists				(class CSG_Grid_System *pSystem);

	class CWKSP_Grid *				Get_Grid			(class CSG_Grid *pGrid);
	bool							Exists				(class CSG_Grid *pGrid);
	class CWKSP_Grid *				Add					(class CSG_Grid *pGrid);
	class CSG_Grid *				Get_byFileName		(const wxChar *File_Name);

	bool							Update				(class CSG_Grid *pGrid, class CSG_Parameters *pParameters);
	bool							Update_Views		(class CSG_Grid *pGrid);
	bool							Show				(class CSG_Grid *pGrid, int Map_Mode);
	bool							asImage				(class CSG_Grid *pGrid, class CSG_Grid *pImage);

	bool							Get_Colors			(class CSG_Grid *pGrid, class CSG_Colors *pColors);
	bool							Set_Colors			(class CSG_Grid *pGrid, class CSG_Colors *pColors);


private:

	class CWKSP_Grid_System *		_Get_System			(class CSG_Grid_System *pSystem);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Grid_Manager_H
