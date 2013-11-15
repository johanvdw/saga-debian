/**********************************************************
 * Version $Id: Filter_Rank.cpp 1365 2012-03-30 14:21:53Z oconrad $
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
//                    Filter_Rank.cpp                    //
//                                                       //
//                 Copyright (C) 2010 by                 //
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
#include "Filter_Rank.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CFilter_Rank::CFilter_Rank(void)
{
	//-----------------------------------------------------
	// 1. Info...

	Set_Name		(_TL("Rank Filter"));

	Set_Author		(SG_T("O.Conrad (c) 2010"));

	Set_Description	(_TW(
		"Rank filter for grids. Set rank to fifty percent to apply a median filter."
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

	Parameters.Add_Value(
		NULL, "RADIUS"		, _TL("Radius"),
		_TL(""),
		PARAMETER_TYPE_Int, 1, 1, true
	);

	Parameters.Add_Value(
		NULL, "RANK"		, _TL("Rank [Percent]"),
		_TL(""),
		PARAMETER_TYPE_Double, 50.0, 0.0, true, 100.0, true
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CFilter_Rank::On_Execute(void)
{
	int			x, y, ix, iy;
	double		Rank;
	CSG_Grid	*pResult;

	//-----------------------------------------------------
	m_pInput	= Parameters("INPUT" )->asGrid();
	pResult		= Parameters("RESULT")->asGrid();
	Rank		= Parameters("RANK"  )->asInt() / 100.0;

	//-----------------------------------------------------
	m_Kernel.Set_Radius(Parameters("RADIUS")->asInt(), Parameters("MODE")->asInt() == 0);

	//-----------------------------------------------------
	if( !pResult || pResult == m_pInput )
	{
		pResult	= SG_Create_Grid(m_pInput);
	}
	else
	{
		pResult->Set_Name(CSG_String::Format(SG_T("%s [%s: %.1f]"), m_pInput->Get_Name(), _TL("Rank"), 100.0 * Rank));

		pResult->Set_NoData_Value(m_pInput->Get_NoData_Value());
	}

	//-----------------------------------------------------
	for(y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		#pragma omp parallel private(x)
		for(x=0; x<Get_NX(); x++)
		{
			double	Value;

			if( Get_Value(x, y, Rank, Value) )
			{
				pResult->Set_Value(x, y, Value);
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

	m_Kernel.Destroy();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CFilter_Rank::Get_Value(int x, int y, double Rank, double &Value)
{
	if( m_pInput->is_InGrid(x, y) )
	{
		CSG_Table	Values;

		Values.Add_Field(SG_T("Z"), SG_DATATYPE_Double);

		for(int i=0; i<m_Kernel.Get_Count(); i++)
		{
			int	ix	= m_Kernel.Get_X(i, x);
			int	iy	= m_Kernel.Get_Y(i, y);

			if( m_pInput->is_InGrid(ix, iy) )
			{
				Values.Add_Record()->Set_Value(0, m_pInput->asDouble(ix, iy));
			}
		}

		switch( Values.Get_Count() )
		{
		case 0:
			return( false );

		case 1:
			Value	= Values[0].asDouble(0);
			return( true );

		case 2:
			Value	= (Values[0].asDouble(0) + Values[1].asDouble(0)) / 2.0;
			return( true );

		default:
			{
				Values.Set_Index(0, TABLE_INDEX_Ascending);

				Rank	= Rank * (Values.Get_Count() - 1.0);

				int	i	= (int)Rank;

				Value	= Values.Get_Record_byIndex(i)->asDouble(0);

				if( Rank - i > 0.0 && i < Values.Get_Count() - 1 )
				{
					Value	= (Value + Values.Get_Record_byIndex(i + 1)->asDouble(0)) / 2.0;
				}
			}

			return( true );
		}
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
