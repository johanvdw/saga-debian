/**********************************************************
 * Version $Id: Polygon_Union.cpp 1230 2011-11-22 11:12:10Z oconrad $
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
//                   Polygon_Union.cpp                   //
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
#include "Polygon_Union.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CPolygon_Dissolve::CPolygon_Dissolve(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name		(_TL("Polygon Dissolve"));

	Set_Author		(SG_T("O.Conrad (c) 2008"));

	Set_Description	(_TW(
		"Merges polygons, which share the same attribute value, and "
		"(optionally) dissolves borders between adjacent polygon parts.\n"
		"Uses the free and open source software library <b>Clipper</b> created by Angus Johnson.\n"
		"<a target=\"_blank\" href=\"http://www.angusj.com/delphi/clipper.php\">Clipper Homepage</a>\n"
		"<a target=\"_blank\" href=\"http://sourceforge.net/projects/polyclipping/\">Clipper at SourceForge</a>\n"
	));

	//-----------------------------------------------------
	pNode	= Parameters.Add_Shapes(
		NULL	, "POLYGONS"	, _TL("Polygons"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Table_Field(
		pNode	, "FIELD_1"		, _TL("1. Attribute"),
		_TL(""),
		false
	);

	Parameters.Add_Table_Field(
		pNode	, "FIELD_2"		, _TL("2. Attribute"),
		_TL(""),
		true
	);

	Parameters.Add_Table_Field(
		pNode	, "FIELD_3"		, _TL("3. Attribute"),
		_TL(""),
		true
	);

	Parameters.Add_Shapes(
		NULL	, "DISSOLVED"	, _TL("Dissolved Polygons"),
		_TL(""),
		PARAMETER_OUTPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Choice(
		NULL	, "DISSOLVE"	, _TL("Dissolve..."),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|"),
			_TL("polygons with same attribute value"),
			_TL("all polygons"),
			_TL("polygons with same attribute value (keep inner boundaries)"),
			_TL("all polygons (keep inner boundaries)")
		), 0
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CPolygon_Dissolve::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("POLYGONS")) && pParameters->Get_Parameter("POLYGONS")->asShapes() != NULL )
	{
		int	nFields	= pParameters->Get_Parameter("POLYGONS")->asShapes()->Get_Field_Count();

		pParameters->Get_Parameter("FIELD_2")->Set_Value(nFields);
		pParameters->Get_Parameter("FIELD_3")->Set_Value(nFields);
	}

	return( 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CPolygon_Dissolve::On_Execute(void)
{
	bool		bAll, bDissolve;
	int			Field_1, Field_2, Field_3, iPolygon;
	CSG_String	Value;
	CSG_Shape	*pPolygon , *pUnion;
	CSG_Shapes	*pPolygons, *pUnions;

	//-----------------------------------------------------
	pPolygons	= Parameters("POLYGONS")	->asShapes();
	pUnions		= Parameters("DISSOLVED")	->asShapes();
	Field_1		= Parameters("FIELD_1")		->asInt();
	Field_2		= Parameters("FIELD_2")		->asInt();
	Field_3		= Parameters("FIELD_3")		->asInt();
	bAll		= Parameters("DISSOLVE")	->asInt() % 2 == 1;
	bDissolve	= Parameters("DISSOLVE")	->asInt() / 2 == 0;

	//-----------------------------------------------------
	if(	pPolygons->is_Valid() )
	{
		pUnions	->Create(SHAPE_TYPE_Polygon);

		//-------------------------------------------------
		if( bAll || Field_1 >= pPolygons->Get_Field_Count() )
		{
			pUnions->Set_Name(CSG_String::Format(SG_T("%s [%s]"), pPolygons->Get_Name(), _TL("Dissolved")));
			pUnions->Add_Field(_TL("ID"), SG_DATATYPE_Int);

			pUnion	= pUnions->Add_Shape(pPolygons->Get_Shape(0), SHAPE_COPY_GEOM);

			for(iPolygon=1; iPolygon<pPolygons->Get_Count() && Set_Progress(iPolygon, pPolygons->Get_Count()); iPolygon++)
			{
				for(int iPart=0; iPart<pPolygon->Get_Part_Count(); iPart++)
				{
					CSG_Shape_Part	*pPart	= ((CSG_Shape_Polygon *)pPolygons->Get_Shape(iPolygon))->Get_Part(iPart);

					for(int iPoint=0, nParts=pUnion->Get_Part_Count(); iPoint<pPart->Get_Count(); iPoint++)
					{
						pUnion->Add_Point(pPart->Get_Point(iPoint), nParts);
					}
				}
			}

			if( bDissolve )
			{
				SG_Polygon_Dissolve(pUnion);
			}
		}

		//-------------------------------------------------
		else
		{
			Value	= pPolygons->Get_Field_Name(Field_1);
			pUnions	->Add_Field(pPolygons->Get_Field_Name(Field_1), pPolygons->Get_Field_Type(Field_1));

			if( Field_2 >= 0 )
			{
				Value	+= CSG_String::Format(SG_T(", %s"), pPolygons->Get_Field_Name(Field_2));
				pUnions	->Add_Field(pPolygons->Get_Field_Name(Field_2), pPolygons->Get_Field_Type(Field_2));
			}

			if( Field_3 >= 0 )
			{
				Value	+= CSG_String::Format(SG_T(", %s"), pPolygons->Get_Field_Name(Field_3));
				pUnions	->Add_Field(pPolygons->Get_Field_Name(Field_3), pPolygons->Get_Field_Type(Field_3));
			}

			pPolygons->Set_Index(Field_1, TABLE_INDEX_Ascending, Field_2, TABLE_INDEX_Ascending, Field_3, TABLE_INDEX_Ascending);

			pUnions->Set_Name(CSG_String::Format(SG_T("%s [%s: %s]"), pPolygons->Get_Name(), _TL("Dissolved"), Value.c_str()));

			for(iPolygon=0, pUnion=NULL; iPolygon<pPolygons->Get_Count() && Set_Progress(iPolygon, pPolygons->Get_Count()); iPolygon++)
			{
				pPolygon	= pPolygons->Get_Shape(pPolygons->Get_Record_byIndex(iPolygon)->Get_Index());

				CSG_String	s(pPolygon->asString(Field_1));

				if( Field_2 >= 0 )	s	+= pPolygon->asString(Field_2);
				if( Field_3 >= 0 )	s	+= pPolygon->asString(Field_3);

				if( iPolygon == 0 || Value.Cmp(s) )
				{
					if( pUnion && bDissolve )
					{
						SG_Polygon_Dissolve(pUnion);
					}

					Value	= s;

					pUnion	= pUnions->Add_Shape(pPolygon, SHAPE_COPY_GEOM);

					pUnion->Set_Value(0, pPolygon->asString(Field_1));

					if( Field_2 >= 0 )	pUnion->Set_Value(1, pPolygon->asString(Field_2));
					if( Field_3 >= 0 )	pUnion->Set_Value(2, pPolygon->asString(Field_3));
				}
				else
				{
					for(int iPart=0; iPart<pPolygon->Get_Part_Count(); iPart++)
					{
						for(int iPoint=0, nParts=pUnion->Get_Part_Count(); iPoint<pPolygon->Get_Point_Count(iPart); iPoint++)
						{
							pUnion->Add_Point(pPolygon->Get_Point(iPoint, iPart), nParts);
						}
					}
				}
			}

			if( pUnion && bDissolve )
			{
				SG_Polygon_Dissolve(pUnion);
			}
		}

		//-------------------------------------------------
		return( pUnions->is_Valid() );
	}

	//-----------------------------------------------------
	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
