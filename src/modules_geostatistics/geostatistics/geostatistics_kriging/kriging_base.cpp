/**********************************************************
 * Version $Id: kriging_base.cpp 1206 2011-10-28 11:54:29Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//            geostatistics_kriging_variogram            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   kriging_base.cpp                    //
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
//                University of Hamburg                  //
//                Bundesstr. 55                          //
//                20146 Hamburg                          //
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
#include "variogram_dialog.h"

#include "kriging_base.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CKriging_Base::CKriging_Base(void)
{
	CSG_Parameter	*pNode;
	CSG_Parameters	*pParameters;

	//-----------------------------------------------------
	pNode	= Parameters.Add_Shapes(
		NULL	, "POINTS"		, _TL("Points"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Point
	);

	Parameters.Add_Table_Field(
		pNode	, "ZFIELD"		, _TL("Attribute"),
		_TL("")
	);

	//-----------------------------------------------------
	Parameters.Add_Choice(
		NULL	, "TARGET"		, _TL("Target Grid"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("user defined"),
			_TL("grid")
		), 0
	);

	Parameters.Add_Choice(
		NULL	, "TQUALITY"	, _TL("Type of Quality Measure"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("standard deviation"),
			_TL("variance")
		), 0
	);

	//-----------------------------------------------------
	Parameters.Add_Value(
		NULL	, "LOG"			, _TL("Logarithmic Transformation"),
		_TL(""),
		PARAMETER_TYPE_Bool
	);

	pNode	= Parameters.Add_Value(
		NULL	, "BLOCK"		, _TL("Block Kriging"),
		_TL(""),
		PARAMETER_TYPE_Bool		, false
	);

	Parameters.Add_Value(
		pNode	, "DBLOCK"		, _TL("Block Size"),
		_TL(""),
		PARAMETER_TYPE_Double	, 100.0, 0.0, true
	);

	///////////////////////////////////////////////////////
	//-----------------------------------------------------
	if( !SG_UI_Get_Window_Main() )
	{
		Parameters.Add_Value(
			NULL	, "VAR_MAXDIST"		, _TL("Maximum Distance"),
			_TL(""),
			PARAMETER_TYPE_Double	, -1.0
		);

		Parameters.Add_Value(
			NULL	, "VAR_NCLASSES"	, _TL("Lag Distance Classes"),
			_TL("initial number of lag distance classes"),
			PARAMETER_TYPE_Int		, 100, 1, true
		);

		Parameters.Add_Value(
			NULL	, "VAR_NSKIP"		, _TL("Skip"),
			_TL(""),
			PARAMETER_TYPE_Int, 1, 1, true
		);

		Parameters.Add_String(
			NULL	, "VAR_MODEL"		, _TL("Model"),
			_TL(""),
			SG_T("a + b * x")
		);
	}

	///////////////////////////////////////////////////////
	//-----------------------------------------------------
	pParameters = Add_Parameters("USER", _TL("User Defined Grid")	, _TL(""));

	pParameters->Add_Value(
		NULL	, "BVARIANCE"	, _TL("Create Quality Grid"),
		_TL(""),
		PARAMETER_TYPE_Bool, true
	);

	m_Grid_Target.Add_Parameters_User(pParameters);

	//-----------------------------------------------------
	pParameters = Add_Parameters("GRID", _TL("Choose Grid")			, _TL(""));

	m_Grid_Target.Add_Parameters_Grid(pParameters);

	//-----------------------------------------------------
	m_Grid_Target.Add_Grid_Parameter(SG_T("VARIANCE"), _TL("Quality Measure"), true);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CKriging_Base::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	return( m_Grid_Target.On_User_Changed(pParameters, pParameter) ? 1 : 0 );
}

//---------------------------------------------------------
int CKriging_Base::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("GLOBAL")) )
	{
		pParameters->Get_Parameter("MAXRADIUS")		->Set_Enabled(pParameter->asInt() == 0);	// local
	}

	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("ALL_POINTS")) )
	{
		pParameters->Get_Parameter("NPOINTS_MAX")	->Set_Enabled(pParameter->asInt() == 0);	// maximum number of points
	}

	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("BLOCK")) )
	{
		pParameters->Get_Parameter("DBLOCK")		->Set_Enabled(pParameter->asBool());		// block size
	}

	return( 1 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CKriging_Base::On_Execute(void)
{
	if( !_Initialise() )
	{
		return( false );
	}

	bool	bResult	= false;

	CSG_Table	Variogram;

	//-----------------------------------------------------
	if( SG_UI_Get_Window_Main() )
	{
		static CVariogram_Dialog	dlg;

		if( dlg.Execute(m_pPoints, m_zField, m_bLog, &Variogram, &m_Model) )
		{
			bResult	= true;
		}
	}

	//-----------------------------------------------------
	else
	{
		int		nSkip		= Parameters("VAR_NSKIP")		->asInt();
		int		nClasses	= Parameters("VAR_NCLASSES")	->asInt();
		double	maxDistance	= Parameters("VAR_MAXDIST")		->asDouble();

		m_Model.Set_Formula(Parameters("VAR_MODEL")->asString());

		if( CSG_Variogram::Calculate(m_pPoints, m_zField, m_bLog, &Variogram, nClasses, maxDistance, nSkip) )
		{
			m_Model.Clr_Data();

			for(int i=0; i<Variogram.Get_Count(); i++)
			{
				CSG_Table_Record	*pRecord	= Variogram.Get_Record(i);

				m_Model.Add_Data(pRecord->asDouble(CSG_Variogram::FIELD_DISTANCE), pRecord->asDouble(CSG_Variogram::FIELD_VAR_EXP));
			}

			bResult	= m_Model.Get_Trend() || m_Model.Get_Parameter_Count() == 0;
		}
	}

	//-----------------------------------------------------
	if( bResult && On_Initialise() )
	{
		Message_Add(CSG_String::Format(SG_T("%s: %s"), _TL("variogram model"), m_Model.Get_Formula(SG_TREND_STRING_Formula_Parameters).c_str()), false);

		bResult	= _Interpolate();
	}

	_Finalise();

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CKriging_Base::_Initialise(void)
{
	//-----------------------------------------------------
	m_Block		= Parameters("DBLOCK")	->asDouble() / 2.0;
	m_bBlock	= Parameters("BLOCK")	->asBool() && m_Block > 0.0;
	m_bStdDev	= Parameters("TQUALITY")->asInt() == 0;
	m_bLog		= Parameters("LOG")		->asBool();

	//-----------------------------------------------------
	m_pPoints	= Parameters("POINTS")	->asShapes();
	m_zField	= Parameters("ZFIELD")	->asInt();

	if( m_pPoints->Get_Count() <= 1 )
	{
		SG_UI_Msg_Add(_TL("not enough points for interpolation"), true);

		return( false );
	}

	//-----------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CKriging_Base::_Initialise_Grids(void)
{
	//-----------------------------------------------------
	m_pGrid		= NULL;
	m_pVariance	= NULL;

	switch( Parameters("TARGET")->asInt() )
	{
	case 0:	// user defined...
		if( m_Grid_Target.Init_User(m_pPoints->Get_Extent()) && Dlg_Parameters("USER") )
		{
			m_pGrid		= m_Grid_Target.Get_User();

			if( Get_Parameters("USER")->Get_Parameter("BVARIANCE")->asBool() )
			{
				m_pVariance	= m_Grid_Target.Get_User(SG_T("VARIANCE"));
			}
		}
		break;

	case 1:	// grid...
		if( Dlg_Parameters("GRID") )
		{
			m_pGrid		= m_Grid_Target.Get_Grid();
			m_pVariance	= m_Grid_Target.Get_Grid(SG_T("VARIANCE"));
		}
		break;
	}

	if( !m_pGrid )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_pGrid->Set_Name(CSG_String::Format(SG_T("%s (%s)"), m_pPoints->Get_Name(), Get_Name()));

	if( m_pVariance )
	{
		m_pVariance->Set_Name(CSG_String::Format(SG_T("%s (%s - %s)"), m_pPoints->Get_Name(), Get_Name(), m_bStdDev ? _TL("Standard Deviation") : _TL("Variance")));
	}

	return( true );
}

//---------------------------------------------------------
bool CKriging_Base::_Finalise(void)
{
	m_Points	.Clear();
	m_Search	.Destroy();
	m_G			.Destroy();
	m_W			.Destroy();
	m_Model		.Clr_Data();

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CKriging_Base::_Interpolate(void)
{
	if( _Initialise_Grids() )
	{
		int		ix, iy;
		double	x, y, z, v;

		for(iy=0, y=m_pGrid->Get_YMin(); iy<m_pGrid->Get_NY() && Set_Progress(iy, m_pGrid->Get_NY()); iy++, y+=m_pGrid->Get_Cellsize())
		{
			for(ix=0, x=m_pGrid->Get_XMin(); ix<m_pGrid->Get_NX(); ix++, x+=m_pGrid->Get_Cellsize())
			{
				if( Get_Value(x, y, z, v) )
				{
					m_pGrid->Set_Value(ix, iy, m_bLog ? exp(z) : z);

					if( m_pVariance )
					{
						m_pVariance->Set_Value(ix, iy, m_bStdDev ? sqrt(v) : v);
					}
				}
				else
				{
					m_pGrid->Set_NoData(ix, iy);

					if( m_pVariance )
					{
						m_pVariance->Set_NoData(ix, iy);
					}
				}
			}
		}

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
