/**********************************************************
 * Version $Id: Filter.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                      Grid_Filter                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                      Filter.cpp                       //
//                                                       //
//                 Copyright (C) 2003 by                 //
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
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
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
#include "Filter.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CFilter::CFilter(void)
{
	//-----------------------------------------------------
	// 1. Info...

	Set_Name		(_TL("Simple Filter"));

	Set_Author		(SG_T("O.Conrad (c) 2003"));

	Set_Description	(_TW(
		"Simple standard filters for grids."
	));


	//-----------------------------------------------------
	// 2. Parameters...

	Parameters.Add_Grid(
		NULL, "INPUT"		, _TL("Grid"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL, "RESULT"		, _TL("Filtered Grid"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Choice(
		NULL, "MODE"		, _TL("Search Mode"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("Square"),
			_TL("Circle")
		), 1
	);

	Parameters.Add_Choice(
		NULL, "METHOD"		, _TL("Filter"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("Smooth"),
			_TL("Sharpen"),
			_TL("Edge")
		), 0
	);

	Parameters.Add_Value(
		NULL, "RADIUS"		, _TL("Radius"),
		_TL(""),
		PARAMETER_TYPE_Int, 1, 1, true
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CFilter::On_Execute(void)
{
	int			Mode, Radius, Method;
	double		Mean;
	CSG_Grid	*pResult;

	//-----------------------------------------------------
	m_pInput	= Parameters("INPUT")	->asGrid();
	pResult		= Parameters("RESULT")	->asGrid();
	Radius		= Parameters("RADIUS")	->asInt();
	Mode		= Parameters("MODE")	->asInt();
	Method		= Parameters("METHOD")	->asInt();

	switch( Mode )
	{
	case 0:								break;
	case 1:	m_Radius.Create(Radius);	break;
	}

	//-----------------------------------------------------
	if( !pResult || pResult == m_pInput )
	{
		pResult	= SG_Create_Grid(m_pInput);
	}
	else
	{
		pResult->Set_Name(CSG_String::Format(SG_T("%s [%s]"), m_pInput->Get_Name(), _TL("Filter")));

		pResult->Set_NoData_Value(m_pInput->Get_NoData_Value());
	}

	//-----------------------------------------------------
	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=0; x<Get_NX(); x++)
		{
			if( m_pInput->is_InGrid(x, y) )
			{
				switch( Mode )
				{
				case 0:		Mean	= Get_Mean_Square(x, y, Radius);	break;
				case 1:		Mean	= Get_Mean_Circle(x, y);			break;
				}

				switch( Method )
				{
				case 0:	default:	// Smooth...
					pResult->Set_Value(x, y, Mean);
					break;

				case 1:				// Sharpen...
					pResult->Set_Value(x, y, m_pInput->asDouble(x, y) + (m_pInput->asDouble(x, y) - Mean));
					break;

				case 2:				// Edge...
					pResult->Set_Value(x, y, m_pInput->asDouble(x, y) - Mean);
					break;
				}
			}
			else
			{
				pResult->Set_NoData(x, y);
			}
		}
	}

	//-----------------------------------------------------
	if( !Parameters("RESULT")->asGrid() || Parameters("RESULT")->asGrid() == m_pInput )
	{
		m_pInput->Assign(pResult);

		delete(pResult);

		DataObject_Update(m_pInput);
	}

	m_Radius.Destroy();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CFilter::Get_Mean_Square(int x, int y, int Radius)
{
	int		ix, iy, n;
	double	s;

	for(n=0, s=0.0, iy=y-Radius; iy<=y+Radius; iy++)
	{
		for(ix=x-Radius; ix<=x+Radius; ix++)
		{
			if( m_pInput->is_InGrid(ix, iy) )
			{
				s	+= m_pInput->asDouble(ix, iy);
				n	++;
			}
		}
	}

	return( n > 0 ? s / n : m_pInput->Get_NoData_Value() );
}

//---------------------------------------------------------
double CFilter::Get_Mean_Circle(int x, int y)
{
	int		i, ix, iy, n;
	double	s;

	for(n=0, s=0.0, i=0; i<m_Radius.Get_nPoints(); i++)
	{
		m_Radius.Get_Point(i, x, y, ix, iy);

		if( m_pInput->is_InGrid(ix, iy) )
		{
			s	+= m_pInput->asDouble(ix, iy);
			n	++;
		}
	}

	return( n > 0 ? s / n : m_pInput->Get_NoData_Value() );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
