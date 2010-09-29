
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
//                   WKSP_Module.cpp                     //
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
#include <wx/utils.h>

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"

#include "active.h"
#include "active_parameters.h"

#include "wksp_data_manager.h"

#include "wksp_module_manager.h"
#include "wksp_module_library.h"
#include "wksp_module_menu.h"
#include "wksp_module.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Module	*g_pModule	= NULL;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Module::CWKSP_Module(CSG_Module *pModule, const wxChar *Menu_Path_default)
{
	m_pModule		= pModule;

	m_Menu_ID		= -1;

	//-----------------------------------------------------
	bool		bLibrary, bModule;
	const wxChar	*sModule	= m_pModule->Get_MenuPath();

	m_Menu_Path.Empty();

	if( sModule && *sModule && *(sModule + 1) == wxT(':') )
	{
		if( *sModule == wxT('A') || *sModule == wxT('a') )
		{
			sModule	+= 2;

			if( *sModule )
			{
				m_Menu_Path.Printf(wxT("%s"), sModule);
			}
		}
		else
		{
			sModule	+= 2;
		}
	}

	if( m_Menu_Path.Length() == 0 )	// Menu path is relative to default menu path...
	{
		bLibrary	= Menu_Path_default	&& *Menu_Path_default;
		bModule		= sModule			&& *sModule;

		if( bLibrary && bModule )
		{
			m_Menu_Path.Printf(wxT("%s|%s"), Menu_Path_default, sModule);
		}
		else if( bLibrary )
		{
			m_Menu_Path.Printf(wxT("%s"), Menu_Path_default);
		}
		else if( bModule )
		{
			m_Menu_Path.Printf(wxT("%s"), sModule);
		}
	}
}

