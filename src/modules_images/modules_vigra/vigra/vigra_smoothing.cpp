/**********************************************************
 * Version $Id: vigra_smoothing.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                        VIGRA                          //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                      vigra.cpp                        //
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

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "vigra_smoothing.h"

//---------------------------------------------------------
#include <vigra/convolution.hxx>
#include <vigra/nonlineardiffusion.hxx>


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CViGrA_Smoothing::CViGrA_Smoothing(void)
{
	Set_Name		(_TL("Smoothing (ViGrA)"));

	Set_Author		(SG_T("O.Conrad (c) 2009"));

	Set_Description	(_TW(
		"Based on the example code \"smooth.cxx\" by Ullrich Koethe.\n"
		"References:\n"
		"ViGrA - Vision with Generic Algorithms\n"
		"<a target=\"_blank\" href=\"http://hci.iwr.uni-heidelberg.de/vigra\">http://hci.iwr.uni-heidelberg.de</a>"
	));

	Parameters.Add_Grid(
		NULL	, "INPUT"	, _TL("Input"),
		_TL(""),
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL	, "OUTPUT"	, _TL("Output"),
		_TL(""),
		PARAMETER_OUTPUT
	);

	Parameters.Add_Choice(
		NULL	, "TYPE"	, _TL("Type of smoothing"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|"),
			_TL("exponential"),
			_TL("nonlinear"),
			_TL("gaussian")
		)
	);

	Parameters.Add_Value(
		NULL	, "SCALE"	, _TL("Size of smoothing filter"),
		_TL(""),
		PARAMETER_TYPE_Double, 2.0, 0.0, true
	);

	Parameters.Add_Value(
		NULL	, "EDGE"	, _TL("Edge threshold for nonlinear smoothing"),
		_TL(""),
		PARAMETER_TYPE_Double, 1.0, 0.0, true
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CViGrA_Smoothing::On_Execute(void)
{
	int				Type;
	double			Scale, Edge;
	CSG_Grid		*pInput, *pOutput;
	vigra::FImage	Input, Output;

	pInput	= Parameters("INPUT")	->asGrid();
	pOutput	= Parameters("OUTPUT")	->asGrid();
	Type	= Parameters("TYPE")	->asInt();
	Scale	= Parameters("SCALE")	->asDouble();
	Edge	= Parameters("EDGE")	->asDouble();

	Copy_Grid_SAGA_to_VIGRA(*pInput, Input, true);

	Output.resize(Get_NX(), Get_NY());

	//-----------------------------------------------------
	switch( Type )
	{
	case 0:	// apply recursive filter (exponential filter) to color image
		{
			recursiveSmoothX(srcImageRange(Input ), destImage(Output), Scale);
			recursiveSmoothY(srcImageRange(Output), destImage(Output), Scale);

			break;
		}

	case 1:	// apply nonlinear diffusion to color image
		{
			nonlinearDiffusion(srcImageRange(Input), destImage(Output), vigra::DiffusivityFunctor<float>(Edge), Scale);

			break;
		}

	case 2:	// apply Gaussian filter to color image
		{
			vigra::FImage			tmp(Get_NX(), Get_NY());
			vigra::Kernel1D<double>	gauss;

			gauss.initGaussian(Scale);

			separableConvolveX(srcImageRange(Input) , destImage(tmp), kernel1d(gauss));
			separableConvolveY(srcImageRange(tmp), destImage(Output), kernel1d(gauss));

			break;
		}
	}

	//-----------------------------------------------------
	Copy_Grid_VIGRA_to_SAGA(*pOutput, Output, false);

	pOutput->Set_Name(CSG_String::Format(SG_T("%s [%s - %s]"), pInput->Get_Name(), Get_Name().c_str(), Parameters("TYPE")->asString()));

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
