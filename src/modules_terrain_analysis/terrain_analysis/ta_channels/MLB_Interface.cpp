/**********************************************************
 * Version $Id: MLB_Interface.cpp 1527 2012-11-09 15:11:22Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      ta_channels                      //
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
#include "MLB_Interface.h"


//---------------------------------------------------------
CSG_String Get_Info(int i)
{
	switch( i )
	{
	case MLB_INFO_Name:	default:
		return( _TL("Terrain Analysis - Channels") );

	case MLB_INFO_Author:
		return( SG_T("O. Conrad, V. Olaya (c) 2002-4") );

	case MLB_INFO_Description:
		return( _TL("Tools for (grid based) digital terrain analysis.") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Terrain Analysis|Channels") );
	}
}


//---------------------------------------------------------
#include "ChannelNetwork.h"
#include "Watersheds.h"
#include "Watersheds_ext.h"
#include "ChannelNetwork_Altitude.h"
#include "ChannelNetwork_Distance.h"
#include "D8_Flow_Analysis.h"
#include "Strahler.h"


//---------------------------------------------------------
CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case  0:	return( new CChannelNetwork );
	case  1:	return( new CWatersheds );
	case  2:	return( new CWatersheds_ext );
	case  3:	return( new CChannelNetwork_Altitude );
	case  4:	return( new CChannelNetwork_Distance );
	case  5:	return( new CD8_Flow_Analysis );
	case  6:	return( new CStrahler );
	case  7:	return( new CValley_Depth );

	default:	return( NULL );
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
