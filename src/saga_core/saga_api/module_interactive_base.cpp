/**********************************************************
 * Version $Id: module_interactive_base.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              module_interactive_base.cpp              //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, version 2.1 of the License.      //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, write to the Free Software Foundation, Inc.,     //
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
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
#include "module.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Interactive_Base::CSG_Module_Interactive_Base(void)
{
	m_pModule		= NULL;

	m_Keys			= 0;
	m_Drag_Mode		= MODULE_INTERACTIVE_DRAG_BOX;

	m_Point			.Assign(0.0, 0.0);
	m_Point_Last	.Assign(0.0, 0.0);
}

//---------------------------------------------------------
CSG_Module_Interactive_Base::~CSG_Module_Interactive_Base(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Module_Interactive_Base::Execute_Position(CSG_Point ptWorld, TSG_Module_Interactive_Mode Mode, int Keys)
{
	bool	bResult	= false;

	if( m_pModule && !m_pModule->m_bExecutes )
	{
		m_pModule->m_bExecutes		= true;
		m_pModule->m_bError_Ignore	= false;

		m_Point_Last				= m_Point;
		m_Point						= ptWorld;

		m_Keys						= Keys;

		bResult						= On_Execute_Position(m_Point, Mode);

		m_Keys						= 0;

		m_pModule->_Synchronize_DataObjects();

		m_pModule->m_bExecutes		= false;

		SG_UI_Process_Set_Okay();
	}

	return( bResult );
}

bool CSG_Module_Interactive_Base::On_Execute_Position(CSG_Point ptWorld, TSG_Module_Interactive_Mode Mode)
{
	return( false );
}

//---------------------------------------------------------
bool CSG_Module_Interactive_Base::Execute_Keyboard(int Character, int Keys)
{
	bool	bResult	= false;

	if( m_pModule && !m_pModule->m_bExecutes )
	{
		m_pModule->m_bExecutes		= true;
		m_pModule->m_bError_Ignore	= false;

		m_Keys						= Keys;

		bResult						= On_Execute_Keyboard(Character);

		m_Keys						= 0;

		m_pModule->_Synchronize_DataObjects();

		m_pModule->m_bExecutes		= false;

		SG_UI_Process_Set_Okay();
	}

	return( bResult );
}

bool CSG_Module_Interactive_Base::On_Execute_Keyboard(int Character)
{
	return( false );
}

//---------------------------------------------------------
bool CSG_Module_Interactive_Base::Execute_Finish(void)
{
	bool	bResult	= false;

	if( m_pModule && !m_pModule->m_bExecutes )
	{
		m_pModule->m_bExecutes		= true;
		m_pModule->m_bError_Ignore	= false;

		bResult						= On_Execute_Finish();

		m_pModule->_Synchronize_DataObjects();

		m_pModule->m_bExecutes		= false;

		SG_UI_Process_Set_Okay();
	}

	return( bResult );
}

bool CSG_Module_Interactive_Base::On_Execute_Finish(void)
{
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Module_Interactive_Base::Set_Drag_Mode(int Drag_Mode)
{
	m_Drag_Mode	= Drag_Mode;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
