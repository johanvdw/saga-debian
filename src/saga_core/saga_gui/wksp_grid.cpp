
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
//                    WKSP_Grid.cpp                      //
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
#include <wx/dc.h>
#include <wx/dcmemory.h>
#include <wx/image.h>
#include <wx/filename.h>

#include "res_commands.h"
#include "res_dialogs.h"

#include "helper.h"
#include "dc_helper.h"

#include "active.h"
#include "active_attributes.h"

#include "wksp_map_control.h"

#include "wksp_layer_classify.h"
#include "wksp_layer_legend.h"

#include "wksp_data_manager.h"
#include "wksp_grid_manager.h"
#include "wksp_grid.h"

#include "view_scatterplot.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Grid::CWKSP_Grid(CSG_Grid *pGrid)
	: CWKSP_Layer(pGrid)
{
	m_pGrid			= pGrid;

	m_pOverlay[0]	= NULL;
	m_pOverlay[1]	= NULL;

	m_Sel_xN		= -1;

	Create_Parameters();
}

//---------------------------------------------------------
CWKSP_Grid::~CWKSP_Grid(void)
{
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define DESC_ADD_STR(label, value)	s.Append(wxString::Format(wxT("<tr><td valign=\"top\">%s</td><td valign=\"top\">%s</td></tr>"), label, value))
#define DESC_ADD_INT(label, value)	s.Append(wxString::Format(wxT("<tr><td valign=\"top\">%s</td><td valign=\"top\">%d</td></tr>"), label, value))
#define DESC_ADD_FLT(label, value)	s.Append(wxString::Format(wxT("<tr><td valign=\"top\">%s</td><td valign=\"top\">%s</td></tr>"), label, SG_Get_String(value, -2).c_str()))

//---------------------------------------------------------
wxString CWKSP_Grid::Get_Description(void)
{
	wxString	s;

	//-----------------------------------------------------
	s	+= wxString::Format(wxT("<b>%s</b>"), LNG("[CAP] Grid"));

	s	+= wxT("<table border=\"0\">");

	DESC_ADD_STR(LNG("[CAP] Name")					, m_pGrid->Get_Name());
	DESC_ADD_STR(LNG("[CAP] Description")			, m_pGrid->Get_Description());
	DESC_ADD_STR(LNG("[CAP] File")					, m_pGrid->Get_File_Name());
	DESC_ADD_STR(LNG("[CAP] Projection")			, m_pGrid->Get_Projection().Get_Description().c_str());
	DESC_ADD_FLT(LNG("[CAP] West")					, m_pGrid->Get_XMin());
	DESC_ADD_FLT(LNG("[CAP] East")					, m_pGrid->Get_XMax());
	DESC_ADD_FLT(LNG("[CAP] West-East")				, m_pGrid->Get_XRange());
	DESC_ADD_FLT(LNG("[CAP] South")					, m_pGrid->Get_YMin());
	DESC_ADD_FLT(LNG("[CAP] North")					, m_pGrid->Get_YMax());
	DESC_ADD_FLT(LNG("[CAP] South-North")			, m_pGrid->Get_YRange());
	DESC_ADD_FLT(LNG("[CAP] Cell Size")				, m_pGrid->Get_Cellsize());
	DESC_ADD_INT(LNG("[CAP] Number of Columns")		, m_pGrid->Get_NX());
	DESC_ADD_INT(LNG("[CAP] Number of Rows")		, m_pGrid->Get_NY());
	DESC_ADD_INT(LNG("[CAP] Number of Cells")		, m_pGrid->Get_NCells());
	DESC_ADD_INT(LNG("[CAP] No Data Cells")			, m_pGrid->Get_NoData_Count());
	DESC_ADD_STR(LNG("[CAP] Value Type")			, SG_Data_Type_Get_Name(m_pGrid->Get_Type()));
	DESC_ADD_FLT(LNG("[CAP] Value Minimum")			, m_pGrid->Get_ZMin());
	DESC_ADD_FLT(LNG("[CAP] Value Maximum")			, m_pGrid->Get_ZMax());
	DESC_ADD_FLT(LNG("[CAP] Value Range")			, m_pGrid->Get_ZRange());
	DESC_ADD_STR(LNG("[CAP] No Data Value")			, m_pGrid->Get_NoData_Value() < m_pGrid->Get_NoData_hiValue() ? CSG_String::Format(SG_T("%f - %f"), m_pGrid->Get_NoData_Value(), m_pGrid->Get_NoData_hiValue()).c_str() : SG_Get_String(m_pGrid->Get_NoData_Value(), -2).c_str());
	DESC_ADD_FLT(LNG("[CAP] Arithmetic Mean")		, m_pGrid->Get_ArithMean(true));
	DESC_ADD_FLT(LNG("[CAP] Standard Deviation")	, m_pGrid->Get_StdDev(true));
	DESC_ADD_FLT(LNG("[CAP] Memory Size [MB]")		, (double)(m_pGrid->Get_NCells() * m_pGrid->Get_nValueBytes()) / N_MEGABYTE_BYTES);

	if( m_pGrid->is_Compressed() )
	{
		DESC_ADD_FLT(LNG("[CAP] Memory Compression")	, 100.0 * m_pGrid->Get_Compression_Ratio());
	}

	if( m_pGrid->is_Cached() )
	{
		DESC_ADD_FLT(LNG("[CAP] File Cache [MB]")		, m_pGrid->Get_Buffer_Size() / (double)N_MEGABYTE_BYTES);
	}

	s	+= wxT("</table>");

	//-----------------------------------------------------
//	s.Append(wxString::Format(wxT("<hr><b>%s</b><font size=\"-1\">"), LNG("[CAP] Data History")));
//	s.Append(m_pGrid->Get_History().Get_HTML());
//	s.Append(wxString::Format(wxT("</font")));

	return( s );
}

//---------------------------------------------------------
wxMenu * CWKSP_Grid::Get_Menu(void)
{
	wxMenu	*pMenu, *pSubMenu;

	pMenu		= new wxMenu(m_pGrid->Get_Name());

	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_WKSP_ITEM_CLOSE);
	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_GRIDS_SAVE);
	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_GRIDS_SAVEAS);
	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_GRIDS_SAVEAS_IMAGE);
	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_GRIDS_SHOW);

	pMenu->AppendSeparator();

	CMD_Menu_Add_Item(pMenu		, true , ID_CMD_GRIDS_HISTOGRAM);

	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_GRIDS_SCATTERPLOT);
	CMD_Menu_Add_Item(pMenu		, false, ID_CMD_WKSP_ITEM_SETTINGS_COPY);

	pSubMenu	= new wxMenu(LNG("[MNU] Classificaton"));
	CMD_Menu_Add_Item(pSubMenu	, false, ID_CMD_GRIDS_EQUALINTERVALS);
	CMD_Menu_Add_Item(pSubMenu	, false, ID_CMD_GRIDS_SET_LUT);
	CMD_Menu_Add_Item(pSubMenu	, false, ID_CMD_GRIDS_RANGE_MINMAX);
	CMD_Menu_Add_Item(pSubMenu	, false, ID_CMD_GRIDS_RANGE_STDDEV150);
	CMD_Menu_Add_Item(pSubMenu	, false, ID_CMD_GRIDS_RANGE_STDDEV200);

	pMenu->Append(ID_CMD_WKSP_FIRST, LNG("[MNU] Classification"), pSubMenu);

	return( pMenu );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Grid::On_Command(int Cmd_ID)
{
	switch( Cmd_ID )
	{
	default:
		return( CWKSP_Layer::On_Command(Cmd_ID) );

	case ID_CMD_GRIDS_SAVEAS_IMAGE:
		_Save_Image();
		break;

	case ID_CMD_GRIDS_HISTOGRAM:
		Histogram_Toggle();
		break;

	case ID_CMD_GRIDS_SCATTERPLOT:
		Add_ScatterPlot(Get_Grid());
		break;

	case ID_CMD_GRIDS_EQUALINTERVALS:
		m_pClassify->Metric2EqualElements();
		break;

	case ID_CMD_GRIDS_RANGE_MINMAX:
		Set_Color_Range(
			m_pGrid->Get_ZMin(true),
			m_pGrid->Get_ZMax(true)
		);
		break;

	case ID_CMD_GRIDS_RANGE_STDDEV150:
		Set_Color_Range(
			m_pGrid->Get_ArithMean(true) - 1.5 * m_pGrid->Get_StdDev(true),
			m_pGrid->Get_ArithMean(true) + 1.5 * m_pGrid->Get_StdDev(true)
		);
		break;

	case ID_CMD_GRIDS_RANGE_STDDEV200:
		Set_Color_Range(
			m_pGrid->Get_ArithMean(true) - 2.0 * m_pGrid->Get_StdDev(true),
			m_pGrid->Get_ArithMean(true) + 2.0 * m_pGrid->Get_StdDev(true)
		);
		break;

	case ID_CMD_GRIDS_SET_LUT:
		_LUT_Create();
		break;
	}

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Grid::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		return( CWKSP_Layer::On_Command_UI(event) );

	case ID_CMD_GRIDS_HISTOGRAM:
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
void CWKSP_Grid::On_Create_Parameters(void)
{
	//-----------------------------------------------------
	// General...

	m_Parameters.Add_String(
		m_Parameters("NODE_GENERAL")	, "OBJECT_DESC"				, LNG("[CAP] Description"),
		LNG(""),
		m_pGrid->Get_Description(), true
	);

	m_Parameters.Add_String(
		m_Parameters("NODE_GENERAL")	, "GENERAL_Z_UNIT"			, LNG("[CAP] Unit"),
		LNG(""),
		m_pGrid->Get_Unit()
	);

	m_Parameters.Add_Value(
		m_Parameters("NODE_GENERAL")	, "GENERAL_Z_FACTOR"		, LNG("[CAP] Z-Factor"),
		LNG(""),
		PARAMETER_TYPE_Double
	);


	//-----------------------------------------------------
	// Memory...

	m_Parameters.Add_Node(
		NULL							, "NODE_MEMORY"				, LNG("[CAP] Memory"),
		LNG("")
	);

	m_Parameters.Add_Choice(
		m_Parameters("NODE_MEMORY")		, "MEMORY_MODE"				, LNG("[CAP] Memory Handling"),
		LNG(""),
		wxString::Format(wxT("%s|%s|%s|"),
			LNG("[VAL] Normal"),
			LNG("[VAL] RTL Compression"),
			LNG("[VAL] File Cache")
		), 0
	);

	m_Parameters.Add_Value(
		m_Parameters("NODE_MEMORY")		, "MEMORY_BUFFER_SIZE"		, LNG("[CAP] Buffer Size MB"),
		LNG(""),
		PARAMETER_TYPE_Double
	);


	//-----------------------------------------------------
	// Display...

	m_Parameters.Add_Value(
		m_Parameters("NODE_DISPLAY")	, "DISPLAY_TRANSPARENCY"	, LNG("[CAP] Transparency [%]"),
		LNG(""),
		PARAMETER_TYPE_Double, 0.0, 0.0, true, 100.0, true
	);

	m_Parameters.Add_Choice(
		m_Parameters("NODE_DISPLAY")	, "DISPLAY_INTERPOLATION"	, LNG("[CAP] Interpolation"),
		LNG(""),
		wxString::Format(wxT("%s|%s|%s|%s|%s|"),
			LNG("[VAL] None"),
			LNG("[VAL] Bilinear"),
			LNG("[VAL] Inverse Distance"),
			LNG("[VAL] Bicubic Spline"),
			LNG("[VAL] B-Spline")
		), 0
	);


	//-----------------------------------------------------
	// Classification...

	((CSG_Parameter_Choice *)m_Parameters("COLORS_TYPE")->Get_Data())->Set_Items(
		wxString::Format(wxT("%s|%s|%s|%s|%s|%s|"),
			LNG("[VAL] Unique Symbol"),
			LNG("[VAL] Lookup Table"),
			LNG("[VAL] Graduated Color"),
			LNG("[VAL] RGB"),
			LNG("[VAL] Shade"),
			LNG("[VAL] RGB Overlay")
		)
	);

	m_Parameters("COLORS_TYPE")->Set_Value(CLASSIFY_METRIC);

	//-----------------------------------------------------
	m_Parameters.Add_Node(
		m_Parameters("NODE_COLORS")		, "NODE_SHADE"		, LNG("[CAP] Shade"),
		LNG("")
	);

	m_Parameters.Add_Choice(
		m_Parameters("NODE_SHADE")		, "SHADE_MODE"		, LNG("[CAP] Coloring"),
		LNG(""),
		wxString::Format(wxT("%s|%s|%s|%s|%s|%s|%s|%s|"),
			LNG("bright - dark"),
			LNG("dark - bright"),
			LNG("white - cyan"),
			LNG("cyan - white"),
			LNG("white - magenta"),
			LNG("magenta - white"),
			LNG("white - yellow"),
			LNG("yellow - white")
		), 0
	);

	//-----------------------------------------------------
	m_Parameters.Add_Node(
		m_Parameters("NODE_COLORS")		, "NODE_OVERLAY"	, LNG("[CAP] RGB Overlay"),
		LNG("")
	);

	m_Parameters.Add_Choice(
		m_Parameters("NODE_OVERLAY")	, "OVERLAY_MODE"	, LNG("[CAP] Coloring"),
		LNG(""),
		wxString::Format(wxT("%s|%s|%s|%s|%s|%s|"),
			LNG("red=this, green=1, blue=2"),
			LNG("red=this, green=2, blue=1"),
			LNG("red=1, green=this, blue=2"),
			LNG("red=2, green=this, blue=1"),
			LNG("red=1, green=2, blue=this"),
			LNG("red=2, green=1, blue=this")
		), 0
	);

	m_Parameters.Add_Grid(
		m_Parameters("NODE_OVERLAY")	, "OVERLAY_1"		, LNG("[CAP] Overlay 1"),
		LNG(""),
		PARAMETER_INPUT_OPTIONAL, false
	);

	m_Parameters.Add_Grid(
		m_Parameters("NODE_OVERLAY")	, "OVERLAY_2"		, LNG("[CAP] Overlay 2"),
		LNG(""),
		PARAMETER_INPUT_OPTIONAL, false
	);


	//-----------------------------------------------------
	// Cell Values...

	m_Parameters.Add_Node(
		NULL							, "NODE_VALUES"		, LNG("[CAP] Display: Cell Values"),
		LNG("")
	);

	m_Parameters.Add_Value(
		m_Parameters("NODE_VALUES")		, "VALUES_SHOW"		, LNG("[CAP] Show"),
		LNG(""),
		PARAMETER_TYPE_Bool, true
	);

	m_Parameters.Add_Font(
		m_Parameters("NODE_VALUES")		, "VALUES_FONT"		, LNG("[CAP] Font"),
		LNG("")
	)->asFont()->SetFamily(wxDECORATIVE);

	m_Parameters.Add_Value(
		m_Parameters("NODE_VALUES")		, "VALUES_SIZE"		, LNG("[CAP] Size"),
		LNG(""),
		PARAMETER_TYPE_Double, 15, 0, true , 100.0, true
	);

	m_Parameters.Add_Value(
		m_Parameters("NODE_VALUES")		, "VALUES_DECIMALS"	, LNG("[CAP] Decimals"),
		LNG(""),
		PARAMETER_TYPE_Int, 2
	);


	//-----------------------------------------------------
	DataObject_Changed((CSG_Parameters *)NULL);
}

//---------------------------------------------------------
void CWKSP_Grid::On_DataObject_Changed(void)
{
	//-----------------------------------------------------
	m_Parameters("GENERAL_Z_UNIT")			->Set_Value((void *)m_pGrid->Get_Unit());
	m_Parameters("GENERAL_Z_FACTOR")		->Set_Value(m_pGrid->Get_ZFactor());

	//-----------------------------------------------------
	m_Parameters("MEMORY_MODE")				->Set_Value(
		m_pGrid->is_Compressed() ? 1 : (m_pGrid->is_Cached() ? 2 : 0)
	);

	m_Parameters("MEMORY_BUFFER_SIZE")		->Set_Value(
		(double)m_pGrid->Get_Buffer_Size() / N_MEGABYTE_BYTES
	);
}

//---------------------------------------------------------
void CWKSP_Grid::On_Parameters_Changed(void)
{
	//-----------------------------------------------------
	m_pGrid->Set_Unit		(m_Parameters("GENERAL_Z_UNIT")		->asString());
	m_pGrid->Set_ZFactor	(m_Parameters("GENERAL_Z_FACTOR")	->asDouble());

	//-----------------------------------------------------
	m_pOverlay[0]	= g_pData->Get_Grids()->Get_Grid(m_Parameters("OVERLAY_1")->asGrid());
	m_pOverlay[1]	= g_pData->Get_Grids()->Get_Grid(m_Parameters("OVERLAY_2")->asGrid());
	m_bOverlay		= m_Parameters("COLORS_TYPE")->asInt() == 5;

	m_pClassify->Set_Shade_Mode(m_Parameters("SHADE_MODE")->asInt());

	//-----------------------------------------------------
	switch( m_Parameters("MEMORY_MODE")->asInt() )
	{
	case 0:
		if( m_pGrid->is_Compressed() )
		{
			m_pGrid->Set_Compression(false);
		}
		else if( m_pGrid->is_Cached() )
		{
			m_pGrid->Set_Cache(false);
		}
		break;

	case 1:
		if( !m_pGrid->is_Compressed() )
		{
			m_pGrid->Set_Compression(true);
		}
		break;

	case 2:
		if( !m_pGrid->is_Cached() )
		{
			m_pGrid->Set_Cache(true);
		}
		break;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CWKSP_Grid::_LUT_Create(void)
{
	static CSG_Parameters	Parameters;

	if( Parameters.Get_Count() == 0 )
	{
		Parameters.Create(NULL, LNG("Create Lookup Table"), LNG(""));
		Parameters.Add_Colors(NULL, "COLOR"	, LNG("Colors")	, LNG(""));
	}

	//-----------------------------------------------------
	if( DLG_Parameters(&Parameters) )
	{
		int					i, n, x, y;
		double				dValue, dStep, eValue;
		CSG_String			sValue;
		CSG_Colors			*pColors;
		CSG_Table_Record	*pRecord_LUT;
		CSG_Table			*pLUT;

		//-------------------------------------------------
		pColors	= Parameters("COLOR")->asColors();

		pLUT	= m_Parameters("LUT")->asTable();
		pLUT	->Del_Records();

		for(i=0, n=0; i<m_pGrid->Get_NCells() && n < pColors->Get_Count() && PROGRESSBAR_Set_Position(i, m_pGrid->Get_NCells()); i++)
		{
			if( m_pGrid->Get_Sorted(i, x, y) && (n == 0 || dValue != m_pGrid->asDouble(x, y)) )
			{
				dValue		= m_pGrid->asDouble(x, y);
				n++;
			}
		}

		//-------------------------------------------------
		if( n >= pColors->Get_Count() )
		{
			dValue	= m_pGrid->Get_ZMin();
			dStep	= m_pGrid->Get_ZRange() / pColors->Get_Count();

			for(i=0; i<pColors->Get_Count(); i++)
			{
				eValue		 = dValue;
				dValue		+= dStep;
				sValue		 = SG_Get_String(eValue, SG_Get_Significant_Decimals(eValue), false);
				sValue		+= SG_T(" - ");
				sValue		+= SG_Get_String(dValue, SG_Get_Significant_Decimals(dValue), false);

				pRecord_LUT	= pLUT->Add_Record();
				pRecord_LUT	->Set_Value(0, pColors->Get_Color(i));
				pRecord_LUT	->Set_Value(1, sValue);			// Name
				pRecord_LUT	->Set_Value(2, sValue);			// Description
				pRecord_LUT	->Set_Value(3, eValue);			// Minimum
				pRecord_LUT	->Set_Value(4, dValue);			// Maximum
			}
		}
		else
		{
			pColors->Set_Count(n);

			for(i=0, n=0; i<m_pGrid->Get_NCells() && PROGRESSBAR_Set_Position(i, m_pGrid->Get_NCells()); i++)
			{
				if( m_pGrid->Get_Sorted(i, x, y) && (pLUT->Get_Record_Count() == 0 || dValue != m_pGrid->asDouble(x, y)) )
				{
					dValue		= m_pGrid->asDouble(x, y);
					sValue		= SG_Get_String(dValue, SG_Get_Significant_Decimals(dValue), false);

					pRecord_LUT	= pLUT->Add_Record();
					pRecord_LUT	->Set_Value(0, pColors->Get_Color(n++));
					pRecord_LUT	->Set_Value(1, sValue);		// Name
					pRecord_LUT	->Set_Value(2, sValue);		// Description
					pRecord_LUT	->Set_Value(3, dValue);		// Minimum
					pRecord_LUT	->Set_Value(4, dValue);		// Maximum
				}
			}
		}

		PROGRESSBAR_Set_Position(0);

		DataObject_Changed();

		m_Parameters("COLORS_TYPE")		->Set_Value(1);		// Lookup Table

		Parameters_Changed();
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxString CWKSP_Grid::Get_Value(CSG_Point ptWorld, double Epsilon)
{
	double		Value;
	wxString	s;

	if( m_pGrid->Get_Value(ptWorld, Value, GRID_INTERPOLATION_NearestNeighbour, true) )
	{
		switch( m_pClassify->Get_Mode() )
		{
		case CLASSIFY_LUT:
			s	= m_pClassify->Get_Class_Name_byValue(Value);
			break;

		case CLASSIFY_METRIC:	default:
		case CLASSIFY_SHADE:
			switch( m_pGrid->Get_Type() )
			{
			case SG_DATATYPE_Byte:
			case SG_DATATYPE_Char:
			case SG_DATATYPE_Word:
			case SG_DATATYPE_Short:
			case SG_DATATYPE_DWord:
			case SG_DATATYPE_Int:
				s.Printf(wxT("%d%s"), (int)Value, m_pGrid->Get_Unit());
				break;

			case SG_DATATYPE_Float:	default:
			case SG_DATATYPE_Double:
				s.Printf(wxT("%f%s"), Value, m_pGrid->Get_Unit());
				break;
			}
			break;

		case CLASSIFY_RGB:
			s.Printf(wxT("R%03d G%03d B%03d"), SG_GET_R((int)Value), SG_GET_G((int)Value), SG_GET_B((int)Value));
			break;
		}
	}

	return( s );
}

//---------------------------------------------------------
double CWKSP_Grid::Get_Value_Range(void)
{
	return( m_pGrid->Get_ZRange() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Grid::On_Edit_On_Key_Down(int KeyCode)
{
	switch( KeyCode )
	{
	default:
		return( false );

	case WXK_DELETE:
		return( _Edit_Del_Selection() );
	}
}

//---------------------------------------------------------
#define SELECTION_MAX	20

//---------------------------------------------------------
bool CWKSP_Grid::On_Edit_On_Mouse_Up(CSG_Point Point, double ClientToWorld, int Key)
{
	int					x, y;
	CSG_Table_Record	*pRecord;
	CSG_Rect			rWorld(m_Edit_Mouse_Down, Point);

	m_Sel_xOff	= m_pGrid->Get_System().Get_xWorld_to_Grid(rWorld.Get_XMin());
	if( m_Sel_xOff < 0 )
		m_Sel_xOff	= 0;
	m_Sel_xN	= m_pGrid->Get_System().Get_xWorld_to_Grid(rWorld.Get_XMax());
	if( m_Sel_xN >= m_pGrid->Get_NX() )
		m_Sel_xN	= m_pGrid->Get_NX() - 1;
	m_Sel_xN	= 1 + m_Sel_xN - m_Sel_xOff;

	m_Sel_yOff	= m_pGrid->Get_System().Get_yWorld_to_Grid(rWorld.Get_YMin());
	if( m_Sel_yOff < 0 )
		m_Sel_yOff	= 0;
	m_Sel_yN	= m_pGrid->Get_System().Get_yWorld_to_Grid(rWorld.Get_YMax());
	if( m_Sel_yN >= m_pGrid->Get_NY() )
		m_Sel_yN	= m_pGrid->Get_NY() - 1;
	m_Sel_yN	= 1 + m_Sel_yN - m_Sel_yOff;

	m_Edit_Attributes.Destroy();

	if( m_Sel_xN < 1 || m_Sel_yN < 1 )
	{
		m_Sel_xN	= -1;
	}
	else
	{
		if( m_Sel_xN > SELECTION_MAX )
		{
			m_Sel_xOff	+= (m_Sel_xN - SELECTION_MAX) / 2;
			m_Sel_xN	= SELECTION_MAX;
		}

		if( m_Sel_yN > SELECTION_MAX )
		{
			m_Sel_yOff	+= (m_Sel_yN - SELECTION_MAX) / 2;
			m_Sel_yN	= SELECTION_MAX;
		}

		for(x=0; x<m_Sel_xN; x++)
		{
			m_Edit_Attributes.Add_Field(wxString::Format(wxT("%d"), x + 1), SG_DATATYPE_Double);
		}

		for(y=0; y<m_Sel_yN; y++)
		{
			pRecord	= m_Edit_Attributes.Add_Record();

			for(x=0; x<m_Sel_xN; x++)
			{
				pRecord->Set_Value(x, m_pGrid->asDouble(m_Sel_xOff + x, m_Sel_yOff + m_Sel_yN - 1 - y, false));
			}
		}
	}

	g_pACTIVE->Get_Attributes()->Set_Attributes();

	Update_Views(true);

	return( true );
}

//---------------------------------------------------------
bool CWKSP_Grid::On_Edit_Set_Attributes(void)
{
	int				x, y;
	CSG_Table_Record	*pRecord;

	if( m_Sel_xN >= 0 )
	{
		for(y=0; y<m_Sel_yN; y++)
		{
			pRecord	= m_Edit_Attributes.Get_Record(y);

			for(x=0; x<m_Sel_xN; x++)
			{
				m_pGrid->Set_Value(m_Sel_xOff + x, m_Sel_yOff + m_Sel_yN - 1 - y, pRecord->asDouble(x));
			}
		}

		Update_Views(true);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
TSG_Rect CWKSP_Grid::On_Edit_Get_Extent(void)
{
	if( m_Sel_xN >= 0 )
	{
		return( CSG_Rect(
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_xGrid_to_World(m_Sel_xOff),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_yGrid_to_World(m_Sel_yOff),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_xGrid_to_World(m_Sel_xOff + m_Sel_xN),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_yGrid_to_World(m_Sel_yOff + m_Sel_yN))
		);
	}

	return( m_pGrid->Get_Extent().m_rect );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Grid::_Edit_Del_Selection(void)
{
	int		x, y;

	if( m_Sel_xN >= 0 && DLG_Message_Confirm(LNG("[DLG] Set selected values to no data."), LNG("[CAP] Delete")) )
	{
		for(y=m_Sel_yOff; y<m_Sel_yOff + m_Sel_yN; y++)
		{
			for(x=m_Sel_xOff; x<m_Sel_xOff + m_Sel_xN; x++)
			{
				m_pGrid->Set_NoData(x, y);
			}
		}

		Update_Views(false);

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
bool CWKSP_Grid::Fit_Color_Range(CSG_Rect rWorld)
{
	int		x, y, xMin, yMin, xMax, yMax;
	double	z, zMin, zMax;

	if( rWorld.Intersect(Get_Extent()) )
	{
		xMin	= m_pGrid->Get_System().Get_xWorld_to_Grid(rWorld.Get_XMin());
		yMin	= m_pGrid->Get_System().Get_yWorld_to_Grid(rWorld.Get_YMin());
		xMax	= m_pGrid->Get_System().Get_xWorld_to_Grid(rWorld.Get_XMax());
		yMax	= m_pGrid->Get_System().Get_yWorld_to_Grid(rWorld.Get_YMax());
		zMin	= 1.0;
		zMax	= 0.0;

		for(y=yMin; y<=yMax; y++)
		{
			for(x=xMin; x<=xMax; x++)
			{
				if( m_pGrid->is_InGrid(x, y) )
				{
					z	= m_pGrid->asDouble(x, y);

					if( zMin > zMax )
					{
						zMin	= zMax	= z;
					}
					else if( z < zMin )
					{
						zMin	= z;
					}
					else if( z > zMax )
					{
						zMax	= z;
					}
				}
			}
		}

		return( Set_Color_Range(zMin, zMax) );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CWKSP_Grid::asImage(CSG_Grid *pImage)
{
	int			x, y;
	wxBitmap	BMP;

	if( pImage && Get_Image_Grid(BMP) )
	{
		wxImage	IMG(BMP.ConvertToImage());

		pImage->Create(m_pGrid, SG_DATATYPE_Int);

		for(y=0; y<pImage->Get_NY() && PROGRESSBAR_Set_Position(y, pImage->Get_NY()); y++)
		{
			for(x=0; x<pImage->Get_NX(); x++)
			{
				pImage->Set_Value(x, y, SG_GET_RGB(IMG.GetRed(x, y), IMG.GetGreen(x, y), IMG.GetBlue(x, y)));
			}
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CWKSP_Grid::_Save_Image(void)
{
	int				type;
	wxString		file;
	wxBitmap		BMP;
	CSG_File		Stream;
	CSG_Parameters	Parms;

	//-----------------------------------------------------
	Parms.Set_Name(LNG("[CAP] Save Grid as Image..."));

	Parms.Add_Value(
		NULL	, "WORLD"	, LNG("Save Georeference"),
		wxT(""),
		PARAMETER_TYPE_Bool, 1
	);

	Parms.Add_Value(
		NULL	, "LG"	, LNG("Legend: Save"),
		wxT(""),
		PARAMETER_TYPE_Bool, 1
	);

	Parms.Add_Value(
		NULL	, "LZ"	, LNG("Legend: Zoom"),
		wxT(""),
		PARAMETER_TYPE_Double, 1.0, 0, true
	);

	//-----------------------------------------------------
	if( DLG_Image_Save(file, type) && DLG_Parameters(&Parms) )
	{
		if( Get_Image_Grid(BMP) )
		{
			BMP.SaveFile(file, (wxBitmapType)type);
		}

		if( Parms("LG")->asBool() && Get_Image_Legend(BMP, Parms("LZ")->asDouble()) )
		{
			wxFileName	fn(file);
			fn.SetName(wxString::Format(wxT("%s_legend"), fn.GetName().c_str()));

			BMP.SaveFile(fn.GetFullPath(), (wxBitmapType)type);
		}

		if( Parms("WORLD")->asBool() )
		{
			wxFileName	fn(file);

			switch( type )
			{
			default:					fn.SetExt(wxT("world"));	break;
			case wxBITMAP_TYPE_BMP:		fn.SetExt(wxT("bpw"));		break;
			case wxBITMAP_TYPE_GIF:		fn.SetExt(wxT("gfw"));		break;
			case wxBITMAP_TYPE_JPEG:	fn.SetExt(wxT("jgw"));		break;
			case wxBITMAP_TYPE_PNG:		fn.SetExt(wxT("pgw"));		break;
			case wxBITMAP_TYPE_PCX:		fn.SetExt(wxT("pxw"));		break;
			case wxBITMAP_TYPE_TIF:		fn.SetExt(wxT("tfw"));		break; 
			}

			if( Stream.Open(fn.GetFullPath().c_str(), SG_FILE_W, false) )
			{
				Stream.Printf(wxT("%.10f\n%.10f\n%.10f\n%.10f\n%.10f\n%.10f\n"),
					 m_pGrid->Get_Cellsize(),
					 0.0, 0.0,
					-m_pGrid->Get_Cellsize(),
					 m_pGrid->Get_XMin(),
					 m_pGrid->Get_YMax()
				);
			}
		}
	}
}

//---------------------------------------------------------
bool CWKSP_Grid::Get_Image_Grid(wxBitmap &BMP, bool bFitSize)
{
	if( bFitSize || (BMP.GetWidth() > 0 && BMP.GetHeight() > 0) )
	{
		Set_Buisy_Cursor(true);

		if( bFitSize )
		{
			BMP.Create(m_pGrid->Get_NX(), m_pGrid->Get_NY());
		}

		wxMemoryDC		dc;
		wxRect			r(0, 0, BMP.GetWidth(), BMP.GetHeight());
		CWKSP_Map_DC	dc_Map(Get_Extent(), r, 1.0, SG_GET_RGB(255, 255, 255));

		On_Draw(dc_Map, false);

		dc.SelectObject(BMP);
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();

		dc_Map.Draw(dc);

		dc.SelectObject(wxNullBitmap);

		Set_Buisy_Cursor(false);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CWKSP_Grid::Get_Image_Legend(wxBitmap &BMP, double Zoom)
{
	if( Zoom > 0.0 )
	{
		wxMemoryDC	dc;
		wxSize		s(Get_Legend()->Get_Size(Zoom, 1.0));

		BMP.Create(s.GetWidth(), s.GetHeight());

		dc.SelectObject(BMP);
		dc.SetBackground(*wxWHITE_BRUSH);
		dc.Clear();

		Get_Legend()->Draw(dc, Zoom, 1.0, wxPoint(0, 0));

		dc.SelectObject(wxNullBitmap);

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
void CWKSP_Grid::On_Draw(CWKSP_Map_DC &dc_Map, bool bEdit)
{
	int		Interpolation;
	double	Transparency;

	if(	Get_Extent().Intersects(dc_Map.m_rWorld) != INTERSECTION_None )
	{
		switch( m_pClassify->Get_Mode() )
		{
		default:				Transparency	= m_Parameters("DISPLAY_TRANSPARENCY")->asDouble() / 100.0;	break;
		case CLASSIFY_SHADE:	Transparency	= 2.0;	break;
		case CLASSIFY_RGB:		Transparency	= m_Parameters("DISPLAY_TRANSPARENCY")->asDouble() / 100.0;	if( Transparency <= 0.0 )	Transparency	= 3.0;	break;
		}

		m_pClassify->Set_Shade_Mode(m_Parameters("SHADE_MODE")->asInt());

		if( dc_Map.IMG_Draw_Begin(Transparency) )
		{
			Interpolation	= m_pClassify->Get_Mode() == CLASSIFY_LUT
							? GRID_INTERPOLATION_NearestNeighbour
							: m_Parameters("DISPLAY_INTERPOLATION")->asInt();

			if(	dc_Map.m_DC2World >= m_pGrid->Get_Cellsize()
			||	Interpolation != GRID_INTERPOLATION_NearestNeighbour || m_bOverlay )
			{
				_Draw_Grid_Points	(dc_Map, Interpolation);
			}
			else
			{
				_Draw_Grid_Cells	(dc_Map);
			}

			dc_Map.IMG_Draw_End();

			_Draw_Values(dc_Map);

			if( bEdit )
			{
				_Draw_Edit(dc_Map);
			}
		}
	}
}

//---------------------------------------------------------
void CWKSP_Grid::_Draw_Grid_Points(CWKSP_Map_DC &dc_Map, int Interpolation)
{
	bool		bByteWise	= m_pClassify->Get_Mode() == CLASSIFY_RGB;
	int			xDC, yDC, axDC, ayDC, bxDC, byDC, Color, r, g, b;
	double		x, y, z, axMap, ayMap;
	CSG_Rect	rMap(dc_Map.m_rWorld);

	switch( m_Parameters("OVERLAY_MODE")->asInt() )
	{
	default:
	case 0:	r = 0; g = 1; b = 2;	break;
	case 1:	r = 0; g = 2; b = 1;	break;
	case 2:	r = 1; g = 0; b = 2;	break;
	case 3:	r = 2; g = 0; b = 1;	break;
	case 4:	r = 1; g = 2; b = 0;	break;
	case 5:	r = 2; g = 1; b = 0;	break;
	}

	m_pOverlay[0]	= g_pData->Get_Grids()->Get_Grid(m_Parameters("OVERLAY_1")->asGrid());
	m_pOverlay[1]	= g_pData->Get_Grids()->Get_Grid(m_Parameters("OVERLAY_2")->asGrid());

	rMap.Intersect(m_pGrid->Get_Extent(true));

	axDC	= (int)dc_Map.xWorld2DC(rMap.Get_XMin());	if( axDC < 0 )	axDC	= 0;
	bxDC	= (int)dc_Map.xWorld2DC(rMap.Get_XMax());	if( bxDC >= dc_Map.m_rDC.GetWidth () )	bxDC	= dc_Map.m_rDC.GetWidth () - 1;
	ayDC	= (int)dc_Map.yWorld2DC(rMap.Get_YMin());	if( ayDC >= dc_Map.m_rDC.GetHeight() )	ayDC	= dc_Map.m_rDC.GetHeight() - 1;
	byDC	= (int)dc_Map.yWorld2DC(rMap.Get_YMax());	if( byDC < 0 )	byDC	= 0;
	axMap	=      dc_Map.xDC2World(axDC);
	ayMap	=      dc_Map.yDC2World(ayDC);

	for(y=ayMap, yDC=ayDC; yDC>=byDC; y+=dc_Map.m_DC2World, yDC--)
	{
		for(x=axMap, xDC=axDC; xDC<bxDC; x+=dc_Map.m_DC2World, xDC++)
		{
			if( m_pGrid->Get_Value(x, y, z, Interpolation, false, bByteWise, true) )
			{
				if( m_bOverlay == false )
				{
					if( m_pClassify->Get_Class_Color_byValue(z, Color) )
					{
						dc_Map.IMG_Set_Pixel(xDC, yDC, Color);
					}
				}
				else
				{
					int		c[3];

					c[0]	= (int)(255.0 * m_pClassify->Get_MetricToRelative(z));

					c[1]	= m_pOverlay[0] && m_pOverlay[0]->Get_Grid()->Get_Value(x, y, z, Interpolation, false, false, true)
							? (int)(255.0 * m_pOverlay[0]->m_pClassify->Get_MetricToRelative(z)) : 255;

					c[2]	= m_pOverlay[1] && m_pOverlay[1]->Get_Grid()->Get_Value(x, y, z, Interpolation, false, false, true)
							? (int)(255.0 * m_pOverlay[1]->m_pClassify->Get_MetricToRelative(z)) : 255;

					dc_Map.IMG_Set_Pixel(xDC, yDC, SG_GET_RGB(
						c[r] < 0 ? 0 : c[r] > 255 ? 255 : c[r],
						c[g] < 0 ? 0 : c[g] > 255 ? 255 : c[g],
						c[b] < 0 ? 0 : c[b] > 255 ? 255 : c[b]
					));
				}
			}
		}
	}
}

//---------------------------------------------------------
void CWKSP_Grid::_Draw_Grid_Cells(CWKSP_Map_DC &dc_Map)
{
	int		x, y, xa, ya, xb, yb, xaDC, yaDC, xbDC, ybDC, Color;
	double	xDC, yDC, axDC, ayDC, dDC;

	//-----------------------------------------------------
	dDC		= m_pGrid->Get_Cellsize() * dc_Map.m_World2DC;

	xa		= m_pGrid->Get_System().Get_xWorld_to_Grid(dc_Map.m_rWorld.Get_XMin());
	ya		= m_pGrid->Get_System().Get_yWorld_to_Grid(dc_Map.m_rWorld.Get_YMin());
	xb		= m_pGrid->Get_System().Get_xWorld_to_Grid(dc_Map.m_rWorld.Get_XMax());
	yb		= m_pGrid->Get_System().Get_yWorld_to_Grid(dc_Map.m_rWorld.Get_YMax());

	if( xa < 0 )	xa	= 0;	if( xb >= m_pGrid->Get_NX() )	xb	= m_pGrid->Get_NX() - 1;
	if( ya < 0 )	ya	= 0;	if( yb >= m_pGrid->Get_NY() )	yb	= m_pGrid->Get_NY() - 1;

	axDC	= dc_Map.xWorld2DC(m_pGrid->Get_System().Get_xGrid_to_World(xa)) + dDC / 2.0;
	ayDC	= dc_Map.yWorld2DC(m_pGrid->Get_System().Get_yGrid_to_World(ya)) - dDC / 2.0;

	//-----------------------------------------------------
	for(y=ya, yDC=ayDC, yaDC=(int)(ayDC), ybDC=(int)(ayDC+dDC); y<=yb; y++, ybDC=yaDC, yaDC=(int)(yDC-=dDC))
	{
		for(x=xa, xDC=axDC, xaDC=(int)(axDC-dDC), xbDC=(int)(axDC); x<=xb; x++, xaDC=xbDC, xbDC=(int)(xDC+=dDC))
		{
			if( m_pGrid->is_InGrid(x, y) && m_pClassify->Get_Class_Color_byValue(m_pGrid->asDouble(x, y), Color) )
			{
				dc_Map.IMG_Set_Rect(xaDC, yaDC, xbDC, ybDC, Color);
			}
		}
	}
}

//---------------------------------------------------------
void CWKSP_Grid::_Draw_Values(CWKSP_Map_DC &dc_Map)
{
	int			x, y, xa, ya, xb, yb, Decimals;
	double		xDC, yDC, axDC, ayDC, dDC, zFactor, Value;
	wxString	s;
	wxFont		Font;

	//-----------------------------------------------------
	if(	m_Parameters("VALUES_SHOW")->asBool() && (dDC = m_pGrid->Get_Cellsize() * dc_Map.m_World2DC) > 40 )
	{
		zFactor		=  m_pGrid->Get_ZFactor();
		Decimals	=  m_Parameters("VALUES_DECIMALS")	->asInt();

		xDC			=  m_Parameters("VALUES_SIZE")		->asDouble() / 100.0;
		x			=  m_Parameters("VALUES_FONT")		->asColor();
		Font		= *m_Parameters("VALUES_FONT")		->asFont();
		Font.SetPointSize((int)(xDC * dDC));
		dc_Map.dc.SetFont(Font);
		dc_Map.dc.SetTextForeground(Get_Color_asWX(x));

		x			=  m_Parameters("VALUES_FONT")		->asInt();
		dc_Map.dc.SetTextForeground(wxColour(SG_GET_R(x), SG_GET_G(x), SG_GET_B(x)));

		//-------------------------------------------------
		xa		= m_pGrid->Get_System().Get_xWorld_to_Grid(dc_Map.m_rWorld.Get_XMin());
		ya		= m_pGrid->Get_System().Get_yWorld_to_Grid(dc_Map.m_rWorld.Get_YMin());
		xb		= m_pGrid->Get_System().Get_xWorld_to_Grid(dc_Map.m_rWorld.Get_XMax());
		yb		= m_pGrid->Get_System().Get_yWorld_to_Grid(dc_Map.m_rWorld.Get_YMax());

		if( xa < 0 )	xa	= 0;	if( xb >= m_pGrid->Get_NX() )	xb	= m_pGrid->Get_NX() - 1;
		if( ya < 0 )	ya	= 0;	if( yb >= m_pGrid->Get_NY() )	yb	= m_pGrid->Get_NY() - 1;

		axDC	= dc_Map.xWorld2DC(m_pGrid->Get_System().Get_xGrid_to_World(xa));
		ayDC	= dc_Map.yWorld2DC(m_pGrid->Get_System().Get_yGrid_to_World(ya));

		//-------------------------------------------------
		for(y=ya, yDC=ayDC; y<=yb; y++, yDC-=dDC)
		{
			for(x=xa, xDC=axDC; x<=xb; x++, xDC+=dDC)
			{
				if( m_pGrid->is_InGrid(x, y) )
				{
					Value	= m_pGrid->asDouble(x, y);

					switch( m_pClassify->Get_Mode() )
					{
					case CLASSIFY_RGB:
						s.Printf(wxT("R%03d G%03d B%03d"), SG_GET_R((int)Value), SG_GET_G((int)Value), SG_GET_B((int)Value));
						break;

					default:
						s.Printf(wxT("%.*f"), Decimals, zFactor * Value);
						break;
					}

					Draw_Text(dc_Map.dc, TEXTALIGN_CENTER, (int)xDC, (int)yDC, s);
				}
			}
		}
	}
}

//---------------------------------------------------------
void CWKSP_Grid::_Draw_Edit(CWKSP_Map_DC &dc_Map)
{
	if( m_Sel_xN >= 0 )
	{
		CSG_Rect	r(
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_xGrid_to_World(m_Sel_xOff),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_yGrid_to_World(m_Sel_yOff),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_xGrid_to_World(m_Sel_xOff + m_Sel_xN),
			-m_pGrid->Get_Cellsize() / 2.0 + m_pGrid->Get_System().Get_yGrid_to_World(m_Sel_yOff + m_Sel_yN)
		);

		TSG_Point_Int		a(dc_Map.World2DC(r.Get_TopLeft())),
						b(dc_Map.World2DC(r.Get_BottomRight()));

		a.x	-= 1;
		b.x	-= 1;
		a.y	-= 1;
		b.y	-= 1;

		dc_Map.dc.SetPen(wxPen(wxColour(255, 0, 0), 2, wxSOLID));
		dc_Map.dc.DrawLine(a.x, a.y, a.x, b.y);
		dc_Map.dc.DrawLine(a.x, b.y, b.x, b.y);
		dc_Map.dc.DrawLine(b.x, b.y, b.x, a.y);
		dc_Map.dc.DrawLine(a.x, a.y, b.x, a.y);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
