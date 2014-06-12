/**********************************************************
 * Version $Id: Georef_Shapes.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                    pj_georeference                    //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   Georef_Shapes.cpp                   //
//                                                       //
//                 Copyright (C) 2006 by                 //
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
//    e-mail:     oconrad@saga-gis.de                    //
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
#include "Georef_Shapes.h"
#include "Georef_Engine.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CGeoref_Shapes::CGeoref_Shapes(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name		(_TL("Warping Shapes"));

	Set_Author		(SG_T("O.Conrad (c) 2006"));

	Set_Description	(_TW(
		"Georeferencing of shapes layers. Either choose the attribute fields (x/y) "
		"with the projected coordinates for the reference points (origin) or supply a "
		"additional points layer with correspondend points in the target projection. "
	));

	//-----------------------------------------------------
	pNode	= Parameters.Add_Shapes(
		NULL	, "REF_SOURCE"	, _TL("Reference Points (Origin)"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Shapes(
		NULL	, "REF_TARGET"	, _TL("Reference Points (Projection)"),
		_TL(""),
		PARAMETER_INPUT_OPTIONAL
	);

	Parameters.Add_Table_Field(
		pNode	, "XFIELD"		, _TL("x Position"),
		_TL("")
	);

	Parameters.Add_Table_Field(
		pNode	, "YFIELD"		, _TL("y Position"),
		_TL("")
	);

	Parameters.Add_Choice(
		NULL	, "METHOD"		, _TL("Method"),
		_TL(""),
		GEOREF_METHODS_CHOICE, 0
	);

	Parameters.Add_Value(
		NULL	, "ORDER"		,_TL("Polynomial Order"),
		_TL(""),
		PARAMETER_TYPE_Int, 3, 1, true
	);

	//-----------------------------------------------------
	Parameters.Add_Shapes(
		NULL	, "INPUT"		, _TL("Input"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Shapes(
		NULL	, "OUTPUT"		, _TL("Output"),
		_TL(""),
		PARAMETER_OUTPUT
	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CGeoref_Shapes::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if( !SG_STR_CMP(pParameter->Get_Identifier(), "REF_TARGET") )
	{
		pParameters->Get_Parameter("XFIELD")->Set_Enabled(pParameter->asShapes() == NULL);
		pParameters->Get_Parameter("YFIELD")->Set_Enabled(pParameter->asShapes() == NULL);
	}

	if( !SG_STR_CMP(pParameter->Get_Identifier(), "METHOD") )
	{
		pParameters->Get_Parameter("ORDER")->Set_Enabled(pParameter->asInt() == GEOREF_Polynomial);	// only show for polynomial, user defined order
	}

	return( 1 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CGeoref_Shapes::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Shapes	*pShapes_A	= Parameters("REF_SOURCE")->asShapes();
	CSG_Shapes	*pShapes_B	= Parameters("REF_TARGET")->asShapes();

	int	xField	= Parameters("XFIELD")->asInt();
	int	yField	= Parameters("YFIELD")->asInt();

	int	Method	= Parameters("METHOD")->asInt();
	int	Order	= Parameters("ORDER" )->asInt();

	//-----------------------------------------------------
	CGeoref_Engine	Engine;

	if( (pShapes_B != NULL && Engine.Set_Reference(pShapes_A, pShapes_B     ) == false) )
	{
		return( false );
	}

	if( (pShapes_B == NULL && Engine.Set_Reference(pShapes_A, xField, yField) == false) )
	{
		return( false );
	}

	if( !Engine.Evaluate(Method, Order) )
	{
		return( false );
	}

	//---------------------------------------------------------
	pShapes_A	= Parameters("INPUT" )->asShapes();
	pShapes_B	= Parameters("OUTPUT")->asShapes();
	pShapes_B	->Create(pShapes_A->Get_Type(), pShapes_A->Get_Name(), pShapes_A);

	for(int iShape=0; iShape<pShapes_A->Get_Count() && Set_Progress(iShape, pShapes_A->Get_Count()); iShape++)
	{
		CSG_Shape	*pShape_A	= pShapes_A->Get_Shape(iShape);
		CSG_Shape	*pShape_B	= pShapes_B->Add_Shape(pShape_A, SHAPE_COPY_ATTR);

		for(int iPart=0; iPart<pShape_A->Get_Part_Count(); iPart++)
		{
			for(int iPoint=0; iPoint<pShape_A->Get_Point_Count(iPart); iPoint++)
			{
				TSG_Point	Point	= pShape_A->Get_Point(iPoint, iPart);

				if( Engine.Get_Converted(Point) )
				{
					pShape_B->Add_Point(Point);
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
