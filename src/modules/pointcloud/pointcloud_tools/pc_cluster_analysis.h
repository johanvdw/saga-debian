
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                   pointcloud_tools                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 pc_cluster_analysis.h                 //
//                                                       //
//                 Copyright (C) 2010 by                 //
//                    Volker Wichmann                    //
//                                                       //
//  This module is a port of the Grid_Cluster_Analysis   //
//  module from the Grid_Discretisation module library,  //
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
//    e-mail:     wichmann@laserdata                     //
//                                                       //
//    contact:    Volker Wichmann                        //
//                LASERDATA GmbH                         //
//                Management and analysis of             //
//                laserscanning data                     //
//                Innsbruck, Austria                     //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__PC_Cluster_Analysis_H
#define HEADER_INCLUDED__PC_Cluster_Analysis_H

//---------------------------------------------------------
#include <vector>
#include <wx/tokenzr.h>

#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CPC_Cluster_Analysis : public CSG_Module
{
public:
	CPC_Cluster_Analysis(void);
	virtual ~CPC_Cluster_Analysis(void);

	virtual const SG_Char *	Get_MenuPath	(void)	{	return( _TL("R:Classification") );	}


protected:

	virtual bool			On_Execute		(void);


private:

	int						nFields, clustField, *nMembers;

	double					**Centroids, *Variances;

	CSG_PointCloud			*pInput, *pResult;

	std::vector<bool>					vFields;
	std::vector< std::vector<double> >	vValues;	

	void					Write_Result	(CSG_Table *pTable, long nElements, int nCluster, double SP);

	double					MinimumDistance	(long &nElements, int nCluster);
	double					HillClimbing	(long &nElements, int nCluster);

};

#endif // #ifndef HEADER_INCLUDED__PC_Cluster_Analysis_H
