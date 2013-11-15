/**********************************************************
 * Version $Id: view_table_control.h 1566 2012-12-13 13:53:35Z oconrad $
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
//                 VIEW_Table_Control.h                  //
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
#ifndef _HEADER_INCLUDED__SAGA_GUI__VIEW_Table_Control_H
#define _HEADER_INCLUDED__SAGA_GUI__VIEW_Table_Control_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include <wx/grid.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define TABLE_CTRL_FIXED_COLS	0x01
#define TABLE_CTRL_FIXED_ROWS	0x02
#define TABLE_CTRL_FIXED_TABLE	(TABLE_CTRL_FIXED_ROWS|TABLE_CTRL_FIXED_COLS)
#define TABLE_CTRL_COL1ISLABEL	0x04


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CVIEW_Table_Control : public wxGrid
{
	DECLARE_CLASS(CVIEW_Table_Control)

public:
	CVIEW_Table_Control(wxWindow *pParent, class CSG_Table *pTable, int Constraint);
	virtual ~CVIEW_Table_Control(void);

	void						On_Field_Add		(wxCommandEvent  &event);
	void						On_Field_Add_UI		(wxUpdateUIEvent &event);
	void						On_Field_Del		(wxCommandEvent  &event);
	void						On_Field_Del_UI		(wxUpdateUIEvent &event);
	void						On_Field_Sort		(wxCommandEvent  &event);
	void						On_Field_Sort_UI	(wxUpdateUIEvent &event);
	void						On_Field_Rename		(wxCommandEvent  &event);
	void						On_Field_Rename_UI	(wxUpdateUIEvent &event);
	void						On_Field_Type		(wxCommandEvent  &event);
	void						On_Field_Type_UI	(wxUpdateUIEvent &event);

	void						On_Record_Add		(wxCommandEvent  &event);
	void						On_Record_Add_UI	(wxUpdateUIEvent &event);
	void						On_Record_Ins		(wxCommandEvent  &event);
	void						On_Record_Ins_UI	(wxUpdateUIEvent &event);
	void						On_Record_Del		(wxCommandEvent  &event);
	void						On_Record_Del_UI	(wxUpdateUIEvent &event);
	void						On_Record_Clr		(wxCommandEvent  &event);
	void						On_Record_Clr_UI	(wxUpdateUIEvent &event);

	void						On_Sel_To_Top		(wxCommandEvent  &event);
	void						On_Sel_To_Top_UI	(wxUpdateUIEvent &event);

	void						On_Sel_Only			(wxCommandEvent  &event);
	void						On_Sel_Only_UI		(wxUpdateUIEvent &event);

	void						On_Autosize_Cols	(wxCommandEvent  &event);
	void						On_Autosize_Rows	(wxCommandEvent  &event);

	void						On_Size				(wxSizeEvent &event);
	void						On_Change			(wxGridEvent &event);
	void						On_LClick			(wxGridEvent &event);
	void						On_LClick_Label		(wxGridEvent &event);
	void						On_RClick_Label		(wxGridEvent &event);
	void						On_LDClick_Label	(wxGridEvent &event);
	void						On_Select			(wxGridRangeSelectEvent &event);

	bool						Load				(const wxString &File_Name);
	bool						Save				(const wxString &File_Name, int Format = 0);

	bool						Add_Record			(void);
	bool						Ins_Record			(void);
	bool						Del_Record			(void);
	bool						Del_Records			(void);

	void						Set_Labeling		(bool bOn);
	void						Update_Selection	(void);
	void						Update_Table		(void);
	void						Sort_Table			(int iField, int Direction);


private:

	bool						m_bUpdating, m_bSelOnly;

	int							m_Constraint, m_Field_Offset;

	class CSG_Table_Record		**m_pRecords;

	class CSG_Table				*m_pTable;


	bool						_Set_Table			(void);

	bool						_Set_Records		(bool bSelection_To_Top = false);
	bool						_Set_Record			(int iRecord, class CSG_Table_Record *pRecord);

	void						_Update_Views		(void);


//---------------------------------------------------------
DECLARE_EVENT_TABLE()
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__VIEW_Table_Control_H
