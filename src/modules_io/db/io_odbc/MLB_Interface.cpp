/**********************************************************
 * Version $Id: MLB_Interface.cpp 1514 2012-11-06 09:47:38Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                        io_odbc                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   MLB_Interface.cpp                   //
//                                                       //
//                 Copyright (C) 2004 by                 //
//                     Olaf Conrad                       //
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

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Import/Export - ODBC/OTL") );

	case MLB_INFO_Author:
		return( SG_T("O.Conrad (c) 2010") );

	case MLB_INFO_Description:
		return( _TW(
			"Database access via Open Data Base Connection (ODBC) interface. "
			"Based on the OTL (Oracle, Odbc and DB2-CLI Template Library), Version 4.0: "
			"<a target=\"_blank\" href=\"http://otl.sourceforge.net/\">http://otl.sourceforge.net/</a>"
		));

	case MLB_INFO_Version:
		return( _TL("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Database") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "get_connection.h"
#include "table.h"
#include "shapes.h"
#include "pgis_shapes.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case  0:	return( new CGet_Connection );
	case  1:	return( new CDel_Connection );
	case  2:	return( new CTransaction );
	case  3:	return( new CExecute_SQL );
	case  4:	return( new CTable_Info );
	case  5:	return( new CTable_Load );
	case  6:	return( new CTable_Save );
	case  7:	return( new CTable_Drop );
	case  8:	return( new CTable_Query );
	case  9:	return( new CPoints_Load );
	case 10:	return( new CPoints_Save );
	case 11:	return( new CPGIS_Shapes_Load );
	case 12:	return( new CPGIS_Shapes_Save );
	case 13:	return( new CShapes_Load );
	case 14:	return( new CShapes_Save );
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
