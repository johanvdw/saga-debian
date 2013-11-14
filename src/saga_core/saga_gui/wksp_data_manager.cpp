/**********************************************************
 * Version $Id: wksp_data_manager.cpp 1739 2013-06-20 14:01:48Z oconrad $
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
//                 WKSP_Data_Manager.cpp                 //
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
#ifdef _SAGA_LINUX
#include <stdlib.h>
#endif

#include <wx/filename.h>

#include <saga_api/saga_api.h>

#include "saga.h"

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"
#include "project.h"

#include "wksp_data_control.h"
#include "wksp_data_manager.h"
#include "wksp_data_menu_files.h"

#include "wksp_layer.h"

#include "wksp_table_manager.h"
#include "wksp_shapes_manager.h"
#include "wksp_tin_manager.h"
#include "wksp_pointcloud_manager.h"
#include "wksp_grid_manager.h"

#include "wksp_map_manager.h"

#include "wksp_module.h"

#include "active.h"
#include "active_parameters.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Manager	*g_pData	= NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Data_Manager::CWKSP_Data_Manager(void)
{
	m_pTables		= NULL;
	m_pShapes		= NULL;
	m_pTINs			= NULL;
	m_pPointClouds	= NULL;
	m_pGrids		= NULL;

	m_pProject		= new CWKSP_Project;
	m_pMenu_Files	= new CWKSP_Data_Menu_Files;

	g_pData			= this;

	//-----------------------------------------------------
	bool		bValue;
	long		lValue;
	double		dValue;
	wxString	sValue;

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("CACHE_TMP_DIR")	, sValue) )
	{
		SG_Grid_Cache_Set_Directory(sValue);
	}

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("CACHE_AUTO")		, bValue) )
	{
		SG_Grid_Cache_Set_Automatic(bValue);
	}

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("CACHE_THRESHOLD")	, lValue) )
	{
		SG_Grid_Cache_Set_Threshold(lValue);
	}

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("CACHE_CONFIRM")	, lValue) )
	{
		SG_Grid_Cache_Set_Confirm  (lValue);
	}

	//-----------------------------------------------------
	CSG_Parameter	*pNode;

	m_Parameters.Create(this, _TL(""), _TL(""));

	pNode	= m_Parameters.Add_Node(NULL, "NODE_GRID_MEM", _TL("Grid File Caching"), _TL(""));

	m_Parameters.Add_FilePath(
		pNode	, "GRID_MEM_CACHE_TMPDIR"	, _TL("Temporary files"),
		_TL("Directory, where temporary cache files shall be saved."),
		NULL, SG_Grid_Cache_Get_Directory(), true, true
	);

	m_Parameters.Add_Value(
		pNode	, "GRID_MEM_CACHE_AUTO"		, _TL("Automatic mode"),
		_TL("Activate file caching automatically, if memory size exceeds the threshold value."),
		PARAMETER_TYPE_Bool, SG_Grid_Cache_Get_Automatic()
	);

	m_Parameters.Add_Value(
		pNode	, "GRID_MEM_CACHE_THRSHLD"	, _TL("Threshold for automatic mode [MB]"),
		_TL(""),
		PARAMETER_TYPE_Double, SG_Grid_Cache_Get_Threshold_MB(), 0.0, true
	);

	m_Parameters.Add_Choice(
		pNode	, "GRID_MEM_CACHE_CONFIRM"	, _TL("Confirm file caching"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("do not confirm"),
			_TL("confirm"),
			_TL("confirm with options")
		),
		SG_Grid_Cache_Get_Confirm()
	);

	//-----------------------------------------------------
	pNode	= m_Parameters.Add_Node(NULL, "NODE_GRID_DISPLAY", _TL("Grid Display Defaults"), _TL(""));

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("DISPLAY_RANGEFIT")	, lValue) == false )
	{
		lValue	= 2;
	}

	m_Parameters.Add_Choice(
		pNode	, "GRID_DISPLAY_RANGEFIT"	, _TL("Display Range"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("Minimum/Maximum"),
			_TL("Standard Deviation")
		), lValue
	);

	if( CONFIG_Read(wxT("/DATA/GRIDS"), wxT("FIT_STDDEV")	, dValue) == false )
	{
		dValue	= 2.0;
	}

	m_Parameters.Add_Value(
		pNode	, "FIT_STDDEV"				, _TL("Fit to Standard Deviation"),
		_TL("Multiple of Standard Deviation used as default for colour classifications."),
		PARAMETER_TYPE_Double, dValue, 0.01, true
	);

	//-----------------------------------------------------
	pNode	= m_Parameters.Add_Node(NULL, "NODE_GENERAL", _TL("General"), _TL(""));

	if( CONFIG_Read(wxT("/DATA"), wxT("PROJECT_START")			, lValue) == false )
	{
		lValue	= 2;
	}

	m_Parameters.Add_Choice(
		pNode	, "PROJECT_START"			, _TL("Start Project"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("empty"),
			_TL("last opened"),
			_TL("automatically save and load")
		), lValue
	);

	//-----------------------------------------------------
	if( CONFIG_Read(wxT("/DATA"), wxT("NUMBERING")				, lValue) == false )
	{
		lValue	= 2;
	}

	m_Parameters.Add_Value(
		pNode	, "NUMBERING"				, _TL("Numbering of Data Sets"),
		_TL("Leading zeros for data set numbering. Set to -1 for not using numbers at all."),
		PARAMETER_TYPE_Int, m_Numbering = lValue, -1, true
	);

	//-----------------------------------------------------
	if( CONFIG_Read(wxT("/DATA"), wxT("HISTORY_DEPTH")			, lValue) == true )
	{
		SG_Set_History_Depth(lValue);
	}

	m_Parameters.Add_Value(
		pNode	, "HISTORY_DEPTH"			, _TL("History Depth"),
		_TL("Depth to which data history is stored. Set -1 keeps all history entries (default), 0 switches history option off."),
		PARAMETER_TYPE_Int, SG_Get_History_Depth(), -1, true
	);
}

//---------------------------------------------------------
CWKSP_Data_Manager::~CWKSP_Data_Manager(void)
{
	g_pData	= NULL;

	delete(m_pProject);
	delete(m_pMenu_Files);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Initialise(void)
{
	wxString	FileName;

	if( m_pProject->Has_File_Name() )
	{
		return( m_pProject->Load(false) );
	}
	else
	{
		return( CONFIG_Read(wxT("/DATA"), wxT("PROJECT_FILE"), FileName) && wxFileExists(FileName) && m_pProject->Load(FileName, false, false) );
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Finalise(void)
{
	//-----------------------------------------------------
#ifdef _SAGA_LINUX
//	wxFileName	fProject(wxString(getenv( "HOME"), wxConvFile ), wxT("saga_gui"), wxT("cfg"));
	CSG_String	sHome(getenv("HOME"));
	wxFileName	fProject(sHome.c_str(), wxT("saga_gui"), wxT("cfg"));
#else
	wxFileName	fProject(g_pSAGA->Get_App_Path(), wxT("saga_gui"), wxT("cfg"));

	if(	( fProject.FileExists() && (!fProject.IsFileReadable() || !fProject.IsFileWritable()))
	||	(!fProject.FileExists() && (!fProject.IsDirReadable () || !fProject.IsDirWritable ())) )
	{
		fProject.Assign(wxGetHomeDir(), wxT("saga_gui"), wxT("cfg"));
	}
#endif

	//-----------------------------------------------------
	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("CACHE_TMP_DIR"   ),		SG_Grid_Cache_Get_Directory());
	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("CACHE_AUTO"      ),		SG_Grid_Cache_Get_Automatic());
	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("CACHE_THRESHOLD" ), (long)SG_Grid_Cache_Get_Threshold());
	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("CACHE_CONFIRM"   ), (long)SG_Grid_Cache_Get_Confirm  ());

	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("DISPLAY_RANGEFIT"), (long)m_Parameters("GRID_DISPLAY_RANGEFIT")->asInt());
	CONFIG_Write(wxT("/DATA/GRIDS"), wxT("FIT_STDDEV"      ),       m_Parameters("FIT_STDDEV"   )->asDouble());

	CONFIG_Write(wxT("/DATA"      ), wxT("PROJECT_START"   ), (long)m_Parameters("PROJECT_START")->asInt());
	CONFIG_Write(wxT("/DATA"      ), wxT("NUMBERING"       ), (long)m_Parameters("NUMBERING"    )->asInt());

	CONFIG_Write(wxT("/DATA"      ), wxT("HISTORY_DEPTH"   ), (long)m_Parameters("HISTORY_DEPTH")->asInt());

	if( Get_Count() == 0 )
	{
		if( fProject.FileExists() )
			wxRemoveFile(fProject.GetFullPath());

		CONFIG_Write(wxT("/DATA"), wxT("PROJECT_FILE"), _TL(""));
	}
	else switch( m_Parameters("PROJECT_START")->asInt() )
	{
	case 0:	// empty
        if( fProject.FileExists() )
            wxRemoveFile(fProject.GetFullPath());
		CONFIG_Write(wxT("/DATA"), wxT("PROJECT_FILE"), _TL(""));
		break;

	case 1:	// last opened
        if( fProject.FileExists() )
            wxRemoveFile(fProject.GetFullPath());
		CONFIG_Write(wxT("/DATA"), wxT("PROJECT_FILE"), m_pProject->Get_File_Name());
		break;

	case 2:	// automatically save and load
		m_pProject->Save(fProject.GetFullPath(), false);
		CONFIG_Write(wxT("/DATA"), wxT("PROJECT_FILE"), fProject.GetFullPath());
		break;
	}

	m_pProject->Clr_File_Name();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Data_Manager::Get_Name(void)
{
	return( _TL("Data") );
}

//---------------------------------------------------------
wxString CWKSP_Data_Manager::Get_Description(void)
{
	wxString	s;

	s.Printf(wxT("<b>%s</b><br>"), _TL("Data"));

	if( Get_Count() <= 0 )
	{
		s.Append(_TL("No data loaded."));
	}
	else
	{
		if( m_pProject->Has_File_Name() )
		{
			s.Append(wxString::Format(wxT("%s: %s<br>"), _TL("Project File"), m_pProject->Get_File_Name()));
		}

		if( Get_Tables() )
		{
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("Tables"), Get_Tables()->Get_Count()));
		}

		if( Get_Shapes() )
		{
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("Shapes"), Get_Shapes()->Get_Items_Count()));
		}

		if( Get_TINs() )
		{
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("TIN"), Get_TINs()->Get_Count()));
		}

		if( Get_PointClouds() )
		{
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("Point Clouds"), Get_PointClouds()->Get_Count()));
		}

		if( Get_Grids() )
		{
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("Grid Systems"), Get_Grids()->Get_Count()));
			s.Append(wxString::Format(wxT("%s: %d<br>"), _TL("Grids"), Get_Grids()->Get_Items_Count()));
		}
	}

	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Data_Manager::Get_Menu(void)
{
	wxMenu	*pMenu;

	pMenu	= new wxMenu(_TL("Data"));

//	CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_CLOSE);

//	pMenu->AppendSeparator();

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_NEW);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_OPEN);
//	CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_OPEN_ADD);

	if( Get_Count() > 0 )
	{
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_SAVE);
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_DATA_PROJECT_SAVE_AS);
		pMenu->AppendSeparator();
		CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_SEARCH);
	}

	return( pMenu );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::On_Command(int Cmd_ID)
{
	//-----------------------------------------------------
	if( Open_CMD(Cmd_ID) )
	{
		return( true );
	}

	if( m_pTables && Cmd_ID >= ID_CMD_TABLES_FIRST && Cmd_ID <= ID_CMD_TABLES_LAST && m_pTables->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pShapes && Cmd_ID >= ID_CMD_SHAPES_FIRST && Cmd_ID <= ID_CMD_SHAPES_LAST && m_pShapes->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pTINs   && Cmd_ID >= ID_CMD_TIN_FIRST    && Cmd_ID <= ID_CMD_TIN_LAST    && m_pTINs  ->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pPointClouds && Cmd_ID >= ID_CMD_POINTCLOUD_FIRST && Cmd_ID <= ID_CMD_POINTCLOUD_LAST && m_pPointClouds->On_Command(Cmd_ID) )
	{
		return( true );
	}

	if( m_pGrids  && Cmd_ID >= ID_CMD_GRIDS_FIRST  && Cmd_ID <= ID_CMD_GRIDS_LAST  && m_pGrids ->On_Command(Cmd_ID) )
	{
		return( true );
	}

	//-----------------------------------------------------
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Base_Manager::On_Command(Cmd_ID) );

	//-----------------------------------------------------
	case ID_CMD_DATA_PROJECT_NEW:		Close(false);						break;
	case ID_CMD_DATA_PROJECT_OPEN:		m_pProject->Load(false);			break;
	case ID_CMD_DATA_PROJECT_OPEN_ADD:	m_pProject->Load(true);				break;
	case ID_CMD_DATA_PROJECT_SAVE:		m_pProject->Save(true);				break;
	case ID_CMD_DATA_PROJECT_SAVE_AS:	m_pProject->Save();					break;

	//-----------------------------------------------------
	case ID_CMD_TABLES_OPEN:			Open(DATAOBJECT_TYPE_Table);		break;
	case ID_CMD_SHAPES_OPEN:			Open(DATAOBJECT_TYPE_Shapes);		break;
	case ID_CMD_TIN_OPEN:				Open(DATAOBJECT_TYPE_TIN);			break;
	case ID_CMD_POINTCLOUD_OPEN:		Open(DATAOBJECT_TYPE_PointCloud);	break;
	case ID_CMD_GRIDS_OPEN:				Open(DATAOBJECT_TYPE_Grid);			break;

	//-----------------------------------------------------
	case ID_CMD_WKSP_ITEM_RETURN:
		break;

	case ID_CMD_WKSP_ITEM_CLOSE:
		Close(false);
		break;
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Base_Manager::On_Command_UI(event) );

	case ID_CMD_DATA_PROJECT_NEW:
		event.Enable(Get_Count() > 0 && g_pModule == NULL);
		break;

	case ID_CMD_WKSP_ITEM_CLOSE:
		event.Enable(Get_Count() > 0 && g_pModule == NULL);
		break;

	case ID_CMD_DATA_PROJECT_SAVE:
		event.Enable(Get_Count() > 0 && m_pProject->Has_File_Name() );
		break;

	case ID_CMD_DATA_PROJECT_SAVE_AS:
		event.Enable(Get_Count() > 0);
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
void CWKSP_Data_Manager::Parameters_Changed(void)
{
	SG_Grid_Cache_Set_Automatic		(m_Parameters("GRID_MEM_CACHE_AUTO")	->asBool());
	SG_Grid_Cache_Set_Threshold_MB	(m_Parameters("GRID_MEM_CACHE_THRSHLD")	->asDouble());
	SG_Grid_Cache_Set_Confirm		(m_Parameters("GRID_MEM_CACHE_CONFIRM")	->asInt());
	SG_Grid_Cache_Set_Directory		(m_Parameters("GRID_MEM_CACHE_TMPDIR")	->asString());

	m_Numbering	= m_Parameters("NUMBERING")->asInt();

	SG_Set_History_Depth(m_Parameters("HISTORY_DEPTH")->asInt());

	CWKSP_Base_Manager::Parameters_Changed();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Base_Item * CWKSP_Data_Manager::Open(const wxString &File, int DataType)
{
	CSG_Data_Object	*pObject	= NULL;

//	SG_Get_Data_Manager().Add(File, DataType);

	switch( DataType )
	{
	case DATAOBJECT_TYPE_Table:			pObject	= SG_Create_Table		(&File);	break;
	case DATAOBJECT_TYPE_Shapes:		pObject	= SG_Create_Shapes		(&File);	break;
	case DATAOBJECT_TYPE_TIN:			pObject	= SG_Create_TIN			(&File);	break;
	case DATAOBJECT_TYPE_PointCloud:	pObject	= SG_Create_PointCloud	(&File);	break;
	case DATAOBJECT_TYPE_Grid:			pObject	= SG_Create_Grid		(&File);	break;
	}

	if( pObject )
	{
		CWKSP_Data_Item	*pItem;

		if( pObject->is_Valid() && (pItem = Add(pObject)) != NULL )
		{
			m_pMenu_Files->Recent_Add(pObject->Get_ObjectType(), File);

			SG_Get_Data_Manager().Add(pObject);

			return( (CWKSP_Base_Item *)pItem );
		}

		delete(pObject);
	}

	m_pMenu_Files->Recent_Del(DataType, File);

	return( NULL );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open(const wxString &File)
{
	if( SG_File_Cmp_Extension(File, SG_T("sprj")) )
	{
		return( m_pProject->Load(File, false, true) );
	}

	if( SG_File_Cmp_Extension(File, SG_T("txt" ))
	||	SG_File_Cmp_Extension(File, SG_T("csv" ))
	||	SG_File_Cmp_Extension(File, SG_T("dbf" )) )
	{
		return( Open(File, DATAOBJECT_TYPE_Table     ) != NULL );
	}

	if( SG_File_Cmp_Extension(File, SG_T("shp" )) )
	{
		return( Open(File, DATAOBJECT_TYPE_Shapes    ) != NULL );
	}

	if( SG_File_Cmp_Extension(File, SG_T("spc" )) )
	{
		return( Open(File, DATAOBJECT_TYPE_PointCloud) != NULL );
	}

	if(	SG_File_Cmp_Extension(File, SG_T("sgrd"))
	||	SG_File_Cmp_Extension(File, SG_T("dgm" ))
	||	SG_File_Cmp_Extension(File, SG_T("grd" )) )
	{
		return( Open(File, DATAOBJECT_TYPE_Grid      ) != NULL );
	}

	return( SG_Get_Data_Manager().Add(&File) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open(int DataType)
{
	bool	bResult	= false;

	int				ID;
	wxArrayString	Files;

	//-----------------------------------------------------
	switch( DataType )
	{
	default:	return( false );
	case DATAOBJECT_TYPE_Table:			ID	= ID_DLG_TABLES_OPEN;		break;
	case DATAOBJECT_TYPE_Shapes:		ID	= ID_DLG_SHAPES_OPEN;		break;
	case DATAOBJECT_TYPE_TIN:			ID	= ID_DLG_TIN_OPEN;			break;
	case DATAOBJECT_TYPE_PointCloud:	ID	= ID_DLG_POINTCLOUD_OPEN;	break;
	case DATAOBJECT_TYPE_Grid:			ID	= ID_DLG_GRIDS_OPEN;		break;
	}

	//-----------------------------------------------------
	if( DLG_Open(Files, ID) )
	{
		MSG_General_Add_Line();

		for(size_t i=0; i<Files.GetCount(); i++)
		{
			if( Open(Files[i], DataType) )
			{
				bResult	= true;
			}
		}
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Open_CMD(int Cmd_ID)
{
	return( m_pMenu_Files->Recent_Open(Cmd_ID) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Save_Modified(CWKSP_Base_Item *pItem)
{
	return( m_pProject->Save_Modified(pItem) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Save_Modified_Sel(void)
{
	return( m_pProject->Save_Modified(this, true) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Close(bool bSilent)
{
	if( Get_Count() == 0 )
	{
		Finalise();

		return( true );
	}
	else if( (bSilent || DLG_Message_Confirm(_TL("Close all data sets"), _TL("Close"))) && Save_Modified(this) )
	{
		Finalise();

		g_pACTIVE->Get_Parameters()->Restore_Parameters();
		g_pMaps->Close(true);

		return( g_pData_Ctrl->Close(true) );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_MANAGER(pManager, Class)	if( !pManager && bAdd ) Add_Item(pManager = new Class); return( pManager );

CWKSP_Base_Manager * CWKSP_Data_Manager::Get_Manager(TSG_Data_Object_Type Type, bool bAdd)
{
	switch( Type )
	{
	default:	return( NULL );

	case DATAOBJECT_TYPE_Table:      GET_MANAGER(m_pTables     , CWKSP_Table_Manager);
	case DATAOBJECT_TYPE_TIN:        GET_MANAGER(m_pTINs       , CWKSP_TIN_Manager);
	case DATAOBJECT_TYPE_PointCloud: GET_MANAGER(m_pPointClouds, CWKSP_PointCloud_Manager);
	case DATAOBJECT_TYPE_Shapes:     GET_MANAGER(m_pShapes     , CWKSP_Shapes_Manager);
	case DATAOBJECT_TYPE_Grid:       GET_MANAGER(m_pGrids      , CWKSP_Grid_Manager);
	}
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Get(CSG_Data_Object *pObject)
{
	if( pObject && Get_Manager(pObject->Get_ObjectType()) )
	{
		switch( pObject->Get_ObjectType() )
		{
		default:	return( NULL );

		case DATAOBJECT_TYPE_Table:      return( (CWKSP_Data_Item *)m_pTables     ->Get_Data((CSG_Table      *)pObject) );
		case DATAOBJECT_TYPE_TIN:        return( (CWKSP_Data_Item *)m_pTINs       ->Get_Data((CSG_TIN        *)pObject) );
		case DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Data_Item *)m_pPointClouds->Get_Data((CSG_PointCloud *)pObject) );
		case DATAOBJECT_TYPE_Shapes:     return( (CWKSP_Data_Item *)m_pShapes     ->Get_Data((CSG_Shapes     *)pObject) );
		case DATAOBJECT_TYPE_Grid:       return( (CWKSP_Data_Item *)m_pGrids      ->Get_Data((CSG_Grid       *)pObject) );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Data_Item * CWKSP_Data_Manager::Add(CSG_Data_Object *pObject)
{
	if( pObject && Get_Manager(pObject->Get_ObjectType(), true) )
	{
		switch( pObject->Get_ObjectType() )
		{
		default:	return( NULL );

		case DATAOBJECT_TYPE_Table:      return( (CWKSP_Data_Item *)m_pTables     ->Add_Data((CSG_Table      *)pObject) );
		case DATAOBJECT_TYPE_TIN:        return( (CWKSP_Data_Item *)m_pTINs       ->Add_Data((CSG_TIN        *)pObject) );
		case DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Data_Item *)m_pPointClouds->Add_Data((CSG_PointCloud *)pObject) );
		case DATAOBJECT_TYPE_Shapes:     return( (CWKSP_Data_Item *)m_pShapes     ->Add_Data((CSG_Shapes     *)pObject) );
		case DATAOBJECT_TYPE_Grid:       return( (CWKSP_Data_Item *)m_pGrids      ->Add_Data((CSG_Grid       *)pObject) );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CWKSP_Layer * CWKSP_Data_Manager::Get_Layer(CSG_Data_Object *pObject)
{
	if( pObject && Get_Manager(pObject->Get_ObjectType()) )
	{
		switch( pObject->Get_ObjectType() )
		{
		default:	return( NULL );

		case DATAOBJECT_TYPE_TIN:        return( (CWKSP_Layer *)m_pTINs       ->Get_Data((CSG_TIN        *)pObject) );
		case DATAOBJECT_TYPE_PointCloud: return( (CWKSP_Layer *)m_pPointClouds->Get_Data((CSG_PointCloud *)pObject) );
		case DATAOBJECT_TYPE_Shapes:     return( (CWKSP_Layer *)m_pShapes     ->Get_Data((CSG_Shapes     *)pObject) );
		case DATAOBJECT_TYPE_Grid:       return( (CWKSP_Layer *)m_pGrids      ->Get_Data((CSG_Grid       *)pObject) );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
void CWKSP_Data_Manager::Del_Manager(CWKSP_Base_Item *pItem)
{
	if( pItem == m_pTables      )	m_pTables		= NULL;
	if( pItem == m_pTINs        )	m_pTINs			= NULL;
	if( pItem == m_pPointClouds )	m_pPointClouds	= NULL;
	if( pItem == m_pShapes      )	m_pShapes		= NULL;
	if( pItem == m_pGrids       )	m_pGrids		= NULL;

	if( Get_Count() == 0 )
	{
		m_pProject->Clr_File_Name();
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Data_Manager::Update(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	return( pItem && pItem->DataObject_Changed(pParameters) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Update_Views(CSG_Data_Object *pObject)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	return( pItem && pItem->Update_Views() );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Show(CSG_Data_Object *pObject, int Flags)
{
	CWKSP_Data_Item	*pItem	= Get(pObject);

	return( pItem && pItem->Show(Flags) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::asImage(CSG_Data_Object *pObject, CSG_Grid *pImage)
{
	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->asImage(pImage) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Get_Colors(CSG_Data_Object *pObject, CSG_Colors *pColors)
{
	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->Get_Colors(pColors) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Set_Colors(CSG_Data_Object *pObject, CSG_Colors *pColors)
{
	CWKSP_Layer	*pLayer	= Get_Layer(pObject);

	return( pLayer && pLayer->Set_Colors(pColors) );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Get_Parameters(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= pParameters ? Get(pObject) : NULL;

	return( pItem && pParameters->Assign(pItem->Get_Parameters()) > 0 );
}

//---------------------------------------------------------
bool CWKSP_Data_Manager::Set_Parameters(CSG_Data_Object *pObject, CSG_Parameters *pParameters)
{
	CWKSP_Data_Item	*pItem	= pParameters ? Get(pObject) : NULL;

	if( pItem && pItem->Get_Parameters()->Assign_Values(pParameters) > 0 )
	{
		pItem->Parameters_Changed();

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
