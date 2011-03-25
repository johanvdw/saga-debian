
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                   Projection_Proj4                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   crs_transform.cpp                   //
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
#include "crs_transform.h"

#include <projects.h>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define PROJ4_FREE(p)	if( p )	{	pj_free((PJ *)p);	p	= NULL;	}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CCRS_Transform::CCRS_Transform(void)
{
	m_Proj4_pSource	= NULL;
	m_Proj4_pTarget	= NULL;
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCRS_Transform::On_Execute(void)
{
	//-----------------------------------------------------
	if( !Get_Projection(m_Target) )
	{
		return( false );
	}

	if(	!_Set_Projection(m_Target, &m_Proj4_pTarget, false) )
	{
		return( false );
	}

	//-----------------------------------------------------
	m_bInverse	= false;

	bool	bResult	= On_Execute_Transformation();

	//-------------------------------------------------
	PROJ4_FREE(m_Proj4_pSource);
	PROJ4_FREE(m_Proj4_pTarget);

	//-----------------------------------------------------
	return( bResult );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCRS_Transform::_Set_Projection(const CSG_Projection &Projection, void **ppProj4, bool bInverse)
{
	PROJ4_FREE(*ppProj4);

	//-------------------------------------------------
	if( (*ppProj4 = pj_init_plus(SG_STR_SGTOMB(Projection.Get_Proj4()))) == NULL )
	{
		Error_Set(CSG_String::Format(SG_T("Proj4 [%s]: %s"), _TL("initialization"), SG_STR_MBTOSG(pj_strerrno(pj_errno))));

		return( false );
	}

	//-------------------------------------------------
	if( bInverse && ((PJ *)(*ppProj4))->inv == NULL )
	{
		Error_Set(CSG_String::Format(SG_T("Proj4 [%s]: %s"), _TL("initialization"), _TL("inverse transformation not available")));

		return( false );
	}

	return( true );
}

//---------------------------------------------------------
bool CCRS_Transform::Set_Source(const CSG_Projection &Projection)
{
	return( _Set_Projection(Projection, &m_Proj4_pSource, true) );
}

//---------------------------------------------------------
bool CCRS_Transform::Set_Inverse(bool bOn)
{
	if( m_bInverse == bOn )
	{
		return( true );
	}

	if( m_Proj4_pTarget && ((PJ *)m_Proj4_pTarget)->inv )
	{
		m_bInverse	= bOn;

		void	*pTMP	= m_Proj4_pSource;
		m_Proj4_pSource	= m_Proj4_pTarget;
		m_Proj4_pTarget	= pTMP;

		return( true );
	}

	Error_Set(CSG_String::Format(SG_T("Proj4 [%s]: %s"), _TL("initialization"), _TL("inverse transformation not available")));

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CCRS_Transform::Get_Transformation(double &x, double &y)
{
	if( m_Proj4_pSource && m_Proj4_pTarget )
	{
		double	z	= 0.0;

		if( pj_is_latlong((PJ *)m_Proj4_pSource) )
		{
			x	*= DEG_TO_RAD;
			y	*= DEG_TO_RAD;
		}

		if( pj_transform((PJ *)m_Proj4_pSource, (PJ *)m_Proj4_pTarget, 1, 0, &x, &y, &z) == 0 )
		{
			if( pj_is_latlong((PJ *)m_Proj4_pTarget) )
			{
				x	*= RAD_TO_DEG;
				y	*= RAD_TO_DEG;
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