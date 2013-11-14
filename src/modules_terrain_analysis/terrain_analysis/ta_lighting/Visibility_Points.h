/**********************************************************
 * Version $Id: Visibility_Points.h 1613 2013-02-22 11:20:39Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      ta_lighting                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Visibility_Points.h                  //
//                                                       //
//                 Copyright (C) 2013 by                 //
//                    Volker Wichmann                    //
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
//    e-mail:     wichmann@laserdata                     //
//                                                       //
//    contact:    Volker Wichmann                        //
//                LASERDATA GmbH                         //
//                Management and analysis of             //
//                laserscanning data                     //
//                Innsbruck, Austria                     //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Visibility_Points_H
#define HEADER_INCLUDED__Visibility_Points_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"
#include "Visibility_BASE.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CVisibility_Points : public CSG_Module_Grid, CVisibility_BASE
{
public:
	CVisibility_Points(void);
	virtual ~CVisibility_Points(void);


protected:

	virtual bool			On_Execute			(void);


private:


};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Visibility_Points_H
