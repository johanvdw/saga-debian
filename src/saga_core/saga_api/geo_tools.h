
///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//           Application Programming Interface           //
//                                                       //
//                  Library: SAGA_API                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                      geo_tools.h                      //
//                                                       //
//          Copyright (C) 2005 by Olaf Conrad            //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'.                              //
//                                                       //
// This library is free software; you can redistribute   //
// it and/or modify it under the terms of the GNU Lesser //
// General Public License as published by the Free       //
// Software Foundation, version 2.1 of the License.      //
//                                                       //
// This library is distributed in the hope that it will  //
// be useful, but WITHOUT ANY WARRANTY; without even the //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU Lesser General Public //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU Lesser     //
// General Public License along with this program; if    //
// not, write to the Free Software Foundation, Inc.,     //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__SAGA_API__geo_tools_H
#define HEADER_INCLUDED__SAGA_API__geo_tools_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "api_core.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define	SG_IS_BETWEEN(a, x, b)	(((a) <= (x) && (x) <= (b)) || ((b) <= (x) && (x) <= (a)))


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum ESG_Intersection
{
	INTERSECTION_None			 = 0,
	INTERSECTION_Identical,
	INTERSECTION_Overlaps,
	INTERSECTION_Contained,
	INTERSECTION_Contains
}
TSG_Intersection;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef struct SSG_Point
{
	double						x, y;
}
TSG_Point;

//---------------------------------------------------------
typedef struct SSG_Point_Int
{
	int							x, y;
}
TSG_Point_Int;

//---------------------------------------------------------
typedef struct SSG_Point_3D
{
	double						x, y, z;
}
TSG_Point_3D;

