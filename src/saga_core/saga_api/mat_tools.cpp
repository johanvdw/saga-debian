/**********************************************************
 * Version $Id: mat_tools.cpp 1160 2011-09-14 15:11:54Z oconrad $
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
//                     mat_tools.cpp                     //
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
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
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


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double			SG_Get_Square(double x)
{
	return( x * x );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Simple_Statistics::CSG_Simple_Statistics(void)
{
	Create(false);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(bool bHoldValues)
{
	Create(bHoldValues);
}

CSG_Simple_Statistics::CSG_Simple_Statistics(const CSG_Simple_Statistics &Statistics)
{
	Create(Statistics);
}

//---------------------------------------------------------
bool CSG_Simple_Statistics::Create(bool bHoldValues)
{
	Invalidate();

	m_Values.Create(bHoldValues ? sizeof(double) : 0, 0, SG_ARRAY_GROWTH_1);

	return( true );
}

bool CSG_Simple_Statistics::Create(const CSG_Simple_Statistics &Statistics)
{
	m_bEvaluated	= Statistics.m_bEvaluated;
	m_nValues		= Statistics.m_nValues;
	m_Weights		= Statistics.m_Weights;
	m_Sum			= Statistics.m_Sum;
	m_Sum2			= Statistics.m_Sum2;

	m_Minimum		= Statistics.m_Minimum;
	m_Maximum		= Statistics.m_Maximum;
	m_Range			= Statistics.m_Range;
	m_Mean			= Statistics.m_Mean;
	m_Variance		= Statistics.m_Variance;
	m_StdDev		= Statistics.m_StdDev;

	m_Values		.Create(Statistics.m_Values);

	return( true );
}

//---------------------------------------------------------
void CSG_Simple_Statistics::Invalidate(void)
{
	m_bEvaluated	= false;
	m_nValues		= 0;
	m_Weights		= 0.0;
	m_Sum			= 0.0;
	m_Sum2			= 0.0;

	m_Minimum		= 0.0;
	m_Maximum		= 0.0;
	m_Range			= 0.0;
	m_Mean			= 0.0;
	m_Variance		= 0.0;
	m_StdDev		= 0.0;

	m_Values		.Destroy();
}

//---------------------------------------------------------
void CSG_Simple_Statistics::Add_Value(double Value, double Weight)
{
	if( m_nValues == 0 )
	{
		m_Minimum	= m_Maximum	= Value;
	}
	else if( m_Minimum > Value )
	{
		m_Minimum	= Value;
	}
	else if( m_Maximum < Value )
	{
		m_Maximum	= Value;
	}

	if( Weight > 0.0 )
	{
		m_Weights		+= Weight;
		m_Sum			+= Weight * Value;
		m_Sum2			+= Weight * Value*Value;

		m_bEvaluated	= false;
	}

	if( m_Values.Get_Value_Size() > 0 && m_Values.Inc_Array() )
	{
		((double *)m_Values.Get_Array())[m_nValues]	= Value;
	}

	m_nValues++;
}

//---------------------------------------------------------
void CSG_Simple_Statistics::_Evaluate(void)
{
	if( m_Weights > 0.0 )
	{
		m_Range			= m_Maximum - m_Minimum;
		m_Mean			= m_Sum  / m_Weights;
		m_Variance		= m_Sum2 / m_Weights - m_Mean*m_Mean;
		m_StdDev		= m_Variance > 0.0 ? sqrt(m_Variance) : 0.0;

		m_bEvaluated	= true;
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Class_Statistics::CSG_Class_Statistics(void)
{
	Create();
}

//---------------------------------------------------------
CSG_Class_Statistics::~CSG_Class_Statistics(void)
{
	Destroy();
}

//---------------------------------------------------------
void CSG_Class_Statistics::Create(void)
{
	m_Array.Create(sizeof(TClass), 0, SG_ARRAY_GROWTH_1);

	m_Classes	= NULL;
}

//---------------------------------------------------------
void CSG_Class_Statistics::Destroy(void)
{
	m_Array.Set_Array(0, (void **)&m_Classes);
}

//---------------------------------------------------------
void CSG_Class_Statistics::Add_Value(double Value)
{
	for(size_t i=0; i<m_Array.Get_Size(); i++)
	{
		if( m_Classes[i].Value == Value )
		{
			m_Classes[i].Count++;

			return;
		}
	}

	if( m_Array.Inc_Array((void **)&m_Classes) )
	{
		m_Classes[Get_Count() - 1].Count	= 1;
		m_Classes[Get_Count() - 1].Value	= Value;
	}
}

//---------------------------------------------------------
int CSG_Class_Statistics::Get_Majority(void)
{
	int		Index	= 0;

	for(int i=1; i<Get_Count(); i++)
	{
		if( m_Classes[i].Count > m_Classes[Index].Count )
		{
			Index	= i;
		}
	}

	return( Index );
}

bool CSG_Class_Statistics::Get_Majority(double &Value)
{
	int		Count;

	return( Get_Class(Get_Majority(), Value, Count) );
}

bool CSG_Class_Statistics::Get_Majority(double &Value, int &Count)
{
	return( Get_Class(Get_Majority(), Value, Count) );
}

//---------------------------------------------------------
int CSG_Class_Statistics::Get_Minority(void)
{
	int		Index	= 0;

	for(int i=1; i<Get_Count(); i++)
	{
		if( m_Classes[i].Count > m_Classes[Index].Count )
		{
			Index	= i;
		}
	}

	return( Index );
}

bool CSG_Class_Statistics::Get_Minority(double &Value)
{
	int		Count;

	return( Get_Class(Get_Minority(), Value, Count) );
}

bool CSG_Class_Statistics::Get_Minority(double &Value, int &Count)
{
	return( Get_Class(Get_Minority(), Value, Count) );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Cluster_Analysis::CSG_Cluster_Analysis(void)
{
	m_Centroid	= NULL;
	m_Variance	= NULL;
	m_nMembers	= NULL;
	m_Cluster	= NULL;
	m_nFeatures	= 0;
	m_nClusters	= 0;
	m_Iteration	= 0;
}

//---------------------------------------------------------
CSG_Cluster_Analysis::~CSG_Cluster_Analysis(void)
{
	Destroy();
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Destroy(void)
{
	for(int i=0; i<m_nClusters; i++)
	{
		SG_Free(m_Centroid[i]);
	}

	SG_FREE_SAFE(m_Centroid);
	SG_FREE_SAFE(m_Variance);
	SG_FREE_SAFE(m_nMembers);
	SG_FREE_SAFE(m_Cluster);

	m_Features.Destroy();

	m_nFeatures	= 0;
	m_nClusters	= 0;

	m_Iteration	= 0;

	return( true );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Create(int nFeatures)
{
	Destroy();

	if( nFeatures > 0 )
	{
		m_nFeatures	= nFeatures;

		m_Features.Create(m_nFeatures * sizeof(double), 0, SG_ARRAY_GROWTH_3);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Add_Element(void)
{
	return( m_nFeatures > 0 && m_Features.Inc_Array() );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Set_Feature(int iElement, int iFeature, double Value)
{
	if( iElement >= 0 && iElement < Get_nElements() && iFeature >= 0 && iFeature < m_nFeatures )
	{
		((double *)m_Features.Get_Entry(iElement))[iFeature]	= Value;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Execute(int Method, int nClusters)
{
	if( Get_nElements() <= 1 || nClusters <= 1 )
	{
		return( false );
	}

	//-----------------------------------------------------
	bool	bResult;
	int		iCluster;

	m_Iteration	= 0;

	m_nClusters	= nClusters;

	m_Cluster	= (int     *)SG_Calloc(Get_nElements()	, sizeof(int));

	m_nMembers	= (int     *)SG_Calloc(m_nClusters		, sizeof(int));
	m_Variance	= (double  *)SG_Calloc(m_nClusters		, sizeof(double));
	m_Centroid	= (double **)SG_Calloc(m_nClusters		, sizeof(double *));

	for(iCluster=0; iCluster<m_nClusters; iCluster++)
	{
		m_Centroid[iCluster]	= (double *)SG_Calloc(m_nFeatures, sizeof(double));
	}

	//-----------------------------------------------------
	switch( Method )
	{
	case 0: default:
		bResult	= Minimum_Distance(true);
		break;

	case 1:
		bResult	= Hill_Climbing(true);
		break;

	case 2:
		bResult	= Minimum_Distance(true) && Hill_Climbing(false);
		break;
	}

	if( bResult )
	{
		for(iCluster=0; iCluster<m_nClusters; iCluster++)
		{
			m_Variance[iCluster]	= m_nMembers[iCluster] ? m_Variance[iCluster] / m_nMembers[iCluster] : 0.0;
		}
	}

	return( bResult );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Minimum_Distance(bool bInitialize)
{
	bool	bContinue;
	int		iElement, iFeature, iCluster, nShifts;
	double	*Feature, SP_Last	= -1.0;

	//-----------------------------------------------------
	for(iElement=0; iElement<Get_nElements(); iElement++)
	{
		iCluster	= m_Cluster[iElement];

		if( bInitialize || iCluster < 0 || iCluster >= m_nClusters )
		{
			m_Cluster[iElement]	= iCluster = iElement % m_nClusters;
		}
	}

	//-----------------------------------------------------
	for(m_Iteration=1, bContinue=true; bContinue && SG_UI_Process_Get_Okay(); m_Iteration++)
	{
		for(iCluster=0; iCluster<m_nClusters; iCluster++)
		{
			m_Variance[iCluster]	= 0.0;
			m_nMembers[iCluster]	= 0;

			for(iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster][iFeature]	= 0.0;
			}
		}

		//-------------------------------------------------
		for(iElement=0, Feature=(double *)m_Features.Get_Array(); iElement<Get_nElements(); iElement++, Feature+=m_nFeatures)
		{
			if( (iCluster = m_Cluster[iElement]) >= 0 )
			{
				m_nMembers[iCluster]++;

				for(iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					m_Centroid[iCluster][iFeature]	+= Feature[iFeature];
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0; iCluster<m_nClusters; iCluster++)
		{
			double	d	= m_nMembers[iCluster] > 0 ? 1.0 / m_nMembers[iCluster] : 0.0;

			for(iFeature=0; iFeature<m_nFeatures; iFeature++)
			{
				m_Centroid[iCluster][iFeature]	*= d;
			}
		}

		//-------------------------------------------------
		for(iElement=0, Feature=(double *)m_Features.Get_Array(), m_SP=0.0, nShifts=0; iElement<Get_nElements() && bContinue; iElement++, Feature+=m_nFeatures)
		{
			double	minVariance	= -1.0;
			int		minCluster	= -1;

			for(iCluster=0; iCluster<m_nClusters; iCluster++)
			{
				double	iVariance	= 0.0;

				for(iFeature=0; iFeature<m_nFeatures; iFeature++)
				{
					iVariance	+= SG_Get_Square(m_Centroid[iCluster][iFeature] - Feature[iFeature]);
				}

				if( minVariance < 0.0 || iVariance < minVariance )
				{
					minVariance	= iVariance;
					minCluster	= iCluster;
				}
			}

			if( m_Cluster[iElement] != minCluster )
			{
				m_Cluster[iElement]	= minCluster;
				nShifts++;
			}

			m_SP					+= minVariance;
			m_Variance[minCluster]	+= minVariance;
		}

		//-------------------------------------------------
		if( nShifts == 0 )
		{
			bContinue	= false;
		}

		m_SP	/= Get_nElements();

		SG_UI_Process_Set_Text(CSG_String::Format(SG_T("%s: %d >> %s %f"),
			LNG("pass")		, m_Iteration,
			LNG("change")	, m_Iteration <= 1 ? m_SP : SP_Last - m_SP
		));

		SP_Last		= m_SP;
	}

	return( true );
}

//---------------------------------------------------------
bool CSG_Cluster_Analysis::Hill_Climbing(bool bInitialize)
{
	bool	bContinue;
	int		iElement, iFeature, iCluster, noShift;
	double	*Feature, Variance, SP_Last	= -1.0;

	//-----------------------------------------------------
	memset(m_Variance, 0, m_nClusters * sizeof(double));
	memset(m_nMembers, 0, m_nClusters * sizeof(int));

	for(iCluster=0; iCluster<m_nClusters; iCluster++)
	{
		memset(m_Centroid[iCluster], 0, m_nFeatures * sizeof(double));
	}

	//-----------------------------------------------------
	for(iElement=0, Feature=(double *)m_Features.Get_Array(); iElement<Get_nElements(); iElement++, Feature+=m_nFeatures)
	{
		iCluster	= m_Cluster[iElement];

		if( bInitialize || iCluster < 0 || iCluster >= m_nClusters )
		{
			m_Cluster[iElement]	= iCluster = iElement % m_nClusters;
		}

		m_nMembers[iCluster]++;

		for(iFeature=0, Variance=0.0; iFeature<m_nFeatures; iFeature++)
		{
			double	d	 = Feature[iFeature];
			m_Centroid[iCluster][iFeature]	+= d;
			Variance						+= d*d;
		}

		m_Variance[iCluster]	+= Variance;
	}

	//-----------------------------------------------------
	for(iCluster=0; iCluster<m_nClusters; iCluster++)
	{
		double	d	= m_nMembers[iCluster] != 0 ? 1.0 / (double)m_nMembers[iCluster] : 0;

		for(iFeature=0, Variance=0.0; iFeature<m_nFeatures; iFeature++)
		{
			m_Centroid[iCluster][iFeature]	*= d;
			Variance						+= SG_Get_Square(m_Centroid[iCluster][iFeature]);
		}

		m_Variance[iCluster]	-= m_nMembers[iCluster] * Variance;
	}

	noShift		= 0;

	//-----------------------------------------------------
	for(m_Iteration=1, bContinue=true; bContinue && SG_UI_Process_Get_Okay(false); m_Iteration++)
	{
		for(iElement=0, Feature=(double *)m_Features.Get_Array(); iElement<Get_nElements() && bContinue; iElement++, Feature+=m_nFeatures)
		{
			if( (iCluster = m_Cluster[iElement]) >= 0 )
			{
				if( noShift++ >= Get_nElements() )
				{
					bContinue	= false;
				}
				else if( m_nMembers[iCluster] > 1 )
				{
					int		jCluster, kCluster;
					double	VMin, V1, V2;

					for(iFeature=0, Variance=0.0; iFeature<m_nFeatures; iFeature++)
					{
						Variance	+= SG_Get_Square(m_Centroid[iCluster][iFeature] - Feature[iFeature]);
					}

					V1		= Variance * m_nMembers[iCluster] / (m_nMembers[iCluster] - 1.0);
					VMin	= -1.0;

					//-----------------------------------------
					// Bestimme Gruppe iCluster mit evtl. groesster Verbesserung...

					for(jCluster=0; jCluster<m_nClusters; jCluster++)
					{
						if( jCluster != iCluster )
						{
							for(iFeature=0, Variance=0.0; iFeature<m_nFeatures; iFeature++)
							{
								Variance	+= SG_Get_Square(m_Centroid[jCluster][iFeature] - Feature[iFeature]);
							}

							V2		= Variance * m_nMembers[jCluster] / (m_nMembers[jCluster] + 1.0);

							if( VMin < 0.0 || V2 < VMin )
							{
								VMin		= V2;
								kCluster	= jCluster;
							}
						}
					}

					//-----------------------------------------
					// Gruppenwechsel und Neuberechnung der Gruppencentroide...

					if( VMin >= 0 && VMin < V1 )
					{
						noShift					= 0;
						m_Variance[iCluster]	-= V1;
						m_Variance[kCluster]	+= VMin;
						V1						= 1.0 / (m_nMembers[iCluster] - 1.0);
						V2						= 1.0 / (m_nMembers[kCluster] + 1.0);

						for(iFeature=0; iFeature<m_nFeatures; iFeature++)
						{
							double	d	= Feature[iFeature];

							m_Centroid[iCluster][iFeature]	= (m_nMembers[iCluster] * m_Centroid[iCluster][iFeature] - d) * V1;
							m_Centroid[kCluster][iFeature]	= (m_nMembers[kCluster] * m_Centroid[kCluster][iFeature] + d) * V2;
						}

						m_Cluster[iElement]	= kCluster;

						m_nMembers[iCluster]--;
						m_nMembers[kCluster]++;
					}
				}
			}
		}

		//-------------------------------------------------
		for(iCluster=0, m_SP=0.0; iCluster<m_nClusters; iCluster++)
		{
			m_SP	+= m_Variance[iCluster];
		}

		m_SP	/= Get_nElements();

		SG_UI_Process_Set_Text(CSG_String::Format(SG_T("%s: %d >> %s %f"),
			LNG("pass")		, m_Iteration,
			LNG("change")	, m_Iteration <= 1 ? m_SP : SP_Last - m_SP
		));

		SP_Last		= m_SP;
	}

	return( true );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
// source: http://psydok.sulb.uni-saarland.de/volltexte/2004/268/html/

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Tail(double T, int df, TSG_Test_Distribution_Type Type)
{	// Hill's approx. to cumulative t-dist, Commun.A.C.M. 13,617-619.
	// See: J.H.Maindonald, Computational Statistics, p.295.
	// Calculates p given t and tail type.

	if( !T || !df || df < 1.0 )
	{
		return( -1.0 );
	}
	
	return( _Change_Tail_Type(Get_T_P(T, df), TESTDIST_TYPE_TwoTail, Type, T < 0.0) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Inverse(double p, int df, TSG_Test_Distribution_Type Type)
{	// Keith Dear & Robert Brennan.
	// Returns an accurate t to tol sig. fig.'s given p & df.  

	if( p <= 0.0 || p >= 1.0 || df < 1 )
	{
		return( -1.0 );
	}

	bool	bNegative	= (Type == TESTDIST_TYPE_Left && p < 0.5) || (Type == TESTDIST_TYPE_Right && p > 0.5);
	double	t, p0, p1, diff;

	p0		= p1 = _Change_Tail_Type(p, Type, TESTDIST_TYPE_TwoTail, bNegative);
	diff	= 1.0;

	while( fabs(diff) > .0001 )
	{
		t		= Get_T_Inv(p1, df);	// initial rough value
		diff	= Get_T_P(t, df) - p0;	// compare result with forward fn
		p1		= p1 - diff;			// small adjustment to p1
	}

	return( bNegative ? -t : t );
}

//---------------------------------------------------------
double CSG_Test_Distribution::_Change_Tail_Type(double p, TSG_Test_Distribution_Type from, TSG_Test_Distribution_Type to, bool bNegative)
{
	if( from != to )
	{
		switch( from )	// convert any tail type to 'left'
		{
		case TESTDIST_TYPE_Left:															break;
		case TESTDIST_TYPE_Right:	p	= 1.0 - p;											break;
		case TESTDIST_TYPE_Middle:	p	= p / 2.0 + 0.5;	if( bNegative )	p	= 1.0 - p;	break;
		case TESTDIST_TYPE_TwoTail:	p	= 1.0 - p / 2.0;	if( bNegative )	p	= 1.0 - p;	break;
	//	case TESTDIST_TYPE_Half:	p	= p + 0.5;			if( bNegative )	p	= 1.0 - p;	break;
		}

		switch( to )	// convert p from tail type 'left' to any other
		{
		case TESTDIST_TYPE_Left:															break;
		case TESTDIST_TYPE_Right:									p	= 1.0 - p;			break;
		case TESTDIST_TYPE_Middle:	if( bNegative )	p	= 1.0 - p;	p	= 2.0 * (1.0 - p);	break;
		case TESTDIST_TYPE_TwoTail:	if( bNegative )	p	= 1.0 - p;	p	= 2.0 * p - 1.0;	break;
	//	case TESTDIST_TYPE_Half:	if( bNegative )	p	= 1.0 - p;	p	= p - 0.5;			break;
		}
	}

	return( p );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Norm_P(double z)
{	// Returns the two-tailed standard normal probability of z
	const double	a1 = 0.0000053830, a2 = 0.0000488906, a3 = 0.0000380036,
					a4 = 0.0032776263, a5 = 0.0211410061, a6 = 0.0498673470;

	double	p;

	z	= fabs(z);
	
	p	= (((((a1 * z + a2) * z + a3) * z + a4) * z + a5) * z + a6) * z + 1.0;

	return( pow(p, -16) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Norm_Z(double p)
{	// Returns z given a half-middle tail type p.
	const double	a0 =  2.5066282,  a1 = -18.6150006,  a2 =  41.3911977,   a3 = -25.4410605,
					b1 = -8.4735109,  b2 =  23.0833674,  b3 = -21.0622410,   b4 =   3.1308291,
					c0 = -2.7871893,  c1 =  -2.2979648,  c2 =   4.8501413,   c3 =   2.3212128,
					d1 =  3.5438892,  d2 =   1.6370678;

	double	r, z;

	if( p > 0.42 )
	{
		r	= sqrt(-log(0.5 - p));
		z	= (((c3 * r + c2) * r + c1) * r + c0) / ((d2 * r + d1) * r + 1.0);
	}
	else
	{
		r	= p * p;
		z	= p * (((a3 * r + a2) * r + a1) * r + a0) / ((((b4 * r + b3) * r + b2) * r + b1) * r + 1.0);
	}

	return( z );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_P(double T, int df)
{	// Returns two-tail probability level given t and df.
	return(	df == 1 ? 1.0 - 2.0 * atan(fabs(T)) / M_PI
		:	df == 2 ? 1.0 - fabs(T) / sqrt(T*T + 2.0)
		:	df == 3 ? 1.0 - 2.0 * (atan(fabs(T) / sqrt(3.0)) + fabs(T) * sqrt(3.0) / (T*T + 3.0)) / M_PI
		:	df == 4 ? 1.0 - fabs(T) * (1.0 + 2.0 / (T*T + 4.0)) / sqrt(T*T + 4.0)
		:	Get_Norm_P(Get_T_Z(fabs(T), df))
	);
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Z(double T, int df)
{	// Converts a t value to an approximate z value w.r.t the given df
	// s.t. std.norm.(z) = t(z, df) at the two-tail probability level.

	double	A9, B9, T9, Z8, P7, B7, z;

	A9	= df - 0.5;
	B9	= 48.0 * A9*A9,
    T9	= T*T / df;
	Z8	= T9 >= 0.04
		? A9 * log(1.0 + T9)
		: A9 * (((1.0 - T9 * 0.75) * T9 / 3.0 - 0.5) * T9 + 1.0) * T9;
	P7	= ((0.4 * Z8 + 3.3) * Z8 + 24.0) * Z8 + 85.5;
	B7	= 0.8 * pow(Z8, 2.0) + 100.0 + B9;
	z	= (1.0 + (-P7 / B7 + Z8 + 3.0) / B9) * sqrt(Z8);

	return( z );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_T_Inv(double p, int df)
{	// Hill's approx. inverse t-dist.: Comm. of A.C.M Vol.13 No.10 1970 pg 620.
	// Calculates t given df and two-tail probability.

	if( df == 1 )
	{
		return( cos(p * M_PI / 2.0) / sin(p * M_PI / 2.0) );
	}

	if( df == 2 )
	{
		return( sqrt(2.0 / (p * (2.0 - p)) - 2.0) );
	}

	double	a, b, c, d, x, y;

	a	= 1.0 / (df - 0.5);
	b	= 48.0 / (a*a);
	c	= ((20700.0 * a / b - 98.0) * a - 16.0) * a + 96.36;
	d	= ((94.5 / (b + c) - 3.0) / b + 1.0) * sqrt(a * M_PI / 2.0) * df;
	x	= d * p;
	y	= pow(x, 2.0 / df);

	if( y > 0.05 + a )
	{
		x	= Get_Norm_Z(0.5 * (1.0 - p));
		y	= x*x;

		if( df < 5 )
		{
			c	= c + 0.3 * (df - 4.5) * (x + 0.6);
		}

		c	= (((0.05 * d * x - 5) * x - 7.0) * x - 2.0) * x + b + c;
		y	= (((((0.4 * y + 6.3) * y + 36.0) * y + 94.5) / c - y - 3.0) / b + 1.0) * x;
		y	= a * y*y;

		if( y > 0.002 )
		{
			y	= exp(y) - 1.0;
		}
		else
		{
			y	= 0.5 * y*y + y;
		}
	}            
	else
	{
		y	= ((1.0 / (((df + 6.0) / (df * y) - 0.089 * d - 0.822) * (df + 2.0) * 3.0) 
			+ 0.5 / (df + 4.0)) * y - 1.0) * (df + 1.0) / (df + 2.0) + 1.0 / y;
	}

	return( sqrt(df * y) );
}


///////////////////////////////////////////////////////////
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Tail_from_R2(double R2, int nPredictors, int nSamples, TSG_Test_Distribution_Type Type)
{
	double	F	= (nSamples - nPredictors - 1) * (R2 / nPredictors) / (1.0 - R2);

	return( CSG_Test_Distribution::Get_F_Tail(F, nPredictors, nSamples - nPredictors - 1, Type) );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Tail(double F, int dfn, int dfd, TSG_Test_Distribution_Type Type)
{
	// calculates for F, dfn(ominator) and dfd(enominator) the "tail" of the F-distribution

	double	p	= 1.0;

	if( F >= 0.00001 && dfn > 0 && dfd > 0 )
	{
		if( F * dfn >= dfd || F > 1.0 + 20.0 / dfn + 10.0 / sqrt((double)dfn) )
		{
			p	= Get_Gamma(F, dfn, dfd);
		}
		else
		{
			p	= 1.0 - Get_Gamma(1.0 / F, dfd, dfn);
		}
	}

	if( p <= 0.0 || p >= 1.0 )
	{
		p	= F > 1.0 ? 0.0 : F < 1.0 ? 1.0 : 0.5;
	}

	return( Type == TESTDIST_TYPE_Right ? p : 1.0 - p );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_F_Inverse(double alpha, int dfn, int dfd, TSG_Test_Distribution_Type Type)
{
	if( alpha < 0.0 || alpha > 1.0 || dfd < 0 || dfn < 0 )
	{
		return( -1 );
	}

	if( Type != TESTDIST_TYPE_Right )
	{
		alpha	= 1.0 - alpha;
	}

	const int		ITERMAX		= 100;
	const double	EPSILON		= 0.0001;

	int		i;
	double	lo, hi, mid, p;

	if( alpha <= 0.5 )
	{
		lo	= 0.5;
		hi	= lo;
		
		for(i=0; i<ITERMAX; i++)
		{
			hi	*= 2.0;
			p	= Get_F_Tail(hi, dfn, dfd);
	
			if( p > alpha )
			{
				lo	= hi;
			}
			else
			{
				break;
			}
		}

		if( p > alpha )
		{
			return( hi );
		}
	}
	else
	{
		hi	= 2;
		lo	= hi;

		for(i=0; i<ITERMAX; i++)
		{
			lo	/= 2.0;
			p	= Get_F_Tail(lo, dfn, dfd);

			if( p < alpha )
			{
				hi	= lo;
			}
			else
			{
				break;
			}
		}

		if( p < alpha )
		{
			return( lo );
		}
	}

	mid	= (hi + lo) / 2.0;

	for(i=0; i<ITERMAX && (hi-lo)>EPSILON*mid; i++)
	{
		mid	= (hi + lo) / 2.0;
		p	= Get_F_Tail(mid, dfn, dfd);

		if( p < alpha )
			hi	= mid;
		else if( p > alpha )
			lo	= mid;
		else
			break;
	}

	return( mid );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Gamma(double F, double dfn, double dfd)
{
	// calculates for F, dfn(ominator) and dfd(enominator) the uncomplete Gamma-function

	const double	EXPMIN	= -30.0;
	const double	SMALL	= 0.00000000001;

	double	x, c, er, s, n, t1, t;

	dfn		/= 2.0;
	dfd		/= 2.0;

	x	= dfd / (dfd + dfn * F);
	c	= Get_Log_Gamma(dfn + dfd) - Get_Log_Gamma(dfn) - Get_Log_Gamma(dfd + 1.0) + dfd * log(x) + dfn * log(1.0 - x);

	if( c < EXPMIN )
	{
		return( -1.0 );
	}

	dfn	+= dfd;
	dfd	+= 1.0;
	c	= exp(c);
	er	= SMALL / c;
	t	= dfn * x / dfd;
	t1	= 0.0;
	s	= t + 1.0;
	n	= 0;

	while( t > er || t > t1 )
	{
		n	+= 1;
		t1	 = t;
		t	*= ((dfn + n) * x / (dfd + n));
		s	+= t;
	}

	return( s * c );
}

//---------------------------------------------------------
double CSG_Test_Distribution::Get_Log_Gamma(double a)
{
	// calculates the logarithm of the Gamma-function

	const int		ARGMIN	= 6;

    const double	HL2PI	= 0.91893853320467275;	//  = log(2.0 * M_PI) / 2.0

	int	n	= (int)floor(ARGMIN - a + 0.0001);

	if( n > 0 )
	{
		a	+= n;
	}

	double	g;

	g	= 1.0 / (a*a);
	g	= (1.0 - g * (1.0 / 30.0 - g * (1.0 / 105.0 - g * (1.0 / 140.0 - g / 99.0)))) / (12.0 * a);
	g	= g + ((a - 0.5) * log(a) - a + HL2PI);

	for(int i=0; i<n; i++)
	{
		a	= a - 1.0;
		g	= g - log(a);
	}

	return( g );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CSG_Matrix	SG_Get_Correlation_Matrix	(const CSG_Matrix &Values, bool bCovariances)
{
	int		nVariables	= Values.Get_NX();
	int		nSamples	= Values.Get_NY();

	//-----------------------------------------------------
	int						i, j, k;
	CSG_Simple_Statistics	*S;
	CSG_Matrix				C;

	C.Create(nVariables, nVariables);

	//-----------------------------------------------------
	S	= new CSG_Simple_Statistics[nVariables];

	for(j=0; j<nVariables; j++)
	{
		for(i=0; i<nSamples; i++)
		{
			S[j]	+= Values[i][j];
		}
	}

	//-----------------------------------------------------
	for(k=0; k<nVariables; k++)
	{
		for(j=k; j<nVariables; j++)
		{
			double	cov	= 0.0;

			for(i=0; i<nSamples; i++)
			{
				cov	+= (Values[i][j] - S[j].Get_Mean()) * (Values[i][k] - S[k].Get_Mean());
			}

			cov	/= nSamples;

			if( !bCovariances )
			{
				cov	/= (S[j].Get_StdDev() * S[k].Get_StdDev());
			}

			C[j][k]	= C[k][j]	= cov;
		}
	}

	//-----------------------------------------------------
	delete[](S);

	return( C );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
