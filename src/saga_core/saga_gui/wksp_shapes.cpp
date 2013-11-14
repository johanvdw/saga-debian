/**********************************************************
 * Version $Id: wksp_shapes.cpp 1646 2013-04-10 16:29:00Z oconrad $
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
//                   WKSP_Shapes.cpp                     //
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
#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"
#include "dc_helper.h"

#include "wksp_layer_classify.h"

#include "wksp_shapes.h"
#include "wksp_table.h"

#include "view_scatterplot.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Shapes::CWKSP_Shapes(CSG_Shapes *pShapes)
	: CWKSP_Layer(pShapes)
{
	m_pTable		= new CWKSP_Table(pShapes);

	m_Edit_Shapes.Create(pShapes->Get_Type());
	m_Edit_pShape	= NULL;

	m_Edit_Attributes.Destroy();
	m_Edit_Attributes.Add_Field(_TL("Name") , SG_DATATYPE_String);
	m_Edit_Attributes.Add_Field(_TL("Value"), SG_DATATYPE_String);

	m_Edit_Color    = wxColor(0, 0, 0);
}

//---------------------------------------------------------
CWKSP_Shapes::~CWKSP_Shapes(void)
{
	delete(m_pTable);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Shapes::Get_Description(void)
{
	wxString	s;

	//-----------------------------------------------------
	s	+= wxString::Format(wxT("<b>%s</b>"), _TL("Shapes"));

	s	+= wxT("<table border=\"0\">");

	DESC_ADD_STR(_TL("Name")			, m_pObject->Get_Name());
	DESC_ADD_STR(_TL("Description")		, m_pObject->Get_Description());
	DESC_ADD_STR(_TL("File")			, SG_File_Exists(m_pObject->Get_File_Name()) ? m_pObject->Get_File_Name() : _TL("memory"));
	DESC_ADD_STR(_TL("Modified")		, m_pObject->is_Modified() ? _TL("yes") : _TL("no"));
	DESC_ADD_STR(_TL("Projection")		, m_pObject->Get_Projection().Get_Description().c_str());
	DESC_ADD_FLT(_TL("West")			, Get_Shapes()->Get_Extent().Get_XMin());
	DESC_ADD_FLT(_TL("East")			, Get_Shapes()->Get_Extent().Get_XMax());
	DESC_ADD_FLT(_TL("West-East")		, Get_Shapes()->Get_Extent().Get_XRange());
	DESC_ADD_FLT(_TL("South")			, Get_Shapes()->Get_Extent().Get_YMin());
	DESC_ADD_FLT(_TL("North")			, Get_Shapes()->Get_Extent().Get_YMax());
	DESC_ADD_FLT(_TL("South-North")		, Get_Shapes()->Get_Extent().Get_YRange());
	DESC_ADD_STR(_TL("Type")			, SG_Get_ShapeType_Name(Get_Shapes()->Get_Type()).c_str());
	DESC_ADD_STR(_TL("Vertex Type")		, Get_Shapes()->Get_Vertex_Type() == 0 ? _TL("X, Y") : Get_Shapes()->Get_Vertex_Type() == 1 ? _TL("X, Y, Z") : _TL("X, Y, Z, M"));
	DESC_ADD_INT(_TL("Number Of Shapes"), Get_Shapes()->Get_Count());

	s	+= wxT("</table>");

	s	+= Get_TableInfo_asHTML(Get_Shapes());

	//-----------------------------------------------------
//	s	+= wxString::Format(wxT("<hr><b>%s</b><font size=\"-1\">"), _TL("Data History"));
//	s	+= Get_Shapes()->Get_History().Get_HTML();
//	s	+= wxString::Format(wxT("</font"));

	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Shapes::Get_Menu(void)
{
	wxMenu	*pMenu	= new wxMenu(m_pObject->Get_Name());

	CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_CLOSE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SHAPES_SAVE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SHAPES_SAVEAS);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SHAPES_SHOW);
	CMD_Menu_Add_Item(pMenu,  true, ID_CMD_SHAPES_HISTOGRAM);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_SHAPES_SET_LUT);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_WKSP_ITEM_SETTINGS_COPY);

	pMenu->AppendSeparator();

	wxMenu	*pTable	= new wxMenu(_TL("Table"));
	CMD_Menu_Add_Item(pTable,  true, ID_CMD_TABLES_SHOW);
	CMD_Menu_Add_Item(pTable,  true, ID_CMD_TABLES_DIAGRAM);
	CMD_Menu_Add_Item(pTable, false, ID_CMD_TABLES_SCATTERPLOT);
	CMD_Menu_Add_Item(pTable, false, ID_CMD_TABLES_SAVE);
	pMenu->Append(ID_CMD_WKSP_FIRST, _TL("Attributes"), pTable);

	pMenu->Append(ID_CMD_WKSP_FIRST, _TL("Edit"), Edit_Get_Menu());

	return( pMenu );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Shapes::On_Command(int Cmd_ID)
{
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Layer::On_Command(Cmd_ID) );

	case ID_CMD_SHAPES_SET_LUT:
		_LUT_Create();
		break;

	case ID_CMD_SHAPES_EDIT_SHAPE:
		_Edit_Shape();
		break;

	case ID_CMD_SHAPES_EDIT_ADD_SHAPE:
		_Edit_Shape_Add();
		break;

	case ID_CMD_SHAPES_EDIT_DEL_SHAPE:
		_Edit_Shape_Del();
		break;

	case ID_CMD_SHAPES_EDIT_ADD_PART:
		_Edit_Part_Add();
		break;

	case ID_CMD_SHAPES_EDIT_DEL_PART:
		_Edit_Part_Del();
		break;

	case ID_CMD_SHAPES_EDIT_DEL_POINT:
		_Edit_Point_Del();
		break;

	case ID_CMD_SHAPES_EDIT_SEL_INVERT:
		Get_Shapes()->Inv_Selection();
		Update_Views();
		break;

	case ID_CMD_TABLES_SHOW:
		m_pTable->Toggle_View();
		break;

	case ID_CMD_TABLES_DIAGRAM:
		m_pTable->Toggle_Diagram();
		break;

	case ID_CMD_TABLES_SCATTERPLOT:
		Add_ScatterPlot(Get_Table()->Get_Table());
		break;

	case ID_CMD_TABLES_SAVE:
		{
			wxString	File(m_pObject->Get_File_Name());

			if( DLG_Save(File, ID_DLG_TABLES_SAVE) )
			{
				CSG_Table	Table(*((CSG_Table *)m_pObject));

				Table.Save(&File);
			}
		}
		break;

	case ID_CMD_SHAPES_HISTOGRAM:
		Histogram_Toggle();
		break;
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Shapes::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Layer::On_Command_UI(event) );

	case ID_CMD_SHAPES_EDIT_SHAPE:
		event.Enable(Get_Shapes()->Get_Selection_Count() > 0 || m_Edit_pShape != NULL);
		event.Check(m_Edit_pShape != NULL);
		break;

	case ID_CMD_SHAPES_EDIT_ADD_SHAPE:
		event.Enable(m_Edit_pShape == NULL);
		break;

	case ID_CMD_SHAPES_EDIT_DEL_SHAPE:
		event.Enable(Get_Shapes()->Get_Selection_Count() > 0 && m_Edit_pShape == NULL);
		break;

	case ID_CMD_SHAPES_EDIT_ADD_PART:
		event.Enable(m_Edit_pShape != NULL);
		break;

	case ID_CMD_SHAPES_EDIT_DEL_PART:
		event.Enable(m_Edit_pShape != NULL && m_Edit_iPart >= 0);
		break;

	case ID_CMD_SHAPES_EDIT_DEL_POINT:
		event.Enable(m_Edit_pShape != NULL && m_Edit_iPart >= 0 && m_Edit_iPoint >= 0);
		break;

	case ID_CMD_SHAPES_EDIT_SEL_INVERT:
		event.Enable(m_Edit_pShape == NULL);
		break;

	case ID_CMD_TABLES_SHOW:
		event.Check(m_pTable->Get_View() != NULL);
		break;

	case ID_CMD_TABLES_DIAGRAM:
		event.Check(m_pTable->Get_Diagram() != NULL);
		break;

	case ID_CMD_SHAPES_HISTOGRAM:
		event.Check(m_pHistogram != NULL);
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
void CWKSP_Shapes::On_Create_Parameters(void)
{
	CWKSP_Layer::On_Create_Parameters();

	//-----------------------------------------------------
	// Classification...

	_AttributeList_Add(
		m_Parameters("NODE_LUT")		, "LUT_ATTRIB"				, _TL("Attribute"),
		_TL("")
	);

	_AttributeList_Add(
		m_Parameters("NODE_METRIC")		, "METRIC_ATTRIB"			, _TL("Attribute"),
		_TL("")
	);

	//-----------------------------------------------------
	// Display...

	m_Parameters.Add_Parameters(
		m_Parameters("NODE_DISPLAY")	, "DISPLAY_CHART"			, _TL("Chart"),
		_TL("")
	);

	//-----------------------------------------------------
	// Label...

	_AttributeList_Add(
		m_Parameters("NODE_LABEL")		, "LABEL_ATTRIB"			, _TL("Attribute"),
		_TL("")
	);

	m_Parameters.Add_Font(
		m_Parameters("LABEL_ATTRIB")	, "LABEL_ATTRIB_FONT"		, _TL("Font"),
		_TL("")
	);

	m_Parameters.Add_Choice(
		m_Parameters("LABEL_ATTRIB")	, "LABEL_ATTRIB_PREC"		, _TL("Numerical Precision"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("fit to value"),
			_TL("standard"),
			SG_T("0|0.1|0.12|0.123|0.1234|0.12345|0.1234567|0.12345678|0.123456789|0.1234567890|0.12345678901|0.123456789012|0.1234567890123|0.12345678901234|0.123456789012345|0.1234567890123456|0.12345678901234567|0.123456789012345678|0.1234567890123456789|0.12345678901234567890")
		), 0
	);

	m_Parameters.Add_Choice(
		m_Parameters("LABEL_ATTRIB")	, "LABEL_ATTRIB_SIZE_TYPE"	, _TL("Size relates to..."),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("Screen"),
			_TL("Map Units")
		), 0
	);

	m_Parameters.Add_Choice(
		m_Parameters("LABEL_ATTRIB")	, "LABEL_ATTRIB_EFFECT"		, _TL("Boundary Effect"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|"),
			_TL("none"),
			_TL("full frame"),
			_TL("top"),
			_TL("top left"),
			_TL("left"),
			_TL("bottom left"),
			_TL("bottom"),
			_TL("bottom right"),
			_TL("right"),
			_TL("top right")
		), 1
	);

	m_Parameters.Add_Value(
		m_Parameters("LABEL_ATTRIB_EFFECT"), "LABEL_ATTRIB_EFFECT_COLOR"	, _TL("Color"),
		_TL(""),
		PARAMETER_TYPE_Color, SG_GET_RGB(255, 255, 255)
	);

	_AttributeList_Add(
		m_Parameters("LABEL_ATTRIB")	, "LABEL_ATTRIB_SIZE_BY"	, _TL("Size by Attribute"),
		_TL("")
	);

	m_Parameters.Add_Value(
		m_Parameters("LABEL_ATTRIB_SIZE_BY"), "LABEL_ATTRIB_SIZE"	, _TL("Default Size"),
		_TL(""),
		PARAMETER_TYPE_Double, 100.0, 0.0, true
	);

	//-----------------------------------------------------
	// Selection...

	m_Parameters.Add_Value(
		m_Parameters("NODE_SELECTION")	, "SEL_COLOR"				, _TL("Color"),
		_TL(""),
		PARAMETER_TYPE_Color, SG_GET_RGB(255, 0, 0)
	);

	//-----------------------------------------------------
	// Edit...

	m_Parameters.Add_Value(
		m_Parameters("NODE_EDIT")		, "EDIT_COLOR"				, _TL("Color"),
		_TL(""),
		PARAMETER_TYPE_Color, SG_GET_RGB(0, 0, 0)
	);

	m_Parameters.Add_Shapes_List(
		m_Parameters("NODE_EDIT")		, "EDIT_SNAP_LIST"			, _TL("Snap to..."),
		_TL(""),
		PARAMETER_INPUT
	);

	m_Parameters.Add_Value(
		m_Parameters("EDIT_SNAP_LIST")	, "EDIT_SNAP_DIST"			, _TL("Snap Distance"),
		_TL(""),
		PARAMETER_TYPE_Int, 10, 0, true
	);

#ifdef USE_HTMLINFO
	//-----------------------------------------------------
	// HTML Extra info

	m_Parameters.Add_Node(
		NULL							, "NODE_EXTRAINFO"			, _TL("Html Extra Info"),
		_TL("")
	);

	_AttributeList_Add(
		m_Parameters("NODE_EXTRAINFO")	, "EXTRAINFO_ATTRIB"		, _TL("Attribute"),
		_TL("")
	);
#endif

	m_Parameters("LUT")->asTable()->Set_Field_Type(LUT_MIN, SG_DATATYPE_String);
	m_Parameters("LUT")->asTable()->Set_Field_Type(LUT_MAX, SG_DATATYPE_String);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes::On_DataObject_Changed(void)
{
	CWKSP_Layer::On_DataObject_Changed();

	_AttributeList_Set(m_Parameters("LUT_ATTRIB")			, false);
	_AttributeList_Set(m_Parameters("METRIC_ATTRIB")		, false);
	_AttributeList_Set(m_Parameters("LABEL_ATTRIB")			, true);
	_AttributeList_Set(m_Parameters("LABEL_ATTRIB_SIZE_BY")	, true);

	_Chart_Set_Options();

	m_pTable->DataObject_Changed();

#ifdef USE_HTMLINFO
	_AttributeList_Set(m_Parameters("EXTRAINFO_ATTRIB") , true);
#endif
}

//---------------------------------------------------------
void CWKSP_Shapes::On_Parameters_Changed(void)
{
	CWKSP_Layer::On_Parameters_Changed();

	m_Def_Color	= Get_Color_asWX(m_Parameters("UNISYMBOL_COLOR")->asColor());

	//-----------------------------------------------------
	switch( m_Parameters("COLORS_TYPE")->asInt() )
	{
	default:
	case 0:	m_iColor	= -1;										break;	// CLASSIFY_UNIQUE
	case 1:	m_iColor	= m_Parameters("LUT_ATTRIB"   )->asInt();	break;	// CLASSIFY_LUT
	case 2:	m_iColor	= m_Parameters("METRIC_ATTRIB")->asInt();	break;	// CLASSIFY_METRIC
	case 3:	m_iColor	= m_Parameters("METRIC_ATTRIB")->asInt();	break;	// CLASSIFY_GRADUATED
	}

	if( m_iColor >= Get_Shapes()->Get_Field_Count() )
	{
		m_iColor	= -1;
	}

	if( m_iColor < 0 && m_pClassify->Get_Mode() != CLASSIFY_UNIQUE )
	{
		m_pClassify->Set_Mode(CLASSIFY_UNIQUE);
	}

	//-----------------------------------------------------
	if( (m_iLabel = m_Parameters("LABEL_ATTRIB")->asInt()) >= Get_Shapes()->Get_Field_Count() )
	{
		m_iLabel	= -1;
	}

	if( (m_iLabel_Size = m_Parameters("LABEL_ATTRIB_SIZE_BY")->asInt()) >= Get_Shapes()->Get_Field_Count() )
	{
		m_iLabel_Size	= -1;
	}

	m_Label_Prec		= m_Parameters("LABEL_ATTRIB_PREC")->asInt() - 2;

	switch( m_Parameters("LABEL_ATTRIB_EFFECT")->asInt() )
	{
	default:	m_Label_Eff	= TEXTEFFECT_NONE;			break;
	case 1:		m_Label_Eff	= TEXTEFFECT_FRAME;			break;
	case 2:		m_Label_Eff	= TEXTEFFECT_TOP;			break;
	case 3:		m_Label_Eff	= TEXTEFFECT_TOPLEFT;		break;
	case 4:		m_Label_Eff	= TEXTEFFECT_LEFT;			break;
	case 5:		m_Label_Eff	= TEXTEFFECT_BOTTOMLEFT;	break;
	case 6:		m_Label_Eff	= TEXTEFFECT_BOTTOM;		break;
	case 7:		m_Label_Eff	= TEXTEFFECT_BOTTOMRIGHT;	break;
	case 8:		m_Label_Eff	= TEXTEFFECT_RIGHT;			break;
	case 9:		m_Label_Eff	= TEXTEFFECT_TOPRIGHT;		break;
	}

	m_Label_Eff_Color	= m_Parameters("LABEL_ATTRIB_EFFECT_COLOR")->asColor();

	//-----------------------------------------------------
#ifdef USE_HTMLINFO
	if( (m_iExtraInfo = m_Parameters("EXTRAINFO_ATTRIB")->asInt()) >= Get_Shapes()->Get_Field_Count() )
	{
		m_iExtraInfo	= -1;
	}
#endif

	//-----------------------------------------------------
	_Chart_Get_Options();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CWKSP_Shapes::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags)
{
	//-----------------------------------------------------
	if( Flags & PARAMETER_CHECK_VALUES )
	{
		if(	!SG_STR_CMP(pParameter->Get_Identifier(), wxT("COLORS_TYPE"))
		||	!SG_STR_CMP(pParameter->Get_Identifier(), wxT("METRIC_ATTRIB")) )
		{
			int		zField	= pParameters->Get_Parameter("METRIC_ATTRIB")->asInt();

			pParameters->Get_Parameter("METRIC_ZRANGE")->asRange()->Set_Range(
				Get_Shapes()->Get_Minimum(zField),
				Get_Shapes()->Get_Maximum(zField)
			);
		}

		if(	!SG_STR_CMP(pParameter->Get_Identifier(), wxT("LUT_ATTRIB"))
		&&  pParameter->asInt() >= 0 && pParameter->asInt() < Get_Shapes()->Get_Field_Count() )
		{
			TSG_Data_Type	Type	= SG_Data_Type_is_Numeric(Get_Shapes()->Get_Field_Type(pParameter->asInt()))
									? SG_DATATYPE_Double : SG_DATATYPE_String;

			m_Parameters("LUT")->asTable()->Set_Field_Type(LUT_MIN, Type);
			m_Parameters("LUT")->asTable()->Set_Field_Type(LUT_MAX, Type);
		}
	}

	//-----------------------------------------------------
	if( Flags & PARAMETER_CHECK_ENABLE )
	{
		if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("OUTLINE")) )
		{
			pParameters->Get_Parameter("OUTLINE_COLOR")->Set_Enabled(pParameter->asBool());
			pParameters->Get_Parameter("OUTLINE_SIZE" )->Set_Enabled(pParameter->asBool());
		}

		if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("LABEL_ATTRIB")) )
		{
			bool	Value	= pParameter->asInt() < Get_Shapes()->Get_Field_Count();

			pParameters->Get_Parameter("LABEL_ATTRIB_FONT"     )->Set_Enabled(Value);
			pParameters->Get_Parameter("LABEL_ATTRIB_SIZE_TYPE")->Set_Enabled(Value);
			pParameters->Get_Parameter("LABEL_ATTRIB_PREC"     )->Set_Enabled(Value);
			pParameters->Get_Parameter("LABEL_ATTRIB_SIZE"     )->Set_Enabled(Value);
			pParameters->Get_Parameter("LABEL_ATTRIB_SIZE_BY"  )->Set_Enabled(Value);
			pParameters->Get_Parameter("LABEL_ATTRIB_EFFECT"   )->Set_Enabled(Value);
		}

		if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("LABEL_ATTRIB_SIZE_BY")) )
		{
			bool	Value	= pParameter->asInt() >= Get_Shapes()->Get_Field_Count();

			pParameters->Get_Parameter("LABEL_ATTRIB_SIZE")->Set_Enabled(Value);
		}

		if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("LABEL_ATTRIB_EFFECT")) )
		{
			bool	Value	= pParameter->asInt() > 0;

			pParameters->Get_Parameter("LABEL_ATTRIB_EFFECT_COLOR")->Set_Enabled(Value);
		}

		if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("EDIT_SNAP_LIST")) )
		{
			pParameters->Get_Parameter("EDIT_SNAP_DIST")->Set_Enabled(pParameter->asList()->Get_Count() > 0);
		}

		if(	!SG_STR_CMP(pParameters->Get_Identifier(), SG_T("DISPLAY_CHART")) )
		{
			CSG_String	s(pParameter->Get_Identifier());

			if( s.Find(wxT("FIELD_")) == 0 )
			{
				s.Replace(wxT("FIELD_"), wxT("COLOR_"));

				if( pParameters->Get_Parameter(s) )
				{
					pParameters->Get_Parameter(s)->Set_Enabled(pParameter->asBool());
				}
			}
		}
	}

	//-----------------------------------------------------
	return( CWKSP_Layer::On_Parameter_Changed(pParameters, pParameter, Flags) );
}

//---------------------------------------------------------
void CWKSP_Shapes::On_Update_Views(void)
{
	m_pTable->Update_Views();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes::_LUT_Create(void)
{
	int				iField, Method;
	CSG_Colors		*pColors;
	CSG_Table		*pLUT;

	//-----------------------------------------------------
	if( Get_Shapes()->Get_Field_Count() <= 0 || Get_Shapes()->Get_Record_Count() < 1 )
	{
		DLG_Message_Show(_TL("Function failed because no attributes are available"), _TL("Create Lookup Table"));

		return;
	}

	//-----------------------------------------------------
	CSG_String	sFields;

	for(iField=0; iField<Get_Shapes()->Get_Field_Count(); iField++)
	{
		sFields	+= Get_Shapes()->Get_Field_Name(iField);	sFields	+= SG_T("|");
	}

	//-----------------------------------------------------
	static CSG_Parameters	Parameters;

	if( Parameters.Get_Count() != 0 )
	{
		Parameters("FIELD")->asChoice()->Set_Items(sFields);
	}
	else
	{
		Parameters.Create(NULL, _TL("Create Lookup Table"), _TL(""));

		Parameters.Add_Choice(
			NULL, "FIELD"	, _TL("Attribute"),
			_TL(""),
			sFields
		);

		Parameters.Add_Colors(
			NULL, "COLOR"	, _TL("Colors"),
			_TL("")
		)->asColors()->Set_Count(10);

		Parameters.Add_Choice(
			NULL, "METHOD"	, _TL("Classification Method"),
			_TL(""),
			CSG_String::Format(SG_T("%s|%s|%s|"),
				_TL("unique values"),
				_TL("equal intervals"),
				_TL("quantiles")
			), 0
		);
	}

	if( !DLG_Parameters(&Parameters) )
	{
		return;
	}

	//-----------------------------------------------------
	pColors	= Parameters("COLOR" )->asColors();
	iField	= Parameters("FIELD" )->asInt();
	Method	= !SG_Data_Type_is_Numeric(Get_Shapes()->Get_Field_Type(iField)) ? 0	// unique values
			: Parameters("METHOD")->asInt();

	pLUT	= m_Parameters("LUT" )->asTable();
	pLUT	->Del_Records();

	switch( Method )
	{
	//-----------------------------------------------------
	case 0:	// unique values
		{
			TSG_Table_Index_Order	old_Order	= Get_Shapes()->Get_Index_Order(0);
			int						old_Field	= Get_Shapes()->Get_Index_Field(0);

			TSG_Data_Type	Type	= SG_Data_Type_is_Numeric(Get_Shapes()->Get_Field_Type(iField))
									? SG_DATATYPE_Double : SG_DATATYPE_String;

			pLUT->Set_Field_Type(LUT_MIN, Type);
			pLUT->Set_Field_Type(LUT_MAX, Type);

			Get_Shapes()->Set_Index(iField, TABLE_INDEX_Ascending);

			CSG_String	sValue;

			for(int iRecord=0; iRecord<Get_Shapes()->Get_Count(); iRecord++)
			{
				CSG_Table_Record	*pRecord	= Get_Shapes()->Get_Record_byIndex(iRecord);

				if( iRecord == 0 || sValue.Cmp(pRecord->asString(iField)) )
				{
					sValue	= pRecord->asString(iField);

					CSG_Table_Record	*pClass	= pLUT->Add_Record();

					pClass->Set_Value(1, sValue);	// Name
					pClass->Set_Value(2, sValue);	// Description
					pClass->Set_Value(3, sValue);	// Minimum
					pClass->Set_Value(4, sValue);	// Maximum
				}
			}

			pColors->Set_Count(pLUT->Get_Count());

			for(int iClass=0; iClass<pLUT->Get_Count(); iClass++)
			{
				pLUT->Get_Record(iClass)->Set_Value(0, pColors->Get_Color(iClass));
			}

			Get_Shapes()->Set_Index(old_Field, old_Order);
		}
		break;

	//-----------------------------------------------------
	case 1:	// equal intervals
		{
			double	Minimum, Maximum, Interval;

			Interval	= Get_Shapes()->Get_Range  (iField) / (double)pColors->Get_Count();
			Minimum		= Get_Shapes()->Get_Minimum(iField);

			pLUT->Set_Field_Type(LUT_MIN, SG_DATATYPE_Double);
			pLUT->Set_Field_Type(LUT_MAX, SG_DATATYPE_Double);

			for(int iClass=0; iClass<pColors->Get_Count(); iClass++, Minimum+=Interval)
			{
				Maximum	= iClass < pColors->Get_Count() - 1 ? Minimum + Interval : Get_Shapes()->Get_Maximum(iField) + 1.0;

				CSG_String	sValue;	sValue.Printf(SG_T("%s - %s"),
					SG_Get_String(Minimum, -2).c_str(),
					SG_Get_String(Maximum, -2).c_str()
				);

				CSG_Table_Record	*pClass	= pLUT->Add_Record();

				pClass->Set_Value(0, pColors->Get_Color(iClass));
				pClass->Set_Value(1, sValue);	// Name
				pClass->Set_Value(2, sValue);	// Description
				pClass->Set_Value(3, Minimum);	// Minimum
				pClass->Set_Value(4, Maximum);	// Maximum
			}
		}
		break;

	//-----------------------------------------------------
	case 2:	// quantiles
		{
			TSG_Table_Index_Order	old_Order	= Get_Shapes()->Get_Index_Order(0);
			int						old_Field	= Get_Shapes()->Get_Index_Field(0);

			Get_Shapes()->Set_Index(iField, TABLE_INDEX_Ascending);

			pLUT->Set_Field_Type(LUT_MIN, SG_DATATYPE_Double);
			pLUT->Set_Field_Type(LUT_MAX, SG_DATATYPE_Double);

			if( Get_Shapes()->Get_Count() < pColors->Get_Count() )
			{
				pColors->Set_Count(Get_Shapes()->Get_Count());
			}

			double	Minimum, Maximum, Count, iRecord;

			Maximum	= Get_Shapes()->Get_Minimum(iField);
			iRecord	= Count	= Get_Shapes()->Get_Count() / (double)pColors->Get_Count();

			for(int iClass=0; iClass<pColors->Get_Count(); iClass++, iRecord+=Count)
			{
				Minimum	= Maximum;
				Maximum	= iRecord < Get_Shapes()->Get_Count() ? Get_Shapes()->Get_Record_byIndex((int)iRecord)->asDouble(iField) : Get_Shapes()->Get_Maximum(iField) + 1.0;

				CSG_String	sValue;	sValue.Printf(SG_T("%s - %s"),
					SG_Get_String(Minimum, -2).c_str(),
					SG_Get_String(Maximum, -2).c_str()
				);

				CSG_Table_Record	*pClass	= pLUT->Add_Record();

				pClass->Set_Value(0, pColors->Get_Color(iClass));
				pClass->Set_Value(1, sValue);	// Name
				pClass->Set_Value(2, sValue);	// Description
				pClass->Set_Value(3, Minimum);	// Minimum
				pClass->Set_Value(4, Maximum);	// Maximum
			}

			Get_Shapes()->Set_Index(old_Field, old_Order);
		}
		break;
	}

	//-----------------------------------------------------
	DataObject_Changed();

	m_Parameters("COLORS_TYPE")->Set_Value(CLASSIFY_LUT);	// Lookup Table
	m_Parameters("LUT_ATTRIB" )->Set_Value(iField);

	Parameters_Changed();
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Shapes::Get_Value(CSG_Point ptWorld, double Epsilon)
{
	CSG_Shape		*pShape;

	if( (pShape = Get_Shapes()->Get_Shape(ptWorld, Epsilon)) != NULL )
	{
		if( m_iColor >= 0 )
		{
			switch( m_pClassify->Get_Mode() )
			{
			case CLASSIFY_LUT:
				return( m_pClassify->Get_Class_Name_byValue(pShape->asString(m_iColor)) );

			case CLASSIFY_METRIC:	default:
				return( pShape->asString(m_iColor) );
			}
		}
		else
		{
			return( wxString::Format(wxT("%s: %d"), _TL("Index"), pShape->Get_Index() + 1) );
		}
	}

	return( _TL("") );
}

//---------------------------------------------------------
double CWKSP_Shapes::Get_Value_Range(void)
{
	return( m_iColor >= 0 ? Get_Shapes()->Get_Range(m_iColor) : 0.0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Shapes::Get_Name_Attribute(void)
{
	return(	m_iColor < 0 || m_pClassify->Get_Mode() == CLASSIFY_UNIQUE ? SG_T("") : Get_Shapes()->Get_Field_Name(m_iColor) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Shapes::On_Draw(CWKSP_Map_DC &dc_Map, bool bEdit)
{
	int			iShape;
	CSG_Shape	*pShape;

	//-----------------------------------------------------
	if( Get_Extent().Intersects(dc_Map.m_rWorld) != INTERSECTION_None )
	{
		double	Transparency	= m_Parameters("DISPLAY_TRANSPARENCY")->asDouble() / 100.0;

		CWKSP_Map_DC	*pDC	= Transparency > 0.0 ? new CWKSP_Map_DC(dc_Map.m_rWorld, dc_Map.m_rDC, dc_Map.m_Scale, SG_GET_RGB(255, 255, 255)) : NULL;
		CWKSP_Map_DC	&dc		= pDC ? *pDC : dc_Map;

		m_Sel_Color		= Get_Color_asWX(m_Parameters("SEL_COLOR") ->asInt());
		m_Edit_Color	= Get_Color_asWX(m_Parameters("EDIT_COLOR")->asInt());

		_Draw_Initialize(dc);

		//-------------------------------------------------
		if( bEdit && (m_Edit_pShape || Get_Shapes()->Get_Selection_Count() > 0) )
		{
			for(iShape=0; iShape<Get_Shapes()->Get_Count(); iShape++)
			{
				pShape	= Get_Shapes()->Get_Shape(iShape);

				if( !pShape->is_Selected() && dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
				{
					_Draw_Shape(dc, pShape, false);
				}
			}

			//---------------------------------------------
			for(iShape=1; iShape<Get_Shapes()->Get_Selection_Count(); iShape++)
			{
				pShape	= Get_Shapes()->Get_Selection(iShape);

				if( dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
				{
					_Draw_Shape(dc, pShape, true);
				}
			}

			if( m_Edit_pShape )
			{
				_Edit_Shape_Draw(dc);
			}
			else
			{
				pShape	= Get_Shapes()->Get_Selection(0);

				if( dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
				{
					_Draw_Shape(dc, pShape, true);
				}
			}
		}

		//-------------------------------------------------
		else
		{
			for(iShape=0; iShape<Get_Shapes()->Get_Count(); iShape++)
			{
				pShape	= Get_Shapes()->Get_Shape(iShape);

				if( (m_pClassify->Get_Mode() != CLASSIFY_METRIC || m_iColor < 0 || !pShape->is_NoData(m_iColor))
				&&	dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
				{
					_Draw_Shape(dc, pShape, false);
				}
			}

			if( _Chart_is_Valid() )
			{
				for(iShape=0; iShape<Get_Shapes()->Get_Count(); iShape++)
				{
					pShape	= Get_Shapes()->Get_Shape(iShape);

					if( dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
					{
						_Draw_Chart(dc, pShape);
					}
				}
			}
		}

		//-------------------------------------------------
		if( m_iLabel >= 0 )
		{
			int		Size;
			double	dSize;
			wxFont	Font	= Get_Font(m_Parameters("LABEL_ATTRIB_FONT"));

			switch( m_Parameters("LABEL_ATTRIB_SIZE_TYPE")->asInt() )
			{
			case 0:	default:
				dSize	= m_iLabel_Size < 0 ? Font.GetPointSize() : 1.0;
				break;

			case 1:
				dSize	= dc.m_World2DC * m_Parameters("LABEL_ATTRIB_SIZE")->asDouble();
				break;
			}

			dc.dc.SetTextForeground(m_Parameters("LABEL_ATTRIB_FONT")->asColor());

			if( m_iLabel_Size < 0 && (Size = (int)(0.5 + dSize)) > 0 )
			{
				Font.SetPointSize(Size);
				dc.dc.SetFont(Font);

				for(iShape=0; iShape<Get_Shapes()->Get_Count(); iShape++)
				{
					pShape	= Get_Shapes()->Get_Shape(iShape);

					if( dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None )
					{
						_Draw_Label(dc, pShape);
					}
				}
			}
			else
			{
				for(iShape=0; iShape<Get_Shapes()->Get_Count(); iShape++)
				{
					pShape	= Get_Shapes()->Get_Shape(iShape);

					if( dc.m_rWorld.Intersects(pShape->Get_Extent()) != INTERSECTION_None
					&&	(Size = (int)(0.5 + dSize * pShape->asDouble(m_iLabel_Size))) > 0 )
					{
						Font.SetPointSize(Size);
						dc.dc.SetFont(Font);

						_Draw_Label(dc, pShape);
					}
				}
			}
		}

		//-------------------------------------------------
		if( pDC )
		{
			dc_Map.Draw_DC(dc, Transparency);

			delete(pDC);
		}
	}

	//-----------------------------------------------------
	else if( m_Edit_pShape )
	{
		_Edit_Shape_Draw(dc_Map);
	}

	//-----------------------------------------------------
	dc_Map.dc.SetBrush(wxNullBrush);
	dc_Map.dc.SetPen  (wxNullPen);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Parameter * CWKSP_Shapes::_AttributeList_Add(CSG_Parameter *pNode, const CSG_String &Identifier, const CSG_String &Name, const CSG_String &Description)
{
	CSG_Parameter *pParameter;

	pParameter	= m_Parameters.Add_Choice(
		pNode, Identifier, Name, Description,
		CSG_String::Format(SG_T("%s|"), _TL("<default>")), 0
	);

	return( pParameter );
}

//---------------------------------------------------------
void CWKSP_Shapes::_AttributeList_Set(CSG_Parameter *pFields, bool bAddNoField)
{
	if( pFields && pFields->Get_Type() == PARAMETER_TYPE_Choice )
	{
		wxString	s;

		for(int i=0; i<Get_Shapes()->Get_Field_Count(); i++)
		{
			s.Append(wxString::Format(wxT("%s|"), Get_Shapes()->Get_Field_Name(i)));
		}

		if( bAddNoField )
		{
			s.Append(wxString::Format(wxT("%s|"), _TL("<none>")));
		}

		pFields->asChoice()->Set_Items(s);

		if( bAddNoField )
		{
			pFields->Set_Value(Get_Shapes()->Get_Field_Count());
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Parameter * CWKSP_Shapes::_BrushList_Add(CSG_Parameter *pNode, const CSG_String &Identifier, const CSG_String &Name, const CSG_String &Description)
{
	CSG_Parameter *pParameter;

	pParameter	= m_Parameters.Add_Choice(
		pNode, Identifier, Name, Description,
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|%s|"),
			_TL("Opaque"),
			_TL("Transparent"),
			_TL("Backward Diagonal"),
			_TL("Cross Diagonal"),
			_TL("Forward Diagonal"),
			_TL("Cross"),
			_TL("Horizontal"),
			_TL("Vertical")
		), 0
	);

	return( pParameter );
}

//---------------------------------------------------------
int CWKSP_Shapes::_BrushList_Get_Style(int Index)
{
	switch( Index )
	{
	default:
	case 0:	return( wxSOLID				);
	case 1:	return( wxTRANSPARENT		);
	case 2:	return( wxBDIAGONAL_HATCH	);
	case 3:	return( wxCROSSDIAG_HATCH	);
	case 4:	return( wxFDIAGONAL_HATCH	);
	case 5:	return( wxCROSS_HATCH		);
	case 6:	return( wxHORIZONTAL_HATCH	);
	case 7:	return( wxVERTICAL_HATCH	);
	}
}

//---------------------------------------------------------
CSG_Parameter * CWKSP_Shapes::_PenList_Add(CSG_Parameter *pNode, const CSG_String &Identifier, const CSG_String &Name, const CSG_String &Description)
{
	CSG_Parameter *pParameter;

	pParameter	= m_Parameters.Add_Choice(
		pNode, Identifier, Name, Description,
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("Solid"),
			_TL("Dotted"),
			_TL("Long Dashed"),
			_TL("Short Dashed"),
			_TL("Dot And Dash")
		), 0
	);

	return( pParameter );
}

//---------------------------------------------------------
int CWKSP_Shapes::_PenList_Get_Style(int Index)
{
	switch( Index )
	{
	default:
	case 0:	return( wxSOLID );
	case 1:	return( wxDOT );
	case 2:	return( wxLONG_DASH );
	case 3:	return( wxSHORT_DASH );
	case 4:	return( wxDOT_DASH );
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Shapes::_Get_Class_Color(CSG_Shape *pShape, int &Color)
{
	if( pShape && Get_Shapes() && m_iColor >= 0 )
	{
		if( m_pClassify->Get_Mode() == CLASSIFY_LUT && !SG_Data_Type_is_Numeric(Get_Shapes()->Get_Field_Type(m_iColor)) )
		{
			return( m_pClassify->Get_Class_Color_byValue(pShape->asString(m_iColor), Color) );
		}

		return( m_pClassify->Get_Class_Color_byValue(pShape->asDouble(m_iColor), Color) );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Shapes::_Chart_Set_Options(void)
{
	CSG_Parameters	*pChart	= m_Parameters("DISPLAY_CHART")->asParameters();

	pChart->Del_Parameters();
	m_Chart.Clear();

	if( 1 )
	{
		int				i, n;
		CSG_Colors		Colors;
		CSG_String		sFields;
		CSG_Parameter	*pFields, *pField;

		for(i=0, n=0; i<Get_Shapes()->Get_Field_Count(); i++)
		{
			if( Get_Shapes()->Get_Field_Type(i) != SG_DATATYPE_String )
				n++;
		}

		if( n > 0 )
		{
			pChart->Add_Choice(
				NULL, "TYPE"	, _TL("Chart Type"),
				_TL(""),
				CSG_String::Format(SG_T("%s|%s|%s|%s|"),
					_TL("bar"),
					_TL("bar (not outlined)"),
					_TL("pie"),
					_TL("pie (not outlined)")
				), 0
			);

			pChart->Add_Choice(
				NULL, "SIZE_FIELD"		, _TL("Attribute (Size)"),
				_TL(""),
				CSG_String::Format(SG_T("%s|"), _TL("<not set>")), 0
			);

			pChart->Add_Choice(
				NULL, "SIZE_TYPE"		, _TL("Size relates to..."),
				_TL(""),
				CSG_String::Format(SG_T("%s|%s|"),
					_TL("Screen"),
					_TL("Map Units")
				), 0
			);

			pChart->Add_Value(
				NULL, "SIZE_DEFAULT"	, _TL("Default Size"),
				_TL(""),
				PARAMETER_TYPE_Double, 15, 0, true
			);

			pChart->Add_Range(
				NULL, "SIZE_RANGE"		, _TL("Size Range"),
				_TL(""),
				5, 25, 0, true
			);

			pFields	= pChart->Add_Node(NULL, "NODE_FIELDS", _TL("Fields"), _TL(""));

			Colors.Set_Count(n);

			for(i=0, n=0; i<Get_Shapes()->Get_Field_Count(); i++)
			{
				if( Get_Shapes()->Get_Field_Type(i) != SG_DATATYPE_String )
				{
					sFields.Append(CSG_String::Format(SG_T("%s|"), Get_Shapes()->Get_Field_Name(i)));

					pField	= pChart->Add_Value(
						pFields	, CSG_String::Format(SG_T("FIELD_%d"), i), Get_Shapes()->Get_Field_Name(i),
						_TL(""),
						PARAMETER_TYPE_Bool , false
					);

					pChart->Add_Value(
						pField	, CSG_String::Format(SG_T("COLOR_%d"), i), SG_T(""),
						_TL(""),
						PARAMETER_TYPE_Color, Colors.Get_Color(n++)
					);
				}
			}

			sFields.Append(CSG_String::Format(SG_T("%s|"), _TL("<none>")));
			pFields	= pChart->Get_Parameter("SIZE_FIELD");
			pFields->asChoice()->Set_Items(sFields);
			pFields->Set_Value(Get_Shapes()->Get_Field_Count());

			return( true );
		}
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Shapes::_Chart_Get_Options(void)
{
	CSG_Parameters	*pChart	= m_Parameters("DISPLAY_CHART")->asParameters();
	CSG_Parameter	*p;

	m_Chart.Clear();
	m_Chart_sField=-1;

	if( pChart->Get_Parameter("NODE_FIELDS") )
	{
		for(int i=0, n=0; i<Get_Shapes()->Get_Field_Count(); i++)
		{
			if(	(p = pChart->Get_Parameter(CSG_String::Format(SG_T("FIELD_%d"), i))) != NULL )
			{
				if( pChart->Get_Parameter("SIZE_FIELD")->asInt() == n++ )
					m_Chart_sField	= i;

				if( p->asBool() && (p = pChart->Get_Parameter(CSG_String::Format(SG_T("COLOR_%d"), i))) != NULL )
					m_Chart.Add(i, p->asColor());
			}
		}

		m_Chart_Type	= pChart->Get_Parameter("TYPE")			->asInt();
		m_Chart_sType	= pChart->Get_Parameter("SIZE_TYPE")	->asInt();
		m_Chart_sSize	= m_Chart_sField < 0
						? pChart->Get_Parameter("SIZE_DEFAULT")	->asDouble()
						: pChart->Get_Parameter("SIZE_RANGE")	->asRange()->Get_LoVal();
		m_Chart_sRange	= pChart->Get_Parameter("SIZE_RANGE")	->asRange()->Get_HiVal() - m_Chart_sSize;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CWKSP_Shapes::_Draw_Chart(CWKSP_Map_DC &dc_Map, CSG_Shape *pShape)
{
	if( _Chart_is_Valid() )
	{
		int			s;
		double		dSize	= m_Chart_sSize;
		TSG_Point_Int	p;

		if( m_Chart_sField >= 0 )
		{
			double	range	= Get_Shapes()->Get_Range(m_Chart_sField);

			if( range > 0.0 )
			{
				dSize	+= m_Chart_sRange * ((pShape->asDouble(m_Chart_sField) - Get_Shapes()->Get_Minimum(m_Chart_sField)) / range);
			}
		}

		s	= (int)(dSize * (m_Chart_sType == 1 ? dc_Map.m_World2DC : dc_Map.m_Scale));

		switch( pShape->Get_Type() )
		{
		default:					p	= dc_Map.World2DC(pShape->Get_Extent().Get_Center());			break;
		case SHAPE_TYPE_Polygon:	p	= dc_Map.World2DC(((CSG_Shape_Polygon *)pShape)->Get_Centroid());	break;
		}

		dc_Map.dc.SetPen(*wxBLACK_PEN);

		switch( m_Chart_Type )
		{
		case 0:	_Draw_Chart_Bar(dc_Map, pShape,  true, p.x, p.y, (int)(0.8 * s), s);	break; // bar outlined
		case 1:	_Draw_Chart_Bar(dc_Map, pShape, false, p.x, p.y, (int)(0.8 * s), s);	break; // bar
		case 2:	_Draw_Chart_Pie(dc_Map, pShape,  true, p.x, p.y, (int)(1.0 * s));	break; // pie outlined
		case 3:	_Draw_Chart_Pie(dc_Map, pShape, false, p.x, p.y, (int)(1.0 * s));	break; // pie
		}
	}
}

//---------------------------------------------------------
void CWKSP_Shapes::_Draw_Chart_Pie(CWKSP_Map_DC &dc_Map, CSG_Table_Record *pRecord, bool bOutline, int x, int y, int size)
{
	int		i, ix, iy, jx, jy;
	double	d, sum;

	for(i=0, sum=0.0; i<m_Chart.Get_Count(); i++)
	{
		sum	+= fabs(pRecord->asDouble(m_Chart[i].x));
	}

	if( sum > 0.0 )
	{
		sum	= M_PI_360 / sum;

		for(i=0, jx=x, jy=y-size, d=0.0; i<m_Chart.Get_Count(); i++)
		{
			if( !bOutline )
			dc_Map.dc.SetPen	(wxPen  (Get_Color_asWX(m_Chart[i].y)));
			dc_Map.dc.SetBrush	(wxBrush(Get_Color_asWX(m_Chart[i].y)));

			d	+= sum * fabs(pRecord->asDouble(m_Chart[i].x));

			ix	= jx;
			iy	= jy;
			jx	= x - (int)(size * sin(-d));
			jy	= y - (int)(size * cos(-d));

			dc_Map.dc.DrawArc(jx, jy, ix, iy, x, y);
		}
	}
}

//---------------------------------------------------------
void CWKSP_Shapes::_Draw_Chart_Bar(CWKSP_Map_DC &dc_Map, CSG_Table_Record *pRecord, bool bOutline, int x, int y, int sx, int sy)
{
	int		i;
	double	d, dx, dy, max, ix;

	y	+= sy / 2;

	for(i=1, max=fabs(pRecord->asDouble(m_Chart[0].x)); i<m_Chart.Get_Count(); i++)
	{
		if( (d = fabs(pRecord->asDouble(m_Chart[i].x))) > max )
			max	= d;
	}

	if( max != 0.0 )
	{
		max	= sy / max;
		dx	= sx / (double)m_Chart.Get_Count();

		for(i=0, ix=x-sx/2.0; i<m_Chart.Get_Count(); i++, ix+=dx)
		{
			if( !bOutline )
			dc_Map.dc.SetPen	(wxPen  (Get_Color_asWX(m_Chart[i].y)));
			dc_Map.dc.SetBrush	(wxBrush(Get_Color_asWX(m_Chart[i].y)));

			dy	= -pRecord->asDouble(m_Chart[i].x) * max;

			dc_Map.dc.DrawRectangle((int)ix, y, (int)dx, (int)dy);
		}

		if( !bOutline )
		{
			dc_Map.dc.SetPen(*wxBLACK_PEN);
			dc_Map.dc.DrawLine(x - sx / 2, y, x + sx / 2, y);
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
