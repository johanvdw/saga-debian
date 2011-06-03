/**********************************************************
 * Version $Id: dlg_colors.cpp 911 2011-02-14 16:38:15Z reklov_w $
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
//                    DLG_Colors.cpp                     //
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
#include <saga_api/saga_api.h>

#include "res_controls.h"
#include "res_dialogs.h"

#include "helper.h"

#include "dlg_colors.h"
#include "dlg_colors_control.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CDLG_Colors, CDLG_Base)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CDLG_Colors, CDLG_Base)
	EVT_BUTTON			(ID_BTN_LOAD			, CDLG_Colors::On_Load)
	EVT_BUTTON			(ID_BTN_SAVE			, CDLG_Colors::On_Save)
	EVT_BUTTON			(ID_BTN_COLORS_COUNT	, CDLG_Colors::On_Count)
	EVT_BUTTON			(ID_BTN_COLORS_MIRROR	, CDLG_Colors::On_Miror)
	EVT_BUTTON			(ID_BTN_COLORS_INVERT	, CDLG_Colors::On_Invert)
	EVT_BUTTON			(ID_BTN_COLORS_RANDOM	, CDLG_Colors::On_Random)
	EVT_BUTTON			(ID_BTN_COLORS_PRESET	, CDLG_Colors::On_Preset)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CDLG_Colors::CDLG_Colors(CSG_Colors *pColors)
	: CDLG_Base(-1, wxT("[CAP] Colors"))
{
	m_pOriginal	= pColors;
	m_pColors	= new CSG_Colors();
	m_pColors->Assign(pColors);

	m_pControl	= new CDLG_Colors_Control(this, m_pColors);

	Add_Button(ID_BTN_LOAD);
	Add_Button(ID_BTN_SAVE);
	Add_Button(-1);
	Add_Button(ID_BTN_COLORS_COUNT);
	Add_Button(ID_BTN_COLORS_MIRROR);
	Add_Button(ID_BTN_COLORS_INVERT);
	Add_Button(ID_BTN_COLORS_RANDOM);
	Add_Button(ID_BTN_COLORS_PRESET);

	Set_Positions();
}

//---------------------------------------------------------
CDLG_Colors::~CDLG_Colors(void)
{
	delete(m_pColors);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CDLG_Colors::Set_Position(wxRect r)
{
	m_pControl->SetSize(r);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CDLG_Colors::Save_Changes(void)
{
	m_pOriginal->Assign(m_pColors);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CDLG_Colors::On_Load(wxCommandEvent &event)
{
	wxString	File_Path;

	if( DLG_Open(File_Path, ID_DLG_COLORS_OPEN) )
	{
		if( m_pColors->Load(File_Path.c_str()) == false )
		{
			DLG_Message_Show(LNG("Colors file could not be imported."), LNG("Load Colors"));
		}
		else
		{
			m_pControl->Refresh(false);
		}
	}
}

//---------------------------------------------------------
void CDLG_Colors::On_Save(wxCommandEvent &event)
{
	wxString	File_Path;

	if( DLG_Save(File_Path, ID_DLG_COLORS_SAVE) )
	{
		if( m_pColors->Save(File_Path.c_str(), true) == false )
		{
			DLG_Message_Show(LNG("Colors file could not be exported."), LNG("Save Colors"));
		}
	}
}

//---------------------------------------------------------
void CDLG_Colors::On_Count(wxCommandEvent &event)
{
	int		Count;

	if( DLG_Get_Number(Count = m_pColors->Get_Count()) )
	{
		m_pColors->Set_Count(Count);

		m_pControl->Refresh(false);
	}
}

//---------------------------------------------------------
void CDLG_Colors::On_Miror(wxCommandEvent &event)
{
	m_pColors->Revert();

	m_pControl->Refresh(false);
}

//---------------------------------------------------------
void CDLG_Colors::On_Invert(wxCommandEvent &event)
{
	m_pColors->Invert();

	m_pControl->Refresh(false);
}

//---------------------------------------------------------
void CDLG_Colors::On_Random(wxCommandEvent &event)
{
	m_pColors->Random();

	m_pControl->Refresh(false);
}

//---------------------------------------------------------
void CDLG_Colors::On_Preset(wxCommandEvent &event)
{
	int		Palette;

	if( DLG_Colors(Palette) )
	{
		m_pColors->Set_Palette(Palette, false, m_pColors->Get_Count());

		m_pControl->Refresh(false);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
