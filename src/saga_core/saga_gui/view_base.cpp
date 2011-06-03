/**********************************************************
 * Version $Id: view_base.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

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
//                    VIEW_Base.cpp                      //
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
#include <wx/window.h>
#include <wx/icon.h>

#include <saga_api/saga_api.h>

#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"

#include "saga_frame.h"

#include "helper.h"

#include "view_base.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Base, wxMDIChildFrame);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Base, wxMDIChildFrame)
	EVT_SIZE			(CVIEW_Base::On_Size)
	EVT_ACTIVATE		(CVIEW_Base::On_Activate)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Base::CVIEW_Base(int View_ID, wxString Caption, int Icon_ID, wxMenu *pMenu, wxString Menu_Label)
	: wxMDIChildFrame(g_pSAGA_Frame, -1, Caption, MDI_Get_Def_Position(), MDI_Get_Def_Size(), wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE)
{
	m_View_ID		= View_ID;

	m_Size_Min.x	= 0;
	m_Size_Min.y	= 0;

	SetStatusBarPane(-1);

	SetIcon(IMG_Get_Icon(Icon_ID));

	SetMenuBar(g_pSAGA_Frame->MB_Create(this));
	GetMenuBar()->Insert(2, pMenu, Menu_Label);
	m_pMenu_File	= GetMenuBar()->GetMenu(0);
	m_pMenu_Modules	= GetMenuBar()->GetMenu(1);

	_Activate();

	Show(true);
}

//---------------------------------------------------------
CVIEW_Base::~CVIEW_Base(void)
{
	_Deactivate();

	if( g_pSAGA_Frame )
	{
		g_pSAGA_Frame->MB_Remove(m_pMenu_File, m_pMenu_Modules);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::Set_Size_Min(int x, int y)
{
	m_Size_Min.x	= x;
	m_Size_Min.y	= y;
}

//---------------------------------------------------------
void CVIEW_Base::On_Size(wxSizeEvent &event)
{
	if( event.GetSize().x < m_Size_Min.x || event.GetSize().y < m_Size_Min.y )
	{
		SetSize(wxSize(
			event.GetSize().x < m_Size_Min.x ? m_Size_Min.x : event.GetSize().x,
			event.GetSize().y < m_Size_Min.y ? m_Size_Min.y : event.GetSize().y)
		);
	}
	else
	{
		event.Skip();
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Activate(wxActivateEvent &event)
{
	if( event.GetActive() )
	{
		_Activate();
	}
	else
	{
		_Deactivate();
	}
}

//---------------------------------------------------------
void CVIEW_Base::_Activate(void)
{
	if( g_pSAGA_Frame )
	{
		g_pSAGA_Frame->On_Child_Activates(this, true);
	}
}

//---------------------------------------------------------
void CVIEW_Base::_Deactivate(void)
{
	if( g_pSAGA_Frame )
	{
		g_pSAGA_Frame->On_Child_Activates(this, false);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Quit(wxCommandEvent &WXUNUSED(event))
{
	Close(true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Base::On_Command_UI(wxUpdateUIEvent &event)
{
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
