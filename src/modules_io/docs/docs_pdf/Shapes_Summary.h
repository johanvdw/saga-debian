/**********************************************************
 * Version $Id: Shapes_Summary.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/
/*******************************************************************************
    Summarize.h
    Copyright (C) Victor Olaya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "doc_pdf.h"

#include <vector>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CShapes_Summary_PDF : public CSG_Doc_PDF 
{
public:

	CShapes_Summary_PDF();
	virtual ~CShapes_Summary_PDF();
	void AddClassSummaryPage(CSG_Shapes*, CSG_Table*, int, CSG_String);
	void AddSummaryStatisticsPage(CSG_Table* pTable);

private:

	void AddBarGraphStatistics(CSG_Points &, CSG_String*,	const CSG_Rect &);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CShapes_Summary : public CSG_Module  
{
public:
	CShapes_Summary(void);
	virtual ~CShapes_Summary(void);

protected:

	virtual bool			On_Execute(void);

private:

	int m_iField;
	CSG_Shapes *m_pShapes;
	CSG_Table *m_pTable;
	bool *m_bIncludeParam;
	std::vector<CSG_String> m_ClassesID;
	int *m_pClasses;
	int *m_pCount;
	CSG_Parameters *m_pExtraParameters;
	CShapes_Summary_PDF m_DocEngine;

	void Summarize();
	void CreatePDFDocs();

};

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