//---------------------------------------------------------
typedef struct SSG_Rect
{
	double						xMin, yMin, xMax, yMax;
}
TSG_Rect;


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Point
{
public:
	CSG_Point(void);
	CSG_Point(const CSG_Point &Point);
	CSG_Point(const TSG_Point &Point);
	CSG_Point(double x, double y);

	~CSG_Point(void);

	operator TSG_Point &						(void)			{	return( m_point );		}

	double						Get_X			(void)	const	{	return( m_point.x );	}
	double						Get_Y			(void)	const	{	return( m_point.y );	}

	bool						operator ==		(const CSG_Point &Point)	const;
	bool						operator !=		(const CSG_Point &Point)	const;

	CSG_Point &					operator  =		(const CSG_Point &Point);
	void						operator +=		(const CSG_Point &Point);
	void						operator -=		(const CSG_Point &Point);

	CSG_Point					operator +		(const CSG_Point &Point)	const;
	CSG_Point					operator -		(const CSG_Point &Point)	const;

	void						Assign			(double x, double y);
	void						Assign			(const CSG_Point &Point);

	bool						is_Equal		(double x, double y)		const;
	bool						is_Equal		(const CSG_Point &Point)	const;


	//-----------------------------------------------------
	TSG_Point					m_point;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Points
{
public:
	CSG_Points(void);
	virtual ~CSG_Points(void);

	void						Clear			(void);

	CSG_Points &				operator  =		(const CSG_Points &Points);
	bool						Assign			(const CSG_Points &Points);

	bool						Add				(double x, double y);
	bool						Add				(const TSG_Point &Point);
	bool						Del				(int Index);

	bool						Set_Count		(int nPoints);
	int							Get_Count		(void)		const	{	return( m_nPoints );	}

	TSG_Point &					operator []		(int Index)			{	return( m_Points[Index]   );	}
	TSG_Point &					Get_Point		(int Index)			{	return( m_Points[Index]   );	}
	double						Get_X			(int Index) const	{	return( m_Points[Index].x );	}
	double						Get_Y			(int Index) const	{	return( m_Points[Index].y );	}


private:

	int							m_nPoints;

	TSG_Point					*m_Points;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Points_Int
{
public:
	CSG_Points_Int(void);
	virtual ~CSG_Points_Int(void);

	void						Clear			(void);

	CSG_Points_Int &			operator  =		(const CSG_Points_Int &Points);
	bool						Assign			(const CSG_Points_Int &Points);

	bool						Add				(int x, int y);
	bool						Add				(const TSG_Point_Int &Point);
	bool						Del				(int Index);

	bool						Set_Count		(int nPoints);
	int							Get_Count		(void)		const	{	return( m_nPoints );	}

	TSG_Point_Int &				operator []		(int Index)			{	return( m_Points[Index]   );	}
	TSG_Point_Int &				Get_Point		(int Index)			{	return( m_Points[Index]   );	}
	int							Get_X			(int Index) const	{	return( m_Points[Index].x );	}
	int							Get_Y			(int Index) const	{	return( m_Points[Index].y );	}


private:

	int							m_nPoints;

	TSG_Point_Int				*m_Points;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Points_3D
{
public:
	CSG_Points_3D(void);
	virtual ~CSG_Points_3D(void);

	void						Clear			(void);

	CSG_Points_3D &				operator  =		(const CSG_Points_3D &Points);
	bool						Assign			(const CSG_Points_3D &Points);

	bool						Add				(double x, double y, double z);
	bool						Add				(const TSG_Point_3D &Point);
	bool						Del				(int Index);

	bool						Set_Count		(int nPoints);
	int							Get_Count		(void)		const	{	return( m_nPoints );	}

	TSG_Point_3D &				operator []		(int Index)			{	return( m_Points[Index]   );	}
	TSG_Point_3D &				Get_Point		(int Index)			{	return( m_Points[Index]   );	}
	double						Get_X			(int Index)	const	{	return( m_Points[Index].x );	}
	double						Get_Y			(int Index)	const	{	return( m_Points[Index].y );	}
	double						Get_Z			(int Index)	const	{	return( m_Points[Index].z );	}


private:

	int							m_nPoints;

	TSG_Point_3D				*m_Points;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Rect
{
public:
	CSG_Rect(void);
	CSG_Rect(const CSG_Rect &Rect);
	CSG_Rect(const TSG_Rect &Rect);
	CSG_Rect(const CSG_Point &A, const CSG_Point &B);
	CSG_Rect(double xMin, double yMin, double xMax, double yMax);

	~CSG_Rect(void);

	operator const TSG_Rect &					(void) const	{	return( m_rect );	}

	bool						operator ==		(const CSG_Rect &Rect) const;
	bool						operator !=		(const CSG_Rect &Rect) const;

	CSG_Rect &					operator  =		(const CSG_Rect &Rect);
	void						operator +=		(const CSG_Point &Point);
	void						operator -=		(const CSG_Point &Point);

	void						Assign			(double xMin, double yMin, double xMax, double yMax);
	void						Assign			(const CSG_Point &A, const CSG_Point &B);
	void						Assign			(const CSG_Rect &Rect);

	void						Set_BottomLeft	(double x, double y);
	void						Set_BottomLeft	(const CSG_Point &Point);
	void						Set_TopRight	(double x, double y);
	void						Set_TopRight	(const CSG_Point &Point);

	bool						is_Equal		(double xMin, double yMin, double xMax, double yMax) const;
	bool						is_Equal		(const CSG_Rect &Rect) const;

	double						Get_XMin		(void) const	{	return( m_rect.xMin );	}
	double						Get_XMax		(void) const	{	return( m_rect.xMax );	}
	double						Get_YMin		(void) const	{	return( m_rect.yMin );	}
	double						Get_YMax		(void) const	{	return( m_rect.yMax );	}

	double						Get_XRange		(void) const	{	return( m_rect.xMax - m_rect.xMin );	}
	double						Get_YRange		(void) const	{	return( m_rect.yMax - m_rect.yMin );	}

	CSG_Point					Get_TopLeft		(void)			{	return( CSG_Point(m_rect.xMin, m_rect.yMax) );	}
	CSG_Point					Get_BottomRight	(void)			{	return( CSG_Point(m_rect.xMax, m_rect.yMin) );	}

	CSG_Point					Get_Center		(void) const	{	return( CSG_Point(Get_XCenter(), Get_YCenter()) );	}
	double						Get_XCenter		(void) const	{	return( (m_rect.xMin + m_rect.xMax) / 2.0 );	}
	double						Get_YCenter		(void) const	{	return( (m_rect.yMin + m_rect.yMax) / 2.0 );	}

	void						Move			(double dx, double dy);
	void						Move			(const CSG_Point &Point);

	void						Inflate			(double d, bool bPercent = true);
	void						Deflate			(double d, bool bPercent = true);
	void						Inflate			(double dx, double dy, bool bPercent = true);
	void						Deflate			(double dx, double dy, bool bPercent = true);

	void						Union			(const CSG_Rect &Rect);
	bool						Intersect		(const CSG_Rect &Rect);

	TSG_Intersection			Intersects		(const CSG_Rect &Rect)		const;

	bool						Contains		(double x, double y)		const;
	bool						Contains		(const CSG_Point &Point)	const;


	//-----------------------------------------------------
	TSG_Rect					m_rect;

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Rects
{
public:
	CSG_Rects(void);
	virtual ~CSG_Rects(void);

	void						Clear			(void);

	CSG_Rects &					operator  =		(const CSG_Rects &Points);
	bool						Assign			(const CSG_Rects &Points);

	bool						Add				(void);
	bool						Add				(double xMin, double yMin, double xMax, double yMax);
	bool						Add				(const CSG_Rect &Rect);

	int							Get_Count		(void)	const	{	return( m_nRects );	}

	CSG_Rect &					operator []		(int Index)		{	return( *m_Rects[Index] );	}
	CSG_Rect &					Get_Rect		(int Index)		{	return( *m_Rects[Index] );	}


private:

	int							m_nRects;

	CSG_Rect					**m_Rects;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
typedef enum ESG_Projection_Type
{
	SG_PROJ_TYPE_CS_Undefined	= 0,
	SG_PROJ_TYPE_CS_Projected,
	SG_PROJ_TYPE_CS_Geographic,
	SG_PROJ_TYPE_CS_Geocentric
}
TSG_Projection_Type;

//---------------------------------------------------------
const SG_Char	gSG_Projection_Type_Identifier[][32]	=
{
	SG_T("Undefined"),
	SG_T("PROJCS"),
	SG_T("GEOGCS"),
	SG_T("GEOCCS")
};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_String	SG_Get_Projection_Type_Name	(TSG_Projection_Type Type);

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Projection
{
public:
	CSG_Projection(void);
	virtual ~CSG_Projection(void);

									CSG_Projection			(const CSG_Projection &Projection);
	bool							Create					(const CSG_Projection &Projection);

									CSG_Projection			(int SRID, const SG_Char *Authority, const SG_Char *OpenGIS, const SG_Char *Proj4);
	bool							Create					(int SRID, const SG_Char *Authority, const SG_Char *OpenGIS, const SG_Char *Proj4);

	bool							Assign					(int SRID, const SG_Char *Authority, const SG_Char *OpenGIS, const SG_Char *Proj4);
	bool							Assign					(const CSG_Projection &Projection);
	CSG_Projection &				operator =				(const CSG_Projection &Projection)			{	Assign(Projection);	return( *this );	}

	bool							is_Equal				(const CSG_Projection &Projection)	const;
	bool							operator ==				(const CSG_Projection &Projection)	const	{	return( is_Equal(Projection) );	}

	int								Get_SRID				(void)	const	{	return( m_SRID );			}
	TSG_Projection_Type				Get_Type				(void)	const	{	return( m_Type );			}
	const CSG_String &				Get_Name				(void)	const	{	return( m_Name );			}
	const CSG_String &				Get_OpenGIS				(void)	const	{	return( m_OpenGIS );		}
	const CSG_String &				Get_Proj4				(void)	const	{	return( m_Proj4 );			}
	const CSG_String &				Get_Authority			(void)	const	{	return( m_Authority );		}
	CSG_String						Get_Type_Name			(void)	const	{	return( gSG_Projection_Type_Identifier[m_Type] );	}
	CSG_String						Get_Type_Identifier		(void)	const	{	return( SG_Get_Projection_Type_Name(m_Type) );	}

	CSG_String						asString				(void)	const;

	bool							from_ESRI				(const CSG_String &ESRI_PRJ);
	bool							to_ESRI					(CSG_String &ESRI_PRJ)	const;


private:

	int								m_SRID;

	TSG_Projection_Type				m_Type;

	CSG_String						m_Name, m_Authority, m_OpenGIS, m_Proj4;


	void							_Reset					(void);

	bool							_Get_OpenGIS_from_Proj4	(const SG_Char *Text);
	bool							_Get_Proj4_from_OpenGIS	(const SG_Char *Text);

};

//---------------------------------------------------------
class SAGA_API_DLL_EXPORT CSG_Projections
{
public:
	CSG_Projections(void);
	virtual ~CSG_Projections(void);

									CSG_Projections			(const CSG_String &File_Name);
	bool							Create					(const CSG_String &File_Name);

									CSG_Projections			(class CSG_Table *pProjections);
	bool							Create					(class CSG_Table *pProjections);

	void							Destroy					(void);

	bool							Load					(const CSG_String &File_Name);
	bool							Save					(const CSG_String &File_Name);

	bool							Add						(const CSG_Projection &Projection);
	bool							Add						(int SRID, const SG_Char *Authority, const SG_Char *OpenGIS, const SG_Char *Proj4);

	int								Get_Count				(void)	const	{	return( m_nProjections );	}
	const CSG_Projection &			Get_Projection			(int i)	const	{	return( i >= 0 && i < m_nProjections ? *(m_pProjections[i]) : m_Undefined );	}
	const CSG_Projection &			operator []				(int i) const	{	return( i >= 0 && i < m_nProjections ? *(m_pProjections[i]) : m_Undefined );	}

	CSG_String						Get_Names				(void)	const;
	int								Get_SRID_byNamesIndex	(int i)	const;


private:

	int								m_nProjections, m_nBuffer;

	CSG_Projection					**m_pProjections, m_Undefined;

	static CSG_Projections			*s_pProjections;

	class CSG_Index					*m_pIdx_Names, *m_pIdx_SRIDs;


	void							_On_Construction		(void);
	CSG_Projection *				_Add					(void);

	static int						_Cmp_Names				(const int iElement_1, const int iElement_2);
	static int						_Cmp_SRIDs				(const int iElement_1, const int iElement_2);

};

//---------------------------------------------------------
SAGA_API_DLL_EXPORT CSG_Projections &	SG_Get_Projections	(void);


///////////////////////////////////////////////////////////
//														 //
//						Functions						 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
SAGA_API_DLL_EXPORT double		SG_Get_Length					(double dx, double dy);
SAGA_API_DLL_EXPORT double		SG_Get_Distance					(double ax, double ay, double bx, double by);
SAGA_API_DLL_EXPORT double		SG_Get_Distance					(const TSG_Point &A, const TSG_Point &B);

SAGA_API_DLL_EXPORT double		SG_Get_Angle_Of_Direction		(double dx, double dy);
SAGA_API_DLL_EXPORT double		SG_Get_Angle_Of_Direction		(double ax, double ay, double bx, double by);
SAGA_API_DLL_EXPORT double		SG_Get_Angle_Of_Direction		(const TSG_Point &A);
SAGA_API_DLL_EXPORT double		SG_Get_Angle_Of_Direction		(const TSG_Point &A, const TSG_Point &B);

SAGA_API_DLL_EXPORT bool		SG_Get_Crossing					(TSG_Point &Crossing, const TSG_Point &a1, const TSG_Point &a2, const TSG_Point &b1, const TSG_Point &b2, bool bExactMatch = true);
SAGA_API_DLL_EXPORT bool		SG_Get_Crossing_InRegion		(TSG_Point &Crossing, const TSG_Point &a , const TSG_Point & b, const TSG_Rect &Region);

SAGA_API_DLL_EXPORT double		SG_Get_Nearest_Point_On_Line	(const TSG_Point &Point, const TSG_Point &Ln_A, const TSG_Point &Ln_B, TSG_Point &Ln_Point, bool bExactMatch = true);

SAGA_API_DLL_EXPORT bool		SG_Get_Triangle_CircumCircle	(TSG_Point Triangle[3], TSG_Point &Point, double &Radius);

SAGA_API_DLL_EXPORT double		SG_Get_Polygon_Area				(TSG_Point *Points, int nPoints);
SAGA_API_DLL_EXPORT double		SG_Get_Polygon_Area				(const CSG_Points &Points);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__SAGA_API__geo_tools_H
