/**********************************************************
 * Version $Id: Bifurcation.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                       Fractals                        //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    Bifurcation.cpp                    //
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
#include "Bifurcation.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CBifurcation::CBifurcation(void)
{
	Set_Name(_TL("Bifurcation"));

	Set_Author		(SG_T("(c) 2003 by O.Conrad"));

	Set_Description	(_TW("Feigenbaum's Bifurcation"));

	Parameters.Add_Table(	NULL, "TABLE"		, _TL("Output")				, _TL(""), PARAMETER_OUTPUT);

	Parameters.Add_Value(	NULL, "ITERATIONS"	, _TL("Value Count")		, _TL(""), PARAMETER_TYPE_Int, 100);
	Parameters.Add_Value(	NULL, "NVALUES"		, _TL("Value Count")		, _TL("Number of the last x Iteration to be plotted."), PARAMETER_TYPE_Int, 8);
	Parameters.Add_Value(	NULL, "SEED"		, _TL("Seed Value")			, _TL(""), PARAMETER_TYPE_Double	, 0.66, 0, true, 1.0, true);
	Parameters.Add_Range(	NULL, "RANGE"		, _TL("Range")				, _TL(""), 1.0, 4.0, 0.0, true, 4.0, true);
	Parameters.Add_Value(	NULL, "STEP"		, _TL("Number of Steps")	, _TL(""), PARAMETER_TYPE_Int		, 1000, 2, true);

}

//---------------------------------------------------------
CBifurcation::~CBifurcation(void)
{}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CBifurcation::On_Execute(void)
{
	int					i;
	double				p, r, dr, max, min, seed, nValues, nPreIterations;
	CSG_Table_Record		*pRecord;
	CSG_Table				*pTable;

	nPreIterations	= Parameters("ITERATIONS")->asInt();
	nValues			= Parameters("NVALUES")->asInt();
	seed			= Parameters("SEED")->asDouble();
	min				= Parameters("RANGE")->asRange()->Get_LoVal();
	max				= Parameters("RANGE")->asRange()->Get_HiVal();
	dr				= (max - min) / 1000.0;

	pTable			= Parameters("TABLE")->asTable();
	pTable->Destroy();
	pTable->Set_Name(_TL("Feigenbaum's Bifurcation"));

	pTable->Add_Field("Growth"	, SG_DATATYPE_Double);

	for(i=0; i<nValues; i++)
	{
		pTable->Add_Field(CSG_String::Format(SG_T("VALUE_%d"), i + 1), SG_DATATYPE_Double);
	}

	for(r=min; r<=max; r+=dr)
	{
		pRecord	= pTable->Add_Record();
		pRecord->Set_Value(0, r);

		p		= seed;

		for(i=0; i<nPreIterations; i++)
		{
			p		= r * p * (1.0 - p);
		}

		for(i=0; i<nValues; i++)
		{
			p	= r * p * (1.0 - p);
			pRecord->Set_Value(i + 1, p);
		}
	}

	return( true );
}
