/**********************************************************
 * Version $Id: CreatePointGrid.cpp 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    CreatePointGrid.cpp
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
    Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, USA
*******************************************************************************/ 

#include "CreatePointGrid.h"


CCreatePointGrid::CCreatePointGrid(void){

	Parameters.Set_Name(_TL("Create Point Grid"));
	Parameters.Set_Description(_TW(
		"(c) 2004 by Victor Olaya. Creates a regular grid of points."));

	Parameters.Add_Shapes(NULL, 
						"POINTS", 
						_TL("Point Grid"), 
						_TL(""), 
						PARAMETER_OUTPUT);

				
	Parameters.Add_Range(NULL, 
							"X_EXTENT", 
							_TL("X-Extent"),
							_TL(""));

	Parameters.Add_Range(NULL, 
							"Y_EXTENT", 
							_TL("Y-Extent"),
							_TL(""));

	Parameters.Add_Value(NULL, 
						"DIST",
						_TL("Distance"), 
						_TL("Distance between points (grid units)."), 
						PARAMETER_TYPE_Double, 
						100);


}//constructor


CCreatePointGrid::~CCreatePointGrid(void)
{}


bool CCreatePointGrid::On_Execute(void){

	CSG_Shape *pShape;
	
	double dXMin	= ((CSG_Parameter_Range *) Parameters("X_EXTENT")->Get_Data())->Get_LoVal();
	double dYMin	= ((CSG_Parameter_Range *) Parameters("Y_EXTENT")->Get_Data())->Get_LoVal();
	double dXMax	= ((CSG_Parameter_Range *) Parameters("X_EXTENT")->Get_Data())->Get_HiVal();
	double dYMax	= ((CSG_Parameter_Range *) Parameters("Y_EXTENT")->Get_Data())->Get_HiVal();

	double dDistance = Parameters("DIST")->asDouble();
	if (dDistance<=0){
		return false;
	}//if

	CSG_Shapes *pShapes = Parameters("POINTS")->asShapes();
	pShapes->Create(SHAPE_TYPE_Point, _TL("Point Grid"));

	pShapes->Add_Field("X", SG_DATATYPE_Double);
	pShapes->Add_Field("Y", SG_DATATYPE_Double);
	
	for (double x=dXMin; x<dXMax; x=x+dDistance){
		for (double y=dYMin; y<dYMax; y=y+dDistance){ 
			pShape = pShapes->Add_Shape();
			pShape->Add_Point(x,y);
			pShape->Set_Value(0, x);
			pShape->Set_Value(1, y);
		}//for
	}//for

	return true;

}//method
