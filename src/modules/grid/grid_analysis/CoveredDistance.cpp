/**********************************************************
 * Version $Id: CoveredDistance.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/
/*******************************************************************************
    CoveredDistance.cpp
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
#include "CoveredDistance.h"

CCoveredDistance::CCoveredDistance(void)
{
	Set_Name		(_TL("Covered Distance"));

	Set_Author		(SG_T("V. Olaya (c) 2005"));

	Set_Description	(_TW(
		""
	));

	Parameters.Add_Grid_List(
		NULL, "INPUT"	, _TL("Grids"),
		_TL(""), PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL, "RESULT"	, _TL("Covered Distance"),
		_TL(""),
		PARAMETER_OUTPUT
	);
}//constructor


bool CCoveredDistance::On_Execute(void)
{
	CSG_Parameter_Grid_List	*pGrids		= Parameters("INPUT")	->asGridList();
	CSG_Grid				*pResult	= Parameters("RESULT")	->asGrid();

	pResult->Assign(0.0);

	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=0; x<Get_NX(); x++)
		{
			bool	bNoData	= false;
			double	dDifSum	= 0.0;

			for(int i=0, j=1; j<pGrids->Get_Count() && !bNoData; i++, j++)
			{
				if( pGrids->asGrid(i)->is_NoData(x, y) || pGrids->asGrid(j)->is_NoData(x, y) )
				{
					bNoData	= true;
				}
				else
				{
					dDifSum	+= fabs(pGrids->asGrid(i)->asDouble(x, y) - pGrids->asGrid(j)->asDouble(x, y));
				}
			}//for

			if( bNoData )
			{
				pResult->Set_NoData(x, y);
			}
			else
			{
				pResult->Set_Value(x, y, dDifSum);
			}
		}//for
	}//for

	return true;
}
