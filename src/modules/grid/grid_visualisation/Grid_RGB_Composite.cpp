/**********************************************************
 * Version $Id: Grid_RGB_Composite.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                  Grid_Visualisation                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 Grid_RGB_Composite.cpp                //
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
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Grid_RGB_Composite.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGrid_RGB_Composite::CGrid_RGB_Composite(void)
{
	CSG_Parameter	*pNode;

	Set_Name		(_TL("RGB Composite"));

	Set_Author		(SG_T("(c) 2002 by O.Conrad"));

	Set_Description	(_TW(
		"Create red-green-blue overlays of grids. "
	));

	//-----------------------------------------------------
	pNode	= NULL;

	pNode	= Parameters.Add_Grid(
		pNode	, "GRID_R"		, _TL("Red"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Choice(
		pNode	, "R_METHOD"	,	_TL("Value Preparation"),
		_TL(""),

		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("0 - 255"),
			_TL("Rescale to 0 - 255"),
			_TL("User defined rescale"),
			_TL("Percentiles"),
			_TL("Percentage of standard deviation")
		), 4
	);

	Parameters.Add_Range(
		pNode	, "R_RANGE"		, _TL("Rescale Range"),
		_TL(""),
		0, 255
	);

	Parameters.Add_Range(
		pNode	, "R_PERCTL"	, _TL("Percentiles"),
		_TL(""),
		1.0, 99.0, 0.0, true, 100.0, true
	);

	Parameters.Add_Value(
		pNode	, "R_PERCENT"	, _TL("Percentage of standard deviation"),
		_TL(""),
		PARAMETER_TYPE_Double	, 150.0, 0.0, true
	);

	//-----------------------------------------------------
	pNode	= NULL;

	pNode	= Parameters.Add_Grid(
		pNode	, "GRID_G"		, _TL("Green"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Choice(
		pNode	, "G_METHOD"	, _TL("Value Preparation"),
		_TL(""),

		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("0 - 255"),
			_TL("Rescale to 0 - 255"),
			_TL("User defined rescale"),
			_TL("Percentiles"),
			_TL("Percentage of standard deviation")
		), 4
	);

	Parameters.Add_Range(
		pNode	, "G_RANGE"		, _TL("Rescale Range"),
		_TL(""),
		0, 255
	);

	Parameters.Add_Range(
		pNode	, "G_PERCTL"	, _TL("Percentiles"),
		_TL(""),
		1.0, 99.0, 0.0, true, 100.0, true
	);

	Parameters.Add_Value(
		pNode	, "G_PERCENT"	, _TL("Percentage of standard deviation"),
		_TL(""),
		PARAMETER_TYPE_Double	, 150.0, 0.0, true
	);

	//-----------------------------------------------------
	pNode	= NULL;

	pNode	= Parameters.Add_Grid(
		pNode	, "GRID_B"		, _TL("Blue"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Choice(
		pNode	, "B_METHOD"	, _TL("Value Preparation"),
		_TL(""),

		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("0 - 255"),
			_TL("Rescale to 0 - 255"),
			_TL("User defined rescale"),
			_TL("Percentiles"),
			_TL("Percentage of standard deviation")
		), 4
	);

	Parameters.Add_Range(
		pNode	, "B_RANGE"		, _TL("Rescale Range"),
		_TL(""),
		0, 255
	);

	Parameters.Add_Range(
		pNode	, "B_PERCTL"	, _TL("Percentiles"),
		_TL(""),
		1.0, 99.0, 0.0, true, 100.0, true
	);

	Parameters.Add_Value(
		pNode	, "B_PERCENT"	, _TL("Percentage of standard deviation"),
		_TL(""),
		PARAMETER_TYPE_Double	, 150.0, 0.0, true
	);

	//-----------------------------------------------------
	pNode	= NULL;

	pNode	= Parameters.Add_Grid(
		pNode	, "GRID_A"		, _TL("Transparency"),
		_TL(""),
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Choice(
		pNode	, "A_METHOD"	, _TL("Value Preparation"),
		_TL(""),

		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|"),
			_TL("0 - 255"),
			_TL("Rescale to 0 - 255"),
			_TL("User defined rescale"),
			_TL("Percentiles"),
			_TL("Percentage of standard deviation")
		), 4
	);

	Parameters.Add_Range(
		pNode	, "A_RANGE"		, _TL("Rescale Range"),
		_TL(""),
		0, 255
	);

	Parameters.Add_Range(
		pNode	, "A_PERCTL"	, _TL("Percentiles"),
		_TL(""),
		1.0, 99.0, 0.0, true, 100.0, true
	);

	Parameters.Add_Value(
		pNode	, "A_PERCENT"	, _TL("Percentage of standard deviation"),
		_TL(""),
		PARAMETER_TYPE_Double	, 150.0, 0.0, true
	);

	//-----------------------------------------------------
	Parameters.Add_Grid(
		NULL	, "GRID_RGB"	, _TL("Composite"),
		_TL(""),
		PARAMETER_OUTPUT, true, SG_DATATYPE_Int
	);
}

//---------------------------------------------------------
CGrid_RGB_Composite::~CGrid_RGB_Composite(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGrid_RGB_Composite::On_Execute(void)
{
	int			x, y, r, g, b, a;
	double		rMin, gMin, bMin, aMin, rRange, gRange, bRange, aRange;
	CSG_Grid	*pR, *pG, *pB, *pA, *pRGB;

	//-----------------------------------------------------
	pR		= _Get_Grid(Parameters("GRID_R")->asGrid(), Parameters("R_METHOD")->asInt(), Parameters("R_RANGE")->asRange(), Parameters("R_PERCTL")->asRange(), Parameters("R_PERCENT")->asDouble(), rMin, rRange);
	pG		= _Get_Grid(Parameters("GRID_G")->asGrid(), Parameters("G_METHOD")->asInt(), Parameters("G_RANGE")->asRange(), Parameters("G_PERCTL")->asRange(), Parameters("G_PERCENT")->asDouble(), gMin, gRange);
	pB		= _Get_Grid(Parameters("GRID_B")->asGrid(), Parameters("B_METHOD")->asInt(), Parameters("B_RANGE")->asRange(), Parameters("B_PERCTL")->asRange(), Parameters("B_PERCENT")->asDouble(), bMin, bRange);
	pA		= _Get_Grid(Parameters("GRID_A")->asGrid(), Parameters("A_METHOD")->asInt(), Parameters("A_RANGE")->asRange(), Parameters("A_PERCTL")->asRange(), Parameters("A_PERCENT")->asDouble(), aMin, aRange);

	//-----------------------------------------------------
	pRGB	= Parameters("GRID_RGB")->asGrid();
	pRGB->Create(pRGB->Get_System(), SG_DATATYPE_Int);
	DataObject_Set_Colors(pRGB, 100, SG_COLORS_BLACK_WHITE);

	CSG_Parameters	Parms;

	if( DataObject_Get_Parameters(pRGB, Parms) && Parms("COLORS_TYPE") )
	{
		Parms("COLORS_TYPE")->Set_Value(3);	// Color Classification Type: RGB

		DataObject_Set_Parameters(pRGB, Parms);
	}

	//-----------------------------------------------------
	for(y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{
			if( pR->is_NoData(x, y) || pG->is_NoData(x, y) || pB->is_NoData(x, y) )
			{
				pRGB->Set_NoData(x, y);
			}
			else
			{
				r	= (int)(rRange * (pR->asDouble(x, y) - rMin));
				if( r > 255 )	r	= 255;	else if( r < 0 )	r	= 0;

				g	= (int)(gRange * (pG->asDouble(x, y) - gMin));
				if( g > 255 )	g	= 255;	else if( g < 0 )	g	= 0;

				b	= (int)(bRange * (pB->asDouble(x, y) - bMin));
				if( b > 255 )	b	= 255;	else if( b < 0 )	b	= 0;

				if( pA )
				{
					a	= (int)(aRange * (pA->asDouble(x, y) - aMin));
					if( a > 255 )	a	= 255;	else if( a < 0 )	a	= 0;

					pRGB->Set_Value(x, y, SG_GET_RGBA(r, g, b, a));
				}
				else
				{
					pRGB->Set_Value(x, y, SG_GET_RGB(r, g, b));
				}
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Grid * CGrid_RGB_Composite::_Get_Grid(CSG_Grid *pGrid, int Method, CSG_Parameter_Range *pRange, CSG_Parameter_Range *pPerctl, double Percent, double &Min, double &Range)
{
	if( pGrid )
	{
		int		x, y;
		long	n;

		switch( Method )
		{
		default:
		case 0:	// 0 - 255
			Min		= 0.0;
			Range	= 1.0;
			break;

		case 1:	// Rescale to 0 - 255
			Min		= pGrid->Get_ZMin();
			Range	= pGrid->Get_ZRange();
			Range	= Range > 0.0 ? 255.0 / Range : 0.0;
			break;

		case 2:	// User defined rescale
			Min		= pRange->Get_LoVal();
			Range	= pRange->Get_HiVal() - pRange->Get_LoVal();
			Range	= Range > 0.0 ? 255.0 / Range : 0.0;
			break;

		case 3:	// Normalise
			n		= (long)(pGrid->Get_NCells() * pPerctl->Get_LoVal() / 100.0);
			pGrid->Get_Sorted(n < 0 ? 0 : (n >= pGrid->Get_NCells() ? pGrid->Get_NCells() - 1 : n), x, y, false, false);
			Min		= pGrid->asDouble(x, y);
			n		= (long)(pGrid->Get_NCells() * pPerctl->Get_HiVal() / 100.0);
			pGrid->Get_Sorted(n < 0 ? 0 : (n >= pGrid->Get_NCells() ? pGrid->Get_NCells() - 1 : n), x, y, false, false);
			Range	= pGrid->asDouble(x, y) - Min;
			Range	= Range > 0.0 ? 255.0 / Range : 0.0;
			break;

		case 4:	// Standard deviation
			Range	= sqrt(pGrid->Get_Variance()) * Percent / 100.0;
			Min		= pGrid->Get_ArithMean() - Range;
			Range	= Range * 2.0;
			Range	= Range > 0.0 ? 255.0 / Range : 0.0;
			break;
		}
	}

	return( pGrid );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
