
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
//                     WKSP_TIN.h                        //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_TIN_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_TIN_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_layer.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_TIN : public CWKSP_Layer
{
public:
	CWKSP_TIN(CSG_TIN *pTIN);
	virtual ~CWKSP_TIN(void);

	virtual TWKSP_Item			Get_Type				(void)	{	return( WKSP_ITEM_TIN );	}

	virtual wxString			Get_Description			(void);

	virtual wxMenu *			Get_Menu				(void);

	virtual bool				On_Command				(int Cmd_ID);
	virtual bool				On_Command_UI			(wxUpdateUIEvent &event);

	CSG_TIN *					Get_TIN					(void)	{	return( m_pTIN );	}
	class CWKSP_Table *			Get_Table				(void)	{	return( m_pTable );	}

	virtual wxString			Get_Value				(CSG_Point ptWorld, double Epsilon);
	virtual double				Get_Value_Range			(void);

	bool						asImage					(CSG_Grid *pImage);


protected:

	typedef struct
	{
		int		x, y;

		double	z;
	}
	TPoint;


protected:

	int							m_Color_Field, m_Brush_Style;

	wxColour					m_Color_Pen, m_Color_Brush;

	CSG_TIN						*m_pTIN;

	class CWKSP_Table			*m_pTable;


	virtual void				On_Create_Parameters	(void);
	virtual void				On_DataObject_Changed	(void);
	virtual void				On_Parameters_Changed	(void);
	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool				On_Edit_On_Mouse_Up		(CSG_Point Point, double ClientToWorld, int Key);
	virtual bool				On_Edit_Set_Attributes	(void);
	virtual TSG_Rect			On_Edit_Get_Extent		(void);

	virtual void				On_Draw					(CWKSP_Map_DC &dc_Map, bool bEdit);

	void						_Draw_Points			(CWKSP_Map_DC &dc_Map);
	void						_Draw_Edges				(CWKSP_Map_DC &dc_Map);
	void						_Draw_Triangles			(CWKSP_Map_DC &dc_Map);
	void						_Draw_Triangle			(CWKSP_Map_DC &dc_Map, TPoint p[3]);
	void						_Draw_Triangle_Line		(CWKSP_Map_DC &dc_Map, int xa, int xb, int y, double za, double zb);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_TIN_H