//---------------------------------------------------------
CWKSP_Module::~CWKSP_Module(void)
{
	if( g_pModule == this )
	{
		if( g_pModule->is_Executing() )
		{
			PROCESS_Set_Okay(false);
		}

		if( m_pModule->is_Interactive() )
		{
			((CSG_Module_Interactive *)m_pModule)->Execute_Finish();
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Module::Get_Name(void)
{
	return( m_pModule->is_Interactive()
		? wxString::Format(wxT("%s [%s]"), m_pModule->Get_Name(), LNG("interactive"))
		: wxString::Format(wxT("%s")     , m_pModule->Get_Name())
	);
}

//---------------------------------------------------------
void CWKSP_Module::Set_File_Name(const wxString &File_Name)
{
	m_File_Name = File_Name;
}

//---------------------------------------------------------
wxString CWKSP_Module::Get_Description(void)
{
	bool		bFirst, bOptionals	= false;
	int			i;
	CSG_Parameter	*pParameter;
	wxString	s, sTmp;

	//-----------------------------------------------------
	s.Append(wxString::Format(wxT("%s: <b>%s</b>")	, LNG("Module"), m_pModule->Get_Name()));

	if( m_pModule->is_Interactive() )
	{
		s.Append(wxString::Format(wxT("<br>- %s -")	, LNG("interactive execution")));
	}

	if( m_pModule->Get_Author() && *(m_pModule->Get_Author()) )
	{
		s.Append(wxString::Format(wxT("<br>%s")		, m_pModule->Get_Author()));
	}

	if( m_Menu_Path.Length() > 0 )
	{
		sTmp	= m_Menu_Path;
		sTmp.Replace(wxT("|"), wxT(" <b>></b> "));
		s.Append(wxString::Format(wxT("<br>%s: <i>%s</i>"), LNG("Menu"), sTmp.c_str()));
	}

	if( m_pModule->Get_Description() && *(m_pModule->Get_Description()) )
	{
		s.Append(wxString::Format(wxT("<hr><b>%s</b><br>%s"), LNG("Description"), m_pModule->Get_Description()));
	}


	//-----------------------------------------------------
	s.Append(wxString::Format(wxT("<hr><b>%s</b><br>"), LNG("Parameters")));
	s.Append(wxString::Format(wxT("<table border=\"1\" width=\"100%%\" valign=\"top\" cellpadding=\"5\" rules=\"all\"><tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>\n"),
		LNG("Name"), LNG("Type"), LNG("Identifier"), LNG("Description"), LNG("Constraints")
	));

	for(i=0, bFirst=true; i<m_pModule->Get_Parameters()->Get_Count(); i++)
	{
		pParameter	= m_pModule->Get_Parameters()->Get_Parameter(i);

		if( pParameter->is_Input() )
		{
			if( bFirst )
			{
				bFirst	= false;
				s.Append(wxString::Format(wxT("<tr><th colspan=\"5\">%s</th></tr>"), LNG("Input")));
			}

			s.Append(wxString::Format(wxT("<tr><td>%s%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>"),
				pParameter->Get_Name(),
				pParameter->is_Optional() ? wxT(" (*)") : wxT(" "),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_TYPE).c_str(),
				pParameter->Get_Identifier(),
				pParameter->Get_Description(),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_PROPERTIES).c_str()
			));
		}
	}

	for(i=0, bFirst=true; i<m_pModule->Get_Parameters()->Get_Count(); i++)
	{
		pParameter	= m_pModule->Get_Parameters()->Get_Parameter(i);

		if( pParameter->is_Output() )
		{
			if( bFirst )
			{
				bFirst	= false;
				s.Append(wxString::Format(wxT("<tr><th colspan=\"5\">%s</th></tr>"), LNG("Output")));
			}

			s.Append(wxString::Format(wxT("<tr><td>%s%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>"),
				pParameter->Get_Name(),
				pParameter->is_Optional() ? wxT(" (*)") : wxT(""),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_TYPE).c_str(),
				pParameter->Get_Identifier(),
				pParameter->Get_Description(),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_PROPERTIES).c_str()
			));
		}
	}

	for(i=0, bFirst=true; i<m_pModule->Get_Parameters()->Get_Count(); i++)
	{
		pParameter	= m_pModule->Get_Parameters()->Get_Parameter(i);

		if( pParameter->is_Option() && pParameter->Get_Type() != PARAMETER_TYPE_Grid_System )
		{
			if( bFirst )
			{
				bFirst	= false;
				s.Append(wxString::Format(wxT("<tr><th colspan=\"5\">%s</th></tr>"), LNG("Options")));
			}

			s.Append(wxString::Format(wxT("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>"),
				pParameter->Get_Name(),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_TYPE).c_str(),
				pParameter->Get_Identifier(),
				pParameter->Get_Description(),
				pParameter->Get_Description(PARAMETER_DESCRIPTION_PROPERTIES).c_str()
			));
		}
		else if( pParameter->is_Optional() )
		{
			bOptionals	= true;
		}
	}

	s.Append(wxT("</table>"));

	if( bOptionals )
	{
		s.Append(wxString::Format(wxT("(*) <i>%s</i>"), LNG("optional")));
	}

	s.Replace(wxT("\n"), wxT("<br>"));

	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Module::Get_Menu(void)
{
	wxMenu	*pMenu;

	pMenu	= new wxMenu(Get_Name());

	pMenu->AppendCheckItem(Get_Menu_ID(), LNG("[CMD] Execute"), LNG("[HLP] Execute Module"));

	pMenu->AppendSeparator();

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_MODULES_SAVE_SCRIPT);

	return( pMenu );
}

