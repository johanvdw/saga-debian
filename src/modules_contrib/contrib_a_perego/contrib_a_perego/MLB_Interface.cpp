/**********************************************************
 * Version $Id: MLB_Interface.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                   contrib_a_perego                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   MLB_Interface.cpp                   //
//                                                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                                                       //
//    by Alessandro Perego (Italy)                       //
//                                                       //
//    e-mail:     alper78@alice.it                       //
//                                                       //
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
		return( _TL("Contributions - A. Perego") );

	case MLB_INFO_Author:
		return( _TL("Alessandro Perego") );

	case MLB_INFO_Description:
		return( _TL("Contributions from Alessandro Perego.") );

	case MLB_INFO_Version:
		return( SG_T("1.0") );

	case MLB_INFO_Menu_Path:
		return( _TL("Contributions|A. Perego") );
	}
}


//---------------------------------------------------------
// 3. Include the headers of your modules here...

#include "A1WiTh.h"
#include "A2WiTh.h"
#include "A3WiTh.h"
#include "AvWiMa1.h"
#include "AvWiMa2.h"
#include "destriping1.h"
#include "destriping2.h"
#include "directional1.h"


//---------------------------------------------------------
// 4. Allow your modules to be created here...

CSG_Module *		Create_Module(int i)
{
	switch( i )
	{
	case 0:		return( new CA1WiTh );
	case 1:		return( new CA2WiTh );
	case 2:		return( new CA3WiTh );
	case 3:		return( new CAvWiMa1 );
	case 4:		return( new CAvWiMa2 );
	case 5:		return( new Cdestriping1 );
	case 6:		return( new Cdestriping2 );
	case 7:		return( new Cdirectional1 );
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
