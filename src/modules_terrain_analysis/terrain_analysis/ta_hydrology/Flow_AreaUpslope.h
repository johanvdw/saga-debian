/**********************************************************
 * Version $Id: Flow_AreaUpslope.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     ta_hydrology                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  Flow_AreaUpslope.h                   //
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
#ifndef HEADER_INCLUDED__Flow_AreaUpslope_H
#define HEADER_INCLUDED__Flow_AreaUpslope_H


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CFlow_AreaUpslope
{
public:
	CFlow_AreaUpslope(void);
	virtual ~CFlow_AreaUpslope(void);

	CSG_String				Get_Description		(void);
	CSG_String				Get_Methods			(void);

	bool					Initialise			(int Method, CSG_Grid *pDTM, CSG_Grid *pRoute, CSG_Grid *pFlow, double MFD_Converge);
	bool					Finalise			(void);

	bool					Add_Target			(int x, int y);
	bool					Clr_Target			(void);

	bool					Get_Area			(int x, int y);
	bool					Get_Area			(void);


private:

	int						m_Method;

	double					m_MFD_Converge;

	CSG_Grid					*m_pDTM, *m_pRoute, *m_pFlow;


	void					Set_Value			(int x, int y);
	void					Set_D8				(int x, int y);
	void					Set_DInf			(int x, int y);
	void					Set_MFD				(int x, int y);

};


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CFlow_AreaUpslope_Interactive : public CSG_Module_Grid_Interactive
{
public:
	CFlow_AreaUpslope_Interactive(void);
	virtual ~CFlow_AreaUpslope_Interactive(void);

	virtual const SG_Char *	Get_MenuPath		(void)	{	return( _TL("R:Catchment Area" ));	}


protected:

	virtual bool			On_Execute			(void);
	virtual bool			On_Execute_Finish	(void);
	virtual bool			On_Execute_Position	(CSG_Point ptWorld, TSG_Module_Interactive_Mode Mode);


private:

	CFlow_AreaUpslope		m_Calculator;

};

//---------------------------------------------------------
class CFlow_AreaUpslope_Area : public CSG_Module_Grid
{
public:
	CFlow_AreaUpslope_Area(void);
	virtual ~CFlow_AreaUpslope_Area(void);

	virtual const SG_Char *	Get_MenuPath		(void)	{	return( _TL("R:Catchment Area" ));	}


protected:

	virtual bool			On_Execute			(void);


private:

	CFlow_AreaUpslope		m_Calculator;

};


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Flow_AreaUpslope_H
