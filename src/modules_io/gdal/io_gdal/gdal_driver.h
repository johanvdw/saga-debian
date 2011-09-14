/**********************************************************
 * Version $Id: gdal_driver.h 1108 2011-06-22 11:16:21Z johanvdw $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library                     //
//                                                       //
//                       io_gdal                         //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    gdal_driver.h                      //
//                                                       //
//            Copyright (C) 2007 O. Conrad               //
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
//    e-mail:     oconrad@saga-gis.de                    //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Bundesstr. 55                          //
//                D-20146 Hamburg                        //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__gdal_driver_H
#define HEADER_INCLUDED__gdal_driver_H

//---------------------------------------------------------
#include "MLB_Interface.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class io_gdal_EXPORT CSG_GDAL_Drivers
{
public:
	CSG_GDAL_Drivers(void);
	virtual ~CSG_GDAL_Drivers(void);

	CSG_String					Get_Version			(void)						const;

	int							Get_Count			(void)						const;

	class GDALDriver *			Get_Driver			(const CSG_String &Name)	const;
	class GDALDriver *			Get_Driver			(int Index)					const;

	CSG_String					Get_Name			(int Index)					const;
	CSG_String					Get_Description		(int Index)					const;
	CSG_String					Get_Extension		(int Index)					const;

	bool						Can_Read			(int Index)					const;
	bool						Can_Write			(int Index)					const;

	static int					Get_GDAL_Type		(TSG_Data_Type Type);
	static TSG_Data_Type		Get_SAGA_Type		(int           Type);


private:

	class GDALDriverManager		*m_pDrivers;

};

//---------------------------------------------------------
io_gdal_EXPORT const CSG_GDAL_Drivers &	SG_Get_GDAL_Drivers	(void);


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#define SG_GDAL_IO_CLOSED		0x00
#define SG_GDAL_IO_READ			0x01
#define SG_GDAL_IO_WRITE		0x02
#define SG_GDAL_IO_READWRITE	(SG_GDAL_IO_READ|IO_WRITE)

//---------------------------------------------------------
class io_gdal_EXPORT CSG_GDAL_DataSet
{
public:
	CSG_GDAL_DataSet(void);
	CSG_GDAL_DataSet(const CSG_String &File_Name);
	virtual ~CSG_GDAL_DataSet(void);

	bool						Open_Read			(const CSG_String &File_Name);
	bool						Open_Write			(const CSG_String &File_Name, const CSG_String &Driver, TSG_Data_Type Type, int NBands, const CSG_Grid_System &System, const CSG_Projection &Projection);
	bool						Close				(void);

	bool						is_Okay				(void)	const	{	return( m_pDataSet != NULL );	}
	bool						is_Reading			(void)	const	{	return( m_pDataSet != NULL && m_Access & SG_GDAL_IO_READ  );	}
	bool						is_Writing			(void)	const	{	return( m_pDataSet != NULL && m_Access & SG_GDAL_IO_WRITE );	}

	int							Get_NX				(void)	const	{	return( m_NX );			}
	int							Get_NY				(void)	const	{	return( m_NY );			}
	double						Get_xMin			(void)	const	{	return( m_xMin );		}
	double						Get_yMin			(void)	const	{	return( m_yMin );		}
	double						Get_Cellsize		(void)	const	{	return( m_Cellsize );	}

	bool						Needs_Transform		(void)	const	{	return( m_bTransform );	}
	void						Get_Transform		(CSG_Vector &A, CSG_Matrix &B)	const	{	A	= m_TF_A;	B	= m_TF_B;	}

	class GDALDriver *			Get_Driver			(void)	const;
	const CSG_String			Get_Name			(void)	const;
	const CSG_String			Get_Description		(void)	const;
	const char *				Get_Projection		(void)	const;
	const char **				Get_MetaData		(const char *pszDomain = "")	const;
	const char *				Get_MetaData_Item	(const char *pszName, const char *pszDomain = "")	const;

	int							Get_Count			(void)	const;
	CSG_Grid *					Read				(int i);
	bool						Write				(int i, CSG_Grid *pGrid);


private:

	bool						m_bTransform;

	int							m_Access, m_NX, m_NY;

	double						m_xMin, m_yMin, m_Cellsize;

	CSG_Vector					m_TF_A;

	CSG_Matrix					m_TF_B, m_TF_BInv;

	class GDALDataset			*m_pDataSet;


public:

	bool						to_World			(double x, double y, double &xWorld, double &yWorld)
	{
		if( m_pDataSet )
		{
			xWorld	= m_TF_A[0] + x * m_TF_B[0][1] + y * m_TF_B[0][1];
			yWorld	= m_TF_A[1] + x * m_TF_B[1][0] + y * m_TF_B[1][1];

			return( true );
		}

		return( false );
	}

	bool						from_World			(double xWorld, double yWorld, double &x, double &y)
	{
		if( m_pDataSet )
		{
			xWorld	-= m_TF_A[0];
			yWorld	-= m_TF_A[1];

			x	= xWorld * m_TF_BInv[0][0] + yWorld * m_TF_BInv[0][1];
			y	= xWorld * m_TF_BInv[1][0] + yWorld * m_TF_BInv[1][1];

			return( true );
		}

		return( false );
	}

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
io_gdal_EXPORT TSG_Data_Type			SG_Get_Grid_Type	(CSG_Parameter_Grid_List *pGrids);


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__gdal_driver_H