//---------------------------------------------------------
CSG_Parameters * CWKSP_Module::Get_Parameters(void)
{
	return( m_pModule->Get_Parameters() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Module::On_Command(int Cmd_ID)
{
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Base_Item::On_Command(Cmd_ID) );

	case ID_CMD_WKSP_ITEM_RETURN:
		Execute(true);
		break;

	case ID_CMD_MODULES_SAVE_SCRIPT:
		_Save_Script();
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
void CWKSP_Module::_Save_Script(void)
{
	wxString	FileName;

	if( DLG_Save(FileName, LNG("[CAP] Create Script Command File"), SG_T("DOS Batch Script (*.bat)|*.bat|Python Script (*.py)|*.py")) )
	{
		CSG_File	File;
		CSG_String	Command;

		if(      SG_File_Cmp_Extension(FileName.c_str(), SG_T("bat")) )
		{
			Command	+= SG_T("@ECHO OFF\n\n");

			Command	+= SG_T("REM SET SAGA_MLB = C:\\SAGA\\Modules\n");
			Command	+= SG_T("REM SET PATH = %PATH%;C:\\SAGA\n\n");

			Command	+= SG_T("saga_cmd ");

			Command	+= SG_File_Get_Name(((CWKSP_Module_Library *)Get_Manager())->Get_File_Name(), false);

			Command	+= SG_T(" \"");
			Command	+= m_pModule->Get_Name();
			Command	+= SG_T("\"");

			_Save_Script_CMD(Command, m_pModule->Get_Parameters());

			for(int i=0; i<m_pModule->Get_Parameters_Count(); i++)
			{
				_Save_Script_CMD(Command, m_pModule->Get_Parameters(i));
			}

			Command	+= SG_T("\n\nPAUSE\n");
		}
		else if( SG_File_Cmp_Extension(FileName.c_str(), SG_T("py" )) )
		{
			Command	+= SG_T("# Python script template for SAGA module execution (automatically created, experimental)\n\n");
			Command	+= SG_T("import saga_api, sys, os\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("def Call_SAGA_Module(in__grid, out_grid, in__shapes, out_shapes):\n");
			Command	+= SG_T("    print saga_api.SAGA_API_Get_Version()\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("    Library = saga_api.CSG_Module_Library()\n");
			Command	+= SG_T("    if Library.Create(saga_api.CSG_String('");
			Command	+= ((CWKSP_Module_Library *)Get_Manager())->Get_File_Name();
			Command	+= SG_T("')) == 0:\n");
			Command	+= SG_T("        print 'unable to load SAGA module library'\n");
			Command	+= SG_T("        return 0\n");
			Command	+= SG_T("\n");

			switch( m_pModule->Get_Type() )
			{
			default:
				Command	+= CSG_String::Format(SG_T("    Module = Library.Get_Module('%s')\n")		, m_pModule->Get_Name());
				break;

			case MODULE_TYPE_Grid:
				Command	+= CSG_String::Format(SG_T("    Module = Library.Get_Module_Grid('%s')\n")	, m_pModule->Get_Name());
				Command	+= SG_T("    Module.Get_System().Assign(in__grid.Get_System())\n");
				break;
			}

			Command	+= SG_T("\n");
			Command	+= SG_T("    Parms = Module.Get_Parameters() # default parameter list\n");
			_Save_Script_Python(Command, m_pModule->Get_Parameters());

			for(int i=0; i<m_pModule->Get_Parameters_Count(); i++)
			{
				Command	+= SG_T("\n");
				Command	+= CSG_String::Format(SG_T("    Parms = Module.Get_Parameters(%d) # additional parameter list\n"), i);
				_Save_Script_Python(Command, m_pModule->Get_Parameters(i));
			}

			Command	+= SG_T("\n");
			Command	+= SG_T("    if Module.Execute() == 0:\n");
			Command	+= SG_T("        print 'module execution failed'\n");
			Command	+= SG_T("        return 0\n");
			Command	+= SG_T("    print 'module successfully executed'\n");
			Command	+= SG_T("    return 1\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("if __name__ == '__main__':\n");
			Command	+= SG_T("    if len( sys.argv ) != 4:\n");
			Command	+= SG_T("        print 'Usage: this_script.py <in: gridfile> <out: gridfile> <in: shapefile> <out: shapefile>'\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("    else:\n");
			Command	+= SG_T("        in__grid    = saga_api.SG_Create_Grid(saga_api.CSG_String(sys.argv[1]))\n");
			Command	+= SG_T("        out_grid    = saga_api.SG_Create_Grid(grid_in.Get_System())\n");
			Command	+= SG_T("        in__shapes  = saga_api.SG_Create_Shapes(saga_api.CSG_String(sys.argv[3]))\n");
			Command	+= SG_T("        out_shapes  = saga_api.SG_Create_Shapes()\n");
			Command	+= SG_T("\n");
			Command	+= SG_T("        if Call_SAGA_Module(in__grid, out_grid, in__shapes, out_shapes) != 0:\n");
			Command	+= SG_T("            grid_out  .Save(saga_api.CSG_String(sys.argv[2]))\n");
			Command	+= SG_T("            shapes_out.Save(saga_api.CSG_String(sys.argv[4]))\n");
		}

		if( File.Open(FileName.c_str(), SG_FILE_W, false) && Command.Length() > 0 )
		{
			File.Write(Command);
		}
	}
}

//---------------------------------------------------------
#define GET_ID1(p)		(p->Get_Owner()->Get_Identifier() && *(p->Get_Owner()->Get_Identifier()) \
						? wxString::Format(wxT("%s_%s"), p->Get_Owner()->Get_Identifier(), p->Get_Identifier()) \
						: wxString::Format(p->Get_Identifier())).c_str()

#define GET_ID2(p, s)	wxString::Format(wxT("%s_%s"), GET_ID1(p), s).c_str()

//---------------------------------------------------------
void CWKSP_Module::_Save_Script_CMD(CSG_String &Command, CSG_Parameters *pParameters)
{
	for(int iParameter=0; iParameter<pParameters->Get_Count(); iParameter++)
	{
		CSG_Parameter	*p	= pParameters->Get_Parameter(iParameter);

		switch( p->Get_Type() )
		{
		default:
			break;

		case PARAMETER_TYPE_Bool:
			if( p->asBool() )
				Command	+= CSG_String::Format(SG_T(" -%s"), GET_ID1(p));
			break;

		case PARAMETER_TYPE_Int:
		case PARAMETER_TYPE_Choice:
		case PARAMETER_TYPE_Table_Field:
			Command	+= CSG_String::Format(SG_T(" -%s=%d"), GET_ID1(p), p->asInt());
			break;

		case PARAMETER_TYPE_Double:
		case PARAMETER_TYPE_Degree:
			Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID1(p), p->asDouble());
			break;

		case PARAMETER_TYPE_Range:
			Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID2(p, "MIN"), p->asRange()->Get_LoVal());
			Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID2(p, "MAX"), p->asRange()->Get_HiVal());
			break;

		case PARAMETER_TYPE_String:
		case PARAMETER_TYPE_Text:
		case PARAMETER_TYPE_FilePath:
			Command	+= CSG_String::Format(SG_T(" -%s=%s"), GET_ID1(p), p->asString());
			break;

		case PARAMETER_TYPE_FixedTable:
			Command	+= CSG_String::Format(SG_T(" -%s=%s"), GET_ID1(p), p->asString());
			break;

		case PARAMETER_TYPE_Grid_System:
			if( p->Get_Children_Count() == 0 )
			{
				Command	+= CSG_String::Format(SG_T(" -%s=%d"), GET_ID2(p, "NX"), p->asGrid_System()->Get_NX());
				Command	+= CSG_String::Format(SG_T(" -%s=%d"), GET_ID2(p, "NY"), p->asGrid_System()->Get_NY());
				Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID2(p,  "X"), p->asGrid_System()->Get_XMin());
				Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID2(p,  "Y"), p->asGrid_System()->Get_YMin());
				Command	+= CSG_String::Format(SG_T(" -%s=%f"), GET_ID2(p,  "D"), p->asGrid_System()->Get_Cellsize());
			}
			break;

		case PARAMETER_TYPE_DataObject_Output:
		case PARAMETER_TYPE_Grid:
		case PARAMETER_TYPE_Table:
		case PARAMETER_TYPE_Shapes:
		case PARAMETER_TYPE_TIN:
			Command	+= CSG_String::Format(SG_T(" -%s=%s"), GET_ID1(p), p->asDataObject() && p->asDataObject()->Get_File_Name() ? p->asDataObject()->Get_File_Name() : SG_T("NULL"));
			break;

		case PARAMETER_TYPE_Grid_List:
		case PARAMETER_TYPE_Table_List:
		case PARAMETER_TYPE_Shapes_List:
		case PARAMETER_TYPE_TIN_List:
		case PARAMETER_TYPE_PointCloud_List:
			if( p->is_Input() )
			{
				Command	+= CSG_String::Format(SG_T(" -%s="), GET_ID1(p));

				if( p->asList()->Get_Count() == 0 )
				{
					Command	+= SG_T("NULL");
				}
				else
				{
					Command	+= p->asList()->asDataObject(0)->Get_File_Name(true);

					for(int iObject=1; iObject<p->asList()->Get_Count(); iObject++)
					{
						Command	+= SG_T(";");
						Command	+= p->asList()->asDataObject(iObject)->Get_File_Name(true);
					}
				}
			}
			break;
		}
	}
}

