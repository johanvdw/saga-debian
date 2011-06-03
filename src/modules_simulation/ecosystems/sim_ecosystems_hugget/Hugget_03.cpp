/**********************************************************
 * Version $Id: Hugget_03.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                 sim_ecosystems_hugget                 //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    Hugget_03.cpp                      //
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
#include "Hugget_03.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CHugget_03::CHugget_03(void)
{
	Set_Name	(_TL("03: Spatially Distributed Simulation of Soil Nitrogen Dynamics"));

	Set_Author		(SG_T("(c) 2003 by O.Conrad"));

	Set_Description	(_TW(
		"Spatially Distributed Simulation of Soil Nitrogen Dynamics. "

		"\nReference:"
		"\nHugget, R.J. (1993): 'Modelling the Human Impact on Nature', Oxford University Press.\n")
	);

	//-----------------------------------------------------
	Parameters.Add_Grid(
		NULL	, "DEM"			, _TL("Elevation"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "NSTORE"		, _TL("Soil Nitrogen"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	//-----------------------------------------------------
	Parameters.Add_Value(
		NULL	, "TIME_SPAN"	, _TL("Time Span [a]"),
		_TL(""),
		PARAMETER_TYPE_Double	, 100.0, 0.0, true
	);

	Parameters.Add_Value(
		NULL	, "TIME_STEP"	, _TL("Time Interval [a]"),
		_TL(""),
		PARAMETER_TYPE_Double	, 0.1, 0.0, true
	);

	Parameters.Add_Value(
		NULL	, "UPDATE"		, _TL("Update View"),
		_TL(""),
		PARAMETER_TYPE_Bool		, true
	);

	//-----------------------------------------------------
	Parameters.Add_Value(
		NULL	, "NINIT"		, _TL("Initial Nitrogen Content [kg/ha]"),
		_TL(""),
		PARAMETER_TYPE_Double	, 5000.0, 0.0, true
	);

	Parameters.Add_Value(
		NULL	, "NRAIN"		, _TL("Nitrogen in Rainfall [kg/ha/a]"),
		_TL(""),
		PARAMETER_TYPE_Double	, 16.0, 0.0, true
	);
}

//---------------------------------------------------------
CHugget_03::~CHugget_03(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CHugget_03::On_Execute(void)
{
	bool		bUpdate;
	int			iStep, nSteps;
	double		sTime, dTime, N_Init, N_Rain;
	CSG_Grid	*pDEM, *pN, N_1, S[8];
	CSG_String	s;

	//-----------------------------------------------------
	sTime	= Parameters("TIME_SPAN")	->asDouble();
	dTime	= Parameters("TIME_STEP")	->asDouble();
	nSteps	= (int)(sTime / dTime);

	bUpdate	= Parameters("UPDATE")		->asBool();

	N_Init	= Parameters("NINIT")		->asDouble();
	N_Rain	= Parameters("NRAIN")		->asDouble();

	pDEM	= Parameters("DEM")			->asGrid();

	pN		= Parameters("NSTORE")		->asGrid();
	pN->Assign(N_Init);

	DataObject_Set_Colors(pN, 100, SG_COLORS_YELLOW_GREEN);

	N_1.Create(pN, SG_DATATYPE_Float);

	Init_Slopes(pDEM, S);

	//-----------------------------------------------------
	for(iStep=0; iStep<=nSteps && Set_Progress(iStep, nSteps); iStep++)
	{
		s.Printf(SG_T("%s: %f (%f)"), _TL("Time [a]"), dTime * iStep, sTime);
		Process_Set_Text(s);

		if( bUpdate )
		{
			DataObject_Update(pN, pN->Get_ZMin(), pN->Get_ZMax(), true);
		}

		//-------------------------------------------------
		Step(S, pN, &N_1, N_Rain, dTime);

		//-------------------------------------------------
		pN->Assign(&N_1);
	}


	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CHugget_03::Init_Slopes(CSG_Grid *pDEM, CSG_Grid S[8])
{
	int		x, y, i, ix, iy;
	double	z;

	//-----------------------------------------------------
	for(i=0; i<8; i++)
	{
		S[i].Create(pDEM, SG_DATATYPE_Float);
	}

	//-----------------------------------------------------
	for(y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{
			if( pDEM->is_NoData(x, y) )
			{
				for(i=0; i<8; i++)
				{
					S[i].Set_NoData(x, y);
				}
			}
			else
			{
				z	= pDEM->asDouble(x, y);

				for(i=0; i<8; i++)
				{
					ix	= Get_xTo(i, x);
					iy	= Get_yTo(i, y);

					if( pDEM->is_InGrid(ix, iy) )
					{
						S[i].Set_Value(x, y, (pDEM->asDouble(ix, iy) - z) / Get_Length(i));
					}
					else
					{
						S[i].Set_Value(x, y, 0.0);
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CHugget_03::Step(CSG_Grid S[8], CSG_Grid *pN, CSG_Grid *pN_1, double N_Rain, double dTime)
{
	int		x, y, i;
	double	s, dN;

	//-----------------------------------------------------
	for(y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(x=0; x<Get_NX(); x++)
		{
			if( !S[0].is_NoData(x, y) )
			{
				for(i=0, dN=0.0; i<8; i++)
				{
					if( (s = S[i].asDouble(x, y)) != 0.0 )
					{
						if( s < 0.0 )
						{
							dN	+= s * pN->asDouble(x, y);
						}
						else
						{
							dN	+= s * pN->asDouble(Get_xTo(i, x), Get_yTo(i, y));
						}
					}
				}

				s	= pN->asDouble(x, y) + (dN + N_Rain) * dTime;

				if( s < 0.0 )
				{
					s	= N_Rain * dTime;
				}

				pN_1->Set_Value(x, y, s);
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
