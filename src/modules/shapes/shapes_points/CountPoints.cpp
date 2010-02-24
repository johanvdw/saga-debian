/*******************************************************************************
    CountPoints.cpp
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
#include "CountPoints.h"

CCountPoints::CCountPoints(void){

	
	Parameters.Set_Name(_TL("Count Points in Polygons"));
	Parameters.Set_Description(_TW(
		"(c) 2004 by Victor Olaya. Count Points in Polygons."));

	Parameters.Add_Shapes(NULL, 
						"POINTS", 
						_TL("Points"), 
						_TL(""), 
						PARAMETER_INPUT);

	Parameters.Add_Shapes(NULL, 
						"POLYGONS", 
						_TL("Polygons"), 
						_TL(""), 
						PARAMETER_INPUT);
	
}//constructor


CCountPoints::~CCountPoints(void)
{}

bool CCountPoints::On_Execute(void){

	int i,j,k;
	int iPoly;
	int iField;
	int iPoints;
	CSG_Shapes *pPoints = Parameters("POINTS")->asShapes();
	CSG_Shapes *pPolygons = Parameters("POLYGONS")->asShapes();
	CSG_Shape *pShape, *pPoly;

	pPolygons->Add_Field(_TL("Points"),SG_DATATYPE_Int);
	iField = pPolygons->Get_Field_Count()-1;

	for(iPoly = 0; iPoly < pPolygons->Get_Count(); iPoly++){
		pPoly = pPolygons->Get_Shape(iPoly);
		iPoints = 0;
		for(i = 0; i < pPoints->Get_Count(); i++){			
			pShape = pPoints->Get_Shape(i);					
			for(j = 0; j < pShape->Get_Part_Count(); j++){	
				for(k = 0; k < pShape->Get_Point_Count(j); k++){
					if (((CSG_Shape_Polygon*)pPoly)->is_Containing(pShape->Get_Point(k, j))){
						iPoints++;
					}//if
				}//for
			}//for
		}//for
		pPoly->Set_Value(iField, iPoints);
	}//for

	return true;

}//method