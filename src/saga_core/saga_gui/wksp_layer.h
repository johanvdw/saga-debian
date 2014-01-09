/**********************************************************
 * Version $Id: wksp_layer.h 1921 2014-01-09 10:24:11Z oconrad $
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
//                     WKSP_Layer.h                      //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Layer_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Layer_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_data_item.h"

#include "wksp_map_dc.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Layer : public CWKSP_Data_Item
{
public:
	CWKSP_Layer(CSG_Data_Object *pObject);
	virtual ~CWKSP_Layer(void);

	virtual bool					On_Command				(int Cmd_ID);
	virtual bool					On_Command_UI			(wxUpdateUIEvent &event);

	CSG_Data_Object *				Get_Object				(void)	{	return( m_pObject );	}
	CSG_Rect						Get_Extent				(void);

	CSG_Colors *					Get_Colors				(void);
	bool							Get_Colors				(CSG_Colors *pColors);
	bool							Set_Colors				(CSG_Colors *pColors);
	bool							Set_Color_Range			(double zMin, double zMax);

	virtual wxString				Get_Value				(CSG_Point ptWorld, double Epsilon)	= 0;
	virtual double					Get_Value_Range			(void)								= 0;

	void							Draw					(CWKSP_Map_DC &dc_Map, bool bEdit);

	class CWKSP_Layer_Classify *	Get_Classifier			(void)	{	return( m_pClassify );	}

	class CWKSP_Layer_Legend *		Get_Legend				(void)	{	return( m_pLegend );	}
	bool							do_Legend				(void);
	bool							do_Show					(CSG_Rect const &rMap);

	virtual bool					Show					(class CWKSP_Map *pMap);
	virtual bool					Show					(int Flags = 0);
	virtual bool					Update					(CWKSP_Layer *pChanged);
	virtual bool					View_Closes				(class wxMDIChildFrame *pView);
	virtual bool					asImage					(CSG_Grid *pImage)		{	return( false );	}


	const wxBitmap &				Get_Thumbnail			(int dx, int dy);

	class CVIEW_Histogram *			Histogram_Get			(void)	{	return( m_pHistogram );	}
	void							Histogram_Show			(bool bShow);
	void							Histogram_Toggle		(void);

	wxMenu *						Edit_Get_Menu			(void);
	TSG_Rect						Edit_Get_Extent			(void);
	CSG_Table *						Edit_Get_Attributes		(void)	{	return( &m_Edit_Attributes );	}
	bool							Edit_Set_Attributes		(void);
	bool							Edit_On_Key_Down		(int KeyCode);
	bool							Edit_On_Mouse_Down		(CSG_Point Point, double WorldToClient, int Key);
	bool							Edit_On_Mouse_Up		(CSG_Point Point, double WorldToClient, int Key);
	bool							Edit_On_Mouse_Move		(wxWindow *pMap, CSG_Rect rWorld, wxPoint pt, wxPoint ptLast, int Key);


protected:

	CSG_Parameter_Range				*m_pZRange;

	CSG_Point						m_Edit_Mouse_Down;

	CSG_Table						m_Edit_Attributes;

	class CWKSP_Layer_Classify		*m_pClassify;

	class CWKSP_Layer_Legend		*m_pLegend;

	class CVIEW_Histogram			*m_pHistogram;

	wxBitmap						m_Thumbnail;


	virtual void					On_Create_Parameters	(void);
	virtual void					On_Parameters_Changed	(void);

	virtual int						On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);

	virtual void					On_Update_Views			(bool bAll);
	virtual void					On_Update_Views			(void)			{}

	virtual void					On_Draw					(CWKSP_Map_DC &dc_Map, bool bEdit)	= 0;

	virtual wxMenu *				On_Edit_Get_Menu		(void)			{	return( NULL );	}
	virtual TSG_Rect				On_Edit_Get_Extent		(void)	= 0;
	virtual bool					On_Edit_Set_Attributes	(void)	= 0;
	virtual bool					On_Edit_On_Key_Down		(int KeyCode)	{	return( false );	}
	virtual bool					On_Edit_On_Mouse_Down	(CSG_Point Point, double ClientToWorld, int Key)	{	return( false );	}
	virtual bool					On_Edit_On_Mouse_Up		(CSG_Point Point, double ClientToWorld, int Key)	{	return( false );	}
	virtual bool					On_Edit_On_Mouse_Move	(wxWindow *pMap, CSG_Rect rWorld, wxPoint pt, wxPoint ptLast, int Key)	{	return( false );	}


private:

	bool							_Set_Thumbnail			(bool bRefresh);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CWKSP_Layer *						Get_Active_Layer		(void);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Layer_H
