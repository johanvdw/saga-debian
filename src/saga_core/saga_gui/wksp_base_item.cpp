
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
//                  WKSP_Base_Item.cpp                   //
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

#include "active.h"

#include "wksp_base_item.h"
#include "wksp_base_manager.h"

#include "wksp_module_control.h"
#include "wksp_data_control.h"
#include "wksp_map_control.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Base_Item *	Get_Active_Item(void)
{
	return( g_pACTIVE ? g_pACTIVE->Get_Item() : NULL );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Base_Item::CWKSP_Base_Item(void)
{
	m_bManager	= false;
	m_pManager	= NULL;
	m_ID		= 0;
}

//---------------------------------------------------------
CWKSP_Base_Item::~CWKSP_Base_Item(void)
{
	if( g_pACTIVE && g_pACTIVE->Get_Item() == this )
	{
		g_pACTIVE->Set_Active(NULL);
	}

	if( m_pManager )
	{
		m_pManager->Del_Item(this);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Base_Item::Get_Index(void)
{
	int		iItem;

	if( m_pManager )
	{
		for(iItem=0; iItem<m_pManager->Get_Count(); iItem++)
		{
			if( this == m_pManager->Get_Item(iItem) )
			{
				return( iItem );
			}
		}
	}

	return( -1 );
}

//---------------------------------------------------------
CWKSP_Base_Control * CWKSP_Base_Item::Get_Control(void)
{
	switch( Get_Type() )
	{
    default:
        return( NULL );

	case WKSP_ITEM_Module_Manager:
	case WKSP_ITEM_Module_Library:
	case WKSP_ITEM_Module:
		return( g_pModule_Ctrl );

	case WKSP_ITEM_Data_Manager:
	case WKSP_ITEM_Table_Manager:
	case WKSP_ITEM_Table:
	case WKSP_ITEM_Shapes_Manager:
	case WKSP_ITEM_Shapes_Type:
	case WKSP_ITEM_Shapes:
	case WKSP_ITEM_TIN_Manager:
	case WKSP_ITEM_TIN:
	case WKSP_ITEM_PointCloud_Manager:
	case WKSP_ITEM_PointCloud:
	case WKSP_ITEM_Grid_Manager:
	case WKSP_ITEM_Grid_System:
	case WKSP_ITEM_Grid:
		return( g_pData_Ctrl );

	case WKSP_ITEM_Map_Manager:
	case WKSP_ITEM_Map:
	case WKSP_ITEM_Map_Layer:
		return( g_pMap_Ctrl );
	}
}

//---------------------------------------------------------
bool CWKSP_Base_Item::is_Selected(void)
{
	CWKSP_Base_Control	*pControl	= Get_Control();

	return( pControl != NULL && pControl->IsSelected(GetId()) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Base_Item::On_Command(int Cmd_ID)
{
	return( false );
}

//---------------------------------------------------------
bool CWKSP_Base_Item::On_Command_UI(wxUpdateUIEvent &event)
{
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Base_Item::Parameters_Changed(void)
{
	if( Get_Control() && GetId().IsOk() )
	{
		Get_Control()->SetItemText(GetId(), Get_Name());
	}

	if( g_pACTIVE )
	{
		g_pACTIVE->Update_Description();
		g_pACTIVE->Update(this, false);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
