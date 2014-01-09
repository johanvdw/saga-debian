/**********************************************************
 * Version $Id: add_polygon_attributes.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     shapes_points                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//               add_polygon_attributes.cpp              //
//                                                       //
//                 Copyright (C) 2009 by                 //
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
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "add_polygon_attributes.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CAdd_Polygon_Attributes::CAdd_Polygon_Attributes(void)
{
	Set_Name		(_TL("Add Polygon Attributes to Points"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"Spatial join for points. Retrieves for each point the selected "
		"attributes from those polygon, which contain the point. "
	));

	//-----------------------------------------------------
	Parameters.Add_Shapes(
		NULL	, "INPUT"		, _TL("Points"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Point
	);

	Parameters.Add_Shapes(
		NULL	, "OUTPUT"		, _TL("Result"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL, SHAPE_TYPE_Point
	);

	CSG_Parameter	*pNode	= Parameters.Add_Shapes(
		NULL	, "POLYGONS"	, _TL("Polygons"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Table_Fields(
		pNode	, "FIELDS"		, _TL("Attributes"),
		_TL("Attributes to add. Select none to add all")
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CAdd_Polygon_Attributes::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Parameter_Table_Fields	*pFields	= Parameters("FIELDS")->asTableFields();

	if( pFields->Get_Count() == 0 )
	{
		Error_Set(_TL("No attributes in selection."));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Shapes	*pInput		= Parameters("INPUT")->asShapes();

	if( !pInput->is_Valid() )
	{
		Error_Set(_TL("Invalid points layer."));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Shapes	*pPolygons	= Parameters("POLYGONS")->asShapes();

	if( !pPolygons->is_Valid() )
	{
		Error_Set(_TL("Invalid polygon layer."));

		return( false );
	}

	//-----------------------------------------------------
	CSG_Shapes	*pOutput	= Parameters("OUTPUT")->asShapes();

	if( pOutput && pOutput != pInput )
	{
		pOutput->Create(*pInput);
	}
	else
	{
		Parameters("OUTPUT")->Set_Value(pOutput	= pInput);
	}

	pOutput->Set_Name(CSG_String::Format(SG_T("%s [%s]"), pInput->Get_Name(), pPolygons->Get_Name()));

	//-----------------------------------------------------
	int	outField	= pOutput->Get_Field_Count();

	{
		for(int iField=0; iField<pFields->Get_Count(); iField++)
		{
			int	jField	= pFields->Get_Index(iField);

			pOutput->Add_Field(pPolygons->Get_Field_Name(jField), pPolygons->Get_Field_Type(jField));
		}
	}

	//-----------------------------------------------------
	for(int iPoint=0; iPoint<pOutput->Get_Count() && Set_Progress(iPoint, pOutput->Get_Count()); iPoint++)
	{
		CSG_Shape	*pPoint		= pOutput	->Get_Shape(iPoint);
		CSG_Shape	*pPolygon	= pPolygons	->Get_Shape(pPoint->Get_Point(0));

		if( pPolygon )
		{
			for(int iField=0; iField<pFields->Get_Count(); iField++)
			{
				int	jField	= pFields->Get_Index(iField);

				switch( pPolygons->Get_Field_Type(jField) )
				{
				case SG_DATATYPE_String:
				case SG_DATATYPE_Date:
					pPoint->Set_Value(outField + iField, pPolygon->asString(jField));
					break;

				default:
					pPoint->Set_Value(outField + iField, pPolygon->asDouble(jField));
					break;
				}
			}
		}
	}

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
