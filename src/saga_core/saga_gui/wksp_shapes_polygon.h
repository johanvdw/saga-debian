/**********************************************************
 * Version $Id: wksp_shapes_polygon.h 1015 2011-04-27 10:19:23Z oconrad $
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
//                WKSP_Shapes_Polygon.h                  //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_shapes.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Shapes_Polygon : public CWKSP_Shapes
{
public:
	CWKSP_Shapes_Polygon(CSG_Shapes *pShapes);
	virtual ~CWKSP_Shapes_Polygon(void);

	bool						Get_Style				(wxPen &Pen, wxBrush &Brush, bool &bOutline, wxString *pName = NULL);


protected:

	bool						m_bPoints, m_bCentroid, m_bOutline;

	wxColour					m_Sel_Color_Fill;

	wxPen						m_Pen;

	wxBrush						m_Brush;


	virtual void				On_Create_Parameters	(void);
	virtual void				On_DataObject_Changed	(void);
	virtual void				On_Parameters_Changed	(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);

	virtual void				_Draw_Initialize		(CWKSP_Map_DC &dc_Map);
	virtual void				_Draw_Shape				(CWKSP_Map_DC &dc_Map, CSG_Shape *pShape, bool bSelection);
	virtual void				_Draw_Label				(CWKSP_Map_DC &dc_Map, CSG_Shape *pShape);

	void						_Draw_Polygon_Points	(CWKSP_Map_DC &dc_Map, CSG_Shape_Polygon *pPolygon);

	virtual void				_Edit_Shape_Draw_Move	(wxDC &dc, CSG_Rect rWorld, double ClientToWorld, wxPoint Point);
	virtual void				_Edit_Shape_Draw		(CWKSP_Map_DC &dc_Map);
	virtual int					_Edit_Shape_HitTest		(CSG_Point Point, double max_Dist, int &iPart, int &iPoint);
	virtual void				_Edit_Snap_Point_ToLine (CSG_Point Point, CSG_Point &snap_Point, double &snap_Dist, CSG_Shape *pShape);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Shapes_Polygon_H
