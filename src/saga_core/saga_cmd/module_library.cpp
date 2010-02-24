 
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                Command Line Interface                 //
//                                                       //
//                   Program: SAGA_CMD                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                  module_library.cpp                   //
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
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/cmdline.h>

#include <saga_api/saga_api.h>

#include "callback.h"

#include "module_library.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define GET_ID1(p)		(p->Get_Owner()->Get_Identifier() && *(p->Get_Owner()->Get_Identifier()) \
						? wxString::Format(wxT("%s_%s"), p->Get_Owner()->Get_Identifier(), p->Get_Identifier()) \
						: wxString::Format(p->Get_Identifier()))

#define GET_ID2(p, s)	wxString::Format(wxT("%s_%s"), GET_ID1(p).c_str(), s)


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CModule_Library::CModule_Library(void)
{
	m_nModules	= 0;
	m_Modules	= NULL;
	m_pSelected	= NULL;
	m_pCMD		= NULL;
}

//---------------------------------------------------------
CModule_Library::~CModule_Library(void)
{
	Destroy();
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CModule_Library::Create(const SG_Char *FileName, const SG_Char *FilePath)
{
	TSG_PFNC_MLB_Initialize		MLB_Initialize;
	TSG_PFNC_MLB_Get_Interface	MLB_Get_Interface;

	CSG_Module_Library_Interface	*pInterface;
	CSG_Module						*pModule;

	//-----------------------------------------------------
	Destroy();

	m_FileName	= SG_File_Make_Path(FilePath, FileName, NULL).c_str();

	m_Library.Load(m_FileName);

	//-----------------------------------------------------
	if( !m_Library.IsLoaded() )
	{
		Print_Error(LNG("[ERR] Library could not be loaded"), FileName);
	}
	else
	{
		MLB_Initialize		= (TSG_PFNC_MLB_Initialize)		m_Library.GetSymbol(SYMBOL_MLB_Initialize);
		MLB_Get_Interface	= (TSG_PFNC_MLB_Get_Interface)	m_Library.GetSymbol(SYMBOL_MLB_Get_Interface);

		if(	!MLB_Get_Interface	|| !(pInterface = MLB_Get_Interface())
		||	!MLB_Initialize		|| !MLB_Initialize(m_FileName) )
		{
			Print_Error(LNG("[ERR] Invalid library"), FileName);
		}
		else
		{
			while( (pModule = pInterface->Get_Module(m_nModules)) != NULL )
			{
				m_Modules	= (CSG_Module **)SG_Realloc(m_Modules, (m_nModules + 1) * sizeof(CSG_Module *));
				m_Modules[m_nModules++]	= pModule;
			}

			if( m_nModules > 0 )
			{
				return( true );
			}

			Print_Error(LNG("[ERR] Library does not contain any modules"), FileName);
		}
	}

	return( false );
}

//---------------------------------------------------------
void CModule_Library::Destroy(void)
{
	if( m_nModules > 0 )
	{
		SG_Free(m_Modules);
		m_nModules	= 0;
		m_Modules	= NULL;
		m_pSelected	= NULL;
	}

	if( m_pCMD )
	{
		delete(m_pCMD);
		m_pCMD		= NULL;
	}

	if( m_Library.IsLoaded() )
	{
		m_Library.Unload();
	}
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Module * CModule_Library::Select(const SG_Char *ModuleName)
{
	int			i;
	wxString	Description;

	//-----------------------------------------------------
	if( m_pCMD )
	{
		delete(m_pCMD);
		m_pCMD	= NULL;
	}

	for(i=0, m_pSelected=NULL; i<m_nModules && !m_pSelected; i++)
	{
		if( !SG_STR_CMP(ModuleName, Get_Module(i)->Get_Name()) )
		{
			m_pSelected	= Get_Module(i);
		}
	}

	if( !m_pSelected )
	{
		long		l;
		wxString	s(ModuleName);

		if( s.ToLong(&l) )
		{
			m_pSelected	= Get_Module((int)l);
		}
	}

	//-----------------------------------------------------
	if( m_pSelected )
	{
		m_pCMD	= new wxCmdLineParser;

		_Set_CMD(m_pSelected->Get_Parameters(), false);

		for(i=0; i<m_pSelected->Get_Parameters_Count(); i++)
		{
			_Set_CMD(m_pSelected->Get_Parameters(i), true);
		}
	}

	return( m_pSelected );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CModule_Library::Execute(int argc, char *argv[])
{
	bool	bResult	= false;

	if( m_pSelected && m_pCMD )
	{
		m_pCMD->SetCmdLine(argc, argv);

		if( argc == 1 )
		{
			m_pCMD->Usage();
			return( true );
		}

		if( _Get_CMD(m_pSelected->Get_Parameters()) )
		{
			bResult	= m_pSelected->Execute();

			_Destroy_DataObjects(bResult);
		}
	}

	if( !bResult )
	{
		Print_Error(LNG("executing module"), m_pSelected->Get_Name());
	}

	return( bResult );
}

//---------------------------------------------------------
bool CModule_Library::Get_Parameters(CSG_Parameters *pParameters)
{
	return( _Get_CMD(pParameters) );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CModule_Library::_Set_CMD(CSG_Parameters *pParameters, bool bExtra)
{
	CSG_Parameter	*pParameter;
	wxString	Description;

	//-----------------------------------------------------
	if( m_pCMD && pParameters )
	{
		m_pCMD->SetSwitchChars(SG_T("-"));

		for(int i=0; i<pParameters->Get_Count(); i++)
		{
			pParameter	= pParameters->Get_Parameter(i);
			Description	= pParameter->Get_Description(
							PARAMETER_DESCRIPTION_NAME|PARAMETER_DESCRIPTION_TYPE|PARAMETER_DESCRIPTION_PROPERTIES, SG_T("\n\t")
						).c_str();

			if( pParameter->is_Input() || pParameter->is_Output() )
			{
				m_pCMD->AddOption(
					GET_ID1(pParameter), wxEmptyString, Description,
					wxCMD_LINE_VAL_STRING,
					wxCMD_LINE_NEEDS_SEPARATOR | (pParameter->is_Optional() || bExtra ? wxCMD_LINE_PARAM_OPTIONAL : wxCMD_LINE_OPTION_MANDATORY)
				);
			}
			else if( pParameter->is_Option() && !pParameter->is_Information() )
			{
				switch( pParameter->Get_Type() )
				{
				default:
					break;

				case PARAMETER_TYPE_Bool:
					m_pCMD->AddSwitch(GET_ID1(pParameter), wxEmptyString, Description, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_Int:
				case PARAMETER_TYPE_Choice:
				case PARAMETER_TYPE_Table_Field:
					m_pCMD->AddOption(GET_ID1(pParameter), wxEmptyString, Description, wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_Double:
				case PARAMETER_TYPE_Degree:
					m_pCMD->AddOption(GET_ID1(pParameter), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_Range:
					m_pCMD->AddOption(GET_ID2(pParameter, wxT("MIN")), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					m_pCMD->AddOption(GET_ID2(pParameter, wxT("MAX")), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_String:
				case PARAMETER_TYPE_Text:
				case PARAMETER_TYPE_FilePath:
					m_pCMD->AddOption(GET_ID1(pParameter), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_FixedTable:
					m_pCMD->AddOption(GET_ID1(pParameter), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					break;

				case PARAMETER_TYPE_Grid_System:
					if( pParameter->Get_Children_Count() == 0 )
					{
						m_pCMD->AddOption(GET_ID2(pParameter, wxT("NX")), wxEmptyString, Description, wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
						m_pCMD->AddOption(GET_ID2(pParameter, wxT("NY")), wxEmptyString, Description, wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL);
						m_pCMD->AddOption(GET_ID2(pParameter, wxT( "X")), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
						m_pCMD->AddOption(GET_ID2(pParameter, wxT( "Y")), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
						m_pCMD->AddOption(GET_ID2(pParameter, wxT( "D")), wxEmptyString, Description, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL);
					}
					break;
				}
			}
		}
	}
}

//---------------------------------------------------------
bool CModule_Library::_Get_CMD(CSG_Parameters *pParameters)
{
	//-----------------------------------------------------
	if( m_pCMD == NULL || pParameters == NULL )
	{
		Print_Error(LNG("[ERR] Internal system error"));

		return( false );
	}

	if( m_pCMD->Parse(false) != 0 || _Create_DataObjects(pParameters) == false )
	{
		m_pCMD->Usage();

		return( false );
	}

	//-----------------------------------------------------
	for(int i=0; i<pParameters->Get_Count(); i++)
	{
		long			l;
		double			d;
		wxString		s;

		CSG_Parameter	*pParameter	= pParameters->Get_Parameter(i);

		switch( pParameter->Get_Type() )
		{
		default:
			break;

		case PARAMETER_TYPE_Bool:
			pParameter->Set_Value(m_pCMD->Found(GET_ID1(pParameter)) ? 1 : 0);
			break;

		case PARAMETER_TYPE_Int:
		case PARAMETER_TYPE_Choice:
		case PARAMETER_TYPE_Table_Field:
			if( m_pCMD->Found(GET_ID1(pParameter), &l) )
			{
				pParameter->Set_Value((int)l);
			}
			break;

		case PARAMETER_TYPE_Double:
		case PARAMETER_TYPE_Degree:
			if( m_pCMD->Found(GET_ID1(pParameter), &s) && s.ToDouble(&d) )
			{
				pParameter->Set_Value(d);
			}
			break;

		case PARAMETER_TYPE_Range:
			if( m_pCMD->Found(GET_ID2(pParameter, wxT("MIN")), &s) && s.ToDouble(&d) )
			{
				pParameter->asRange()->Set_LoVal(d);
			}

			if( m_pCMD->Found(GET_ID2(pParameter, wxT("MAX")), &s) && s.ToDouble(&d) )
			{
				pParameter->asRange()->Set_HiVal(d);
			}
			break;

		case PARAMETER_TYPE_String:
		case PARAMETER_TYPE_Text:
			if( m_pCMD->Found(GET_ID1(pParameter), &s) )
			{
				pParameter->Set_Value(s.c_str());
			}
			break;

		case PARAMETER_TYPE_FilePath:
			if( m_pCMD->Found(GET_ID1(pParameter), &s) )
			{
				if( pParameter->asFilePath()->is_Multiple() )
				{
					s.Prepend(wxT("\""));
					s.Replace(wxT(";"), wxT("\" \""));
					s.Append (wxT("\""));
				}

				pParameter->Set_Value(s.c_str());
			}
			break;

		case PARAMETER_TYPE_FixedTable:
			if( m_pCMD->Found(GET_ID1(pParameter), &s) )
			{
				CSG_Table	Table(s.c_str());
				pParameter->asTable()->Assign_Values(&Table);
			}
			break;

		case PARAMETER_TYPE_Grid_System:
			if( pParameter->Get_Children_Count() == 0 )
			{
				long	nx, ny;
				double	d, x, y;

				if(	!m_pCMD->Found(GET_ID2(pParameter, wxT("NX")), &nx)
				||	!m_pCMD->Found(GET_ID2(pParameter, wxT("NY")), &ny)
				||	!m_pCMD->Found(GET_ID2(pParameter, wxT( "X")), &s) || !s.ToDouble(&x)
				||	!m_pCMD->Found(GET_ID2(pParameter, wxT( "Y")), &s) || !s.ToDouble(&y)
				||	!m_pCMD->Found(GET_ID2(pParameter, wxT( "D")), &s) || !s.ToDouble(&d) )
				{
					d	= -1.0;
				}

				pParameter->asGrid_System()->Assign(d, x, y, (int)nx, (int)ny);
			}
			break;
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CModule_Library::_Create_DataObjects(CSG_Parameters *pParameters)
{
	//-----------------------------------------------------
	if( m_pCMD == NULL || pParameters == NULL )
	{
		Print_Error(LNG("[ERR] Internal system error"));

		return( false );
	}

	//-----------------------------------------------------
	bool	bObjects	= false;
	int		nObjects	= 0;

	for(int i=0; i<pParameters->Get_Count(); i++)
	{
		wxString		s;
		CSG_Data_Object	*pObject;

		CSG_Parameter	*pParameter	= pParameters->Get_Parameter(i);

		if(	pParameter->is_DataObject() || pParameter->is_DataObject_List() )
		{
			bObjects	= true;

			if( m_pCMD->Found(GET_ID1(pParameter), &s) )
			{
				if( pParameter->is_Input() )
				{
					if( pParameter->is_DataObject() )
					{
						switch( pParameter->Get_Type() )
						{
						default:
							pObject	= NULL;
							break;

						case PARAMETER_TYPE_Grid:
							pObject	= new CSG_Grid  (s.c_str());
							if( pObject && pObject->is_Valid() )
							{
								pParameter->Get_Parent()->asGrid_System()->Assign(((CSG_Grid *)pObject)->Get_System());
								pParameter->Set_Value(pObject);
							}
							break;

						case PARAMETER_TYPE_TIN:
							pParameter->Set_Value(pObject = new CSG_TIN   (s.c_str()));
							break;

						case PARAMETER_TYPE_Shapes:
							pParameter->Set_Value(pObject = new CSG_Shapes(s.c_str()));
							break;

						case PARAMETER_TYPE_Table:
							pParameter->Set_Value(pObject = new CSG_Table (s.c_str()));
							break;
						}

						if( !pObject || !pObject->is_Valid() )
						{
							Print_Error(LNG("input file"), s);

							return( false );
						}

						nObjects++;
					}
					else if( pParameter->is_DataObject_List() )
					{
						if( !_Create_DataObject_List(pParameter, s) && !pParameter->is_Optional() )
						{
							Print_Error(LNG("empty input list"), GET_ID1(pParameter));

							return( false );
						}

						nObjects++;
					}
				}
				else if( pParameter->is_Output() )
				{
					pParameter->Set_Value(DATAOBJECT_CREATE);

					nObjects++;
				}
			}
			else if( !pParameter->is_Optional() )
			{
				return( false );
			}
		}
	}

	return( bObjects == false || nObjects > 0 );
}

//---------------------------------------------------------
bool CModule_Library::_Create_DataObject_List(CSG_Parameter *pParameter, wxString sList)
{
	CSG_Data_Object		*pObject;
	wxString		s;

	if( pParameter && pParameter->is_DataObject_List() )
	{
		do
		{
			s		= sList.BeforeFirst	(';');
			sList	= sList.AfterFirst	(';');

			switch( pParameter->Get_Type() )
			{
			default:								pObject	= NULL;								break;
			case PARAMETER_TYPE_Grid_List:			pObject	= new CSG_Grid      (s.c_str());	break;
			case PARAMETER_TYPE_TIN_List:			pObject	= new CSG_TIN       (s.c_str());	break;
			case PARAMETER_TYPE_PointCloud_List:	pObject	= new CSG_PointCloud(s.c_str());	break;
			case PARAMETER_TYPE_Shapes_List:		pObject	= new CSG_Shapes    (s.c_str());	break;
			case PARAMETER_TYPE_Table_List:			pObject	= new CSG_Table     (s.c_str());	break;
			}

			if( pObject && pObject->is_Valid() )
			{
				if( pParameter->Get_Type() == PARAMETER_TYPE_Grid_List && (pParameter->Get_Parent() && pParameter->Get_Parent()->Get_Type() == PARAMETER_TYPE_Grid_System) )
				{	// grid system dependent grid list: first grid defines the grid system to be used!
					if( pParameter->asList()->Get_Count() == 0 )
					{
						pParameter->Get_Parent()->asGrid_System()->Assign  (((CSG_Grid *)pObject)->Get_System());
					}

					if( pParameter->Get_Parent()->asGrid_System()->is_Equal(((CSG_Grid *)pObject)->Get_System()) )
					{
						pParameter->asList()->Add_Item(pObject);
					}
				}
				else
				{
					pParameter->asList()->Add_Item(pObject);
				}
			}
			else if( pObject )
			{
				delete(pObject);

				Print_Error(LNG("input file"), s);
			}
		}
		while( sList.Length() > 0 );

		return( pParameter->asList()->Get_Count() > 0 );
	}

	return( false );
}

//---------------------------------------------------------
bool CModule_Library::Add_DataObject(CSG_Data_Object *pObject)
{
	// leaves unsaved data and a memory leak, if <pObject> is not kept in parameters list either !!!
	// to be done...

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CModule_Library::_Destroy_DataObjects(bool bSave)
{
	if( m_pSelected && m_pCMD )
	{
		_Destroy_DataObjects(bSave, m_pSelected->Get_Parameters());

		for(int i=0; i<m_pSelected->Get_Parameters_Count(); i++)
		{
			// to be done...
			_Destroy_DataObjects(bSave, m_pSelected->Get_Parameters(i));
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CModule_Library::_Destroy_DataObjects(bool bSave, CSG_Parameters *pParameters)
{
	int			i, j;
	CSG_Parameter	*pParameter;
	wxString	s;

	if( pParameters && m_pCMD )
	{
		for(j=0; j<pParameters->Get_Count(); j++)
		{
			pParameter	= pParameters->Get_Parameter(j);

			if( bSave && pParameter->is_Output() && m_pCMD->Found(GET_ID1(pParameter), &s) )
			{
				if( pParameter->is_DataObject() )
				{
					if( pParameter->asDataObject() )
					{
						pParameter->asDataObject()->Save(s.c_str());
					}
				}
				else if( pParameter->is_DataObject_List() )
				{
					if( pParameter->asList()->Get_Count() == 1 )
					{
						pParameter->asList()->asDataObject(0)->Save(s.c_str());
					}
					else
					{
						for(i=0; i<pParameter->asList()->Get_Count(); i++)
						{
							pParameter->asList()->asDataObject(i)->Save(CSG_String::Format(SG_T("%s_%02d"), s.c_str(), i + 1));
						}
					}
				}
			}

			if( pParameter->is_DataObject() )
			{
				if( pParameter->asDataObject() )
				{
					delete(pParameter->asDataObject());
					pParameter->Set_Value(DATAOBJECT_NOTSET);
				}
			}
			else if( pParameter->is_DataObject_List() )
			{
				for(i=pParameter->asList()->Get_Count()-1; i>=0; i--)
				{
					delete(pParameter->asList()->asDataObject(i));
				}

				pParameter->asList()->Del_Items();
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
