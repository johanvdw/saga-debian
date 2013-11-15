/**********************************************************
 * Version $Id: Polygon_StatisticsFromPoints.h 1449 2012-07-16 13:04:43Z oconrad $
 *********************************************************/
/*******************************************************************************
    PolygonStatisticsFromPoints.h
    Copyright (C) Victor Olaya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__PolygonStatisticsFromPoints_H
#define HEADER_INCLUDED__PolygonStatisticsFromPoints_H

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CPolygonStatisticsFromPoints : public CSG_Module
{
public:
	CPolygonStatisticsFromPoints(void);


protected:

	virtual bool	On_Execute		(void);


private:

	CSG_String		Get_Field_Name	(const CSG_String &Type, const CSG_String &Name);


};


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__PolygonStatisticsFromPoints_H
