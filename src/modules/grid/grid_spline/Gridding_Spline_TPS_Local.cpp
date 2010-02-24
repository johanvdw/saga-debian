
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     grid_spline                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//              Gridding_Spline_TPS_Local.cpp            //
//                                                       //
//                 Copyright (C) 2006 by                 //
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
#include "Gridding_Spline_TPS_Local.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGridding_Spline_TPS_Local::CGridding_Spline_TPS_Local(void)
	: CGridding_Spline_TPS_Global()
{
	Set_Name		(_TL("Thin Plate Spline (Local)"));

	Set_Author		(SG_T("(c) 2006 by O.Conrad"));

	Set_Description	(_TW(
		"Creates a 'Thin Plate Spline' function for each grid point "
		"based on all of the scattered data points that are within a "
		"given distance. The number of points can be limited to a "
		"maximum number of closest points. "
		"\n\n"
		"References:\n"
		"- Donato G., Belongie S. (2002):"
		" 'Approximation Methods for Thin Plate Spline Mappings and Principal Warps',"
		" In Heyden, A., Sparr, G., Nielsen, M., Johansen, P. (Eds.):"
		" 'Computer Vision � ECCV 2002: 7th European Conference on Computer Vision, Copenhagen, Denmark, May 28�31, 2002',"
		" Proceedings, Part III, Lecture Notes in Computer Science."
		" Springer-Verlag Heidelberg; pp.21-31."
		"\n"
		"\n"
		"- Elonen, J. (2005):"
		" 'Thin Plate Spline editor - an example program in C++',"
		" <a target=\"_blank\" href=\"http://elonen.iki.fi/code/tpsdemo/index.html\">http://elonen.iki.fi/code/tpsdemo/index.html</a>."
		"\n"
	));

	//-----------------------------------------------------
	Parameters.Add_Value(
		NULL	, "RADIUS"		, _TL("Search Radius"),
		_TL(""),
		PARAMETER_TYPE_Double	, 100.0
	);

	Parameters.Add_Choice(
		NULL	, "MODE"		, _TL("Search Mode"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("all directions"),
			_TL("quadrants")
		)
	);

	Parameters.Add_Choice(
		NULL	, "SELECT"		, _TL("Points Selection"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|"),
			_TL("all points in search radius"),
			_TL("maximum number of points")
		), 1
	);

	Parameters.Add_Value(
		NULL	, "MAXPOINTS"	, _TL("Maximum Number of Points"),
		_TL(""),
		PARAMETER_TYPE_Int		, 10.0, 1, true
	);

	//-----------------------------------------------------
	Parameters("REGUL")->Set_Value(0.0001);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGridding_Spline_TPS_Local::On_Execute(void)
{
	bool	bResult	= false;

	if( Initialise() && m_Search.Create(m_pShapes, m_zField) )
	{
		m_Radius		= Parameters("RADIUS")		->asDouble();
		m_nPoints_Max	= Parameters("SELECT")		->asInt() == 1
						? Parameters("MAXPOINTS")	->asInt()  : -1;
		m_Mode			= Parameters("MODE")		->asInt();

		int			x, y;
		TSG_Point	p;

		for(y=0, p.y=m_pGrid->Get_YMin(); y<m_pGrid->Get_NY() && Set_Progress(y, m_pGrid->Get_NY()); y++, p.y+=m_pGrid->Get_Cellsize())
		{
			for(x=0, p.x=m_pGrid->Get_XMin(); x<m_pGrid->Get_NX(); x++, p.x+=m_pGrid->Get_Cellsize())
			{
				Set_Value(x, y, p);
			}
		}

		m_Search.Destroy();
		m_Spline.Destroy();

		bResult	= true;
	}

	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGridding_Spline_TPS_Local::Get_Points(const TSG_Point &p, int iQuadrant)
{
	double	x, y, z;

	if( m_Search.Select_Nearest_Points(p.x, p.y, m_nPoints_Max, m_Radius, iQuadrant) > 0 )
	{
		for(int i=0; i<m_Search.Get_Selected_Count(); i++)
		{
			if( m_Search.Get_Selected_Point(i, x, y, z) )
			{
				m_Spline.Add_Point(x, y, z);
			}
		}
	}

	return( m_Search.Get_Selected_Count() );
}

//---------------------------------------------------------
bool CGridding_Spline_TPS_Local::Set_Value(int x, int y, const TSG_Point &p)
{
	int		nPoints	= 0;

	m_Spline.Destroy();

	switch( m_Mode )
	{
	default:
		nPoints	+= Get_Points(p);
		break;

	case 1:
		nPoints	+= Get_Points(p, 0);
		nPoints	+= Get_Points(p, 1);
		nPoints	+= Get_Points(p, 2);
		nPoints	+= Get_Points(p, 3);
		break;
	}

	//-----------------------------------------------------
	if( nPoints >= 3 && m_Spline.Create(m_Regularisation, true) )
	{
		m_pGrid->Set_Value(x, y, m_Spline.Get_Value(p.x, p.y));

		return( true );
	}

	m_pGrid->Set_NoData(x, y);

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
