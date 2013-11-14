/**********************************************************
 * Version $Id: io_table_txt.h 1319 2012-01-26 13:47:45Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       table_io                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    io_table_txt.h                     //
//                                                       //
//                 Copyright (C) 2008 by                 //
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
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__io_table_txt_H
#define HEADER_INCLUDED__io_table_txt_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CTable_Text_Export : public CSG_Module  
{
public:
	CTable_Text_Export(void);

	virtual CSG_String		Get_MenuPath(void)		{	return( _TL("R:Export") );	}


protected:

	virtual bool			On_Execute(void);

};

//---------------------------------------------------------
class CTable_Text_Import : public CSG_Module  
{
public:
	CTable_Text_Import(void);

	virtual CSG_String		Get_MenuPath(void)		{	return( _TL("R:Import") );	}


protected:

	virtual bool			On_Execute(void);

};

//---------------------------------------------------------
class CTable_Text_Import_Numbers : public CSG_Module  
{
public:
	CTable_Text_Import_Numbers(void);

	virtual CSG_String		Get_MenuPath(void)		{	return( _TL("R:Import") );	}


protected:

	virtual bool			On_Execute(void);

};

//---------------------------------------------------------
class CTable_Text_Import_Fixed_Cols : public CSG_Module  
{
public:
	CTable_Text_Import_Fixed_Cols(void);

	virtual CSG_String		Get_MenuPath(void)		{	return( _TL("R:Import") );	}


protected:

	virtual bool			On_Execute(void);

	virtual int				On_Parameters_Enable	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__io_table_txt_H
