/**********************************************************
 * Version $Id: MLB_Interface.cpp 1246 2011-11-25 13:42:38Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                        Grid_IO                        //
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

CSG_String Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Import/Export - Grids") );

	case MLB_INFO_Author:
		return( _TL("SAGA User Group Associaton (c) 2002") );

	case MLB_INFO_Description:
		return( _TL("Tools for the import and export of gridded data.") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("File|Grid") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "esri_arcinfo.h"
#include "surfer.h"
#include "raw.h"
#include "xyz.h"
#include "usgs_srtm.h"
#include "mola.h"
#include "srtm30.h"
#include "bmp_export.h"
#include "erdas_lan.h"
#include "grid_table.h"
#include "wrf.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case 0:		return( new CESRI_ArcInfo_Export );
	case 1:		return( new CESRI_ArcInfo_Import );
	case 2:		return( new CSurfer_Export );
	case 3:		return( new CSurfer_Import );
	case 4:		return( new CRaw_Import );
	case 5:		return( new CXYZ_Export );
	case 6:		return( new CXYZ_Import );
	case 7:		return( new CUSGS_SRTM_Import );
	case 8:		return( new CMOLA_Import );
	case 9:		return( new CSRTM30_Import );
	case 10:	return( new CBMP_Export );
	case 11:	return( new CErdas_LAN_Import );
	case 12:	return( new CGrid_Table_Import );
	case 13:	return( new CWRF_Import );
	case 14:	return( new CWRF_Export );
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
