/**********************************************************
 * Version $Id: mat_regression_multiple.cpp 1921 2014-01-09 10:24:11Z oconrad $
 *********************************************************/

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
//              mat_regression_multiple.cpp              //
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
// 51 Franklin Street, 5th Floor, Boston, MA 02110-1301, //
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
#include "mat_tools.h"
#include "table.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum ESG_Multiple_Regression_Info_Model
{
	MLR_MODEL_R2	= 0,
	MLR_MODEL_R2_ADJ,
	MLR_MODEL_SE,
	MLR_MODEL_SSR,
	MLR_MODEL_SSE,
	MLR_MODEL_SST,
	MLR_MODEL_MSR,
	MLR_MODEL_MSE,
	MLR_MODEL_F,
	MLR_MODEL_SIG,
	MLR_MODEL_NPREDICT,
	MLR_MODEL_NSAMPLES,
	MLR_MODEL_CV_MSE,
	MLR_MODEL_CV_RMSE,
	MLR_MODEL_CV_NRMSE,
	MLR_MODEL_CV_R2,
	MLR_MODEL_CV_NSAMPLES
};

//---------------------------------------------------------
enum ESG_Multiple_Regression_Info_Steps
{
	MLR_STEP_NR		= 0,
	MLR_STEP_R,
	MLR_STEP_R2,
	MLR_STEP_R2_ADJ,
	MLR_STEP_SE,
	MLR_STEP_SSR,
	MLR_STEP_SSE,
	MLR_STEP_MSR,
	MLR_STEP_MSE,
	MLR_STEP_DF,
	MLR_STEP_F,
	MLR_STEP_SIG,
	MLR_STEP_VAR_F,
	MLR_STEP_VAR_SIG,
	MLR_STEP_DIR,
	MLR_STEP_VAR
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Regression_Multiple::CSG_Regression_Multiple(bool bIntercept)
{
	m_pRegression	= new CSG_Table;

	m_pRegression	->Add_Field("VAR_ID"		, SG_DATATYPE_Int);		// MLR_VAR_ID
	m_pRegression	->Add_Field("VAR_NAME"		, SG_DATATYPE_String);	// MLR_VAR_NAME
	m_pRegression	->Add_Field("REGCOEFF"		, SG_DATATYPE_Double);	// MLR_VAR_RCOEFF
	m_pRegression	->Add_Field("R"				, SG_DATATYPE_Double);	// MLR_VAR_R
	m_pRegression	->Add_Field("R2"			, SG_DATATYPE_Double);	// MLR_VAR_R2
	m_pRegression	->Add_Field("R2_ADJ"		, SG_DATATYPE_Double);	// MLR_VAR_R2_ADJ
	m_pRegression	->Add_Field("STD_ERROR"		, SG_DATATYPE_Double);	// MLR_VAR_SE
	m_pRegression	->Add_Field("T"				, SG_DATATYPE_Double);	// MLR_VAR_T
	m_pRegression	->Add_Field("SIG"			, SG_DATATYPE_Double);	// MLR_VAR_SIG
	m_pRegression	->Add_Field("P"				, SG_DATATYPE_Double);	// MLR_VAR_P

	//-----------------------------------------------------
	m_pSteps		= new CSG_Table;

	m_pSteps		->Add_Field("MODEL"			, SG_DATATYPE_Int);		// MLR_STEP_NR
	m_pSteps		->Add_Field("R"				, SG_DATATYPE_Double);	// MLR_STEP_R
	m_pSteps		->Add_Field("R2"			, SG_DATATYPE_Double);	// MLR_STEP_R2
	m_pSteps		->Add_Field("R2_ADJ"		, SG_DATATYPE_Double);	// MLR_STEP_R2_ADJ
	m_pSteps		->Add_Field("STD_ERROR"		, SG_DATATYPE_Double);	// MLR_STEP_SE
	m_pSteps		->Add_Field("SSR"			, SG_DATATYPE_Double);	// MLR_STEP_SSR
	m_pSteps		->Add_Field("SSE"			, SG_DATATYPE_Double);	// MLR_STEP_SSE
	m_pSteps		->Add_Field("MSR"			, SG_DATATYPE_Double);	// MLR_STEP_MSR
	m_pSteps		->Add_Field("MSE"			, SG_DATATYPE_Double);	// MLR_STEP_MSE
	m_pSteps		->Add_Field("DF"			, SG_DATATYPE_Double);	// MLR_STEP_DF
	m_pSteps		->Add_Field("F"				, SG_DATATYPE_Double);	// MLR_STEP_F
	m_pSteps		->Add_Field("SIG"			, SG_DATATYPE_Double);	// MLR_STEP_SIG
	m_pSteps		->Add_Field("VAR_F"			, SG_DATATYPE_Double);	// MLR_STEP_VAR_F
	m_pSteps		->Add_Field("VAR_SIG"		, SG_DATATYPE_Double);	// MLR_STEP_VAR_SIG
	m_pSteps		->Add_Field("DIR"			, SG_DATATYPE_String);	// MLR_STEP_DIR
	m_pSteps		->Add_Field("VARIABLE"		, SG_DATATYPE_String);	// MLR_STEP_VAR

	//-----------------------------------------------------
	m_pModel		= new CSG_Table;

	m_pModel		->Add_Field("PARAMETER"		, SG_DATATYPE_String);
	m_pModel		->Add_Field("VALUE"			, SG_DATATYPE_Double);

	m_pModel		->Add_Record()->Set_Value(0, SG_T("R2"			));	// MLR_MODEL_R2
	m_pModel		->Add_Record()->Set_Value(0, SG_T("R2_ADJ"		));	// MLR_MODEL_R2_ADJ
	m_pModel		->Add_Record()->Set_Value(0, SG_T("STD_ERROR"	));	// MLR_MODEL_SE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("SSR"			));	// MLR_MODEL_SSR
	m_pModel		->Add_Record()->Set_Value(0, SG_T("SSE"			));	// MLR_MODEL_SSE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("SST"			));	// MLR_MODEL_SST
	m_pModel		->Add_Record()->Set_Value(0, SG_T("MSR"			));	// MLR_MODEL_MSR
	m_pModel		->Add_Record()->Set_Value(0, SG_T("MSE"			));	// MLR_MODEL_MSE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("F"			));	// MLR_MODEL_F
	m_pModel		->Add_Record()->Set_Value(0, SG_T("SIG"			));	// MLR_MODEL_SIG
	m_pModel		->Add_Record()->Set_Value(0, SG_T("PREDICTORS"	));	// MLR_MODEL_NPREDICT
	m_pModel		->Add_Record()->Set_Value(0, SG_T("SAMPLES"		));	// MLR_MODEL_NSAMPLES
	m_pModel		->Add_Record()->Set_Value(0, SG_T("CV_MSE"		));	// MLR_MODEL_CV_MSE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("CV_RMSE"		));	// MLR_MODEL_CV_RMSE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("CV_NRMSE"	));	// MLR_MODEL_CV_RMSE
	m_pModel		->Add_Record()->Set_Value(0, SG_T("CV_R2"		));	// MLR_MODEL_CV_R2
	m_pModel		->Add_Record()->Set_Value(0, SG_T("CV_SAMPLES"	));	// MLR_MODEL_CV_NSAMPLES

	//-----------------------------------------------------
	m_Predictor		= NULL;
	m_nPredictors	= 0;

	m_bIntercept	= bIntercept;
}

//---------------------------------------------------------
CSG_Regression_Multiple::~CSG_Regression_Multiple(void)
{
	Destroy();

	delete(m_pRegression);
	delete(m_pModel);
	delete(m_pSteps);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
void CSG_Regression_Multiple::Destroy(void)
{
	m_Names        .Clear  ();
	m_Samples      .Destroy();
	m_Samples_Model.Destroy();

	m_pRegression	->Del_Records();
	m_pSteps		->Del_Records();

	for(int i=0; i<m_pModel->Get_Count(); i++)
	{
		m_pModel->Get_Record(i)->Set_NoData(1);
	}

	if( m_Predictor )
	{
		delete[](m_bIncluded);
		delete[](m_Predictor);

		m_Predictor		= NULL;
		m_nPredictors	= 0;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Regression_Multiple::Set_Data(const CSG_Matrix &Samples, CSG_Strings *pNames)
{
	Destroy();

	int	i, nPredictors	= Samples.Get_NCols() - 1;

	if(	nPredictors < 1 || Samples.Get_NRows() <= nPredictors )
	{
		return( false );
	}

	//-------------------------------------------------
	for(i=0; i<=nPredictors; i++)
	{
		m_Names	+= pNames && pNames->Get_Count() == Samples.Get_NCols() ? pNames->Get_String(i) : i == 0
			? CSG_String::Format(SG_T(    "%s"),        _TL("Dependent"))
			: CSG_String::Format(SG_T("%d. %s"), i + 1, _TL("Predictor"));
	}

	m_Samples	= Samples;

	m_bIncluded		= new int[nPredictors];
	m_Predictor		= new int[nPredictors];

	//-------------------------------------------------
	return( true );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::_Initialize(bool bInclude)
{
	int	i, nPredictors	= m_Samples.Get_NCols() - 1;

	if( nPredictors < 1 || m_Samples.Get_NRows() <= nPredictors )
	{
		return( false );
	}

	//-------------------------------------------------
	if( bInclude )
	{
		m_nPredictors	= nPredictors;
		m_Samples_Model.Create(m_Samples);
	}
	else
	{
		m_nPredictors	= 0;
		m_Samples_Model.Create(1, m_Samples.Get_NRows(), m_Samples.Get_Col(0).Get_Data());
	}

	for(i=0; i<nPredictors; i++)
	{
		m_Predictor[i]	= i;
		m_bIncluded[i]	= bInclude;
	}

	for(i=0; i<m_pModel->Get_Count(); i++)
	{
		m_pModel->Get_Record(i)->Set_NoData(1);
	}

	//-------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Model(const CSG_Matrix &Samples, CSG_Strings *pNames)
{
	return( Set_Data(Samples, pNames) && Get_Model() );
}

bool CSG_Regression_Multiple::Get_Model_Forward(const CSG_Matrix &Samples, double P_in, CSG_Strings *pNames)
{
	return( Set_Data(Samples, pNames) && Get_Model_Forward(P_in) );
}

bool CSG_Regression_Multiple::Get_Model_Backward(const CSG_Matrix &Samples, double P_out, CSG_Strings *pNames)
{
	return( Set_Data(Samples, pNames) && Get_Model_Backward(P_out) );
}

bool CSG_Regression_Multiple::Get_Model_Stepwise(const CSG_Matrix &Samples, double P_in, double P_out, CSG_Strings *pNames)
{
	return( Set_Data(Samples, pNames) && Get_Model_Stepwise(P_in, P_out) );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Model(void)
{
	return( _Initialize(true) && _Get_Regression(m_Samples) );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Model_Forward(double P_in)
{
	if( _Initialize(false) )
	{
		double	R2	= 0.0;

		while( _Get_Step_In(m_Samples_Model, P_in, R2, m_Samples) >= 0 );

		return( _Set_Step_Info(m_Samples_Model) );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Model_Backward(double P_out)
{
	if( _Initialize(true) )
	{
		double	R2	= 0.0;

		while( _Get_Step_Out(m_Samples_Model, P_out, R2) >= 0 );

		return( _Set_Step_Info(m_Samples_Model) );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Model_Stepwise(double P_in, double P_out)
{
	if( _Initialize(false) )
	{
		double	R2	= 0.0;

		if( P_out <= P_in )
		{
			P_out	= P_in + 0.001;
		}

		while( _Get_Step_In(m_Samples_Model, P_in, R2, m_Samples) >= 0 && SG_UI_Process_Get_Okay() )
		{
			if( m_nPredictors > 1 )
			{
				_Get_Step_Out(m_Samples_Model, P_out, R2);
			}
		}

		return( _Set_Step_Info(m_Samples_Model) );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_CrossValidation(int nSubSamples)
{
	if( m_Samples_Model.Get_NCols() <= 1 )
	{
		return( false );
	}

	//-----------------------------------------------------
	CSG_Regression_Multiple	Model(m_bIntercept);
	CSG_Simple_Statistics	Stats, SR, SE;

	int		i, nModels	= 0;

	for(i=0; i<m_Samples_Model.Get_NRows(); i++)
	{
		Stats	+= m_Samples_Model[i][0];
	}

	//-----------------------------------------------------
	// leave-one-out cross validation (LOOCV)
	if( nSubSamples <= 1 || nSubSamples > m_Samples_Model.Get_NRows() / 2 )
	{
		for(i=0; i<m_Samples_Model.Get_NRows() && SG_UI_Process_Get_Okay(); i++)
		{
			CSG_Matrix	Samples(m_Samples_Model);
			Samples.Del_Row(i);

			if( Model.Get_Model(Samples) )
			{
				nModels++;

				double	dObsrv	= m_Samples_Model[i][0];
				double	dModel	= Model.Get_Value(CSG_Vector(m_nPredictors, m_Samples_Model[i] + 1));

				SE	+= SG_Get_Square(dModel - dObsrv);
				SR	+= SG_Get_Square(dModel - (Stats.Get_Sum() - dObsrv) / Samples.Get_NRows());
			}
		}
	}

	//-----------------------------------------------------
	// k-fold cross validation
	else
	{
		int	*SubSet	= new int[m_Samples_Model.Get_NRows()];

		for(i=0; i<m_Samples_Model.Get_NRows(); i++)
		{
			SubSet[i]	= i % nSubSamples;
		}

		//-------------------------------------------------
		for(int iSubSet=0; iSubSet<nSubSamples && SG_UI_Process_Get_Okay(); iSubSet++)
		{
			CSG_Simple_Statistics	Samples_Stats;
			CSG_Matrix	Samples(m_Samples_Model), Validation;

			for(i=Samples.Get_NRows()-1; i>=0; i--)
			{
				if( SubSet[i] == iSubSet )
				{
					Validation.Add_Row(Samples.Get_Row(i));
					Samples   .Del_Row(i);
				}
				else
				{
					Samples_Stats	+= Samples[i][0];
				}
			}

			//---------------------------------------------
			if( Model.Get_Model(Samples) )
			{
				nModels++;

				for(i=0; i<Validation.Get_NRows(); i++)
				{
					double	dObsrv	= Validation[i][0];
					double	dModel	= Model.Get_Value(CSG_Vector(m_nPredictors, Validation[i] + 1));

					SE	+= SG_Get_Square(dModel - dObsrv);
					SR	+= SG_Get_Square(dModel - Samples_Stats.Get_Mean());
				}
			}
		}

		delete[](SubSet);
	}

	//-----------------------------------------------------
	m_pModel->Get_Record(MLR_MODEL_CV_MSE     )->Set_Value(1, SE.Get_Mean());
	m_pModel->Get_Record(MLR_MODEL_CV_RMSE    )->Set_Value(1, sqrt(SE.Get_Mean()));
	m_pModel->Get_Record(MLR_MODEL_CV_NRMSE   )->Set_Value(1, sqrt(SE.Get_Mean()) / Stats.Get_Range());
	m_pModel->Get_Record(MLR_MODEL_CV_R2      )->Set_Value(1, SR.Get_Sum() / (SR.Get_Sum() + SE.Get_Sum()));
	m_pModel->Get_Record(MLR_MODEL_CV_NSAMPLES)->Set_Value(1, nModels);

	//-----------------------------------------------------
	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CSG_Regression_Multiple::Get_Value(const CSG_Vector &Predictors)	const
{
	double	Value;

	Get_Value(Predictors, Value);

	return( Value );
}

bool CSG_Regression_Multiple::Get_Value(const CSG_Vector &Predictors, double &Value)	const
{
	if( m_nPredictors == Predictors.Get_N() )
	{
		Value	= Get_RConst();

		for(int i=0; i<m_nPredictors; i++)
		{
			Value	+= Get_RCoeff(i) * Predictors(i);
		}

		return( true );
	}

	Value	= 0.0;

	return( false );
}

//---------------------------------------------------------
double CSG_Regression_Multiple::Get_Residual(int iSample)	const
{
	double	Value;

	Get_Residual(iSample, Value);

	return( Value );
}

bool CSG_Regression_Multiple::Get_Residual(int iSample, double &Residual)	const
{
	if( iSample >= 0 && iSample < m_Samples_Model.Get_NRows() )
	{
		Residual	= Get_RConst();

		for(int i=0; i<m_nPredictors; i++)
		{
			Residual	+= Get_RCoeff(i) * m_Samples_Model[iSample][1 + i];
		}

		Residual	-= m_Samples_Model[iSample][0];

		return( true );
	}

	Residual	= 0.0;

	return( false );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::Get_Residuals(CSG_Vector &Residuals)	const
{
	Residuals.Create(m_Samples_Model.Get_NRows());

	for(int i=0; i<Residuals.Get_N(); i++)
	{
		Get_Residual(i, Residuals[i]);
	}

	return( Residuals.Get_N() > 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CSG_Regression_Multiple::_Get_F(int nPredictors, int nSamples, double r2_full, double r2_reduced)
{
	return( (nSamples - nPredictors - 1) * (r2_full - r2_reduced) / (1.0 - r2_full) );
}

//---------------------------------------------------------
double CSG_Regression_Multiple::_Get_P(int nPredictors, int nSamples, double r2_full, double r2_reduced)
{
	double	f	= (nSamples - nPredictors - 1) * (r2_full - r2_reduced) / (1.0 - r2_full);

	return( CSG_Test_Distribution::Get_F_Tail(f, nPredictors, nSamples - nPredictors - 1, TESTDIST_TYPE_Right) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Regression_Multiple::_Get_Regression(const CSG_Matrix &Samples)
{
	int		nPredictors = Samples.Get_NX() - 1;
	int		nSamples    = Samples.Get_NY();

	//-----------------------------------------------------
	int			i, j;
	double		Ym, SSR, SSE, SST, MSR, MSE, SE, R2, F;
	CSG_Vector	Y, Yr, B;
	CSG_Matrix	X, Xt, C;

	Y.Create(nSamples);
	X.Create(nPredictors + (m_bIntercept ? 1 : 0), nSamples);

	//-----------------------------------------------------
	for(i=0, Ym=0.0; i<nSamples; i++)
	{
		Ym	+= Y[i]	= Samples[i][0];

		if( m_bIntercept )
		{
			X [i][0]	= 1.0;

			for(j=1; j<=nPredictors; j++)
			{
				X[i][j]	= Samples[i][j];
			}
		}
		else
		{
			for(j=0; j<nPredictors; j++)
			{
				X[i][j]	= Samples[i][j + 1];
			}
		}
	}

	Ym	/= nSamples;

	//-----------------------------------------------------
	Xt	= X.Get_Transpose();

	C	= (Xt * X).Get_Inverse();

	B	= C * (Xt * Y);

	//-----------------------------------------------------
	Yr	= X * B;

	for(i=0, SSE=0.0, SSR=0.0, SST=0.0; i<nSamples; i++)
	{
		SSE	+= SG_Get_Square(Yr[i] - Y[i]);
		SSR	+= SG_Get_Square(Yr[i] - Ym);
	//	SST	+= SG_Get_Square(Y [i] - Ym);
	}

//	SSE	= SST - SSR;
	SST	= SSR + SSE;
	MSR	= SSR / nPredictors;
	MSE	= SSE / (nSamples - nPredictors - 1);
	SE	= sqrt(SSE / (nSamples - nPredictors));
	R2	= SSR / SST;
	F	= MSR / MSE;	// 	= (nSamples - nPredictors - 1) * (R2 / nPredictors) / (1.0 - R2);

	//-----------------------------------------------------
	m_pModel->Get_Record(MLR_MODEL_R2      )->Set_Value(1, R2);
	m_pModel->Get_Record(MLR_MODEL_R2_ADJ  )->Set_Value(1, SG_Regression_Get_Adjusted_R2(R2, nSamples, nPredictors));
	m_pModel->Get_Record(MLR_MODEL_SE      )->Set_Value(1, SE);
	m_pModel->Get_Record(MLR_MODEL_SSR     )->Set_Value(1, SSR);
	m_pModel->Get_Record(MLR_MODEL_SSE     )->Set_Value(1, SSE);
	m_pModel->Get_Record(MLR_MODEL_SST     )->Set_Value(1, SST);
	m_pModel->Get_Record(MLR_MODEL_MSR     )->Set_Value(1, MSR);
	m_pModel->Get_Record(MLR_MODEL_MSE     )->Set_Value(1, MSE);
	m_pModel->Get_Record(MLR_MODEL_F       )->Set_Value(1, F);
	m_pModel->Get_Record(MLR_MODEL_SIG     )->Set_Value(1, CSG_Test_Distribution::Get_F_Tail_from_R2(R2, nPredictors, nSamples));
	m_pModel->Get_Record(MLR_MODEL_NPREDICT)->Set_Value(1, nPredictors);
	m_pModel->Get_Record(MLR_MODEL_NSAMPLES)->Set_Value(1, nSamples);

	//-----------------------------------------------------
	CSG_Matrix	P	= SG_Get_Correlation_Matrix(Samples, true).Get_Inverse();	// get partial correlation

	if( !m_bIntercept )
	{
		m_pRegression->Add_Record()->Set_Value(MLR_VAR_NAME, m_Names[0]);
	}

	for(j=0; j<B.Get_N(); j++)
	{
		double	se	= SE * sqrt(fabs(C[j][j]));
		double	b	= B[j];
		double	t	= b / se;
		int     k   = (m_bIntercept ? j : j + 1);
		double	r	= -P[k][0] / sqrt(P[k][k] * P[0][0]);

		CSG_Table_Record	*pRecord	= m_pRegression->Add_Record();

		pRecord->Set_Value(MLR_VAR_ID		, m_bIntercept ? j - 1 : j);
		pRecord->Set_Value(MLR_VAR_NAME		, m_Names[k]);
		pRecord->Set_Value(MLR_VAR_RCOEFF	, b);
		pRecord->Set_Value(MLR_VAR_R		, r);
		pRecord->Set_Value(MLR_VAR_R2		, r*r);
		pRecord->Set_Value(MLR_VAR_R2_ADJ	, SG_Regression_Get_Adjusted_R2(r*r, nSamples, nPredictors));
		pRecord->Set_Value(MLR_VAR_SE		, se);
		pRecord->Set_Value(MLR_VAR_T		, t);
		pRecord->Set_Value(MLR_VAR_SIG		, CSG_Test_Distribution::Get_T_Tail(t, nSamples - nPredictors, TESTDIST_TYPE_TwoTail));
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
int CSG_Regression_Multiple::_Get_Step_In(CSG_Matrix &X, double P_in, double &R2, const CSG_Matrix &Samples)
{
	int		iBest, iPredictor;
	double	rBest;

	CSG_Regression_Multiple R(m_bIntercept);

	X.Add_Cols(1);

	//-----------------------------------------------------
	for(iPredictor=0, iBest=-1, rBest=0.0; iPredictor<Samples.Get_NX()-1; iPredictor++)
	{
		if( !m_bIncluded[iPredictor] )
		{
			X.Set_Col(1 + m_nPredictors, Samples.Get_Col(1 + iPredictor));

			if( R.Get_Model(X) && (iBest < 0 || rBest < R.Get_R2()) )
			{
				iBest	= iPredictor;
				rBest	= R.Get_R2();
			}
		}
	}

	//-----------------------------------------------------
	if( iBest >= 0 && _Get_P(1, Samples.Get_NY() - m_nPredictors, rBest, R2) <= P_in )
	{
		m_bIncluded[iBest]			= true;
		m_Predictor[m_nPredictors]	= iBest;

		m_nPredictors++;

		X.Set_Col(m_nPredictors, Samples.Get_Col(1 + iBest));
		_Set_Step_Info(X, R2, iBest, true);
		R2	= rBest;

		return( iBest );
	}

	X.Del_Col(X.Get_NX() - 1);

	return( -1 );
}

//---------------------------------------------------------
int CSG_Regression_Multiple::_Get_Step_Out(CSG_Matrix &X, double P_out, double &R2)
{
	int		iBest, iPredictor;
	double	rBest;

	CSG_Regression_Multiple R(m_bIntercept);

	if( R2 <= 0.0 )
	{
		R.Get_Model(X);

		R2	= R.Get_R2();
	}

	//-----------------------------------------------------
	for(iPredictor=0, iBest=-1, rBest=0.0; iPredictor<m_nPredictors; iPredictor++)
	{
		CSG_Matrix	X_reduced(X);

		X_reduced.Del_Col(1 + iPredictor);

		if( R.Get_Model(X_reduced) && (iBest < 0 || rBest < R.Get_R2()) )
		{
			iBest	= iPredictor;
			rBest	= R.Get_R2();
		}
	}

	//-----------------------------------------------------
	if( iBest >= 0 && _Get_P(1, X.Get_NY() - (m_nPredictors - 1), R2, rBest) > P_out )
	{
		m_nPredictors--;

		X.Del_Col(1 + iBest);
		_Set_Step_Info(X, R2, m_Predictor[iBest], false);
		R2	= rBest;

		m_bIncluded[m_Predictor[iBest]]	= false;

		for(iPredictor=iBest; iPredictor<m_nPredictors; iPredictor++)
		{
			m_Predictor[iPredictor]	= m_Predictor[iPredictor + 1];
		}

		return( iBest );
	}

	return( -1 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CSG_Regression_Multiple::_Set_Step_Info(const CSG_Matrix &X)
{
	CSG_Regression_Multiple	R(m_bIntercept);

	if( m_nPredictors > 0 && R.Get_Model(X) )
	{
		m_pModel		->Assign(R.m_pModel);
		m_pRegression	->Assign(R.m_pRegression);

		m_pRegression->Get_Record(0)->Set_Value(MLR_VAR_NAME, m_Names[0]);

		for(int i=0; i<m_nPredictors; i++)
		{
			CSG_Table_Record	*pRecord	= m_pRegression->Get_Record(1 + i);

			pRecord->Set_Value(MLR_VAR_ID  , m_Predictor[i]);
			pRecord->Set_Value(MLR_VAR_NAME, m_Names[1 + m_Predictor[i]]);
		}

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Regression_Multiple::_Set_Step_Info(const CSG_Matrix &X, double R2_prev, int iVariable, bool bIn)
{
	CSG_Regression_Multiple R(m_bIntercept);

	R.Get_Model(X);

	CSG_Table_Record	*pRecord	= m_pSteps->Add_Record();

	pRecord->Set_Value(MLR_STEP_NR		, m_pSteps->Get_Count());
	pRecord->Set_Value(MLR_STEP_R		, sqrt(R.Get_R2()));
	pRecord->Set_Value(MLR_STEP_R2		, R.Get_R2());
	pRecord->Set_Value(MLR_STEP_R2_ADJ	, R.Get_R2_Adj());
	pRecord->Set_Value(MLR_STEP_SE		, R.Get_StdError());
	pRecord->Set_Value(MLR_STEP_SSR		, R.m_pModel->Get_Record(MLR_MODEL_SSR)->asDouble(1));
	pRecord->Set_Value(MLR_STEP_SSE		, R.m_pModel->Get_Record(MLR_MODEL_SSE)->asDouble(1));
	pRecord->Set_Value(MLR_STEP_MSR		, R.m_pModel->Get_Record(MLR_MODEL_MSR)->asDouble(1));
	pRecord->Set_Value(MLR_STEP_MSE		, R.m_pModel->Get_Record(MLR_MODEL_MSE)->asDouble(1));
	pRecord->Set_Value(MLR_STEP_DF		, X.Get_NRows() - m_nPredictors - 1);
	pRecord->Set_Value(MLR_STEP_F		, R.m_pModel->Get_Record(MLR_MODEL_F  )->asDouble(1));
	pRecord->Set_Value(MLR_STEP_SIG		, R.m_pModel->Get_Record(MLR_MODEL_SIG)->asDouble(1));
	pRecord->Set_Value(MLR_STEP_VAR_F	, _Get_F(1, X.Get_NRows() - (m_nPredictors - 1), bIn ? R.Get_R2() : R2_prev, bIn ? R2_prev : R.Get_R2()));
	pRecord->Set_Value(MLR_STEP_VAR_SIG	, _Get_P(1, X.Get_NRows() - (m_nPredictors - 1), bIn ? R.Get_R2() : R2_prev, bIn ? R2_prev : R.Get_R2()));
	pRecord->Set_Value(MLR_STEP_DIR		, bIn ? SG_T(">>") : SG_T("<<"));
	pRecord->Set_Value(MLR_STEP_VAR		, m_Names[1 + iVariable]);

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CSG_Regression_Multiple::Get_R2			(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_R2         )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_R2_Adj		(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_R2_ADJ     )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_StdError	(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_SE         )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_F			(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_F          )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_P			(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_SIG        )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_CV_RMSE		(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_CV_RMSE    )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_CV_NRMSE	(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_CV_NRMSE   )->asDouble(1) );	}
double CSG_Regression_Multiple::Get_CV_R2		(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_CV_R2      )->asDouble(1) );	}
int    CSG_Regression_Multiple::Get_CV_nSamples	(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_CV_NSAMPLES)->asInt   (1) );	}
int    CSG_Regression_Multiple::Get_nPredictors	(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_NPREDICT   )->asInt   (1) );	}
int    CSG_Regression_Multiple::Get_nSamples	(void)	const	{	return( m_pModel->Get_Record(MLR_MODEL_NSAMPLES   )->asInt   (1) );	}
int    CSG_Regression_Multiple::Get_DegFreedom	(void)	const	{	return( Get_nSamples() - Get_nPredictors() - 1 );	}

//---------------------------------------------------------
const SG_Char * CSG_Regression_Multiple::Get_Name(int iVariable) const
{
	if( iVariable >= 0 && iVariable < m_pRegression->Get_Count() - 1 )
	{
		return( m_pRegression->Get_Record(1 + iVariable)->asString(MLR_VAR_NAME) );
	}

	return( SG_T("") );
}

//---------------------------------------------------------
double CSG_Regression_Multiple::Get_RConst(void) const
{
	if( m_pRegression->Get_Count() > 0 )
	{
		return( m_pRegression->Get_Record(0)->asDouble(MLR_VAR_RCOEFF) );
	}

	return( 0.0 );
}

//---------------------------------------------------------
double CSG_Regression_Multiple::Get_Parameter(int iVariable, int Parameter)	const
{
	if( iVariable >= 0 && iVariable < m_pRegression->Get_Count() - 1 && Parameter >= 0 && Parameter <= MLR_VAR_P )
	{
		return( m_pRegression->Get_Record(1 + iVariable)->asDouble(Parameter) );
	}

	return( 0.0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_String CSG_Regression_Multiple::Get_Info(void)	const
{
	int			i;
	CSG_String	s;

	if( Get_nPredictors() < 1 )
	{
		return( s );
	}

	//-----------------------------------------------------
	if( m_pSteps->Get_Count() > 0 )
	{
		s	+= CSG_String::Format(SG_T("\n%s:\n\n"), _TL("Steps"));
		s	+= CSG_String::Format(SG_T("No.   \tR     \tR2    \tR2 adj\tStdErr\tF     \tP     \tF step\tP step\tVariable\n"));
		s	+= CSG_String::Format(SG_T("------\t------\t------\t------\t------\t------\t------\t------\t------\t------\n"));

		for(i=0; i<m_pSteps->Get_Count(); i++)
		{
			CSG_Table_Record	*pRecord	= m_pSteps->Get_Record(i);

			s	+= CSG_String::Format(SG_T("%d.\t%.2f\t%.2f\t%.2f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%s %s\n"),
				pRecord->asInt   (MLR_STEP_NR     ),
				pRecord->asDouble(MLR_STEP_R      ),
				pRecord->asDouble(MLR_STEP_R2     ) * 100.0,
				pRecord->asDouble(MLR_STEP_R2_ADJ ) * 100.0,
				pRecord->asDouble(MLR_STEP_SE     ),
				pRecord->asDouble(MLR_STEP_F      ),
				pRecord->asDouble(MLR_STEP_SIG    ) * 100.0,
				pRecord->asDouble(MLR_STEP_VAR_F  ),
				pRecord->asDouble(MLR_STEP_VAR_SIG) * 100.0,
				pRecord->asString(MLR_STEP_DIR    ),
				pRecord->asString(MLR_STEP_VAR    )
			);
		}
	}

	//-----------------------------------------------------
	s	+= CSG_String::Format(SG_T("\n%s:\n\n"), _TL("Correlation"));
	s	+= CSG_String::Format(SG_T("No.   \tR     \tR2    \tR2 adj\tStdErr\tt     \tSig.  \tb     \tVariable\n"));
	s	+= CSG_String::Format(SG_T("------\t------\t------\t------\t------\t------\t------\t------\t------\n"));

	for(i=0; i<m_pRegression->Get_Count(); i++)
	{
		CSG_Table_Record	*pRecord	= m_pRegression->Get_Record(i);

		s	+= CSG_String::Format(SG_T("%d.\t%.2f\t%.2f\t%.2f\t%.3f\t%.3f\t%.3f\t%.3f\t%s\n"),
			i,
			pRecord->asDouble(MLR_VAR_R     ),
			pRecord->asDouble(MLR_VAR_R2    ) * 100.0,
			pRecord->asDouble(MLR_VAR_R2_ADJ) * 100.0,
			pRecord->asDouble(MLR_VAR_SE    ),
			pRecord->asDouble(MLR_VAR_T     ),
			pRecord->asDouble(MLR_VAR_SIG   ) * 100.0,
			pRecord->asDouble(MLR_VAR_RCOEFF),
			pRecord->asString(MLR_VAR_NAME  )
		);
	}

	//-----------------------------------------------------
	s	+= SG_T("\n");

	s	+= CSG_String::Format(SG_T("%s: %f (%s: %d)\n"), _TL("Residual standard error"), Get_StdError(), _TL("degrees of freedom"), Get_DegFreedom());
	s	+= CSG_String::Format(SG_T("%s: %f (%s: %f)\n"), _TL("Multiple R-squared"), 100.0 * Get_R2(), _TL("adjusted"), 100.0 * Get_R2_Adj());
	s	+= CSG_String::Format(SG_T("%s: %f (%d/%d DF), %s: %g\n"), _TL("F-statistic"), Get_F(), Get_nPredictors(), Get_DegFreedom(), _TL("p-value"), Get_P());

	//-----------------------------------------------------
	return( s );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
