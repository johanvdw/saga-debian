/**********************************************************
 * Version $Id: kriging_base.h 911 2011-02-14 16:38:15Z reklov_w $
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
//                    kriging_base.h                     //
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
#ifndef HEADER_INCLUDED__kriging_base_H
#define HEADER_INCLUDED__kriging_base_H


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
class geostatistics_kriging_EXPORT CKriging_Base : public CSG_Module
{
public:
	CKriging_Base(void);


protected:

	bool						m_bBlock;

	int							m_zField;

	double						m_Block;

	CSG_Points_Z				m_Points;

	CSG_Vector					m_G;

	CSG_Matrix					m_W;

	CSG_PRQuadTree				m_Search;

	CSG_Shapes					*m_pPoints;


	virtual bool				On_Execute				(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);

	virtual bool				On_Initialise			(void)					{	return( true );	}

	virtual bool				Get_Value				(double x, double y, double &z, double &Variance)	= 0;

	double						Get_Weight				(double d)				{	return( m_Variogram.Get_Value(d) );	}
	double						Get_Weight				(double dx, double dy)	{	return( m_Variogram.Get_Value(sqrt(dx*dx + dy*dy)) );	}


private:

	bool						m_bStdDev;

	CSG_Parameters_Grid_Target	m_Grid_Target;

	CSG_Table					m_Variances;

	CSG_Trend					m_Variogram;

	CSG_Grid					*m_pGrid, *m_pVariance;


	bool						_Initialise				(void);
	bool						_Initialise_Grids		(void);
	bool						_Finalise				(void);

	bool						_Interpolate			(void);

	bool						_Get_Variances			(void);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__kriging_base_H
