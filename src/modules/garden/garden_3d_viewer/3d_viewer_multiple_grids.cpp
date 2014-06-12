/**********************************************************
 * Version $Id: 3d_viewer_multiple_grids.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       3d_viewer                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              3d_viewer_multiple_grids.cpp             //
//                                                       //
//                 Copyright (C) 2014 by                 //
//                      Olaf Conrad                      //
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
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "3d_viewer_multiple_grids.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class C3D_Viewer_Multiple_Grids_Panel : public CSG_3DView_Panel
{
public:
	C3D_Viewer_Multiple_Grids_Panel(wxWindow *pParent, CSG_Parameter_Grid_List *pGrids);


protected:

	virtual int					On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual void				Update_Statistics		(void);
	virtual void				Update_Parent			(void);

	virtual void				On_Key_Down				(wxKeyEvent   &event);

	virtual bool				On_Before_Draw			(void);
	virtual bool				On_Draw					(void);

	virtual int					Get_Color				(double Value);


private:

	bool						m_Color_bGrad;

	double						m_Color_Min, m_Color_Scale;

	CSG_Colors					m_Colors;

	CSG_Parameter_Grid_List		*m_pGrids;


	bool						Get_Node				(CSG_Grid *pGrid, int x, int y, TSG_Triangle_Node &Node);

	void						Draw_Grid				(CSG_Grid *pGrid);


	//-----------------------------------------------------
	DECLARE_EVENT_TABLE()

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
BEGIN_EVENT_TABLE(C3D_Viewer_Multiple_Grids_Panel, CSG_3DView_Panel)
	EVT_KEY_DOWN	(C3D_Viewer_Multiple_Grids_Panel::On_Key_Down)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_Multiple_Grids_Panel::C3D_Viewer_Multiple_Grids_Panel(wxWindow *pParent, CSG_Parameter_Grid_List *pGrids)
	: CSG_3DView_Panel(pParent)
{
	m_pGrids	= pGrids;

	//-----------------------------------------------------
	CSG_Parameter	*pNode, *pNode_1;

	//-----------------------------------------------------
	pNode	= m_Parameters("NODE_GENERAL");

	m_Parameters.Add_Value(
		pNode	, "Z_SCALE"			, _TL("Exaggeration"),
		_TL(""),
		PARAMETER_TYPE_Double, 1.0
	);

	//-----------------------------------------------------
	pNode	= m_Parameters.Add_Node(
		NULL	, "NODE_VIEW"		, _TL("Grid View Settings"),
		_TL("")
	);

	pNode_1	= m_Parameters.Add_Colors(
		pNode	, "COLORS"			, _TL("Colours"),
		_TL("")
	);

	m_Parameters.Add_Value(
		pNode_1	, "COLORS_GRAD"		, _TL("Graduated"),
		_TL(""),
		PARAMETER_TYPE_Bool, true
	);

	pNode_1	= m_Parameters.Add_Choice(
		pNode	, "SHADING"			, _TL("Shading"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("none"),
			_TL("shading")
		), 1
	);

	m_Parameters.Add_Value(
		pNode_1	, "SHADE_DEC"		, _TL("Light Source Height"),
		_TL(""),
		PARAMETER_TYPE_Double, 0.0, -90.0, true, 90.0, true
	);

	m_Parameters.Add_Value(
		pNode_1	, "SHADE_AZI"		, _TL("Light Source Direction"),
		_TL(""),
		PARAMETER_TYPE_Double, 315.0, 0.0, true, 360.0, true
	);

	//-----------------------------------------------------
	Update_Statistics();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_Multiple_Grids_Panel::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( !SG_STR_CMP(pParameter->Get_Identifier(), "SHADING") )
	{
		pParameters->Get_Parameter("SHADE_DEC")->Set_Enabled(pParameter->asBool());
		pParameters->Get_Parameter("SHADE_AZI")->Set_Enabled(pParameter->asBool());
	}

	return( CSG_3DView_Panel::On_Parameters_Enable(pParameters, pParameter) );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Panel::Update_Statistics(void)
{
	m_Data_Min.x	= m_pGrids->asGrid(0)->Get_Extent().Get_XMin();
	m_Data_Max.x	= m_pGrids->asGrid(0)->Get_Extent().Get_XMax();

	m_Data_Min.y	= m_pGrids->asGrid(0)->Get_Extent().Get_YMin();
	m_Data_Max.y	= m_pGrids->asGrid(0)->Get_Extent().Get_YMax();

	m_Data_Min.z	= m_pGrids->asGrid(0)->Get_ZMin();
	m_Data_Max.z	= m_pGrids->asGrid(0)->Get_ZMax();

	for(int i=1; i<m_pGrids->Get_Count(); i++)
	{
		CSG_Grid	*pGrid	= m_pGrids->asGrid(i);

		if( m_Data_Min.x > pGrid->Get_Extent().Get_XMin() )
			m_Data_Min.x = pGrid->Get_Extent().Get_XMin();	else
		if( m_Data_Max.x < pGrid->Get_Extent().Get_XMax() )
			m_Data_Max.x = pGrid->Get_Extent().Get_XMax();

		if( m_Data_Min.y > pGrid->Get_Extent().Get_YMin() )
			m_Data_Min.y = pGrid->Get_Extent().Get_YMin();	else
		if( m_Data_Max.y < pGrid->Get_Extent().Get_YMax() )
			m_Data_Max.y = pGrid->Get_Extent().Get_YMax();

		if( m_Data_Min.z > pGrid->Get_ZMin() )
			m_Data_Min.z = pGrid->Get_ZMin();	else
		if( m_Data_Max.z < pGrid->Get_ZMax() )
			m_Data_Max.z = pGrid->Get_ZMax();
	}

	//-----------------------------------------------------
	Update_View();
}

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Panel::Update_Parent(void)
{
	((CSG_3DView_Dialog *)GetParent())->Update_Controls();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Panel::On_Key_Down(wxKeyEvent &event)
{
	switch( event.GetKeyCode() )
	{
	default:	CSG_3DView_Panel::On_Key_Down(event);	return;

	case WXK_F1:	m_Parameters("Z_SCALE")->Set_Value(m_Parameters("Z_SCALE")->asDouble() -  0.5);	break;
	case WXK_F2:	m_Parameters("Z_SCALE")->Set_Value(m_Parameters("Z_SCALE")->asDouble() +  0.5);	break;
	}

	//-----------------------------------------------------
	Update_View();
	Update_Parent();
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_Multiple_Grids_Panel::On_Before_Draw(void)
{
	if( m_Play_State == SG_3DVIEW_PLAY_STOP )
	{
		m_Projector.Set_zScaling(m_Projector.Get_xScaling() * m_Parameters("Z_SCALE")->asDouble());
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int C3D_Viewer_Multiple_Grids_Panel::Get_Color(double Value)
{
	if( m_Color_Scale <= 0.0 )
	{
		return( (int)Value );
	}

	double	c	= m_Color_Scale * (Value - m_Color_Min);

	return( m_Color_bGrad ? m_Colors.Get_Interpolated(c) : m_Colors[(int)c] );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool C3D_Viewer_Multiple_Grids_Panel::On_Draw(void)
{
	//-------------------------------------------------
	for(int i=0; i<m_pGrids->Get_Count(); i++)
	{
		Draw_Grid(m_pGrids->asGrid(i));
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
inline bool C3D_Viewer_Multiple_Grids_Panel::Get_Node(CSG_Grid *pGrid, int x, int y, TSG_Triangle_Node &Node)
{
	if( pGrid->is_InGrid(x, y) )
	{
		TSG_Point_Z	p;

		p.x	= pGrid->Get_System().Get_xGrid_to_World(x);
		p.y	= pGrid->Get_System().Get_yGrid_to_World(y);
		p.z	= Node.c = pGrid->asDouble(x, y);

		m_Projector.Get_Projection(p);

		Node.x	= p.x;
		Node.y	= p.y;
		Node.z	= p.z;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Panel::Draw_Grid(CSG_Grid *pGrid)
{
	//-----------------------------------------------------
	if( !SG_UI_DataObject_Colors_Get(pGrid, &m_Colors) )
	{
		m_Colors	= *m_Parameters("COLORS")->asColors();
	}

	m_Color_bGrad	= m_Parameters("COLORS_GRAD")->asBool();

	m_Color_Min		= pGrid->Get_ZMin();
	m_Color_Scale	= pGrid->Get_ZRange() > 0.0 ? m_Colors.Get_Count() / pGrid->Get_ZRange() : 0.0;

	//-----------------------------------------------------
	int		Shading		= m_Parameters("SHADING"  )->asInt   ();
	double	Shade_Dec	= m_Parameters("SHADE_DEC")->asDouble() * -M_DEG_TO_RAD;
	double	Shade_Azi	= m_Parameters("SHADE_AZI")->asDouble() *  M_DEG_TO_RAD;

	//-----------------------------------------------------
	#pragma omp parallel for
	for(int y=1; y<pGrid->Get_NY(); y++)
	{
		for(int x=1; x<pGrid->Get_NX(); x++)
		{
			TSG_Triangle_Node	a, b, p[3];

			if( Get_Node(pGrid, x - 1, y - 1, a)
			&&  Get_Node(pGrid, x    , y    , b) )
			{
				if( Get_Node(pGrid, x    , y - 1, p[2]) )
				{
					p[0]	= a;
					p[1]	= b;

					if( Shading ) Draw_Triangle(p, Shade_Dec, Shade_Azi); else Draw_Triangle(p);
				}

				if( Get_Node(pGrid, x - 1, y    , p[2]) )
				{
					p[0]	= a;
					p[1]	= b;

					if( Shading ) Draw_Triangle(p, Shade_Dec, Shade_Azi); else Draw_Triangle(p);
				}
			}
		}
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class C3D_Viewer_Multiple_Grids_Dialog : public CSG_3DView_Dialog
{
public:
	C3D_Viewer_Multiple_Grids_Dialog(CSG_Parameter_Grid_List *pGrids)
		: CSG_3DView_Dialog(_TL("Multiple Grids Viewer"))
	{
		Create(new C3D_Viewer_Multiple_Grids_Panel(this, pGrids));
	}


protected:

	virtual void				Set_Menu				(wxMenu &Menu);
	virtual void				On_Menu					(wxCommandEvent &event);

};

//---------------------------------------------------------
enum
{
	MENU_SCALE_Z_DEC	= MENU_USER_FIRST,
	MENU_SCALE_Z_INC
};

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Dialog::Set_Menu(wxMenu &Menu)
{
	wxMenu	*pMenu	= Menu.FindChildItem(Menu.FindItem(_TL("Display")))->GetSubMenu();

	pMenu->AppendSeparator();
	pMenu->Append(MENU_SCALE_Z_DEC, _TL("Decrease Exaggeration [F1]"));
	pMenu->Append(MENU_SCALE_Z_INC, _TL("Increase Exaggeration [F2]"));
}

//---------------------------------------------------------
void C3D_Viewer_Multiple_Grids_Dialog::On_Menu(wxCommandEvent &event)
{
	switch( event.GetId() )
	{
	case MENU_SCALE_Z_DEC:	m_pPanel->m_Parameters("Z_SCALE")->Set_Value(m_pPanel->m_Parameters("Z_SCALE")->asDouble() - 0.5); m_pPanel->Update_View();	return;
	case MENU_SCALE_Z_INC:	m_pPanel->m_Parameters("Z_SCALE")->Set_Value(m_pPanel->m_Parameters("Z_SCALE")->asDouble() + 0.5); m_pPanel->Update_View();	return;
	}

	CSG_3DView_Dialog::On_Menu(event);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
C3D_Viewer_Multiple_Grids::C3D_Viewer_Multiple_Grids(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Multiple Grids Viewer"));

	Set_Author		("O. Conrad (c) 2014");

	Set_Description	(_TW(
		""
	));

	//-----------------------------------------------------
	Parameters.Add_Grid_List(
		NULL	, "GRIDS"	, _TL("Grids"),
		_TL(""),
		PARAMETER_INPUT
	);
}

//---------------------------------------------------------
bool C3D_Viewer_Multiple_Grids::On_Execute(void)
{
	CSG_Parameter_Grid_List	*pGrids	= Parameters("GRIDS")->asGridList();

	if( pGrids->Get_Count() <= 0 )
	{
		Message_Add(_TL("invalid input"));

		return( false );
	}

	C3D_Viewer_Multiple_Grids_Dialog	dlg(pGrids);

	dlg.ShowModal();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
