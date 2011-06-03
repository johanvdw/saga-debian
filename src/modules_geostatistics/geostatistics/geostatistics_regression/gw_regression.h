/**********************************************************
 * Version $Id: gw_regression.h 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                  Geostatistics_Grid                   //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    gw_regression.h                    //
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
#ifndef HEADER_INCLUDED__GW_Regression_H
#define HEADER_INCLUDED__GW_Regression_H

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CGW_Regression : public CSG_Module
{
public:
	CGW_Regression(void);


protected:

	virtual bool				On_Execute				(void);

	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter);


private:

	int							m_iDependent, m_iPredictor, m_nPoints_Min, m_nPoints_Max, m_Mode;

	double						m_Radius;

	CSG_Parameters_Grid_Target	m_Grid_Target;

	CSG_Distance_Weighting		m_Weighting;

	CSG_PRQuadTree				m_Search;

	CSG_Vector					m_y, m_z, m_w;

	CSG_Shapes					*m_pPoints;

	CSG_Grid					*m_pIntercept, *m_pSlope, *m_pQuality;


	int							Set_Variables			(int x, int y);

	bool						Get_Regression			(int x, int y);

	bool						Set_Residuals			(void);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__GW_Regression_H
