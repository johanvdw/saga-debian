/**********************************************************
 * Version $Id: Polygon_Intersection.h 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                    shapes_polygons                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                 Polygon_Intersection.h                //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
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
//                                                       //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Polygon_Intersection_H
#define HEADER_INCLUDED__Polygon_Intersection_H


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
class CPolygon_Overlay : public CSG_Module
{
public:
	CPolygon_Overlay(const CSG_String &Name);

	virtual CSG_String		Get_MenuPath		(void)	{	return( _TL("R:Overlay") );	}


protected:

	bool					Add_Description		(const CSG_String &Text);

	bool					Initialize			(CSG_Shapes **ppA, CSG_Shapes **ppB, bool bBothAttributes);

	bool					Get_Intersection	(CSG_Shapes *pA, CSG_Shapes *pB);

	bool					Get_Difference		(CSG_Shapes *pA, CSG_Shapes *pB, bool bInvert = false);


private:

	bool					m_bSplit, m_bInvert;

	CSG_Shapes				*m_pA, *m_pB, *m_pAB;


	CSG_Shape *				Add_Polygon			(int id_A, int id_B);
	void					Add_Polygon			(CSG_Shape *pShape, int id_A, int id_B = -1);

};


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CPolygon_Intersection : public CPolygon_Overlay
{
public:
	CPolygon_Intersection(void);


protected:

	virtual bool			On_Execute			(void);

};

//---------------------------------------------------------
class CPolygon_Difference : public CPolygon_Overlay
{
public:
	CPolygon_Difference(void);


protected:

	virtual bool			On_Execute			(void);

};

//---------------------------------------------------------
class CPolygon_SymDifference : public CPolygon_Overlay
{
public:
	CPolygon_SymDifference(void);


protected:

	virtual bool			On_Execute			(void);

};

//---------------------------------------------------------
class CPolygon_Union : public CPolygon_Overlay
{
public:
	CPolygon_Union(void);


protected:

	virtual bool			On_Execute			(void);

};

//---------------------------------------------------------
class CPolygon_Identity : public CPolygon_Overlay
{
public:
	CPolygon_Identity(void);


protected:

	virtual bool			On_Execute			(void);

};

//---------------------------------------------------------
class CPolygon_Update : public CPolygon_Overlay
{
public:
	CPolygon_Update(void);


protected:

	virtual bool			On_Execute			(void);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Polygon_Intersection_H
