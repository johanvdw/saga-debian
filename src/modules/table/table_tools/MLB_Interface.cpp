/**********************************************************
 * Version $Id: MLB_Interface.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Table_Tools                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   MLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2003 by                 //
//              SAGA User Group Associaton               //
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
//    e-mail:     aringel@gwdg.de                        //
//                                                       //
//    contact:    SAGA User Group Associaton             //
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

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Table - Tools") );

	case MLB_INFO_Author:
		return( _TL("SAGA User Group Associaton (c) 2002-2013") );

	case MLB_INFO_Description:
		return( _TL("") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Table|Tools") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "Table_Create_Empty.h"
#include "Table_Rotate.h"
#include "Table_Enumerate.h"
#include "Join_Tables.h"
#include "table_change_date_format.h"
#include "table_change_field_type.h"
#include "table_change_color_format.h"
#include "table_text_replacer.h"
#include "table_field_deletion.h"
#include "table_selection.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case  0:	return( new CTable_Create_Empty );
	case  1:	return( new CTable_Rotate );
	case  2:	return( new CTable_Enumerate );
	case  3:	return( new CJoin_Tables );
	case  4:	return( new CJoin_Tables_Shapes );
	case  5:	return( new CTable_Change_Date_Format );
	case  6:	return( new CTable_Change_Time_Format );
	case  7:	return( new CTable_Change_Field_Type );
	case  8:	return( new CTable_Append_Rows );
	case  9:	return( new CTable_Change_Color_Format );
	case 10:	return( new CTable_Text_Replacer );
	case 11:	return( new CTable_Field_Deletion );

	case 15:	return( new CSelection_Copy );
	case 16:	return( new CSelection_Delete );
	case 17:	return( new CSelection_Invert );
	case 18:	return( new CSelect_Numeric );
	case 19:	return( new CSelect_String );

	case 20:	return( NULL );
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
