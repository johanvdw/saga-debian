/**********************************************************
 * Version $Id: Data_Source.cpp 911 2011-02-14 16:38:15Z reklov_w $
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
//                   data_source.cpp                     //
//                                                       //
//          Copyright (C) 2011 by Olaf Conrad            //
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
//                University of Hamburg                  //
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
#include <wx/treectrl.h>
#include <wx/imaglist.h>

#include <saga_api/saga_api.h>

#include "res_controls.h"
#include "res_images.h"

#include "helper.h"

#include "data_source.h"
#include "data_source_files.h"

#include "wksp_layer.h"
#include "wksp_map_layer.h"

#include "wksp_data_manager.h"
#include "wksp_data_layers.h"
#include "wksp_table.h"

#include "wksp_module_library.h"
#include "wksp_module.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	IMG_FILES	= 0,
	IMG_DATABASE,
	IMG_WEBSERVICE
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CData_Source	*g_pData_Source	= NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CData_Source, wxNotebook)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CData_Source::CData_Source(wxWindow *pParent)
	: wxNotebook(pParent, ID_WND_DATA_SOURCE, wxDefaultPosition, wxDefaultSize, NOTEBOOK_STYLE, LNG("[CAP] Data Source"))
{
	g_pData_Source	= this;

	//-----------------------------------------------------
	AssignImageList(new wxImageList(IMG_SIZE_NOTEBOOK, IMG_SIZE_NOTEBOOK, true, 0));

	IMG_ADD_TO_NOTEBOOK(ID_IMG_NB_DATA_SOURCE_FILES);
	IMG_ADD_TO_NOTEBOOK(ID_IMG_NB_DATA_SOURCE_DATABASE);
	IMG_ADD_TO_NOTEBOOK(ID_IMG_NB_DATA_SOURCE_WEBSERVICE);

	//-----------------------------------------------------
	m_pFiles	= new CData_Source_Files	(this);	m_pFiles	->SetName(LNG("[CAP] File System"));

#if defined(_SAGA_MSW)
	m_pFiles	->Hide();
#endif
}

//---------------------------------------------------------
void CData_Source::Add_Pages(void)
{
	_Show_Page(m_pFiles);
}

//---------------------------------------------------------
CData_Source::~CData_Source(void)
{
	g_pData_Source		= NULL;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CData_Source::Set_Data_Source(CWKSP_Base_Item *pItem)
{
	if( pItem == NULL )
	{
		return( true );
	}

	//-----------------------------------------------------
	switch( pItem->Get_Type() )
	{
	default:
		break;

	case WKSP_ITEM_Map_Layer:
		m_pFiles->SetPath(((CWKSP_Map_Layer      *)pItem)->Get_Layer()->Get_Object()->Get_File_Name());
		break;

	case WKSP_ITEM_Table:
		m_pFiles->SetPath(((CWKSP_Table          *)pItem)->Get_Table()->Get_File_Name());
		break;

	case WKSP_ITEM_Shapes:
	case WKSP_ITEM_TIN:
	case WKSP_ITEM_PointCloud:
	case WKSP_ITEM_Grid:
		m_pFiles->SetPath(((CWKSP_Layer          *)pItem)->Get_Object()->Get_File_Name());
		break;

	case WKSP_ITEM_Module_Library:
		m_pFiles->SetPath(((CWKSP_Module_Library *)pItem)->Get_File_Name());
		break;

	case WKSP_ITEM_Module:
		m_pFiles->SetPath(((CWKSP_Module         *)pItem)->Get_File_Name());
		break;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CData_Source::_Show_Page(wxWindow *pPage)
{
	int		Image_ID	= -1;

	//-----------------------------------------------------
	if( pPage == m_pFiles )		Image_ID	= IMG_FILES;

	//-----------------------------------------------------
	if( pPage )
	{
		for(int i=0; i<(int)GetPageCount(); i++)
		{
			if( GetPage(i) == pPage )
			{
				return( true );
			}
		}

		AddPage(pPage, pPage->GetName(), false, Image_ID);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CData_Source::_Hide_Page(wxWindow *pPage)
{
	for(int i=0; i<(int)GetPageCount(); i++)
	{
		if( GetPage(i) == pPage )
		{
			if( i == GetSelection() )
			{
				SetSelection(i - 1);
			}

			RemovePage(i);

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