//---------------------------------------------------------
void CWKSP_Module::_Save_Script_Python(CSG_String &Command, CSG_Parameters *pParameters)
{
	for(int iParameter=0; iParameter<pParameters->Get_Count(); iParameter++)
	{
		CSG_Parameter	*p	= pParameters->Get_Parameter(iParameter);

		switch( p->Get_Type() )
		{
		default:
			break;

		case PARAMETER_TYPE_Bool:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(%d)\n"), p->Get_Identifier(), p->asBool() ? 1 : 0);
			break;

		case PARAMETER_TYPE_Int:
		case PARAMETER_TYPE_Choice:
		case PARAMETER_TYPE_Table_Field:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(%d)\n"), p->Get_Identifier(), p->asInt());
			break;

		case PARAMETER_TYPE_Double:
		case PARAMETER_TYPE_Degree:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(%f)\n"), p->Get_Identifier(), p->asDouble());
			break;

		case PARAMETER_TYPE_Range:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').asRange().Set_LoVal(%f)\n"), p->Get_Identifier(), p->asRange()->Get_LoVal());
			Command	+= CSG_String::Format(SG_T("    Parms('%s').asRange().Set_HiVal(%f)\n"), p->Get_Identifier(), p->asRange()->Get_HiVal());
			break;

		case PARAMETER_TYPE_String:
		case PARAMETER_TYPE_Text:
		case PARAMETER_TYPE_FilePath:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(%s)\n"), p->Get_Identifier(), p->asString());
			break;

		case PARAMETER_TYPE_FixedTable:
			Command	+= CSG_String::Format(SG_T("#   Parms('%s').Set_Value(saga_api.SG_Create_Table('table.txt'))\n"), p->Get_Identifier());
			break;

		case PARAMETER_TYPE_Grid_System:
			if( p->Get_Children_Count() == 0 )
			{
				Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(saga_api.CSG_Grid_System(%f, %f, %f, %d, %d))\n"), p->Get_Identifier(),
					p->asGrid_System()->Get_Cellsize(),
					p->asGrid_System()->Get_XMin()	, p->asGrid_System()->Get_YMin(),
					p->asGrid_System()->Get_NX()	, p->asGrid_System()->Get_NY());
			}
			break;

		case PARAMETER_TYPE_Grid:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # %s %s grid\n"), p->Get_Identifier(),
				p->is_Input()    ? SG_T("input")    : SG_T("output"), p->is_Optional() ? SG_T("optional") : SG_T("NOT optional")
			);
			break;

		case PARAMETER_TYPE_Table:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # %s %s table\n"), p->Get_Identifier(),
				p->is_Input()    ? SG_T("input")    : SG_T("output"), p->is_Optional() ? SG_T("optional") : SG_T("NOT optional")
			);
			break;

		case PARAMETER_TYPE_Shapes:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # %s %s shapes\n"), p->Get_Identifier(),
				p->is_Input()    ? SG_T("input")    : SG_T("output"), p->is_Optional() ? SG_T("optional") : SG_T("NOT optional")
			);
			break;

		case PARAMETER_TYPE_TIN:
			Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # %s %s TIN\n"), p->Get_Identifier(),
				p->is_Input()    ? SG_T("input")    : SG_T("output"), p->is_Optional() ? SG_T("optional") : SG_T("NOT optional")
			);
			break;

		case PARAMETER_TYPE_Grid_List:
		case PARAMETER_TYPE_Table_List:
		case PARAMETER_TYPE_Shapes_List:
		case PARAMETER_TYPE_TIN_List:
		case PARAMETER_TYPE_PointCloud_List:
			if( p->is_Input() )
			{
				if( !p->is_Optional() )
					Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # data object list\n"), p->Get_Identifier());
				else
					Command	+= CSG_String::Format(SG_T("    Parms('%s').Set_Value(0) # optional data object list\n"), p->Get_Identifier());
			}
			break;
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Module::Set_Menu_ID(int aMenu_ID)
{
	m_Menu_ID	= aMenu_ID;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Module::is_Interactive(void)
{
	return( m_pModule->is_Interactive() );
}

//---------------------------------------------------------
bool CWKSP_Module::is_Executing(void)
{
	return( m_pModule->is_Executing() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define MSG_ADD(s)			MSG_General_Add  (s, true, true);\
							MSG_Execution_Add(s, true, true);

#define MSG_ADD2(b, s1, s2)	MSG_General_Add  (b ? s1 : s2, true, true, b ? SG_UI_MSG_STYLE_SUCCESS : SG_UI_MSG_STYLE_FAILURE);\
							MSG_Execution_Add(b ? s1 : s2, true, true, b ? SG_UI_MSG_STYLE_SUCCESS : SG_UI_MSG_STYLE_FAILURE);

//---------------------------------------------------------
bool CWKSP_Module::Execute(bool bDialog)
{
	bool	bResult	= false;

	//-----------------------------------------------------
	if( g_pModule )
 	{
		if( g_pModule == this )
		{
			if( g_pModule->is_Executing() )
			{
				if( DLG_Message_Confirm(LNG("[MSG] Shall execution be stopped?"), LNG("[CAP] Module Execution")) )
				{
					PROCESS_Set_Okay(false);
				}
			}
			else if( m_pModule->is_Interactive() )
			{
				bResult		= ((CSG_Module_Interactive *)m_pModule)->Execute_Finish();
				g_pModule	= NULL;

				PROCESS_Set_Okay(true);

				MSG_ADD2(bResult,
					LNG("[MSG] Interactive module execution has been stopped"),
					LNG("[MSG] Interactive module execution failed")
				);
			}
		}
		else
		{
			DLG_Message_Show(LNG("[ERR] Can't execute a module while another runs"), LNG("[CAP] Module Execution"));
		}
	}

	//-----------------------------------------------------
	else
	{
		g_pModule	= this;

		if( m_pModule->On_Before_Execution() && (!bDialog || DLG_Parameters(m_pModule->Get_Parameters())) )
		{
			g_pModules->Get_Modules_Menu()->Set_Recent(this);

			g_pData->Check_Parameters(m_pModule->Get_Parameters());

			for(int i=0; i<m_pModule->Get_Parameters_Count(); i++)
			{
				g_pData->Check_Parameters(m_pModule->Get_Parameters(i));
			}

			MSG_General_Add_Line();
			MSG_Execution_Add_Line();
			MSG_ADD(wxString::Format(wxT("%s: %s"), LNG("[MSG] Executing module"), m_pModule->Get_Name()));

			STATUSBAR_Set_Text(m_pModule->Get_Name());

			bResult		= m_pModule->Execute();

			g_pACTIVE->Get_Parameters()->Update_Parameters(m_pModule->Get_Parameters(), false);

			if( m_pModule->is_Interactive() )
			{
				MSG_ADD2(bResult,
					LNG("[MSG] Interactive module execution has been started"),
					LNG("[MSG] Interactive module execution failed")
				);
			}
			else
			{
				MSG_ADD2(bResult,
					LNG("[MSG] Module execution succeeded"),
					LNG("[MSG] Module execution failed")
				);
			}

			if( g_pModules && g_pModules->Do_Beep() )	{	Do_Beep();	}
		}

		if( !m_pModule->is_Interactive() || !bResult )
		{
			g_pModule	= NULL;
		}
	}

	//-----------------------------------------------------
	return( bResult );
}

//---------------------------------------------------------
bool CWKSP_Module::Execute(CSG_Point ptWorld, TSG_Module_Interactive_Mode Mode, int Keys)
{
	bool	bResult	= false;

	if( g_pModule == this && m_pModule->is_Interactive() && !m_pModule->is_Executing() )
	{
		bResult	= ((CSG_Module_Interactive *)m_pModule)->Execute_Position(ptWorld, Mode, Keys);

		PROCESS_Set_Okay();
	}

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
