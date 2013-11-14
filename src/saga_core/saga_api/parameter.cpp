/**********************************************************
 * Version $Id: parameter.cpp 1756 2013-06-27 10:39:19Z reklov_w $
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
//                     parameter.cpp                     //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
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
#include "parameters.h"
#include "data_manager.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Parameter::CSG_Parameter(CSG_Parameters *pOwner, CSG_Parameter *pParent, const CSG_String &Identifier, const CSG_String &Name, const CSG_String &Description, TSG_Parameter_Type Type, int Constraint)
{
	m_pOwner		= pOwner;
	m_pParent		= pParent;

	m_Identifier	= Identifier;
	m_Name			= Name;
	m_Description	= Description;

	m_bEnabled		= true;

	//-----------------------------------------------------
	m_nChildren		= 0;
	m_Children		= NULL;

	if( m_pParent )
	{
		m_pParent->_Add_Child(this);
	}

	//-----------------------------------------------------
	switch( Type )
	{
	default:								m_pData	= NULL;														break;

	case PARAMETER_TYPE_Node:				m_pData	= new CSG_Parameter_Node				(this, Constraint);	break;

	case PARAMETER_TYPE_Bool:				m_pData	= new CSG_Parameter_Bool				(this, Constraint);	break;
	case PARAMETER_TYPE_Int:				m_pData	= new CSG_Parameter_Int					(this, Constraint);	break;
	case PARAMETER_TYPE_Double:				m_pData	= new CSG_Parameter_Double				(this, Constraint);	break;
	case PARAMETER_TYPE_Degree:				m_pData	= new CSG_Parameter_Degree				(this, Constraint);	break;
	case PARAMETER_TYPE_Range:				m_pData	= new CSG_Parameter_Range				(this, Constraint);	break;
	case PARAMETER_TYPE_Choice:				m_pData	= new CSG_Parameter_Choice				(this, Constraint);	break;

	case PARAMETER_TYPE_String:				m_pData	= new CSG_Parameter_String				(this, Constraint);	break;
	case PARAMETER_TYPE_Text:				m_pData	= new CSG_Parameter_Text				(this, Constraint);	break;
	case PARAMETER_TYPE_FilePath:			m_pData	= new CSG_Parameter_File_Name			(this, Constraint);	break;

	case PARAMETER_TYPE_Font:				m_pData	= new CSG_Parameter_Font				(this, Constraint);	break;
	case PARAMETER_TYPE_Color:				m_pData	= new CSG_Parameter_Color				(this, Constraint);	break;
	case PARAMETER_TYPE_Colors:				m_pData	= new CSG_Parameter_Colors				(this, Constraint);	break;
	case PARAMETER_TYPE_FixedTable:			m_pData	= new CSG_Parameter_Fixed_Table			(this, Constraint);	break;
	case PARAMETER_TYPE_Grid_System:		m_pData	= new CSG_Parameter_Grid_System			(this, Constraint);	break;
	case PARAMETER_TYPE_Table_Field:		m_pData	= new CSG_Parameter_Table_Field			(this, Constraint);	break;
	case PARAMETER_TYPE_Table_Fields:		m_pData	= new CSG_Parameter_Table_Fields		(this, Constraint);	break;

	case PARAMETER_TYPE_DataObject_Output:	m_pData	= new CSG_Parameter_Data_Object_Output	(this, Constraint);	break;
	case PARAMETER_TYPE_Grid:				m_pData	= new CSG_Parameter_Grid				(this, Constraint);	break;
	case PARAMETER_TYPE_Table:				m_pData	= new CSG_Parameter_Table				(this, Constraint);	break;
	case PARAMETER_TYPE_Shapes:				m_pData	= new CSG_Parameter_Shapes				(this, Constraint);	break;
	case PARAMETER_TYPE_TIN:				m_pData	= new CSG_Parameter_TIN					(this, Constraint);	break;
	case PARAMETER_TYPE_PointCloud:			m_pData	= new CSG_Parameter_PointCloud			(this, Constraint);	break;

	case PARAMETER_TYPE_Grid_List:			m_pData	= new CSG_Parameter_Grid_List			(this, Constraint);	break;
	case PARAMETER_TYPE_Table_List:			m_pData	= new CSG_Parameter_Table_List			(this, Constraint);	break;
	case PARAMETER_TYPE_Shapes_List:		m_pData	= new CSG_Parameter_Shapes_List			(this, Constraint);	break;
	case PARAMETER_TYPE_TIN_List:			m_pData	= new CSG_Parameter_TIN_List			(this, Constraint);	break;
	case PARAMETER_TYPE_PointCloud_List:	m_pData	= new CSG_Parameter_PointCloud_List		(this, Constraint);	break;

	case PARAMETER_TYPE_Parameters:			m_pData	= new CSG_Parameter_Parameters			(this, Constraint);	break;
	}

	//-----------------------------------------------------
	switch( Type )
	{
	default:
		break;

	case PARAMETER_TYPE_Range:
		SG_Free(m_Children);
		m_nChildren		= 0;
		m_Children		= NULL;
		break;
	}
}

//---------------------------------------------------------
CSG_Parameter::~CSG_Parameter(void)
{
	if( m_Children )
	{
		SG_Free(m_Children);
	}

	if( m_pData )
	{
		delete(m_pData);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Parameter::_Add_Child(CSG_Parameter *pChild)
{
	m_Children	= (CSG_Parameter **)SG_Realloc(m_Children, (m_nChildren + 1) * sizeof(CSG_Parameter *));
	m_Children[m_nChildren++]	= pChild;
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameter::Set_Enabled(bool bEnabled)
{
	if( m_bEnabled != bEnabled )
	{
		m_bEnabled	= bEnabled;

		return( !bEnabled );
	}

	return( bEnabled );
}

//---------------------------------------------------------
bool CSG_Parameter::is_Option(void)	const
{
	if( !is_Information() )
	{
		switch( Get_Type() )
		{
		default:
			return( false );

		case PARAMETER_TYPE_Bool:
		case PARAMETER_TYPE_Int:
		case PARAMETER_TYPE_Double:
		case PARAMETER_TYPE_Degree:
		case PARAMETER_TYPE_Range:
		case PARAMETER_TYPE_Choice:

		case PARAMETER_TYPE_String:
		case PARAMETER_TYPE_Text:
		case PARAMETER_TYPE_FilePath:

		case PARAMETER_TYPE_Font:

		case PARAMETER_TYPE_Color:
		case PARAMETER_TYPE_Colors:

		case PARAMETER_TYPE_FixedTable:

		case PARAMETER_TYPE_Grid_System:
		case PARAMETER_TYPE_Table_Field:
		case PARAMETER_TYPE_Table_Fields:

		case PARAMETER_TYPE_Parameters:

			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::is_DataObject(void)	const
{
	switch( Get_Type() )
	{
	default:
		return( false );

	case PARAMETER_TYPE_DataObject_Output:
	case PARAMETER_TYPE_Grid:
	case PARAMETER_TYPE_Table:
	case PARAMETER_TYPE_Shapes:
	case PARAMETER_TYPE_TIN:
	case PARAMETER_TYPE_PointCloud:

		return( true );
	}
}

//---------------------------------------------------------
bool CSG_Parameter::is_DataObject_List(void)	const
{
	switch( Get_Type() )
	{
	default:
		return( false );

	case PARAMETER_TYPE_Grid_List:
	case PARAMETER_TYPE_Table_List:
	case PARAMETER_TYPE_Shapes_List:
	case PARAMETER_TYPE_TIN_List:
	case PARAMETER_TYPE_PointCloud_List:

		return( true );
	}
}

//---------------------------------------------------------
bool CSG_Parameter::is_Parameters(void)	const
{
	return( Get_Type() == PARAMETER_TYPE_Parameters );
}

//---------------------------------------------------------
bool CSG_Parameter::is_Serializable(void)	const
{
	switch( Get_Type() )
	{
	case PARAMETER_TYPE_Undefined:
	case PARAMETER_TYPE_Node:
	case PARAMETER_TYPE_DataObject_Output:
		return( false );

	case PARAMETER_TYPE_String:
		return( ((CSG_Parameter_String *)Get_Data())->is_Password() == false );

	default:
		return( !is_Information() );
	}
}

//---------------------------------------------------------
TSG_Data_Object_Type CSG_Parameter::Get_DataObject_Type(void)	const
{
	switch( Get_Type() )
	{
	default:								return( DATAOBJECT_TYPE_Undefined );
	case PARAMETER_TYPE_Grid:
	case PARAMETER_TYPE_Grid_List:			return( DATAOBJECT_TYPE_Grid );
	case PARAMETER_TYPE_Table:
	case PARAMETER_TYPE_Table_List:			return( DATAOBJECT_TYPE_Table );
	case PARAMETER_TYPE_Shapes:
	case PARAMETER_TYPE_Shapes_List:		return( DATAOBJECT_TYPE_Shapes );
	case PARAMETER_TYPE_TIN:
	case PARAMETER_TYPE_TIN_List:			return( DATAOBJECT_TYPE_TIN );
	case PARAMETER_TYPE_PointCloud:
	case PARAMETER_TYPE_PointCloud_List:	return( DATAOBJECT_TYPE_PointCloud );
	}
}

//---------------------------------------------------------
CSG_Data_Manager * CSG_Parameter::Get_Manager(void)	const
{
	return( m_pOwner ? m_pOwner->Get_Manager() : NULL );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
const SG_Char * CSG_Parameter::Get_Identifier(void)	const
{
	return( m_Identifier );
}

//---------------------------------------------------------
const SG_Char * CSG_Parameter::Get_Name(void)	const
{
	return( m_Name );
}

//---------------------------------------------------------
const SG_Char * CSG_Parameter::Get_Description(void)	const
{
	return( m_Description );
}


//---------------------------------------------------------
CSG_String CSG_Parameter::Get_Description(int Flags)	const
{
	return( Get_Description(Flags, SG_T("\n")) );
}

//---------------------------------------------------------
#define SEPARATE	if( bSeparate )	s.Append(Separator);	bSeparate	= true;

//---------------------------------------------------------
CSG_String CSG_Parameter::Get_Description(int Flags, const SG_Char *Separator)	const
{
	if( !Separator || !Separator[0] )
	{
		return( Get_Description(Flags) );
	}

	bool		bSeparate	= false;
	int			i;
	CSG_String	s;

	//-----------------------------------------------------
	if( (Flags & PARAMETER_DESCRIPTION_NAME) != 0 )
	{
		SEPARATE;	s	+= CSG_String::Format(SG_T("%s"), Get_Name());
	}

	//-----------------------------------------------------
	if( (Flags & PARAMETER_DESCRIPTION_TYPE) != 0 )
	{
		SEPARATE;	s	+= CSG_String::Format(SG_T("%s"), Get_Type_Name().c_str());

		if( is_DataObject() || is_DataObject_List() )
		{
			if( is_Input() )
			{
				if( is_Optional() )
					s	+= CSG_String::Format(SG_T(" (%s)"), _TL("optional input"));
				else
					s	+= CSG_String::Format(SG_T(" (%s)"), _TL("input"));
			}
			else if( is_Output() )
			{
				if( is_Optional() )
					s	+= CSG_String::Format(SG_T(" (%s)"), _TL("optional output"));
				else
					s	+= CSG_String::Format(SG_T(" (%s)"), _TL("output"));
			}
		}
	}

	//-----------------------------------------------------
	if( (Flags & PARAMETER_DESCRIPTION_OPTIONAL) != 0 && is_Optional() )
	{
		SEPARATE;	s	+= CSG_String::Format(SG_T("%s"), _TL("optional"));
	}

	//-----------------------------------------------------
	if( (Flags & PARAMETER_DESCRIPTION_PROPERTIES) != 0 )
	{
		switch( Get_Type() )
		{
		default:
			break;

		case PARAMETER_TYPE_Choice:
			SEPARATE;	s	+= CSG_String::Format(SG_T("%s:"), _TL("Available Choices"));

			for(i=0; i<asChoice()->Get_Count(); i++)
			{
				s	+= CSG_String::Format(SG_T("%s[%d] %s"), Separator, i, asChoice()->Get_Item(i));
			}
			break;

		case PARAMETER_TYPE_Int:
			if( asValue()->has_Minimum() )
			{
				SEPARATE;	s	+= CSG_String::Format(SG_T("%s: %d"), _TL("Minimum"), (int)asValue()->Get_Minimum());
			}
			if( asValue()->has_Maximum() )
			{
				SEPARATE;	s	+= CSG_String::Format(SG_T("%s: %d"), _TL("Maximum"), (int)asValue()->Get_Maximum());
			}
			break;

		case PARAMETER_TYPE_Double:
		case PARAMETER_TYPE_Degree:
//		case PARAMETER_TYPE_Range:
			if( asValue()->has_Minimum() )
			{
				SEPARATE;	s	+= CSG_String::Format(SG_T("%s: %f"), _TL("Minimum"), asValue()->Get_Minimum());
			}
			if( asValue()->has_Maximum() )
			{
				SEPARATE;	s	+= CSG_String::Format(SG_T("%s: %f"), _TL("Maximum"), asValue()->Get_Maximum());
			}
			break;

		case PARAMETER_TYPE_FixedTable:
			SEPARATE;	s	+= CSG_String::Format(SG_T("%d %s:%s"), asTable()->Get_Field_Count(), _TL("Fields"), Separator);

			for(i=0; i<asTable()->Get_Field_Count(); i++)
			{
				s	+= CSG_String::Format(SG_T("- %d. [%s] %s%s"), i + 1, SG_Data_Type_Get_Name(asTable()->Get_Field_Type(i)).c_str(), asTable()->Get_Field_Name(i), Separator);
			}
			break;

		case PARAMETER_TYPE_Parameters:
			SEPARATE;	s	+= CSG_String::Format(SG_T("%d %s:%s"), asParameters()->Get_Count(), _TL("Parameters"), Separator);

			for(i=0; i<asParameters()->Get_Count(); i++)
			{
				s	+= CSG_String::Format(SG_T("- %d. %s%s"), i + 1, asParameters()->Get_Parameter(i)->Get_Description(Flags, Separator).c_str(), Separator);
			}
			break;
		}

		if( !m_pData->Get_Default().is_Empty() )
		{
			SEPARATE;	s	+= CSG_String::Format(SG_T("%s: %s"), _TL("Default"), m_pData->Get_Default().c_str());
		}
	}

	//-----------------------------------------------------
	if( (Flags & PARAMETER_DESCRIPTION_TEXT) != 0 && m_Description.Length() > 0 )
	{
		SEPARATE;

		s	+= m_Description;
	}

	//-----------------------------------------------------
	return( s );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameter::Set_Value(int Value)
{
	if( m_pData->Set_Value(Value) )
	{
		has_Changed();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::Set_Value(double Value)
{
	if( m_pData->Set_Value(Value) )
	{
		has_Changed();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::Set_Value(const CSG_String &Value)
{
	if( m_pData->Set_Value(Value) )
	{
		has_Changed();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::Set_Value(void *Value)
{
	if( m_pData->Set_Value(Value) )
	{
		has_Changed();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::Set_Value(CSG_Parameter *Value)
{
	if( Value )
	{
		switch( Value->Get_Type() )
		{
		default:								return( Assign(Value) );

		case PARAMETER_TYPE_Choice:				return( Set_Value(Value->asInt()) );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::has_Changed(int Check_Flags)
{
	if( m_pOwner )
	{
		return( m_pOwner->_On_Parameter_Changed(this, Check_Flags) );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Parameter::Check(bool bSilent)
{
	if( !is_Enabled() )
	{
		return( true );
	}

	//-----------------------------------------------------
	if( Get_Type() == PARAMETER_TYPE_Parameters )
	{
		return( asParameters()->DataObjects_Check(bSilent) );
	}

	//-----------------------------------------------------
	if( Get_Type() == PARAMETER_TYPE_Grid_System )
	{
		if( m_pOwner->Get_Manager() && !m_pOwner->Get_Manager()->Exists(*asGrid_System()) )
		{
			Set_Value((void *)NULL);
		}

		return( true );	// ( false );
	}

	//-----------------------------------------------------
	if( is_DataObject() )
	{
		if( is_Input() || (is_Output() && asDataObject() != DATAOBJECT_CREATE) )
		{
			if( m_pOwner->Get_Manager() && !m_pOwner->Get_Manager()->Exists(asDataObject()) )
			{
				Set_Value(DATAOBJECT_NOTSET);
			}
		}

		return( asDataObject() || is_Optional() );
	}

	//-----------------------------------------------------
	else if( is_DataObject_List() )
	{
		for(int j=asList()->Get_Count()-1; j>=0; j--)
		{
			CSG_Data_Object	*pDataObject	= asList()->asDataObject(j);

			if( !pDataObject || (m_pOwner->Get_Manager() && !m_pOwner->Get_Manager()->Exists(pDataObject)) )
			{
				asList()->Del_Item(j);
			}
		}

		return( is_Output() || is_Optional() || asList()->Get_Count() > 0 );
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameter::Assign(CSG_Parameter *pSource)
{
	m_bEnabled	= pSource->m_bEnabled;

	return( m_pData->Assign(pSource->m_pData) );
}

//---------------------------------------------------------
CSG_MetaData * CSG_Parameter::Serialize(CSG_MetaData &Entry, bool bSave)
{
	if( bSave )
	{
		if( !is_Information() && Get_Type() != PARAMETER_TYPE_Node && Get_Type() != PARAMETER_TYPE_Undefined )
		{
			CSG_MetaData	*pEntry	= Entry.Add_Child(
				is_Option()          ? SG_T("OPTION")    :
				is_DataObject()      ? SG_T("DATA")      :
				is_DataObject_List() ? SG_T("DATA_LIST") :
				                       SG_T("PARAMETER")
			);

			pEntry->Add_Property(SG_T("type"), Get_Type_Identifier());
			pEntry->Add_Property(SG_T("id")  , Get_Identifier());
			pEntry->Add_Property(SG_T("name"), Get_Name());

			m_pData->Serialize(*pEntry, bSave);

			return( pEntry );
		}
	}
	else
	{
		if(	Entry.Cmp_Property(SG_T("type"), Get_Type_Identifier())
		&&	Entry.Cmp_Property(SG_T("id")  , Get_Identifier()) )
		{
			return( m_pData->Serialize(Entry, bSave) ? &Entry : NULL );
		}
	}

	return( NULL );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Parameters_Grid_Target::CSG_Parameters_Grid_Target(void)
{
	Create();
}

//---------------------------------------------------------
void CSG_Parameters_Grid_Target::Create(void)
{
	m_pUser		= NULL;
	m_pGrid		= NULL;
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Add_Parameters_User(CSG_Parameters *pParameters, bool bAddDefaultGrid)
{
	if( pParameters == NULL )
	{
		return( false );
	}

	m_pUser	= pParameters;

	pParameters->Add_Value(
		NULL, "XMIN"		, _TL("Left")		, _TL(""), PARAMETER_TYPE_Double
	);

	pParameters->Add_Value(
		NULL, "XMAX"		, _TL("Right")		, _TL(""), PARAMETER_TYPE_Double
	);

	pParameters->Add_Value(
		NULL, "YMIN"		, _TL("Bottom")		, _TL(""), PARAMETER_TYPE_Double
	);

	pParameters->Add_Value(
		NULL, "YMAX"		, _TL("Top")		, _TL(""), PARAMETER_TYPE_Double
	);

	pParameters->Add_Value(
		NULL, "SIZE"		, _TL("Cellsize")	, _TL(""), PARAMETER_TYPE_Double, 1.0, 0.0, true
	);

	pParameters->Add_Info_Value(
		NULL, "COLS"		, _TL("Columns")	, _TL(""), PARAMETER_TYPE_Int
	);

	pParameters->Add_Info_Value(
		NULL, "ROWS"		, _TL("Rows")		, _TL(""), PARAMETER_TYPE_Int
	);

	if( bAddDefaultGrid )
	{
		pParameters->Add_Grid_Output(
			NULL, "GRID"		, _TL("Grid")		, _TL("")
		);
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::On_User_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( !m_pUser || !pParameters || !pParameter || SG_STR_CMP(m_pUser->Get_Identifier(), pParameters->Get_Identifier()) )
	{
		return( false );
	}

	CSG_Parameter	*xMin	= pParameters->Get_Parameter("XMIN");
	CSG_Parameter	*xMax	= pParameters->Get_Parameter("XMAX");
	CSG_Parameter	*yMin	= pParameters->Get_Parameter("YMIN");
	CSG_Parameter	*yMax	= pParameters->Get_Parameter("YMAX");
	CSG_Parameter	*Size	= pParameters->Get_Parameter("SIZE");
	CSG_Parameter	*Rows	= pParameters->Get_Parameter("ROWS");
	CSG_Parameter	*Cols	= pParameters->Get_Parameter("COLS");

	if( !xMin || !xMax || !yMin || !yMax || !Size || !Rows || !Cols )
	{
		return( false );
	}

	if(      !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("SIZE")) )
	{
		xMax->Set_Value(xMin->asDouble() + ((int)((xMax->asDouble() - xMin->asDouble()) / Size->asDouble())) * Size->asDouble());
		yMax->Set_Value(yMin->asDouble() + ((int)((yMax->asDouble() - yMin->asDouble()) / Size->asDouble())) * Size->asDouble());
	}
	else if( !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("XMIN")) )
	{
		if( xMin->asDouble() >= xMax->asDouble() )
		{
			xMin->Set_Value(xMax->asDouble() - Cols->asInt() * Size->asDouble());
		}

		xMax->Set_Value(xMin->asDouble() + ((int)((xMax->asDouble() - xMin->asDouble()) / Size->asDouble())) * Size->asDouble());
	}
	else if( !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("XMAX")) )
	{
		if( xMin->asDouble() >= xMax->asDouble() )
		{
			xMax->Set_Value(xMin->asDouble() + Cols->asInt() * Size->asDouble());
		}

		xMin->Set_Value(xMax->asDouble() - ((int)((xMax->asDouble() - xMin->asDouble()) / Size->asDouble())) * Size->asDouble());
	}
	else if( !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("YMIN")) )
	{
		if( yMin->asDouble() >= yMax->asDouble() )
		{
			yMin->Set_Value(yMax->asDouble() - Rows->asInt() * Size->asDouble());
		}

		yMax->Set_Value(yMin->asDouble() + ((int)((yMax->asDouble() - yMin->asDouble()) / Size->asDouble())) * Size->asDouble());
	}
	else if( !SG_STR_CMP(pParameter->Get_Identifier(), SG_T("YMAX")) )
	{
		if( yMin->asDouble() >= yMax->asDouble() )
		{
			yMax->Set_Value(yMin->asDouble() + Rows->asInt() * Size->asDouble());
		}

		yMin->Set_Value(yMax->asDouble() - ((int)((yMax->asDouble() - yMin->asDouble()) / Size->asDouble())) * Size->asDouble());
	}

	Cols->Set_Value(1 + (int)((xMax->asDouble() - xMin->asDouble()) / Size->asDouble()));
	Rows->Set_Value(1 + (int)((yMax->asDouble() - yMin->asDouble()) / Size->asDouble()));

	return( true );
}

//---------------------------------------------------------
/**
  * Initializes the grid system from Extent and number of rows.
  * If bFitToCells is true, the extent is deflated by half a cell size,
  * so automatically adjusting the extent based on grid nodes to
  * to an extent based on the grid cells.
*/
bool CSG_Parameters_Grid_Target::Init_User(const TSG_Rect &Extent, int Rows, bool bFitToCells)
{
	if( !m_pUser || Extent.xMin >= Extent.xMax || Extent.yMin >= Extent.yMax || Rows < 1 )
	{
		return( false );
	}

	double	Size	= (Extent.yMax - Extent.yMin) / (double)Rows;

	CSG_Rect        GridExtent(Extent);

    if( bFitToCells )
        GridExtent.Deflate(0.5 * Size, false);

	m_pUser->Get_Parameter("XMIN")->Set_Value(GridExtent.Get_XMin());
	m_pUser->Get_Parameter("XMAX")->Set_Value(GridExtent.Get_XMax());
	m_pUser->Get_Parameter("YMIN")->Set_Value(GridExtent.Get_YMin());
	m_pUser->Get_Parameter("YMAX")->Set_Value(GridExtent.Get_YMax());
	m_pUser->Get_Parameter("SIZE")->Set_Value(Size);
	m_pUser->Get_Parameter("COLS")->Set_Value(1 + (int)((GridExtent.Get_XMax() - GridExtent.Get_XMin()) / Size));
	m_pUser->Get_Parameter("ROWS")->Set_Value(1 + (int)((GridExtent.Get_YMax() - GridExtent.Get_YMin()) / Size));

	return( true );
}

