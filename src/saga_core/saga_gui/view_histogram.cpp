/**********************************************************
 * Version $Id: view_histogram.cpp 2061 2014-03-20 11:48:01Z oconrad $
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
//                  VIEW_Histogram.cpp                   //
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
#include <wx/window.h>
#include <wx/toolbar.h>
#include <wx/scrolwin.h>

#include "res_commands.h"
#include "res_controls.h"
#include "res_images.h"

#include "helper.h"
#include "dc_helper.h"

#include "wksp_data_manager.h"
#include "wksp_layer_classify.h"
#include "wksp_grid.h"
#include "wksp_shapes.h"
#include "wksp_pointcloud.h"

#include "view_histogram.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CVIEW_Histogram_Control : public wxScrolledWindow
{
public:
	CVIEW_Histogram_Control(wxWindow *pParent, class CWKSP_Layer *pLayer);

	bool							Update_Histogram	(void);

	bool							Get_Cumulative		(void)	{	return( m_bCumulative );	}
	void							Set_Cumulative		(bool bOn);

	void							Draw				(wxDC &dc, wxRect rDraw);


private:

	bool							m_bCumulative, m_bMouse_Down;

	wxPoint							m_Mouse_Down, m_Mouse_Move;

	class CWKSP_Layer				*m_pLayer;


	void							On_Mouse_Motion		(wxMouseEvent &event);
	void							On_Mouse_LDown		(wxMouseEvent &event);
	void							On_Mouse_LUp		(wxMouseEvent &event);
	void							On_Mouse_RDown		(wxMouseEvent &event);

	void							On_Size				(wxSizeEvent  &event);
	void							On_Paint			(wxPaintEvent &event);

	void							_Draw_Histogram		(wxDC &dc, wxRect r);
	void							_Draw_Frame			(wxDC &dc, wxRect r);
	wxRect							_Draw_Get_rDiagram	(wxRect r);


private:

	DECLARE_EVENT_TABLE()
	DECLARE_CLASS(CVIEW_Histogram_Control)

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Histogram_Control, wxScrolledWindow);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Histogram_Control, wxScrolledWindow)
	EVT_PAINT			(CVIEW_Histogram_Control::On_Paint)
	EVT_SIZE			(CVIEW_Histogram_Control::On_Size)
	EVT_MOTION			(CVIEW_Histogram_Control::On_Mouse_Motion)
	EVT_LEFT_DOWN		(CVIEW_Histogram_Control::On_Mouse_LDown)
	EVT_LEFT_UP			(CVIEW_Histogram_Control::On_Mouse_LUp)
	EVT_RIGHT_DOWN		(CVIEW_Histogram_Control::On_Mouse_RDown)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Histogram_Control::CVIEW_Histogram_Control(wxWindow *pParent, CWKSP_Layer *pLayer)
	: wxScrolledWindow(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxFULL_REPAINT_ON_RESIZE)
{
	SYS_Set_Color_BG_Window(this);

	m_pLayer		= pLayer;

	m_bCumulative	= false;
	m_bMouse_Down	= false;

	Update_Histogram();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CVIEW_Histogram_Control::Update_Histogram(void)
{
	if( m_pLayer->Get_Classifier()->Histogram_Update() )
	{
		Refresh();

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::Set_Cumulative(bool bOn)
{
	if( m_bCumulative != bOn )
	{
		m_bCumulative	= bOn;

		Refresh();
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Mouse_Motion(wxMouseEvent &event)
{
	if( m_bMouse_Down )
	{
		wxClientDC	dc(this);
		wxRect		r(_Draw_Get_rDiagram(wxRect(wxPoint(0, 0), GetClientSize())));
		dc.SetLogicalFunction(wxINVERT);

		dc.DrawRectangle(m_Mouse_Down.x, r.GetTop(), m_Mouse_Move.x - m_Mouse_Down.x, r.GetHeight());
		m_Mouse_Move	= event.GetPosition();
		dc.DrawRectangle(m_Mouse_Down.x, r.GetTop(), m_Mouse_Move.x - m_Mouse_Down.x, r.GetHeight());
	}
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Mouse_LDown(wxMouseEvent &event)
{
	switch( m_pLayer->Get_Classifier()->Get_Mode() )
	{
	default:
		break;

	case CLASSIFY_GRADUATED:
	case CLASSIFY_METRIC:
	case CLASSIFY_SHADE:
	case CLASSIFY_OVERLAY:
		m_bMouse_Down	= true;
		m_Mouse_Move	= m_Mouse_Down	= event.GetPosition();

		CaptureMouse();
	}
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Mouse_LUp(wxMouseEvent &event)
{
	if( m_bMouse_Down )
	{
		ReleaseMouse();

		m_bMouse_Down	= false;
		m_Mouse_Move	= event.GetPosition();

		wxRect	r(_Draw_Get_rDiagram(wxRect(wxPoint(0, 0), GetClientSize())));
		double	zFactor	= m_pLayer->Get_Type() == WKSP_ITEM_Grid ? ((CWKSP_Grid *)m_pLayer)->Get_Grid()->Get_ZFactor() : 1.0;

		m_pLayer->Set_Color_Range(
			zFactor * m_pLayer->Get_Classifier()->Get_RelativeToMetric(
				(double)(m_Mouse_Down.x - r.GetLeft()) / (double)r.GetWidth()),
			zFactor * m_pLayer->Get_Classifier()->Get_RelativeToMetric(
				(double)(m_Mouse_Move.x - r.GetLeft()) / (double)r.GetWidth())
		);
	}
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Mouse_RDown(wxMouseEvent &event)
{
	switch( m_pLayer->Get_Classifier()->Get_Mode() )
	{
	default:
		break;

	case CLASSIFY_GRADUATED:
	case CLASSIFY_METRIC:
	case CLASSIFY_SHADE:
	case CLASSIFY_OVERLAY:
		switch( m_pLayer->Get_Type() )
		{
		default:
			return;

		case WKSP_ITEM_Grid:
			m_pLayer->Set_Color_Range(
				((CWKSP_Grid *)m_pLayer)->Get_Grid()->Get_ZMin(true),
				((CWKSP_Grid *)m_pLayer)->Get_Grid()->Get_ZMax(true)
			);
			break;

		case WKSP_ITEM_Shapes:
			m_pLayer->Set_Color_Range(
				((CWKSP_Shapes *)m_pLayer)->Get_Shapes()->Get_Minimum(m_pLayer->Get_Parameter("METRIC_ATTRIB")->asInt()),
				((CWKSP_Shapes *)m_pLayer)->Get_Shapes()->Get_Maximum(m_pLayer->Get_Parameter("METRIC_ATTRIB")->asInt())
			);
			break;

		case WKSP_ITEM_PointCloud:
			m_pLayer->Set_Color_Range(
				((CWKSP_PointCloud *)m_pLayer)->Get_PointCloud()->Get_Minimum(m_pLayer->Get_Parameter("METRIC_ATTRIB")->asInt()),
				((CWKSP_PointCloud *)m_pLayer)->Get_PointCloud()->Get_Maximum(m_pLayer->Get_Parameter("METRIC_ATTRIB")->asInt())
			);
			break;
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Size(wxSizeEvent &WXUNUSED(event))
{
	Refresh();
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::On_Paint(wxPaintEvent &event)
{
	wxPaintDC	dc(this);
	wxRect		r(wxPoint(0, 0), GetClientSize());

	Draw_Edge(dc, EDGE_STYLE_SUNKEN, r);

	Draw(dc, r);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram_Control::Draw(wxDC &dc, wxRect rDraw)
{
	wxRect		r(_Draw_Get_rDiagram(rDraw));
	wxFont		Font;

	Font.SetFamily(wxSWISS);
	dc.SetFont(Font);

	_Draw_Histogram	(dc, r);
	_Draw_Frame		(dc, r);
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::_Draw_Histogram(wxDC &dc, wxRect r)
{
	int		nClasses	= m_pLayer->Get_Classifier()->Get_Class_Count();

	if( nClasses > 1 )
	{
		int		ax, ay, bx, by;
		double	dx, Value;

		dx	= (double)r.GetWidth() / (double)nClasses;
		ay	= r.GetBottom();
		bx	= r.GetLeft();

		for(int iClass=0; iClass<nClasses; iClass++)
		{
			Value	= m_bCumulative
					? m_pLayer->Get_Classifier()->Histogram_Get_Cumulative(iClass)
					: m_pLayer->Get_Classifier()->Histogram_Get_Count     (iClass);

			ax	= bx;
			bx	= r.GetLeft() + (int)(dx * (iClass + 1.0));
			by	= ay - (int)(r.GetHeight() * Value);

			Draw_FillRect(dc, m_pLayer->Get_Classifier()->Get_Class_Color(iClass), ax, ay, bx, by);
		}
	}
	else
	{
		Draw_Text(dc, TEXTALIGN_CENTER, r.GetLeft() + r.GetWidth() / 2, r.GetBottom() - r.GetHeight() / 2, _TL("no histogram for unclassified data"));
	}
}

//---------------------------------------------------------
void CVIEW_Histogram_Control::_Draw_Frame(wxDC &dc, wxRect r)
{
	const int	dyFont		= 12,
				Precision	= 3;

	//-----------------------------------------------------
	Draw_Edge(dc, EDGE_STYLE_SIMPLE, r);

	int	Maximum	= m_bCumulative
		? m_pLayer->Get_Classifier()->Histogram_Get_Total()
		: m_pLayer->Get_Classifier()->Histogram_Get_Maximum();

	if( Maximum > 0 )
	{
		Draw_Scale(dc, wxRect(r.GetLeft() - 20, r.GetTop(), 20, r.GetHeight()), 0, Maximum, false, false, false);
	}

	//-----------------------------------------------------
	int		iPixel, iStep, nSteps, nClasses;
	double	dPixel, dPixelFont, dz;
	wxFont	Font;

	Font	= dc.GetFont();
	Font.SetPointSize((int)(0.7 * dyFont));
	dc.SetFont(Font);

	nClasses	= m_pLayer->Get_Classifier()->Get_Class_Count();
	dPixelFont	= dyFont + 5;

	if( (dPixel = r.GetWidth() / (double)nClasses) < dPixelFont )
	{
		dPixel	= dPixel * (1 + (int)(dPixelFont / dPixel));
	}

	nSteps	= (int)(r.GetWidth() / dPixel);
	dz		= dPixel / (double)r.GetWidth();

	if( m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_LUT )
	{
		for(iStep=0; iStep<nSteps; iStep++)
		{
			iPixel	= r.GetLeft() + (int)(dPixel * iStep);
			dc.DrawLine(iPixel, r.GetBottom(), iPixel, r.GetBottom() + 5);
			Draw_Text(dc, TEXTALIGN_TOPRIGHT, iPixel, r.GetBottom() + 7, 45.0,
				m_pLayer->Get_Classifier()->Get_Class_Name((int)(nClasses * iStep * dz))
			);
		}
	}
	else
	{
		double	zFactor	= m_pLayer->Get_Type() == WKSP_ITEM_Grid ? ((CWKSP_Grid *)m_pLayer)->Get_Grid()->Get_ZFactor() : 1.0;

		for(iStep=0; iStep<=nSteps; iStep++)
		{
			iPixel	= r.GetLeft() + (int)(dPixel * iStep);
			dc.DrawLine(iPixel, r.GetBottom(), iPixel, r.GetBottom() + 5);
			Draw_Text(dc, TEXTALIGN_CENTERRIGHT, iPixel, r.GetBottom() + 7, 45.0,
			//	wxString::Format(wxT("%.*f"), Precision, zFactor * m_pLayer->Get_Classifier()->Get_RelativeToMetric(iStep * dz))
				SG_Get_String(zFactor * m_pLayer->Get_Classifier()->Get_RelativeToMetric(iStep * dz), -2).c_str()
			);
		}
	}
}

//---------------------------------------------------------
wxRect CVIEW_Histogram_Control::_Draw_Get_rDiagram(wxRect r)
{
	if( m_pLayer->Get_Classifier()->Get_Mode() == CLASSIFY_LUT )
	{
		return(	wxRect(
			wxPoint(r.GetLeft()  + 30, r.GetTop()    +  10),
			wxPoint(r.GetRight() - 10, r.GetBottom() - 100)
		));
	}

	return(	wxRect(
		wxPoint(r.GetLeft()  + 30, r.GetTop()    + 10),
		wxPoint(r.GetRight() - 10, r.GetBottom() - 40)
	));
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
IMPLEMENT_CLASS(CVIEW_Histogram, CVIEW_Base);

//---------------------------------------------------------
BEGIN_EVENT_TABLE(CVIEW_Histogram, CVIEW_Base)
	EVT_MENU			(ID_CMD_HISTOGRAM_CUMULATIVE	, CVIEW_Histogram::On_Cumulative)
	EVT_UPDATE_UI		(ID_CMD_HISTOGRAM_CUMULATIVE	, CVIEW_Histogram::On_Cumulative_UI)
	EVT_MENU			(ID_CMD_HISTOGRAM_AS_TABLE		, CVIEW_Histogram::On_AsTable)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CVIEW_Histogram::CVIEW_Histogram(CWKSP_Layer *pLayer)
	: CVIEW_Base(pLayer, ID_VIEW_HISTOGRAM, pLayer->Get_Name(), ID_IMG_WND_HISTOGRAM)
{
	m_pControl	= new CVIEW_Histogram_Control(this, pLayer);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
wxMenu * CVIEW_Histogram::_Create_Menu(void)
{
	wxMenu	*pMenu	= new wxMenu;

	CMD_Menu_Add_Item(pMenu, true , ID_CMD_HISTOGRAM_CUMULATIVE);
	CMD_Menu_Add_Item(pMenu, false, ID_CMD_HISTOGRAM_AS_TABLE);

	return( pMenu );
}

//---------------------------------------------------------
wxToolBarBase * CVIEW_Histogram::_Create_ToolBar(void)
{
	wxToolBarBase	*pToolBar	= CMD_ToolBar_Create(ID_TB_VIEW_HISTOGRAM);

	CMD_ToolBar_Add_Item(pToolBar, true , ID_CMD_HISTOGRAM_CUMULATIVE);
	CMD_ToolBar_Add_Item(pToolBar, false, ID_CMD_HISTOGRAM_AS_TABLE);

	CMD_ToolBar_Add(pToolBar, _TL("Histogram"));

	return( pToolBar );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::Do_Update(void)
{
	m_pControl->Update_Histogram();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CVIEW_Histogram::On_Command_UI(wxUpdateUIEvent &event)
{
	switch( event.GetId() )
	{
	default:
		break;

	case ID_CMD_HISTOGRAM_CUMULATIVE:
		On_Cumulative_UI(event);
		break;
	}
}

//---------------------------------------------------------
void CVIEW_Histogram::On_Cumulative(wxCommandEvent &event)
{
	m_pControl->Set_Cumulative(!m_pControl->Get_Cumulative());
}

void CVIEW_Histogram::On_Cumulative_UI(wxUpdateUIEvent &event)
{
	event.Check(m_pControl->Get_Cumulative());
}

//---------------------------------------------------------
void CVIEW_Histogram::On_AsTable(wxCommandEvent &event)
{
	CWKSP_Layer_Classify	*pClassifier	= ((CWKSP_Layer *)m_pOwner)->Get_Classifier();

	if( pClassifier->Get_Class_Count() > 0 )
	{
		CSG_Data_Object	*pObject	= ((CWKSP_Layer *)m_pOwner)->Get_Object();

		CSG_Table	*pTable	= new CSG_Table;

		pTable->Set_Name(CSG_String::Format(SG_T("%s: %s"), _TL("Histogram"), pObject->Get_Name()));

		pTable->Add_Field(_TL("CLASS" ), SG_DATATYPE_Int);
		pTable->Add_Field(_TL("AREA"  ), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("COUNT" ), SG_DATATYPE_Int);
		pTable->Add_Field(_TL("CUMUL" ), SG_DATATYPE_Int);
		pTable->Add_Field(_TL("NAME"  ), SG_DATATYPE_String);
		pTable->Add_Field(_TL("MIN"   ), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("CENTER"), SG_DATATYPE_Double);
		pTable->Add_Field(_TL("MAX"   ), SG_DATATYPE_Double);

		double	dArea	= pObject->asGrid() != NULL ? pObject->asGrid()->Get_Cellarea() : 1.0;

		for(int i=0; i<pClassifier->Get_Class_Count(); i++)
		{
			CSG_Table_Record	*pRecord	= pTable->Add_Record();

			pRecord->Set_Value(0, i + 1);
			pRecord->Set_Value(2, pClassifier->Histogram_Get_Count     (i, false) * dArea);
			pRecord->Set_Value(1, pClassifier->Histogram_Get_Count     (i, false));
			pRecord->Set_Value(3, pClassifier->Histogram_Get_Cumulative(i, false));
			pRecord->Set_Value(4, pClassifier->Get_Class_Name          (i).wx_str());
			pRecord->Set_Value(5, pClassifier->Get_Class_Value_Minimum (i));
			pRecord->Set_Value(6, pClassifier->Get_Class_Value_Center  (i));
			pRecord->Set_Value(7, pClassifier->Get_Class_Value_Maximum (i));
		}

		g_pData->Add(pTable);
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
