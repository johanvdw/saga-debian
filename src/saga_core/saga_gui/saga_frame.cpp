/**********************************************************
 * Version $Id: saga_frame.cpp 1200 2011-10-25 15:23:04Z oconrad $
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
//                    SAGA_Frame.cpp                     //
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
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/filename.h>
#include <wx/statusbr.h>
#include <wx/icon.h>
#include <wx/gauge.h>
#include <wx/choicdlg.h>
#include <wx/toolbar.h>
#include <wx/tipdlg.h>
#include <wx/aui/aui.h>

#include <saga_api/saga_api.h>

#include "callback.h"

#include "res_commands.h"
#include "res_controls.h"
#include "res_dialogs.h"
#include "res_images.h"

#include "helper.h"

#include "saga.h"
#include "saga_frame.h"
#include "saga_frame_droptarget.h"

#include "info.h"
#include "data_source.h"
#include "active.h"
#include "wksp.h"

#include "wksp_module_manager.h"
#include "wksp_module.h"
#include "wksp_module_menu.h"

#include "wksp_data_manager.h"
#include "wksp_data_menu_files.h"

#include "wksp_map_manager.h"

#include "view_map.h"
#include "view_map_3d.h"
#include "view_table.h"
#include "view_table_diagram.h"
#include "view_histogram.h"
#include "view_scatterplot.h"
#include "view_layout.h"

#include "dlg_about.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifdef _DEBUG
	#define SAGA_CAPTION	wxT("SAGA [Debug]")
#else
	#define SAGA_CAPTION	wxT("SAGA")
#endif


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CSAGA_Frame_StatusBar : public wxStatusBar
{
public:
	CSAGA_Frame_StatusBar(wxWindow *parent, wxWindowID id, long style = wxST_SIZEGRIP, const wxString& name = wxT("statusBar"))
		: wxStatusBar(parent, id, style, name)
	{
		m_pProgressBar	= new wxGauge(this, ID_WND_PROGRESSBAR, 100);
	}

	void		On_Size		(wxSizeEvent     &event)
	{
		wxRect	r;

		if( m_pProgressBar && GetFieldRect(STATUSBAR_PROGRESS, r) )
		{
			m_pProgressBar->SetSize(r);
		}

		event.Skip();
	}

	wxGauge		*m_pProgressBar;

	DECLARE_EVENT_TABLE()
};

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CSAGA_Frame_StatusBar, wxStatusBar)
	EVT_SIZE			(CSAGA_Frame_StatusBar::On_Size)
END_EVENT_TABLE()

//---------------------------------------------------------
wxStatusBar * CSAGA_Frame::OnCreateStatusBar(int number, long style, wxWindowID id, const wxString& name)
{
	CSAGA_Frame_StatusBar	*sb	= new CSAGA_Frame_StatusBar(this, id, style, name);

	sb->SetFieldsCount(number);

	return( sb );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA_Frame	*g_pSAGA_Frame	= NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CSAGA_Frame, wxMDIParentFrame)
	EVT_CLOSE			(CSAGA_Frame::On_Close)
	EVT_SIZE			(CSAGA_Frame::On_Size)

	EVT_MENU			(ID_CMD_FRAME_QUIT				, CSAGA_Frame::On_Quit)
	EVT_MENU			(ID_CMD_FRAME_HELP				, CSAGA_Frame::On_Help)
	EVT_MENU			(ID_CMD_FRAME_ABOUT				, CSAGA_Frame::On_About)
	EVT_MENU			(ID_CMD_FRAME_TIPS				, CSAGA_Frame::On_Tips)

	EVT_MENU			(ID_CMD_FRAME_CASCADE			, CSAGA_Frame::On_Frame_Cascade)
	EVT_UPDATE_UI		(ID_CMD_FRAME_CASCADE			, CSAGA_Frame::On_Frame_Cascade_UI)
	EVT_MENU			(ID_CMD_FRAME_TILE_HORZ			, CSAGA_Frame::On_Frame_hTile)
	EVT_UPDATE_UI		(ID_CMD_FRAME_TILE_HORZ			, CSAGA_Frame::On_Frame_hTile_UI)
	EVT_MENU			(ID_CMD_FRAME_TILE_VERT			, CSAGA_Frame::On_Frame_vTile)
	EVT_UPDATE_UI		(ID_CMD_FRAME_TILE_VERT			, CSAGA_Frame::On_Frame_vTile_UI)
	EVT_MENU			(ID_CMD_FRAME_ARRANGEICONS		, CSAGA_Frame::On_Frame_ArrangeIcons)
	EVT_UPDATE_UI		(ID_CMD_FRAME_ARRANGEICONS		, CSAGA_Frame::On_Frame_ArrangeIcons_UI)
	EVT_MENU			(ID_CMD_FRAME_NEXT				, CSAGA_Frame::On_Frame_Next)
	EVT_UPDATE_UI		(ID_CMD_FRAME_NEXT				, CSAGA_Frame::On_Frame_Next_UI)
	EVT_MENU			(ID_CMD_FRAME_PREVIOUS			, CSAGA_Frame::On_Frame_Previous)
	EVT_UPDATE_UI		(ID_CMD_FRAME_PREVIOUS			, CSAGA_Frame::On_Frame_Previous_UI)
	EVT_MENU			(ID_CMD_FRAME_CLOSE				, CSAGA_Frame::On_Frame_Close)
	EVT_UPDATE_UI		(ID_CMD_FRAME_CLOSE				, CSAGA_Frame::On_Frame_Close_UI)
	EVT_MENU			(ID_CMD_FRAME_CLOSE_ALL			, CSAGA_Frame::On_Frame_Close_All)
	EVT_UPDATE_UI		(ID_CMD_FRAME_CLOSE_ALL			, CSAGA_Frame::On_Frame_Close_All_UI)

	EVT_MENU			(ID_CMD_FRAME_WKSP_SHOW			, CSAGA_Frame::On_WKSP_Show)
	EVT_UPDATE_UI		(ID_CMD_FRAME_WKSP_SHOW			, CSAGA_Frame::On_WKSP_Show_UI)
	EVT_MENU			(ID_CMD_FRAME_ACTIVE_SHOW		, CSAGA_Frame::On_Active_Show)
	EVT_UPDATE_UI		(ID_CMD_FRAME_ACTIVE_SHOW		, CSAGA_Frame::On_Active_Show_UI)
	EVT_MENU			(ID_CMD_FRAME_DATA_SOURCE_SHOW	, CSAGA_Frame::On_Data_Source_Show)
	EVT_UPDATE_UI		(ID_CMD_FRAME_DATA_SOURCE_SHOW	, CSAGA_Frame::On_Data_Source_Show_UI)
	EVT_MENU			(ID_CMD_FRAME_INFO_SHOW			, CSAGA_Frame::On_INFO_Show)
	EVT_UPDATE_UI		(ID_CMD_FRAME_INFO_SHOW			, CSAGA_Frame::On_INFO_Show_UI)

	EVT_MENU_RANGE		(ID_CMD_WKSP_FIRST				, ID_CMD_WKSP_LAST		, CSAGA_Frame::On_Command_Workspace)
	EVT_UPDATE_UI_RANGE	(ID_CMD_WKSP_FIRST				, ID_CMD_WKSP_LAST		, CSAGA_Frame::On_Command_Workspace_UI)
	EVT_MENU_RANGE		(ID_CMD_MODULE_FIRST			, ID_CMD_MODULE_LAST	, CSAGA_Frame::On_Command_Module)
	EVT_UPDATE_UI_RANGE	(ID_CMD_MODULE_FIRST			, ID_CMD_MODULE_LAST	, CSAGA_Frame::On_Command_Module_UI)

	EVT_MENU_RANGE		(ID_CMD_CHILD_FIRST				, ID_CMD_CHILD_LAST		, CSAGA_Frame::On_Command_Child)
	EVT_UPDATE_UI_RANGE	(ID_CMD_MAP_FIRST				, ID_CMD_MAP_LAST		, CSAGA_Frame::On_Command_Child_UI)
	EVT_UPDATE_UI_RANGE	(ID_CMD_HISTOGRAM_FIRST			, ID_CMD_HISTOGRAM_LAST	, CSAGA_Frame::On_Command_Child_UI)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSAGA_Frame::CSAGA_Frame(void)
	: wxMDIParentFrame(NULL, ID_WND_MAIN, SAGA_CAPTION, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE|wxNO_FULL_REPAINT_ON_RESIZE|wxHSCROLL|wxVSCROLL|wxFRAME_NO_WINDOW_MENU)
{
	//-----------------------------------------------------
	g_pSAGA_Frame		= this;

	m_nTopWindows		= 0;
	m_pTopWindows		= NULL;

	m_pINFO				= NULL;
	m_pData_Source		= NULL;
	m_pActive			= NULL;
	m_pWKSP				= NULL;

	SG_Set_UI_Callback	(Get_Callback());

	SetIcon				(IMG_Get_Icon(ID_IMG_SAGA_ICON_32));

	SetDropTarget		(new CSAGA_Frame_DropTarget);

	//-----------------------------------------------------
	int		STATUSBAR_Sizes[STATUSBAR_COUNT]	= {	-1, -1, 90, 90, 90, -1	};

	CreateStatusBar		(STATUSBAR_COUNT);
	SetStatusWidths		(STATUSBAR_COUNT, STATUSBAR_Sizes);
	SetStatusBarPane	(STATUSBAR_DEFAULT);
	StatusBar_Set_Text	(LNG("[VAL] ready"));

	m_pProgressBar		= ((CSAGA_Frame_StatusBar *)GetStatusBar())->m_pProgressBar;

	//-----------------------------------------------------
	m_pLayout			= new wxAuiManager(this);

	m_pLayout->GetArtProvider()->SetColor	(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,
		wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION)
	);

	m_pLayout->GetArtProvider()->SetColor	(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,
		wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE)
	);

	m_pLayout->GetArtProvider()->SetMetric	(wxAUI_DOCKART_GRADIENT_TYPE	, wxAUI_GRADIENT_NONE);
	m_pLayout->GetArtProvider()->SetMetric	(wxAUI_DOCKART_CAPTION_SIZE		, 14);

	m_pLayout->SetFlags(m_pLayout->GetFlags() ^ wxAUI_MGR_TRANSPARENT_DRAG);
//	m_pLayout->SetFlags(m_pLayout->GetFlags() ^ wxAUI_MGR_ALLOW_ACTIVE_PANE);

	//-----------------------------------------------------
	_Bar_Add(m_pINFO		= new CINFO       (this), 0);	m_pINFO			->Add_Pages();
	_Bar_Add(m_pWKSP		= new CWKSP       (this), 2);	m_pWKSP			->Add_Pages();
	_Bar_Add(m_pActive		= new CACTIVE     (this), 1);	m_pActive		->Add_Pages();
	_Bar_Add(m_pData_Source	= new CData_Source(this), 2);	m_pData_Source	->Add_Pages();

	//-----------------------------------------------------
	SetMenuBar(MB_Create(NULL));

	//-----------------------------------------------------
	m_pTB_Main			= 						  _Create_ToolBar();
	m_pTB_Map			= CVIEW_Map				::_Create_ToolBar();
	m_pTB_Map_3D		= CVIEW_Map_3D			::_Create_ToolBar();
	m_pTB_Layout		= CVIEW_Layout			::_Create_ToolBar();
	m_pTB_Table			= CVIEW_Table			::_Create_ToolBar();
	m_pTB_Diagram		= CVIEW_Table_Diagram	::_Create_ToolBar();
	m_pTB_Histogram		= CVIEW_Histogram		::_Create_ToolBar();
	m_pTB_ScatterPlot	= CVIEW_ScatterPlot		::_Create_ToolBar();

	//-----------------------------------------------------
	m_pLayout->GetPane(GetClientWindow()).Show().Center();

	wxString	s;

	if( CONFIG_Read(wxT("/FL"), wxT("MANAGER"), s) )
	{
		m_pLayout->LoadPerspective(s);
	}
	else
	{
		m_pLayout->GetPane(m_pWKSP       ).BestSize(400, 400).FloatingSize(400, 400).Left();
		m_pLayout->GetPane(m_pData_Source).BestSize(400, 400).FloatingSize(400, 400).Left();
		m_pLayout->GetPane(m_pActive     ).BestSize(400, 400).FloatingSize(400, 400).Right();
		m_pLayout->GetPane(m_pINFO       ).BestSize(400, 100).FloatingSize(600, 400).Bottom();
	}

	_Bar_Show(m_pTB_Main, true);

	//-----------------------------------------------------
	m_pLayout->Update();

#if !defined(_SAGA_LINUX)
	Show(true);
#endif

	int		x, y, dx, dy;
	long	l;

	x	= CONFIG_Read(wxT("/FL"), wxT("X" ), l) ? l : -1;
	y	= CONFIG_Read(wxT("/FL"), wxT("Y" ), l) ? l : -1;
	dx	= CONFIG_Read(wxT("/FL"), wxT("DX"), l) ? l : 800;
	dy	= CONFIG_Read(wxT("/FL"), wxT("DY"), l) ? l : 600;

	SetSize(x, y, dx, dy);

	if( !(CONFIG_Read(wxT("/FL"), wxT("STATE"), l) && l == 0) )
	{
		Maximize();
	}

#if defined(_SAGA_LINUX)
	Show(true);
#endif

	Update();

	//-----------------------------------------------------
	if( g_pSAGA->argc <= 1 && g_pData->Initialise() )
	{
		Refresh(false);
	}

	ProgressBar_Set_Position(0);
}

//---------------------------------------------------------
CSAGA_Frame::~CSAGA_Frame(void)
{
	//-----------------------------------------------------
	if( IsIconized() )
	{
		Iconize(false);
	}

	if( IsMaximized() )
	{
		CONFIG_Write(wxT("/FL"), wxT("STATE"), (long)1);
	}
	else
	{
		CONFIG_Write(wxT("/FL"), wxT("STATE"), (long)0);
		CONFIG_Write(wxT("/FL"), wxT("X"    ), (long)GetPosition().x);
		CONFIG_Write(wxT("/FL"), wxT("Y"    ), (long)GetPosition().y);
		CONFIG_Write(wxT("/FL"), wxT("DX"   ), (long)GetSize().x);
		CONFIG_Write(wxT("/FL"), wxT("DY"   ), (long)GetSize().y);
	}

	//-----------------------------------------------------
	CONFIG_Write(wxT("/FL"), wxT("MANAGER"), m_pLayout->SavePerspective().c_str());

	m_pLayout->UnInit();

	delete(m_pLayout);

	//-----------------------------------------------------
	if( m_pTopWindows )
	{
		delete(m_pTopWindows);
	}

	SG_Set_UI_Callback(NULL);

	g_pSAGA_Frame	= NULL;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_Close(wxCloseEvent &event)
{
	if( event.CanVeto() )
	{
		if( !g_pModule && DLG_Message_Confirm(ID_DLG_CLOSE) && g_pData->Close(true) )
		{
			g_pModules->Finalise();

			Destroy();
		}
		else
		{
			if( g_pModule )
			{
				DLG_Message_Show(LNG("Please stop module execution before exiting SAGA."), LNG("Exit SAGA"));
			}

			event.Veto();
		}
	}
	else
	{
		g_pModules->Finalise();

		g_pData->Close(true);

		event.Skip();
	}
}

//---------------------------------------------------------
void CSAGA_Frame::On_Size(wxSizeEvent &event)
{
	event.Skip();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_Quit(wxCommandEvent &WXUNUSED(event))
{
	Close(false);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Help(wxCommandEvent &WXUNUSED(event))
{
	if( !Open_WebBrowser(SG_T("http://sourceforge.net/apps/trac/saga-gis/wiki/WikiStart")) )
	{
	//	DLG_Message_Show(LNG("Online Help"), LNG("SAGA Help"));
	}
}

//---------------------------------------------------------
void CSAGA_Frame::On_About(wxCommandEvent &WXUNUSED(event))
{
	CDLG_About	dlg;

	dlg.ShowModal();
}

//---------------------------------------------------------
void CSAGA_Frame::On_Tips(wxCommandEvent &WXUNUSED(event))
{
	Show_Tips(true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_Cascade(wxCommandEvent &WXUNUSED(event))
{
	Cascade();
}

void CSAGA_Frame::On_Frame_Cascade_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_hTile(wxCommandEvent &WXUNUSED(event))
{
	Tile(wxHORIZONTAL);
}

void CSAGA_Frame::On_Frame_hTile_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_vTile(wxCommandEvent &WXUNUSED(event))
{
	Tile(wxVERTICAL);
}

void CSAGA_Frame::On_Frame_vTile_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_ArrangeIcons(wxCommandEvent &WXUNUSED(event))
{
	ArrangeIcons();
}

void CSAGA_Frame::On_Frame_ArrangeIcons_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_Next(wxCommandEvent &WXUNUSED(event))
{
	ActivateNext();
}

void CSAGA_Frame::On_Frame_Next_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_Previous(wxCommandEvent &WXUNUSED(event))
{
	ActivatePrevious();
}

void CSAGA_Frame::On_Frame_Previous_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_Close(wxCommandEvent &WXUNUSED(event))
{
	if( GetActiveChild() != NULL )
	{
		GetActiveChild()->Close();
	}
}

void CSAGA_Frame::On_Frame_Close_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Frame_Close_All(wxCommandEvent &WXUNUSED(event))
{
	while( GetActiveChild() != NULL )
	{
		delete(GetActiveChild());
	}
}

void CSAGA_Frame::On_Frame_Close_All_UI(wxUpdateUIEvent &event)
{
	event.Enable(GetActiveChild() != NULL);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_WKSP_Show(wxCommandEvent &WXUNUSED(event))
{
	_Bar_Toggle(m_pWKSP);
}

void CSAGA_Frame::On_WKSP_Show_UI(wxUpdateUIEvent &event)
{
	event.Check(m_pWKSP->IsShown() && m_pLayout->GetPane(m_pWKSP).IsShown());
}

//---------------------------------------------------------
void CSAGA_Frame::On_Active_Show(wxCommandEvent &WXUNUSED(event))
{
	_Bar_Toggle(m_pActive);
}

void CSAGA_Frame::On_Active_Show_UI(wxUpdateUIEvent &event)
{
	event.Check(m_pActive->IsShown() && m_pLayout->GetPane(m_pActive).IsShown());
}

//---------------------------------------------------------
void CSAGA_Frame::On_Data_Source_Show(wxCommandEvent &WXUNUSED(event))
{
	_Bar_Toggle(m_pData_Source);
}

void CSAGA_Frame::On_Data_Source_Show_UI(wxUpdateUIEvent &event)
{
	event.Check(m_pData_Source->IsShown() && m_pLayout->GetPane(m_pData_Source).IsShown());
}

//---------------------------------------------------------
void CSAGA_Frame::On_INFO_Show(wxCommandEvent &WXUNUSED(event))
{
	_Bar_Toggle(m_pINFO);
}

void CSAGA_Frame::On_INFO_Show_UI(wxUpdateUIEvent &event)
{
	event.Check(m_pINFO->IsShown() && m_pLayout->GetPane(m_pINFO).IsShown());
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_Command_Workspace(wxCommandEvent &event)
{
	m_pWKSP->On_Command(event);
}

void CSAGA_Frame::On_Command_Workspace_UI(wxUpdateUIEvent &event)
{
	m_pWKSP->On_Command_UI(event);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Command_Module(wxCommandEvent &event)
{
	m_pWKSP->On_Command_Module(event);
}

void CSAGA_Frame::On_Command_Module_UI(wxUpdateUIEvent &event)
{
	m_pWKSP->On_Command_UI_Module(event);
}

//---------------------------------------------------------
void CSAGA_Frame::On_Command_Child(wxCommandEvent &event)
{
	wxMDIChildFrame	*pChild;

	if( (pChild = GetActiveChild()) != NULL )
	{
		pChild->AddPendingEvent(event);
	}
}

void CSAGA_Frame::On_Command_Child_UI(wxUpdateUIEvent &event)
{
	CVIEW_Base	*pChild;

	if( (pChild = wxDynamicCast(GetActiveChild(), CVIEW_Base)) != NULL )
	{
		pChild->On_Command_UI(event);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::Show_Tips(bool bShow)
{
	bool			bTip;
	long			iTip;

#ifdef SHARE_PATH
        wxFileName      fTip(wxT(SHARE_PATH), wxT("saga_tip.txt"));
#else
        wxFileName      fTip(g_pSAGA->Get_App_Path(), wxT("saga_gui"), wxT("tip"));
#endif

	wxTipProvider	*pTip;

	bTip	= CONFIG_Read(wxT("/TIPS"), wxT("ATSTART"), bTip) ? bTip : true;

	if( bShow || (bTip && fTip.FileExists()) )
	{
		iTip	= CONFIG_Read(wxT("/TIPS"), wxT("CURRENT"), iTip) ? iTip : 0;
		pTip	= wxCreateFileTipProvider(fTip.GetFullPath(), iTip);

		bTip	= wxShowTip(this, pTip, bTip);
		iTip	= pTip->GetCurrentTip();

		CONFIG_Write(wxT("/TIPS"), wxT("ATSTART"), bTip);
		CONFIG_Write(wxT("/TIPS"), wxT("CURRENT"), iTip);

		delete(pTip);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSAGA_Frame::Process_Get_Okay(bool bBlink)
{
	if( bBlink )
	{
		m_pProgressBar->Pulse();
	}

	return( g_pSAGA->Process_Get_Okay() );
}

//---------------------------------------------------------
bool CSAGA_Frame::Process_Set_Okay(bool bOkay)
{
	StatusBar_Set_Text(LNG("[VAL] ready"));

	ProgressBar_Set_Position(0);

	g_pSAGA->Process_Set_Okay(bOkay);

	return( bOkay );
}

//---------------------------------------------------------
bool CSAGA_Frame::ProgressBar_Set_Position(int Position)
{
	if( Position < 0 )
	{
		Position	= 0;
	}
	else if( Position > 100 )
	{
		Position	= 100;
	}

	if( m_pProgressBar->GetValue() != Position )
	{
		m_pProgressBar->SetValue(Position);
	}

	return( Process_Get_Okay(false) );
}

//---------------------------------------------------------
bool CSAGA_Frame::ProgressBar_Set_Position(double Position, double Range)
{
	return( ProgressBar_Set_Position(Range != 0.0 ? (int)(100.0 * Position / Range) : 0) );
}

//---------------------------------------------------------
void CSAGA_Frame::StatusBar_Set_Text(const wxChar *Text, int iPane)
{
	if( iPane < 0 || iPane >= STATUSBAR_PROGRESS )
	{
		iPane	= STATUSBAR_DEFAULT;
	}

	if( iPane == STATUSBAR_ACTIVE )
	{
		Set_Pane_Caption(m_pActive, Text && SG_STR_LEN(Text) > 0 ? Text : LNG("[CAP] Object Properties"));
	}

	SetStatusText(Text, iPane);
}

//---------------------------------------------------------
void CSAGA_Frame::Set_Project_Name(wxString Project_Name)
{
	if( Project_Name.Length() > 0 )
	{
		SetTitle(wxString::Format(wxT("%s [%s]"), SAGA_CAPTION, Project_Name.c_str()));
	}
	else
	{
		SetTitle(SAGA_CAPTION);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::Top_Window_Push(wxWindow *pWindow)
{
	if( pWindow )
	{
		for(int i=0; i<m_nTopWindows; i++)
		{
			if( m_pTopWindows[i] == pWindow )
			{
				return;
			}
		}

		m_pTopWindows	= (wxWindow **)SG_Realloc(m_pTopWindows, (m_nTopWindows + 1) * sizeof(wxWindow *));
		m_pTopWindows[m_nTopWindows++]	= pWindow;
	}
}

//---------------------------------------------------------
void CSAGA_Frame::Top_Window_Pop(wxWindow *pWindow)
{
	if( pWindow )
	{
		int		i, j;

		for(i=j=0; j<m_nTopWindows; i++, j++)
		{
			if( m_pTopWindows[i] == pWindow )
				j++;

			if( i < j && j < m_nTopWindows )
				m_pTopWindows[i]	= m_pTopWindows[j];
		}

		if( i < j )
		{
			m_nTopWindows--;
			m_pTopWindows	= (wxWindow **)SG_Realloc(m_pTopWindows, m_nTopWindows * sizeof(wxWindow *));
		}
	}
}

//---------------------------------------------------------
wxWindow * CSAGA_Frame::Top_Window_Get(void)
{
	return( m_nTopWindows > 0 ? m_pTopWindows[m_nTopWindows - 1] : this );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::On_Child_Activates(CVIEW_Base *pChild, bool bActivates)
{
	switch( pChild->Get_View_ID() )
	{
	case ID_VIEW_TABLE:
		_Bar_Show(m_pTB_Table		, bActivates);
		break;

	case ID_VIEW_TABLE_DIAGRAM:
		_Bar_Show(m_pTB_Diagram		, bActivates);
		break;

	case ID_VIEW_MAP:
		_Bar_Show(m_pTB_Map			, bActivates);
		break;

	case ID_VIEW_MAP_3D:
		_Bar_Show(m_pTB_Map_3D		, bActivates);
		break;

	case ID_VIEW_HISTOGRAM:
		_Bar_Show(m_pTB_Histogram	, bActivates);
		break;

	case ID_VIEW_SCATTERPLOT:
		_Bar_Show(m_pTB_ScatterPlot	, bActivates);
		break;

	case ID_VIEW_LAYOUT:
		_Bar_Show(m_pTB_Layout		, bActivates);
		break;
	}
}


///////////////////////////////////////////////////////////
//														 //
//						MenuBar							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxMenuBar * CSAGA_Frame::MB_Create(CVIEW_Base *pChild)
{
	wxMenu		*pMenu;
	wxMenuBar	*pMenuBar	= new wxMenuBar;

	//-----------------------------------------------------
	// 1. File...

	pMenu		= new wxMenu;

	g_pData->Get_FileMenus()->Add(pMenu);

	pMenuBar->Append(pMenu, LNG("[MNU] File"));


	//-----------------------------------------------------
	// 2. Modules...

	pMenu		= new wxMenu;

	g_pModules->Get_Modules_Menu()->Add(pMenu);

	pMenuBar->Append(pMenu, LNG("[MNU] Modules"));


	//-----------------------------------------------------
	// 3. Window...

	pMenu		= new wxMenu;

	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_FRAME_WKSP_SHOW);
	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_FRAME_ACTIVE_SHOW);
	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_FRAME_DATA_SOURCE_SHOW);
	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_FRAME_INFO_SHOW);

#ifdef __WXMSW__
	pMenu->AppendSeparator();
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_CASCADE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_TILE_HORZ);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_TILE_VERT);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_ARRANGEICONS);
#endif	// #ifdef __WXMSW__

	pMenu->AppendSeparator();
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_NEXT);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_PREVIOUS);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_CLOSE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_CLOSE_ALL);

	pMenuBar->Append(pMenu, LNG("[MNU] Window"));


	//-----------------------------------------------------
	// 4. Help...

	pMenu		= new wxMenu;

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_HELP);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_TIPS);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_FRAME_ABOUT);

	pMenuBar->Append(pMenu, LNG("[MNU] ?"));


	//-----------------------------------------------------
	return( pMenuBar );
}

//---------------------------------------------------------
void CSAGA_Frame::MB_Remove(wxMenu *pMenu_File, wxMenu *pMenu_Modules)
{
	g_pData		->Get_FileMenus()		->Del(pMenu_File);
	g_pModules	->Get_Modules_Menu()	->Del(pMenu_Modules);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSAGA_Frame::Set_Pane_Caption(wxWindow *pWindow, wxString Caption)
{
	if( m_pLayout && pWindow )
	{
		m_pLayout->GetPane(pWindow).Caption(Caption);
		m_pLayout->Update();
	}
}

//---------------------------------------------------------
void CSAGA_Frame::_Bar_Add(wxWindow *pWindow, int Position)
{
	m_pLayout->AddPane(pWindow, wxAuiPaneInfo()
		.Name		(pWindow->GetName())
		.Caption	(pWindow->GetName())
		.Layer		(0)
		.Row		(0)
		.Position	(0)
		.MinSize	(100, 100)
		.BestSize	(200, 100)
	);

	switch( Position )
	{
	default:
	case 0:	m_pLayout->GetPane(pWindow).Bottom().FloatingSize(400, 400);	break;
	case 1:	m_pLayout->GetPane(pWindow).Right ().FloatingSize(400, 400);	break;
	case 2:	m_pLayout->GetPane(pWindow).Left  ().FloatingSize(400, 400);	break;
	case 3:	m_pLayout->GetPane(pWindow).Top   ().FloatingSize(400, 400);	break;
	}
}

//---------------------------------------------------------
void CSAGA_Frame::_Bar_Toggle(wxWindow *pWindow)
{
	if( pWindow->IsShown() && m_pLayout->GetPane(pWindow).IsShown() )
	{
		m_pLayout->GetPane(pWindow).Hide();

		if( m_pLayout->GetPane(pWindow).IsToolbar() )
			pWindow->Hide();
	}
	else
	{
		if( m_pLayout->GetPane(pWindow).IsToolbar() )
			pWindow->Show();

		m_pLayout->GetPane(pWindow).Show();
	}

	m_pLayout->Update();
}

//---------------------------------------------------------
void CSAGA_Frame::_Bar_Show(wxWindow *pWindow, bool bShow)
{
	if( pWindow && pWindow->IsShown() != bShow )
	{
		_Bar_Toggle(pWindow);
	}
}


///////////////////////////////////////////////////////////
//														 //
//						ToolBar							 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define TOOLBAR_SIZE_IMG		16

//---------------------------------------------------------
wxToolBarBase * CSAGA_Frame::TB_Create(int ID)
{
	wxToolBar	*pToolBar	= new wxToolBar(this, ID, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_FLAT|wxTB_NODIVIDER);

	pToolBar->SetToolBitmapSize(wxSize(TOOLBAR_SIZE_IMG, TOOLBAR_SIZE_IMG));

	return( pToolBar );
}

//---------------------------------------------------------
void CSAGA_Frame::TB_Add(wxToolBarBase *pToolBar, const wxChar *Name)
{
	pToolBar->Realize();
	pToolBar->Hide();

	m_pLayout->AddPane(pToolBar, wxAuiPaneInfo()
	//	.Name			(pToolBar->GetName())
		.Name			(Name)
		.Caption		(Name)
		.ToolbarPane	()
		.Top			()
		.LeftDockable	(false)
		.RightDockable	(false)
		.Hide			()
		.BestSize		(pToolBar->GetBestSize())
	);
}

//---------------------------------------------------------
void CSAGA_Frame::TB_Add_Item(wxToolBarBase *pToolBar, bool bCheck, int Cmd_ID)
{
	if( bCheck )
		((wxToolBar *)pToolBar)->AddTool(Cmd_ID, CMD_Get_Name(Cmd_ID), IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), CMD_Get_Help(Cmd_ID), wxITEM_CHECK);
	else
		((wxToolBar *)pToolBar)->AddTool(Cmd_ID, CMD_Get_Name(Cmd_ID), IMG_Get_Bitmap(CMD_Get_ImageID(Cmd_ID), TOOLBAR_SIZE_IMG), CMD_Get_Help(Cmd_ID));
}

//---------------------------------------------------------
void CSAGA_Frame::TB_Add_Separator(wxToolBarBase *pToolBar)
{
	((wxToolBar *)pToolBar)->AddSeparator();
}

//---------------------------------------------------------
wxToolBarBase * CSAGA_Frame::_Create_ToolBar(void)
{
	wxToolBarBase	*pToolBar	= TB_Create(ID_TB_MAIN);

	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_WKSP_OPEN);
	CMD_ToolBar_Add_Separator(pToolBar);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_FRAME_WKSP_SHOW);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_FRAME_ACTIVE_SHOW);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_FRAME_DATA_SOURCE_SHOW);
	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_FRAME_INFO_SHOW);
	CMD_ToolBar_Add_Separator(pToolBar);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_FRAME_HELP);

	TB_Add(pToolBar, LNG("[CAP] Standard"));

	return( pToolBar );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
