/**********************************************************
 * Version $Id: Exercise_06.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       Lectures                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    Exercise_06.h                      //
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
//                                                       //												
//                                                       //												
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Exercise_06_H
#define HEADER_INCLUDED__Exercise_06_H

//---------------------------------------------------------
#include "MLB_Interface.h"

//---------------------------------------------------------
class CExercise_06 : public CSG_Module_Grid
{
public: ////// public members and functions: //////////////

	CExercise_06(void);							// constructor
	virtual ~CExercise_06(void);				// destructor


protected: /// protected members and functions: ///////////

	virtual bool		On_Execute(void);		// always override this function


private: ///// private members and functions: /////////////

	CSG_Grid				*m_pInput, *m_pOutput;


	bool				Method_01(int Radius);
	bool				Method_02(int Radius);
	bool				Method_03(int Radius);

};


//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Exercise_06_H
