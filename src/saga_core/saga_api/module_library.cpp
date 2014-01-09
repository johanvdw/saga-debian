/**********************************************************
 * Version $Id: module_library.cpp 1921 2014-01-09 10:24:11Z oconrad $
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
//                  module_library.cpp                   //
//                                                       //
//          Copyright (C) 2006 by Olaf Conrad            //
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
#include <wx/dynlib.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/utils.h>

#include "saga_api.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Library::CSG_Module_Library(const CSG_String &File_Name)
{
	m_File_Name		= SG_File_Get_Path_Absolute(File_Name);
	m_Library_Name	= SG_File_Get_Name(File_Name, false);

#if !defined(_SAGA_MSW)
	if( m_Library_Name.Find(SG_T("lib")) == 0 )
	{
		m_Library_Name	= m_Library_Name.Right(m_Library_Name.Length() - 3).c_str();
	}
#endif

	//-----------------------------------------------------
	m_pLibrary	= new wxDynamicLibrary(m_File_Name.c_str(), wxDL_DEFAULT|wxDL_QUIET);

	if(	m_pLibrary->IsLoaded()
	&&	m_pLibrary->HasSymbol(SYMBOL_MLB_Get_Interface)
	&&	m_pLibrary->HasSymbol(SYMBOL_MLB_Initialize)
	&&	m_pLibrary->HasSymbol(SYMBOL_MLB_Finalize)
	&&	((TSG_PFNC_MLB_Initialize)m_pLibrary->GetSymbol(SYMBOL_MLB_Initialize))(m_File_Name) )
	{
		m_pInterface	= ((TSG_PFNC_MLB_Get_Interface)m_pLibrary->GetSymbol(SYMBOL_MLB_Get_Interface))();
	}
	else
	{
		m_pInterface	= NULL;
	}

	//-----------------------------------------------------
	if( Get_Count() <= 0 )
	{
		_Destroy();
	}
}

//---------------------------------------------------------
CSG_Module_Library::~CSG_Module_Library(void)
{
	_Destroy();
}

//---------------------------------------------------------
bool CSG_Module_Library::_Destroy(void)
{
	if( m_pLibrary )
	{
		if( m_pLibrary->IsLoaded() && m_pLibrary->HasSymbol(SYMBOL_MLB_Finalize) )
		{
			TSG_PFNC_MLB_Finalize	MLB_Finalize	= (TSG_PFNC_MLB_Finalize)m_pLibrary->GetSymbol(SYMBOL_MLB_Finalize);

			MLB_Finalize();
		}

		delete(m_pLibrary);

		m_pLibrary	= NULL;
	}

	m_pInterface	= NULL;

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const SG_Char * CSG_Module_Library::Get_Info(int Type) const
{
	if( m_pInterface != NULL )
	{
		return( m_pInterface->Get_Info(Type) );
	}

	return( SG_T("") );
}

//---------------------------------------------------------
CSG_String CSG_Module_Library::Get_Summary(int Format) const
{
	int			i;
	CSG_String	s;

	switch( Format )
	{
	//-----------------------------------------------------
	case SG_SUMMARY_FMT_FLAT: case SG_SUMMARY_FMT_FLAT_NO_INTERACTIVE:
		s	+= CSG_String::Format(SG_T("\n%s:\n"), _TL("modules"));

		for(i=0; i<Get_Count(); i++)
		{
			if( Get_Module(i) && (Format == SG_SUMMARY_FMT_FLAT || !Get_Module(i)->is_Interactive()) )
			{
				s	+= CSG_String::Format(SG_T(" %d\t- %s\n"), i, Get_Module(i)->Get_Name().c_str());
			}
		}

		break;

	//-----------------------------------------------------
	case SG_SUMMARY_FMT_HTML: default:
		s	+= CSG_String::Format(SG_T("%s: <b>%s</b><br>%s: <i>%s</i><br>%s: <i>%s</i><br>%s: <i>%s</i><hr>%s"),
				_TL("Module Library"), Get_Info(MLB_INFO_Name),
				_TL("Author"        ), Get_Info(MLB_INFO_Author),
				_TL("Version"       ), Get_Info(MLB_INFO_Version),
				_TL("File"          ), Get_File_Name().c_str(),
				Get_Info(MLB_INFO_Description)
			);

		s	+= CSG_String::Format(SG_T("<hr><b>%s:<ul>"), _TL("Modules"));

		for(i=0; i<Get_Count(); i++)
		{
			if( Get_Module(i) )
			{
				s	+= CSG_String::Format(SG_T("<li>%s</li>"), Get_Module(i)->Get_Name().c_str());
			}
		}

		s	+= SG_T("</ul>");

		s.Replace(SG_T("\n"), SG_T("<br>"));

		break;

	//-----------------------------------------------------
	case SG_SUMMARY_FMT_XML: case SG_SUMMARY_FMT_XML_NO_INTERACTIVE:
		s	+= SG_T("<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>\n");
		s	+= CSG_String::Format(SG_T("<%s>\n"), SG_XML_LIBRARY);
		s	+= CSG_String::Format(SG_T("\t<%s>%s</%s>\n"), SG_XML_LIBRARY_PATH, Get_File_Name().c_str(), SG_XML_LIBRARY_PATH);
		s	+= CSG_String::Format(SG_T("\t<%s>%s</%s>\n"), SG_XML_LIBRARY_NAME, Get_Name     ().c_str(), SG_XML_LIBRARY_NAME);

		for(i=0; i<Get_Count(); i++)
		{
			if( Get_Module(i) && (Format == SG_SUMMARY_FMT_XML || !Get_Module(i)->is_Interactive()) )
			{
				s	+= CSG_String::Format(SG_T("\t<%s %s=\"%d\" %s=\"%s\">\n"), SG_XML_MODULE,
					SG_XML_MODULE_ATT_ID, i, SG_XML_MODULE_ATT_NAME, Get_Module(i)->Get_Name().c_str()
				);
			}
		}

		s	+= CSG_String::Format(SG_T("</%s>\n"), SG_XML_LIBRARY);

		break;
	}

	return( s );
}

//---------------------------------------------------------
bool CSG_Module_Library::Get_Summary(const CSG_String &Path)	const
{
	CSG_File	f;

	if( f.Open(SG_File_Make_Path(Path, Get_Library_Name(), SG_T("html")), SG_FILE_W) )
	{
		f.Write(Get_Summary());
	}

	for(int j=0; j<Get_Count(); j++)
	{
		CSG_Module	*pModule	= Get_Module(j);

		if( pModule )
		{
			CSG_String	fName	= CSG_String::Format(SG_T("%s_%02d"), Get_Library_Name().c_str(), pModule->Get_ID());

			if( f.Open(SG_File_Make_Path(Path, fName, SG_T("html")), SG_FILE_W) )
			{
				f.Write(Get_Module(j)->Get_Summary());
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module * CSG_Module_Library::Get_Module(int Index, TSG_Module_Type Type) const
{
	CSG_Module	*pModule	= Index >= 0 && Index < Get_Count() ? m_pInterface->Get_Module(Index) : NULL;

	return(	pModule && (Type == MODULE_TYPE_Base || Type == pModule->Get_Type()) ? pModule : NULL );
}

//---------------------------------------------------------
CSG_Module * CSG_Module_Library::Get_Module(const SG_Char *Name, TSG_Module_Type Type) const
{
	for(int i=0; i<Get_Count(); i++)
	{
		CSG_Module	*pModule	= Get_Module(i, Type);

		if( pModule && !pModule->Get_Name().Cmp(Name) )
		{
			return( pModule );
		}
	}

	return( NULL );
}

//---------------------------------------------------------
CSG_Module_Grid * CSG_Module_Library::Get_Module_Grid(int Index) const
{	return( (CSG_Module_Grid *)Get_Module(Index, MODULE_TYPE_Grid) );	}

CSG_Module_Grid * CSG_Module_Library::Get_Module_Grid(const SG_Char *Name) const
{	return( (CSG_Module_Grid *)Get_Module(Name , MODULE_TYPE_Grid) );	}

//---------------------------------------------------------
CSG_Module_Interactive * CSG_Module_Library::Get_Module_Interactive(int Index) const
{	return( (CSG_Module_Interactive *)Get_Module(Index, MODULE_TYPE_Interactive) );	}

CSG_Module_Interactive * CSG_Module_Library::Get_Module_Interactive(const SG_Char *Name) const
{	return( (CSG_Module_Interactive *)Get_Module(Name , MODULE_TYPE_Interactive) );	}

//---------------------------------------------------------
CSG_Module_Grid_Interactive * CSG_Module_Library::Get_Module_Grid_Interactive(int Index) const
{	return( (CSG_Module_Grid_Interactive *)Get_Module(Index, MODULE_TYPE_Grid_Interactive) );	}

CSG_Module_Grid_Interactive * CSG_Module_Library::Get_Module_Grid_Interactive(const SG_Char *Name) const
{	return( (CSG_Module_Grid_Interactive *)Get_Module(Name , MODULE_TYPE_Grid_Interactive) );	}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_Module_Library::Get_Menu(int i) const
{
	CSG_String	Menu;

	if( Get_Module(i) )
	{
		Menu	= Get_Module(i)->Get_MenuPath();

		if( Menu.Length() > 2 && Menu[1] == ':' )
		{
			if( Menu[0] == 'A' || Menu[0] == 'a' )
			{	// menu path is absolute, i.e. relative to top menu...
				return( Menu.Right(2) + '|' + Get_Info(MLB_INFO_Name) );
			}

			Menu	= Menu.Right(2);	// menu path is relative to library menu...
		}

		CSG_String	Root(Get_Info(MLB_INFO_Menu_Path));

		if( Menu.is_Empty() )
		{
			Menu	= Root;
		}
		else if( !Root.is_Empty() )
		{
			Menu	= Root + '|' + Menu;
		}

		if( Menu.Length() > 0 )
		{
			Menu	+= '|';
		}

		Menu	+= Get_Info(MLB_INFO_Name);
	}

	return( Menu );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Library_Manager		g_Module_Library_Manager;

//---------------------------------------------------------
CSG_Module_Library_Manager &	SG_Get_Module_Library_Manager	(void)
{
	return( g_Module_Library_Manager );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Library_Manager::CSG_Module_Library_Manager(void)
{
	m_pLibraries	= NULL;
	m_nLibraries	= 0;
}

//---------------------------------------------------------
CSG_Module_Library_Manager::~CSG_Module_Library_Manager(void)
{
	Destroy();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Library * CSG_Module_Library_Manager::Add_Library(const SG_Char *File_Name)
{
	//-----------------------------------------------------
	if( !SG_File_Cmp_Extension(File_Name, SG_T("mlb"  ))
	&&	!SG_File_Cmp_Extension(File_Name, SG_T("dll"  ))
	&&	!SG_File_Cmp_Extension(File_Name, SG_T("so"   ))
	&&	!SG_File_Cmp_Extension(File_Name, SG_T("dylib")) )
	{
		return( NULL );
	}

	SG_UI_Msg_Add(CSG_String::Format(SG_T("%s: %s..."), _TL("Load library"), File_Name), true);

	//-----------------------------------------------------
	for(int i=0; i<Get_Count(); i++)
	{
		if( SG_STR_CMP(File_Name, Get_Library(i)->Get_File_Name()) == 0 )
		{
			SG_UI_Msg_Add(_TL("has already been loaded"), false);

			return( NULL );
		}
	}

	//-----------------------------------------------------
	CSG_Module_Library	*pLibrary	= new CSG_Module_Library(File_Name);

	if( pLibrary->is_Valid() )
	{
		m_pLibraries	= (CSG_Module_Library **)SG_Realloc(m_pLibraries, (m_nLibraries + 1) * sizeof(CSG_Module_Library *));
		m_pLibraries[m_nLibraries++]	= pLibrary;

		SG_UI_Msg_Add(_TL("okay"), false, SG_UI_MSG_STYLE_SUCCESS);

		return( pLibrary );
	}

	delete(pLibrary);

	SG_UI_Msg_Add(_TL("failed"), false, SG_UI_MSG_STYLE_FAILURE);

	return( NULL );
}

//---------------------------------------------------------
int CSG_Module_Library_Manager::Add_Directory(const SG_Char *Directory, bool bOnlySubDirectories)
{
	int			nOpened	= 0;
	wxDir		Dir;
	wxString	File_Name;

	if( Dir.Open(Directory) )
	{
		if( !bOnlySubDirectories && Dir.GetFirst(&File_Name, wxEmptyString, wxDIR_FILES) )
		{
			do
			{	if( File_Name.Find(wxT("saga_")) < 0 && File_Name.Find(wxT("wx")) < 0 )
				if( Add_Library(SG_File_Make_Path(Dir.GetName(), File_Name, NULL)) )
				{
					nOpened++;
				}
			}
			while( Dir.GetNext(&File_Name) );
		}

		if( Dir.GetFirst(&File_Name, wxEmptyString, wxDIR_DIRS) )
		{
			do
			{
				if( File_Name.CmpNoCase(wxT("dll")) )
				{
					nOpened	+= Add_Directory(SG_File_Make_Path(Dir.GetName(), File_Name, NULL), false);
				}
			}
			while( Dir.GetNext(&File_Name) );
		}
	}

	return( nOpened );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Module_Library_Manager::Destroy(void)
{
	if( m_pLibraries )
	{
		for(int i=0; i<Get_Count(); i++)
		{
//			#ifndef _SAGA_MSW
			if( !SG_UI_Get_Window_Main() )
			{
				m_pLibraries[i]->m_pLibrary->Detach();
			}
//			#endif

			delete(m_pLibraries[i]);
		}

		SG_Free(m_pLibraries);

		m_pLibraries	= NULL;
		m_nLibraries	= 0;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

bool CSG_Module_Library_Manager::Del_Library(CSG_Module_Library *pLibrary)
{
	for(int i=0; i<Get_Count(); i++)
	{
		if( pLibrary == Get_Library(i) )
		{
			return( Del_Library(i) );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Module_Library_Manager::Del_Library(int i)
{
	if( i >= 0 && i < Get_Count() )
	{
		delete(m_pLibraries[i]);

		for(m_nLibraries--; i<m_nLibraries; i++)
		{
			m_pLibraries[i]	= m_pLibraries[i + 1];
		}

		m_pLibraries	= (CSG_Module_Library **)SG_Realloc(m_pLibraries, m_nLibraries * sizeof(CSG_Module_Library *));

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module_Library * CSG_Module_Library_Manager::Get_Library(const SG_Char *Name, bool bLibrary) const
{
	for(int i=0; i<Get_Count(); i++)
	{
		CSG_Module_Library	*pLibrary	= Get_Library(i);

		if( pLibrary && !SG_STR_CMP(Name, bLibrary ? pLibrary->Get_Library_Name() : pLibrary->Get_Name()) )
		{
			return( pLibrary );
		}
	}

	return( NULL );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module * CSG_Module_Library_Manager::Get_Module(const SG_Char *Library, int Module)	const
{
	CSG_Module_Library	*pLibrary	= Get_Library(Library, true);

	return( pLibrary ? pLibrary->Get_Module(Module) : NULL );
}

//---------------------------------------------------------
CSG_Module * CSG_Module_Library_Manager::Get_Module(const SG_Char *Library, const SG_Char *Module)	const
{
	CSG_Module_Library	*pLibrary	= Get_Library(Library, true);

	return( pLibrary ? pLibrary->Get_Module(Module) : NULL );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SUMMARY_HTML_ADD_INT(label, value)	s += CSG_String::Format(SG_T("<tr><td valign=\"top\">%s</td><td valign=\"top\">%d</td></tr>"), label, value)

//---------------------------------------------------------
CSG_String CSG_Module_Library_Manager::Get_Summary(int Format)	const
{
	//-----------------------------------------------------
	int			i, nModules;

	for(i=0, nModules=0; i<Get_Count(); i++)
	{
		nModules	+= Get_Library(i)->Get_Count();
	}

	//-----------------------------------------------------
	CSG_String	s;

	switch( Format )
	{
	//-----------------------------------------------------
	case SG_SUMMARY_FMT_FLAT: case SG_SUMMARY_FMT_FLAT_NO_INTERACTIVE:
		s	+= CSG_String::Format(SG_T("\n%d %s (%d %s):\n"), Get_Count(), _TL("loaded module libraries"), nModules, _TL("modules"));

		for(i=0; i<Get_Count(); i++)
		{
			s	+= CSG_String::Format(SG_T("- %s\n"), Get_Library(i)->Get_Library_Name().c_str());
		}

		break;

	//-----------------------------------------------------
	case SG_SUMMARY_FMT_HTML: default:
		s	+= CSG_String::Format(SG_T("<b>%s</b>"), _TL("Module Libraries"));

		s	+= SG_T("<table border=\"0\">");
		SUMMARY_HTML_ADD_INT(_TL("Available Libraries"), Get_Count());
		SUMMARY_HTML_ADD_INT(_TL("Available Modules"  ), nModules);
		s	+= SG_T("</table>");

		s	+= CSG_String::Format(SG_T("<hr><b>%s:</b><table border=\"1\">"), _TL("Module Libraries"));

		s	+= CSG_String::Format(SG_T("<tr><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>"),
				_TL("Library"),
				_TL("Modules"),
				_TL("Name"),
				_TL("Location")
			);

		for(i=0; i<Get_Count(); i++)
		{
			s	+= CSG_String::Format(SG_T("<tr><td>%s</td><td>%d</td><td>%s</td><td>%s</td></tr>"),
					SG_File_Get_Name(Get_Library(i)->Get_File_Name(), false).c_str(),
					Get_Library(i)->Get_Count(),
					Get_Library(i)->Get_Name().c_str(),
					SG_File_Get_Path(Get_Library(i)->Get_File_Name()).c_str()
				);
		}

		s	+= SG_T("</table>");

		break;

	//-----------------------------------------------------
	case SG_SUMMARY_FMT_XML: case SG_SUMMARY_FMT_XML_NO_INTERACTIVE:
		s	+= SG_T("<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>\n");
		s	+= CSG_String::Format(SG_T("\n<%s>"), SG_XML_SYSTEM);
		s	+= CSG_String::Format(SG_T("\n<%s>%s</%s>"), SG_XML_SYSTEM_VER, SAGA_VERSION, SG_XML_SYSTEM_VER);

		for(int i=0; i<SG_Get_Module_Library_Manager().Get_Count(); i++)
		{
			s	+= CSG_String::Format(SG_T("\n\t<%s %s=\"%s\"/>"), SG_XML_LIBRARY, SG_XML_LIBRARY_NAME,
				SG_Get_Module_Library_Manager().Get_Library(i)->Get_Library_Name().c_str()
			);
		}

		s	+= CSG_String::Format(SG_T("\n</%s>"), SG_XML_SYSTEM);

		break;
	}

	//-----------------------------------------------------
	return( s );
}

//---------------------------------------------------------
bool CSG_Module_Library_Manager::Get_Summary(const CSG_String &Path)	const
{
	for(int i=0; i<Get_Count(); i++)
	{
		CSG_Module_Library	*pLibrary	= Get_Library(i);

		CSG_String	Directory	= SG_File_Make_Path(Path, pLibrary->Get_Library_Name());

		if( SG_Dir_Create(Directory) )
		{
			pLibrary->Get_Summary(Directory);
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
