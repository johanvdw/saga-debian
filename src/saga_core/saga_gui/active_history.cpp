
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
//                  ACTIVE_History.cpp                   //
//                                                       //
//          Copyright (C) 2008 by Olaf Conrad            //
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
#include <wx/imaglist.h>

#include "res_controls.h"
#include "res_images.h"

#include "wksp_table.h"
#include "wksp_layer.h"

#include "active_history.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	IMG_DATA	= 0,
	IMG_DATE,
	IMG_NAME,
	IMG_ENTRY,
	IMG_ENTRY_DATA
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CACTIVE_History, wxTreeCtrl)

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CACTIVE_History, wxTreeCtrl)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CACTIVE_History::CACTIVE_History(wxWindow *pParent)
	: wxTreeCtrl(pParent, ID_WND_ACTIVE_HISTORY , wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS)
{
	AssignImageList(new wxImageList(IMG_SIZE_TREECTRL, IMG_SIZE_TREECTRL, true, 0));
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_DATA_MANAGER);	// DATA
	IMG_ADD_TO_TREECTRL(ID_IMG_NB_INFO_EXECUTION);	// DATE
	IMG_ADD_TO_TREECTRL(ID_IMG_WKSP_MODULE);		// NAME
	IMG_ADD_TO_TREECTRL(ID_IMG_TB_INFO);			// ENTRY
	IMG_ADD_TO_TREECTRL(ID_IMG_TB_MAP_ZOOM_NEXT);	// ENTRY_DATA

	Set_Item(NULL);
}

//---------------------------------------------------------
CACTIVE_History::~CACTIVE_History(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CACTIVE_History::Set_Item(CWKSP_Base_Item *pItem)
{
	CSG_Data_Object	*pObject	= NULL;

	if( pItem )
	{
		switch( pItem->Get_Type() )
		{
		default:
			pObject	= NULL;
			break;

		case WKSP_ITEM_Table:
			pObject	= ((CWKSP_Table *)pItem)->Get_Table();
			break;

		case WKSP_ITEM_Shapes:
		case WKSP_ITEM_TIN:
		case WKSP_ITEM_PointCloud:
		case WKSP_ITEM_Grid:
			pObject	= ((CWKSP_Layer *)pItem)->Get_Object();
			break;
		}
	}

	Freeze();

	DeleteAllItems();

	if( pObject == NULL || pObject->Get_History().Get_Children_Count() <= 0 )
	{
		AddRoot(LNG("No history available"), IMG_DATA);
	}
	else
	{
		_Add_History(AddRoot(pObject->Get_Name(), IMG_DATA), pObject->Get_History());

		Expand(GetRootItem());
	}

	Thaw();

	return( true );
}

//---------------------------------------------------------
bool CACTIVE_History::_Add_History(wxTreeItemId Parent, CSG_MetaData &History)
{
	if( Parent.IsOk() && History.Get_Children_Count() > 0 )
	{
		CSG_MetaData	*pEntry;

		if( (pEntry = History.Get_Child("MODULE")) != NULL )
		{
			AppendItem(Parent, pEntry->Get_Content().c_str(), IMG_NAME);
		}

		for(int i=0; i<History.Get_Children_Count(); i++)
		{
			wxTreeItemId	Entry;
			wxString		s;
			CSG_String		Name, Type;

			pEntry	= History.Get_Child(i);

			if( !pEntry->Get_Name().Cmp(SG_T("MODULE")) )
			{
				// nop
			}
			else if( !pEntry->Get_Name().Cmp(SG_T("OPTION")) )
			{
				if( !pEntry->Get_Property(SG_T("name"), Name) )	Name	= LNG("Option");

				Entry	= AppendItem(Parent, wxString::Format(wxT("%s [%s]"), Name.c_str(), pEntry->Get_Content().c_str()), IMG_ENTRY);
			}
			else if( !pEntry->Get_Name().Cmp(SG_T("DATA")) )
			{
				if( !pEntry->Get_Property(SG_T("name"), Name) )	Name	= LNG("Data");
				if( !pEntry->Get_Property(SG_T("type"), Type) )	Type	= LNG("unknown type");

				Entry	= AppendItem(Parent, wxString::Format(wxT("%s [%s]"), Name.c_str(), Type.c_str()), IMG_ENTRY_DATA);

				_Add_History(Entry, *pEntry);
			}
			else if( pEntry->Get_Children_Count() )
			{
				Entry	= AppendItem(Parent, wxString::Format(wxT("%s"), pEntry->Get_Name().c_str()), IMG_NAME);

				_Add_History(Entry, *pEntry);
			}
			else
			{
				Entry	= AppendItem(Parent, wxString::Format(wxT("%s [%s]"), pEntry->Get_Name().c_str(), pEntry->Get_Content().c_str()), IMG_ENTRY_DATA);
			}

			if( Entry.IsOk() )
			{
				Expand(Entry);
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