bool CSG_Parameters_Grid_Target::Init_User(double xMin, double yMin, double Size, int nx, int ny)
{
	if( !m_pUser || Size <= 0.0 || nx <= 1 || ny <= 1 )
	{
		return( false );
	}

	m_pUser->Get_Parameter("XMIN")->Set_Value(xMin);
	m_pUser->Get_Parameter("XMAX")->Set_Value(xMin + Size * nx);
	m_pUser->Get_Parameter("YMIN")->Set_Value(yMin);
	m_pUser->Get_Parameter("YMAX")->Set_Value(yMin + Size * ny);
	m_pUser->Get_Parameter("SIZE")->Set_Value(Size);
	m_pUser->Get_Parameter("COLS")->Set_Value(nx);
	m_pUser->Get_Parameter("ROWS")->Set_Value(ny);

	return( true );
}

//---------------------------------------------------------
CSG_Grid * CSG_Parameters_Grid_Target::Get_User(TSG_Data_Type Type)
{
	return( Get_User(SG_T("GRID"), Type) );
}

//---------------------------------------------------------
CSG_Grid * CSG_Parameters_Grid_Target::Get_User(const CSG_String &Identifier, TSG_Data_Type Type)
{
	CSG_Grid	*pGrid	= NULL;

	if( m_pUser )
	{
		CSG_Grid_System	System(
			m_pUser->Get_Parameter("SIZE")->asDouble(),
			m_pUser->Get_Parameter("XMIN")->asDouble(),
			m_pUser->Get_Parameter("YMIN")->asDouble(),
			m_pUser->Get_Parameter("XMAX")->asDouble(),
			m_pUser->Get_Parameter("YMAX")->asDouble()
		);

		if( System.is_Valid() )
		{
			pGrid	= SG_Create_Grid(System, Type);
		}

		if( Identifier.Length() > 0 && m_pUser->Get_Parameter(Identifier) )
		{
			m_pUser->Get_Parameter(Identifier)->Set_Value(pGrid);
		}
	}

	return( pGrid );
}

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Get_System_User(CSG_Grid_System &System)
{
	if( m_pUser )
	{
		if( m_pUser->Get_Parameter("SIZE")->asDouble() > 0.0
		&&	m_pUser->Get_Parameter("COLS")->asInt()    > 0
		&&	m_pUser->Get_Parameter("ROWS")->asInt()    > 0	)
		{
			On_User_Changed(m_pUser, m_pUser->Get_Parameter("SIZE"));

			System.Assign(
				m_pUser->Get_Parameter("SIZE")->asDouble(),
				m_pUser->Get_Parameter("XMIN")->asDouble(),
				m_pUser->Get_Parameter("YMIN")->asDouble(),
				m_pUser->Get_Parameter("COLS")->asInt(),
				m_pUser->Get_Parameter("ROWS")->asInt()
			);

			return( System.is_Valid() );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Add_Grid_Parameter(const CSG_String &Identifier, const CSG_String &Name, bool bOptional)
{
	if( Identifier.Length() == 0 )
	{
		return( false );
	}

	if( m_pUser && m_pUser->Get_Parameter(Identifier) == NULL )
	{
		m_pUser->Add_Grid_Output(NULL, Identifier, Name, _TL(""));
	}

	if( m_pGrid && m_pGrid->Get_Parameter(Identifier) == NULL )
	{
		CSG_Parameter	*pSystem	= NULL;

		for(int i=0; i<m_pGrid->Get_Count() && !pSystem; i++)
		{
			if( m_pGrid->Get_Parameter(i)->Get_Type() == PARAMETER_TYPE_Grid_System )
			{
				pSystem	= m_pGrid->Get_Parameter(i);
			}
		}

		m_pGrid->Add_Grid(pSystem, Identifier, Name, _TL(""), SG_UI_Get_Window_Main()
			? (bOptional ? PARAMETER_OUTPUT_OPTIONAL : PARAMETER_OUTPUT)
			: (bOptional ? PARAMETER_INPUT_OPTIONAL  : PARAMETER_INPUT ), false
		);
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Add_Parameters_Grid(CSG_Parameters *pParameters, bool bAddDefaultGrid)
{
	if( pParameters == NULL )
	{
		return( false );
	}

	m_pGrid	= pParameters;

	if( bAddDefaultGrid )
	{
		pParameters->Add_Grid(
			NULL	, "GRID"	, _TL("Grid")		, _TL(""), SG_UI_Get_Window_Main() ? PARAMETER_OUTPUT : PARAMETER_INPUT_OPTIONAL, false
		);
	}

	return( true );
}

//---------------------------------------------------------
CSG_Grid * CSG_Parameters_Grid_Target::Get_Grid(TSG_Data_Type Type)
{
	return( Get_Grid(SG_T("GRID"), Type) );
}

//---------------------------------------------------------
CSG_Grid * CSG_Parameters_Grid_Target::Get_Grid(const CSG_String &Identifier, TSG_Data_Type Type)
{
	CSG_Parameter	*pParameter	= m_pGrid && Identifier.Length() > 0 ? m_pGrid->Get_Parameter(Identifier) : NULL;

	if( !pParameter || !pParameter->Get_Parent()->asGrid_System()->is_Valid() )
	{
		return( NULL );
	}

	CSG_Grid	*pGrid	= pParameter->asGrid();

	if( (pGrid == DATAOBJECT_NOTSET && !pParameter->is_Optional()) || pGrid == DATAOBJECT_CREATE )
	{
		pParameter->Set_Value(pGrid	= SG_Create_Grid(*pParameter->Get_Parent()->asGrid_System(), Type));

		if( pParameter->Get_Manager() )
		{
			pParameter->Get_Manager()->Add(pGrid);
		}
	}

	return( pGrid );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Add_Parameters_System(CSG_Parameters *pParameters)
{
	if( pParameters == NULL )
	{
		return( false );
	}

	m_pSystem	= pParameters;

	m_pSystem->Add_Grid_System(NULL, "SYSTEM", _TL("Grid System"), _TL(""));

	return( true );
}

//---------------------------------------------------------
bool CSG_Parameters_Grid_Target::Get_System(CSG_Grid_System &System)
{
	if( m_pSystem && m_pSystem->Get_Parameter("SYSTEM") && m_pSystem->Get_Parameter("SYSTEM")->asGrid_System() )
	{
		System.Assign(*m_pSystem->Get_Parameter("SYSTEM")->asGrid_System());

		return( System.is_Valid() );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
