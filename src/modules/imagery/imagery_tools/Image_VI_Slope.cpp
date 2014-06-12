/**********************************************************
 * Version $Id: Image_VI_Slope.cpp 2072 2014-03-31 08:50:43Z reklov_w $
 *********************************************************/
/*******************************************************************************
    Image_VI_Slope.cpp
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


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Image_VI_Slope.h"


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CImage_VI_Slope::CImage_VI_Slope(void)
{
	Set_Name		(_TL("Vegetation Index (Slope Based)"));

	Set_Author		(SG_T("V.Olaya (c) 2004, O.Conrad (c) 2011"));

	Set_Description	(_TW(
		"Slope based vegetation indices.\n"
		"\n<ul>"
		"<li>Ratio Vegetation Index (Richardson and Wiegand, 1977)\n"
		"    RVI = R / NIR</li>\n"
		"<li>Normalized Ratio Vegetation Index (Baret and Guyot, 1991)\n"
		"    NRVI = (RVI - 1) / (RVI + 1)</li>\n"
		"<li>Normalized Difference Vegetation Index (Rouse et al. 1974)\n"
		"    NDVI = (NIR - R) / (NIR + R)</li>\n"
		"<li>Transformed Vegetation Index (Deering et al., 1975)\n"
		"    TVI = [(NIR - R) / (NIR + R)]^0.5 + 0.5 </li>\n"
		"<li>Corrected Transformed Ratio Vegetation Index (Perry and Lautenschlager, 1984)\n"
		"    CTVI = [(NDVI + 0.5) / abs(NDVI + 0.5)] * [abs(NDVI + 0.5)]^0.5</li>\n"
		"<li>Thiam's Transformed Vegetation Index (Thiam, 1997)\n"
		"    RVI = [abs(NDVI) + 0.5]^0.5</li>\n"
		"<li>Soil Adjusted Vegetation Index (Huete, 1988)\n"
		"    SAVI = [(NIR - R) / (NIR + R)] * (1 + S)</li>\n"
		"</ul>(NIR = near infrared, R = red, S = soil adjustment factor)\n"
		"\n"
		"References:\n"
		"K.R. McCloy (2006): Resource Management Information Systems: Remote Sensing, GIS and Modelling. 2nd Edition, CRC Taylor & Francis, 575pp.\n"
		"\n"
		"N.G. Silleos, T.K. Alexandridis, I.Z. Gitas & K. Perakis (2006): "
		"Vegetation Indices: Advances Made in Biomass Estimation and Vegetation Monitoring in the Last 30 Years, "
		"Geocarto International, 21:4, 21-28, "
		"<a target=\"_blank\" href=\"http://dx.doi.org/10.1080/10106040608542399\">online</a>.\n"
	));

	Parameters.Add_Grid(
		NULL, "RED"		, _TL("Red Reflectance"),
		_TL(""), 
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL, "NIR"		, _TL("Near Infrared Reflectance"), 
		_TL(""), 
		PARAMETER_INPUT
	);

	Parameters.Add_Grid(
		NULL, "DVI"		, _TL("Difference Vegetation Index"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "NDVI"	, _TL("Normalized Difference Vegetation Index"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);
	
	Parameters.Add_Grid(
		NULL, "RVI"		, _TL("Ratio Vegetation Index"), 
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "NRVI"	, _TL("Normalized Ratio Vegetation Index"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "TVI"		, _TL("Transformed Vegetation Index"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "CTVI"	, _TL("Corrected Transformed Vegetation Index"), 
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "TTVI"	, _TL("Thiam's Transformed Vegetation Index"), 
		_TL(""), 
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Grid(
		NULL, "SAVI"		, _TL("Soil Adjusted Vegetation Index"), 
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL
	);

	Parameters.Add_Value(
		NULL, "SOIL"	, _TL("Soil Adjustment Factor"), 
		_TW("Soil adjustment factor for SAVI calculation. "
			"Suggested values are 1.0 in case of very low vegetation, "
			"0.5 for intermediate 0.5, and 0.25 for high densities (Silleos et al. 2006)."),
		PARAMETER_TYPE_Double, 0.5, 0.0, true, 1.0, true
	);
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CImage_VI_Slope::On_Execute(void)
{
	CSG_Grid	*pRed		= Parameters("RED"  )->asGrid();
	CSG_Grid	*pNIR		= Parameters("NIR"  )->asGrid();

	CSG_Grid	*pDVI		= Parameters("DVI"  )->asGrid();
	CSG_Grid	*pNDVI		= Parameters("NDVI" )->asGrid();
	CSG_Grid	*pRVI		= Parameters("RVI"  )->asGrid();
	CSG_Grid	*pNRVI		= Parameters("NRVI" )->asGrid();
	CSG_Grid	*pTVI		= Parameters("TVI"  )->asGrid();
	CSG_Grid	*pCTVI		= Parameters("CTVI" )->asGrid();
	CSG_Grid	*pTTVI		= Parameters("TTVI" )->asGrid();
	CSG_Grid	*pSAVI		= Parameters("SAVI" )->asGrid();

	DataObject_Set_Colors(pDVI  , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pNDVI , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pRVI  , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pNRVI , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pTVI  , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pCTVI , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pTTVI , 100, SG_COLORS_WHITE_GREEN, false);
	DataObject_Set_Colors(pSAVI , 100, SG_COLORS_WHITE_GREEN, false);

	m_Soil	= Parameters("SOIL")->asDouble();

	for(int y=0; y<Get_NY() && Set_Progress(y); y++)
	{
		for(int x=0; x<Get_NX(); x++)
		{
			if( pRed->is_NoData(x, y) || pNIR->is_NoData(x, y) )
			{
				if( pDVI   )	pDVI	->Set_NoData(x, y);
				if( pNDVI  )	pNDVI	->Set_NoData(x, y);
				if( pRVI   )	pRVI	->Set_NoData(x, y);
				if( pNRVI  )	pNRVI	->Set_NoData(x, y);
				if( pTVI   )	pTVI	->Set_NoData(x, y);
				if( pTTVI  )	pTTVI	->Set_NoData(x, y);
				if( pCTVI  )	pCTVI	->Set_NoData(x, y);
				if( pSAVI  )	pSAVI	->Set_NoData(x, y);
			}
			else
			{
				double	R, NIR, Value;

				R	= pRed->asDouble(x, y);
				NIR	= pNIR->asDouble(x, y);

				if( pDVI   ) { if( Get_DVI  (R, NIR, Value) ) pDVI  ->Set_Value(x, y, Value); else pDVI  ->Set_NoData(x, y); }
				if( pNDVI  ) { if( Get_NDVI (R, NIR, Value) ) pNDVI ->Set_Value(x, y, Value); else pNDVI ->Set_NoData(x, y); }
				if( pRVI   ) { if( Get_RVI  (R, NIR, Value) ) pRVI  ->Set_Value(x, y, Value); else pRVI  ->Set_NoData(x, y); }
				if( pNRVI  ) { if( Get_NRVI (R, NIR, Value) ) pNRVI ->Set_Value(x, y, Value); else pNRVI ->Set_NoData(x, y); }
				if( pTVI   ) { if( Get_TVI  (R, NIR, Value) ) pTVI  ->Set_Value(x, y, Value); else pTVI  ->Set_NoData(x, y); }
				if( pCTVI  ) { if( Get_CTVI (R, NIR, Value) ) pCTVI ->Set_Value(x, y, Value); else pCTVI ->Set_NoData(x, y); }
				if( pTTVI  ) { if( Get_TTVI (R, NIR, Value) ) pTTVI ->Set_Value(x, y, Value); else pTTVI ->Set_NoData(x, y); }
				if( pSAVI  ) { if( Get_SAVI (R, NIR, Value) ) pSAVI ->Set_Value(x, y, Value); else pSAVI ->Set_NoData(x, y); }
			}
		}
	}

	return( true );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_DVI(double R, double NIR, double &Value)
{
	Value	= NIR - R;

	return( true );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_NDVI(double R, double NIR, double &Value)
{
	if( R + NIR != 0.0 )
	{
		Value	= (NIR - R) / (NIR + R);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_RVI(double R, double NIR, double &Value)
{
	if( R != 0.0 )
	{
		Value	= NIR / R;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_NRVI(double R, double NIR, double &Value)
{
	if( Get_RVI(R, NIR, Value) && Value + 1.0 != 0.0 )
	{
		Value	= (Value - 1.0) / (Value + 1.0);

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_TVI(double R, double NIR, double &Value)
{
	if( Get_NDVI(R, NIR, Value) && Value > 0.0 )
	{
		Value	= sqrt(Value) + 0.5;

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_CTVI(double R, double NIR, double &Value)
{
	if( Get_NDVI(R, NIR, Value) )
	{
		Value	+= 0.5;

		Value	= Value > 0.0 ? sqrt(fabs(Value)) : -sqrt(fabs(Value));

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_TTVI(double R, double NIR, double &Value)
{
	if( Get_NDVI(R, NIR, Value) )
	{
		Value	= sqrt(fabs(Value + 0.5));

		return( true );
	}

	return( false );
}

//---------------------------------------------------------
inline bool CImage_VI_Slope::Get_SAVI(double R, double NIR, double &Value)
{
	if( Get_NDVI(R, NIR, Value) )
	{
		Value	= Value * (1.0 + m_Soil);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//                                                       //
//                                                       //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
