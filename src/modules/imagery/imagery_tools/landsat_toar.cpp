/**********************************************************
 * Version $Id: landsat_toar.cpp 1938 2014-01-12 13:44:06Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     imagery_tools                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   landsat_toar.cpp                    //
//                                                       //
//                 Copyright (C) 2013 by                 //
//            Benjamin Bechtel & Olaf Conrad             //
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
#include "landsat_toar.h"

//---------------------------------------------------------
#include "landsat_toar_core.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
enum
{
	mss1 = 0,	// Landsat-1 MSS
	mss2,		// Landsat-2 MSS
	mss3,		// Landsat-3 MSS
	mss4,		// Landsat-4 MSS
	mss5,		// Landsat-5 MSS
	tm4,		// Landsat-4 TM
	tm5,		// Landsat-5 TM
	tm7			// Landsat-7 ETM+
};

//---------------------------------------------------------
#define PRM_IN(id)				(CSG_String("DN_") + id)
#define PRM_OUT(id)				(CSG_String("RF_") + id)
#define PRM_ADD_BAND__IN(id)	Parameters.Add_Grid(pNode, PRM_IN (id), CSG_String::Format(SG_T("%s %s"), _TL("DN"), _TL("Band")) + id, _TL(""), PARAMETER_INPUT_OPTIONAL);
#define PRM_ADD_BAND_OUT(id)	Parameters.Add_Grid(pNode, PRM_OUT(id), CSG_String::Format(SG_T("%s %s"), _TL("Reflectance"), _TL("Band")) + id, _TL(""), PARAMETER_OUTPUT_OPTIONAL);
#define PRM_ADD_BAND(id)		PRM_ADD_BAND_IN(id); PRM_ADD_BAND_OUT(id)
#define PRM_ENABLE_OUTPUT(id)	pParameters->Get_Parameter(PRM_OUT(id))->Set_Enabled(pParameters->Get_Parameter(PRM_IN(id))->is_Enabled() && pParameters->Get_Parameter(PRM_IN(id))->asGrid())

//---------------------------------------------------------
#define GET_DESC_INT(name, value)		CSG_String::Format(SG_T("%s: %d\n"), name, value)
#define GET_DESC_FLT(name, value)		CSG_String::Format(SG_T("%s: %f\n"), name, value)
#define GET_DESC_RNG(name, min, max)	CSG_String::Format(SG_T("%s: %f / %f\n"), name, min, max)
#define GET_DESC_STR(name, value)		CSG_String(name) + ": " + CSG_String(value) + "\n"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool	Set_Date_from_String	(char *date, const CSG_String s)
{
	// yyyy-mm-dd
	// 0123456789

	if( s.Length() == 10 )
	{
		strncpy(date, s.b_str(), 11);

		return( true );
	}

	return( false );
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool	Load_MetaFile	(const CSG_String &MetaFile, lsat_data &lsat)
{
	if( !SG_File_Exists(MetaFile) )	// Data from metadata file
	{
		return( false );
	}

	if( !lsat_newdata(MetaFile, &lsat)
	&&  !lsat_mtldata(MetaFile, &lsat)
	&&  !lsat_metdata(MetaFile, &lsat)	)
	{
		return( false );
	}

	return( true );
}

//---------------------------------------------------------
int		Get_Sensor_Index	(int LSat_Number, const CSG_String &LSat_Sensor)
{
	if( !LSat_Sensor.CmpNoCase("MSS") )
	{
		switch( LSat_Number )
		{
		case 1:	return(0);	break;	// Landsat-1 MSS
		case 2:	return(1);	break;	// Landsat-2 MSS
		case 3:	return(2);	break;	// Landsat-3 MSS
		case 4:	return(3);	break;	// Landsat-4 MSS
		case 5:	return(4);	break;	// Landsat-5 MSS
		}
	}
	else if( !CSG_String(LSat_Sensor).CmpNoCase("TM") )
	{
		switch( LSat_Number )
		{
		case 4:	return(5);	break;	// Landsat-1 MSS
		case 5:	return(6);	break;	// Landsat-2 MSS
		}
	}
	else if( !CSG_String(LSat_Sensor).Find("ETM") && LSat_Number == 7 )
	{
		return(7);	// Landsat-7 ETM+
	}
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CLandsat_TOAR::CLandsat_TOAR(void)
{
	CSG_Parameter	*pNode;

	//-----------------------------------------------------
	Set_Name		(_TL("Top of Atmosphere Reflectance"));

	Set_Author		(_TL("B.Bechtel, O.Conrad (c) 2013"));

	Set_Description	(_TW(
		"Calculation of top-of-atmosphere radiance or reflectance and temperature (TOAR) for Landsat MSS/TM/ETM+. "
		"This module incorporates E.J. Tizado's GRASS GIS implementation (i.landsat.toar).\n"
		"\n"
		"References:\n"
		"<a target=\"_blank\" href=\"http://landsathandbook.gsfc.nasa.gov/\">Landsat 7 Science Data Users Handbook</a>"
	));

	//-----------------------------------------------------
	pNode	= Parameters.Add_Grid_System(NULL, "GS_SPECTRAL", _TL("Spectral"    ), _TL(""));
	PRM_ADD_BAND__IN("10"); PRM_ADD_BAND__IN("20"); PRM_ADD_BAND__IN("30"); PRM_ADD_BAND__IN("40"); PRM_ADD_BAND__IN("50"); PRM_ADD_BAND__IN("70");
	PRM_ADD_BAND_OUT("10"); PRM_ADD_BAND_OUT("20"); PRM_ADD_BAND_OUT("30"); PRM_ADD_BAND_OUT("40"); PRM_ADD_BAND_OUT("50"); PRM_ADD_BAND_OUT("70");

	pNode	= Parameters.Add_Grid_System(NULL, "GS_THERMAL"	, _TL("Thermal"     ), _TL(""));
	PRM_ADD_BAND__IN("60"); PRM_ADD_BAND__IN("61"); PRM_ADD_BAND__IN("62");
	PRM_ADD_BAND_OUT("60"); PRM_ADD_BAND_OUT("61"); PRM_ADD_BAND_OUT("62");

	pNode	= Parameters.Add_Grid_System(NULL, "GS_PAN"		, _TL("Panchromatic"), _TL(""));
	PRM_ADD_BAND__IN("80");
	PRM_ADD_BAND_OUT("80");

	//-----------------------------------------------------
	Parameters.Add_FilePath(
		NULL	, "METAFILE"	, _TL("Metadata File"),
		_TL("Name of Landsat metadata file (.met or MTL.txt)"),
		CSG_String::Format(SG_T("%s|*.met;*.txt|%s|*.*"),
			_TL("Metadata Files (*.met, *.txt)"),
			_TL("All Files")
		) 
	);

	//-----------------------------------------------------
	Parameters.Add_Choice(
		NULL	, "SENSOR"		, _TL("Spacecraft Sensor"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|%s|"),
			_TL("Landsat-1 MSS"),
			_TL("Landsat-2 MSS"),
			_TL("Landsat-3 MSS"),
			_TL("Landsat-4 MSS"),
			_TL("Landsat-5 MSS"),
			_TL("Landsat-4 TM"),
			_TL("Landsat-5 TM"),
			_TL("Landsat-7 ETM+")
		), 7
	);

	//-----------------------------------------------------
	Parameters.Add_String(
		NULL	, "DATE_ACQU"	, _TL("Image Acquisition Date"),
		_TL("Image acquisition date (yyyy-mm-dd)"),
		"2001-01-01"
	);

	Parameters.Add_String(
		NULL	, "DATE_PROD"	, _TL("Image Creation Date"),
		_TL("Image creation date (yyyy-mm-dd)"),
		"2001-01-01"
	);

	Parameters.Add_Value(
		NULL	, "SUN_HGT"		, _TL("Suns's Height"),
		_TL("Sun's height above horizon in degree"),
		PARAMETER_TYPE_Double, 45.0, 0.0, true, 90.0, true
	);

	Parameters.Add_Value(
		NULL	, "AS_RAD"		, _TL("At-Sensor Radiance"),
		_TL("Output at-sensor radiance for all bands"),
		PARAMETER_TYPE_Bool, false
	);

	//-----------------------------------------------------
	pNode	= Parameters.Add_Choice(
		NULL	, "AC_METHOD"	, _TL("Atmospheric Correction"),
		_TL(""),
		CSG_String::Format(SG_T("%s|%s|%s|%s|%s|%s|%s|"),
			_TL("uncorrected"),
			_TL("corrected"),
			_TL("dos1"),
			_TL("dos2"),
			_TL("dos2b"),
			_TL("dos3"),
			_TL("dos4")
		), 0
	);

	Parameters.Add_Value(
		pNode	, "AC_DO_CELLS"	, _TL("Minimum Number of Dark Object Cells"),
		_TL("Minimum pixels to consider digital number as dark object"),
		PARAMETER_TYPE_Int, 1000, 0, true
	);

	Parameters.Add_Value(
		pNode	, "AC_RAYLEIGH"	, _TL("Rayleigh Scattering"),
		_TL("Rayleigh atmosphere (diffuse sky irradiance)"),
		PARAMETER_TYPE_Double, 0.0
	);

	Parameters.Add_Value(
		pNode	, "AC_SUN_RAD"	, _TL("Solar Radiance"),
		_TL("Percent of solar radiance in path radiance"),
		PARAMETER_TYPE_Double, 1.0, 0.0, true, 100.0, true
	);

	//-----------------------------------------------------
	pNode	= Parameters.Add_Node(
		NULL	, "ETM_GAIN"	, _TL("ETM+ Gain"),
		_TL("Gain (H/L) of all Landsat ETM+ bands (1-5,61,62,7,8)")
	);

	Parameters.Add_Choice(pNode, "ETM_GAIN_10", _TL("Band 1" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_20", _TL("Band 2" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_30", _TL("Band 3" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_40", _TL("Band 4" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_50", _TL("Band 5" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_61", _TL("Band 61"), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 0);
	Parameters.Add_Choice(pNode, "ETM_GAIN_62", _TL("Band 62"), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_70", _TL("Band 7" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 1);
	Parameters.Add_Choice(pNode, "ETM_GAIN_80", _TL("Band 8" ), _TL(""), CSG_String::Format(SG_T("%s|%s|"), _TL("low"), _TL("high")), 0);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
int CLandsat_TOAR::On_Parameter_Changed(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("METAFILE")) && *pParameter->asString() )
	{
		lsat_data	lsat;

		if( !Load_MetaFile(pParameter->asString(), lsat) )
		{
			pParameter->Set_Value((const char *)"");

			Error_Set(_TL("could not read metadata file"));
		}
		else
		{
			pParameters->Get_Parameter("SENSOR"   )->Set_Value(Get_Sensor_Index(lsat.number, lsat.sensor));
			pParameters->Get_Parameter("DATE_ACQU")->Set_Value((const char *)lsat.date);
			pParameters->Get_Parameter("DATE_PROD")->Set_Value((const char *)lsat.creation);
			pParameters->Get_Parameter("SUN_HGT"  )->Set_Value(lsat.sun_elev);
		}
	}

	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("SENSOR")) )
	{
		pParameters->Get_Parameter("METAFILE")->Set_Value((const char *)"");
	}

	return( 0 );
}

//---------------------------------------------------------
int CLandsat_TOAR::On_Parameters_Enable(CSG_Parameters *pParameters, CSG_Parameter *pParameter)
{
	if(	!SG_STR_CMP(pParameter->Get_Identifier(), SG_T("AC_METHOD")) )
	{
		pParameters->Get_Parameter("AC_DO_CELLS")->Set_Enabled(pParameter->asInt() > 1);
	}

	if(	1 )
	{
		int	Sensor	= pParameters->Get_Parameter("SENSOR")->asInt();

		pParameters->Get_Parameter("ETM_GAIN"  )->Set_Enabled(Sensor == tm7 && !(*pParameters->Get_Parameter("METAFILE")->asString()));

		pParameters->Get_Parameter("GS_PAN"    )->Set_Enabled(Sensor == tm7);
		pParameters->Get_Parameter("GS_THERMAL")->Set_Enabled(Sensor >= tm4);

		pParameters->Get_Parameter(PRM_IN("50"))->Set_Enabled(Sensor >= tm4);
		pParameters->Get_Parameter(PRM_IN("70"))->Set_Enabled(Sensor >= tm4);
		pParameters->Get_Parameter(PRM_IN("60"))->Set_Enabled(Sensor == tm4 || Sensor == tm5);
		pParameters->Get_Parameter(PRM_IN("61"))->Set_Enabled(Sensor == tm7);
		pParameters->Get_Parameter(PRM_IN("62"))->Set_Enabled(Sensor == tm7);

		PRM_ENABLE_OUTPUT("10");
		PRM_ENABLE_OUTPUT("20");
		PRM_ENABLE_OUTPUT("30");
		PRM_ENABLE_OUTPUT("40");
		PRM_ENABLE_OUTPUT("50");
		PRM_ENABLE_OUTPUT("60");
		PRM_ENABLE_OUTPUT("61");
		PRM_ENABLE_OUTPUT("62");
		PRM_ENABLE_OUTPUT("70");
		PRM_ENABLE_OUTPUT("80");
	}

	return( 0 );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CLandsat_TOAR::On_Execute(void)
{
	//-----------------------------------------------------
	int			Sensor, iBand, dn_dark[MAX_BANDS], dn_mode[MAX_BANDS], AC_Method, AC_nCells;
	double		AC_Rayleigh, AC_SunRad;
	CSG_Grid	*pInput, *pOutput;
	lsat_data	lsat;

	//-----------------------------------------------------
	if( *Parameters("METAFILE")->asString()  )
	{
		if( !Load_MetaFile(Parameters("METAFILE")->asString(), lsat) )
		{
			Error_Set(_TL("could not read metadata file"));

			return( false );
		}

		Sensor		= Get_Sensor_Index(lsat.number, lsat.sensor);
	}

	//-----------------------------------------------------
	else
	{
		memset(&lsat, 0, sizeof(lsat));

		if( !Set_Date_from_String(lsat.date    , Parameters("DATE_ACQU")->asString()) )
		{
			Error_Set(_TL("invalid acquisition date"));

			return( false );
		}

		if( !Set_Date_from_String(lsat.creation, Parameters("DATE_PROD")->asString()) )
		{
			Error_Set(_TL("invalid production date"));

			return( false );
		}

		lsat.sun_elev	= Parameters("SUN_HGT")->asDouble();

		switch( Sensor = Parameters("SENSOR")->asInt() )
		{
		case mss1:	set_MSS1(&lsat);	break;
		case mss2:	set_MSS2(&lsat);	break;
		case mss3:	set_MSS3(&lsat);	break;
		case mss4:	set_MSS4(&lsat);	break;
		case mss5:	set_MSS5(&lsat);	break;
		case tm4:	set_TM4 (&lsat);	break;
		case tm5:	set_TM5 (&lsat);	break;
		case tm7:
			{
				char	gain[9];

				gain[0]	= Parameters("ETM_GAIN_10")->asInt() == 0 ? 'L' : 'H';
				gain[1]	= Parameters("ETM_GAIN_20")->asInt() == 0 ? 'L' : 'H';
				gain[2]	= Parameters("ETM_GAIN_30")->asInt() == 0 ? 'L' : 'H';
				gain[3]	= Parameters("ETM_GAIN_40")->asInt() == 0 ? 'L' : 'H';
				gain[4]	= Parameters("ETM_GAIN_50")->asInt() == 0 ? 'L' : 'H';
				gain[5]	= Parameters("ETM_GAIN_61")->asInt() == 0 ? 'L' : 'H';
				gain[6]	= Parameters("ETM_GAIN_62")->asInt() == 0 ? 'L' : 'H';
				gain[7]	= Parameters("ETM_GAIN_70")->asInt() == 0 ? 'L' : 'H';
				gain[8]	= Parameters("ETM_GAIN_80")->asInt() == 0 ? 'L' : 'H';

				set_ETM (&lsat, gain);	// Landsat-7 requires band gain with 9 (H/L) data
				break;
			}
		}
    }


	///////////////////////////////////////////////////////
	//-----------------------------------------------------
	// Atmospheric Correction

	switch( Parameters("AC_METHOD")->asInt() )
	{
	default:	AC_Method	= UNCORRECTED;	break;
	case 1:		AC_Method	= CORRECTED;	break;
	case 2:		AC_Method	= DOS1;			break;
	case 3:		AC_Method	= DOS2;			break;
	case 4:		AC_Method	= DOS2b;		break;
	case 5:		AC_Method	= DOS3;			break;
	case 6:		AC_Method	= DOS4;			break;
	}

	AC_nCells	= Parameters("AC_DO_CELLS")->asInt();
	AC_Rayleigh	= Parameters("AC_RAYLEIGH")->asDouble();
	AC_SunRad	= Parameters("AC_SUN_RAD" )->asDouble() / 100.0;

	//-----------------------------------------------------
	for(iBand=0; iBand<lsat.bands && Process_Get_Okay(); iBand++)
	{
		dn_mode[iBand] = 0;
		dn_dark[iBand] = (int)lsat.band[iBand].qcalmin;

		if( !(pInput = Get_Band_Input(iBand, Sensor)) )
		{
			continue;
		}

		//-------------------------------------------------
		if( AC_Method > DOS && !lsat.band[iBand].thermal )	// calculate dark pixel
		{
			Process_Set_Text(CSG_String::Format(SG_T("%s [%d/%d]"), _TL("Dark Object Subtraction"), lsat.band[iBand].number, lsat.bands));

			int				j;
			unsigned long	hist[256], h_max;

			memset(hist, 0, 256 * sizeof(unsigned long));

			//---------------------------------------------
			// histogram
			for(int y=0; y<pInput->Get_NY() && Set_Progress(y, pInput->Get_NY()); y++)
			{
				for(int x=0, q; x<pInput->Get_NX(); x++)
				{
					if( !pInput->is_NoData(x, y) && (q = pInput->asInt(x, y)) != 0 && q >= lsat.band[iBand].qcalmin && q <= 255 )
					{
						hist[q]++;
					}
				}
			}

			//---------------------------------------------
			// DN of dark object
			for(j=(int)lsat.band[iBand].qcalmin; j<256; j++)
			{
				if( hist[j] >= (unsigned int)AC_nCells )
				{
					dn_dark[iBand] = j;
			
					break;
				}
			}

			//---------------------------------------------
			// Mode of DN
			for(j=(int)lsat.band[iBand].qcalmin, h_max=0L; j<241; j++)
			{
				if( hist[j] > h_max )	// Exclude potentially saturated < 240
				{
					h_max = hist[j];
					dn_mode[iBand] = j;
				}
			}

			Message_Add(CSG_String::Format(SG_T("DN = %.2d [%lu] : mode %.2d [%lu] %s"),
				dn_dark[iBand], hist[dn_dark[iBand]],
				dn_mode[iBand], hist[dn_mode[iBand]],
				hist[255] > hist[dn_mode[iBand]] ? ", excluding DN > 241" : ""
			));
		}

		//-------------------------------------------------
		lsat_bandctes(&lsat, iBand, AC_Method, AC_SunRad, dn_dark[iBand], AC_Rayleigh);	// calculate transformation constants
	}


	///////////////////////////////////////////////////////
	//-----------------------------------------------------
	bool	bRadiance	= Parameters("AS_RAD")->asBool();

	CSG_String	sAll	= _TL("Top of Atmosphere Reflectance");	sAll	+= "\n";

	sAll	+= GET_DESC_STR(_TL("Platform")              , CSG_String::Format(SG_T("Landsat-%d"), lsat.number));
	sAll	+= GET_DESC_STR(_TL("Sensor")                , lsat.sensor);
	sAll	+= GET_DESC_STR(_TL("Acquisition")           , lsat.date);
	sAll	+= GET_DESC_STR(_TL("Production")            , lsat.creation);
	sAll	+= GET_DESC_FLT(_TL("Earth-Sun Distance")    , lsat.dist_es);
	sAll	+= GET_DESC_FLT(_TL("Solar Height")          , lsat.sun_elev);
	sAll	+= GET_DESC_STR(_TL("Atmospheric Correction"), Parameters("AC_METHOD")->asString());

	//-----------------------------------------------------
	for(iBand=0; iBand<lsat.bands && Process_Get_Okay(); iBand++)
	{
		if( !(pInput  = Get_Band_Input (iBand, Sensor))
		||  !(pOutput = Get_Band_Output(iBand, Sensor)) )
		{
			continue;
		}

		Process_Set_Text(CSG_String::Format(SG_T("%s [%d/%d]"),
			bRadiance ? _TL("Radiance") : lsat.band[iBand].thermal ? _TL("Temperature") : _TL("Reflectance"),
			lsat.band[iBand].number, lsat.bands
		));

		//-------------------------------------------------
		for(int y=0; y<pInput->Get_NY() && Set_Progress(y, pInput->Get_NY()); y++)
		{
			#pragma omp parallel for
			for(int x=0; x<pInput->Get_NX(); x++)
			{
				double	qcal;

				if( pInput->is_NoData(x, y) || (qcal = pInput->asDouble(x, y)) == 0.0 || qcal < lsat.band[iBand].qcalmin )
				{
					pOutput->Set_NoData(x, y);
				}
				else
				{
					double	r	= lsat_qcal2rad(qcal, &lsat.band[iBand]);

					if( bRadiance )
					{
						pOutput->Set_Value(x, y, r);
					}
					else if( lsat.band[iBand].thermal )
					{
						pOutput->Set_Value(x, y, lsat_rad2temp(r, &lsat.band[iBand]));
					}
					else
					{
						r	= lsat_rad2ref(r, &lsat.band[iBand]);

						pOutput->Set_Value(x, y, r < 0.0 && AC_Method > DOS ? 0.0 : r);
					}
				}
			}
		}

		//-------------------------------------------------
		CSG_String	sBand;

		sBand	+= GET_DESC_INT(_TL("Band"                     ), lsat.band[iBand].number);
		sBand	+= GET_DESC_STR(_TL("Type"                     ), bRadiance ? _TL("Radiance") : lsat.band[iBand].thermal ? _TL("Temperature") : _TL("Reflectance"));
		sBand	+= GET_DESC_RNG(_TL("Calibrated Digital Number"), lsat.band[iBand].qcalmin, lsat.band[iBand].qcalmax);
		sBand	+= GET_DESC_RNG(_TL("Calibration Constants"    ), lsat.band[iBand].lmin   , lsat.band[iBand].lmax);
		sBand	+= GET_DESC_STR(AC_Method > DOS ? _TL("At-Surface Radiance") : _TL("At-Sensor Radiance"), CSG_String::Format(SG_T("%.5lf * DN + %.5lf"), lsat.band[iBand].gain, lsat.band[iBand].bias));

		if( lsat.band[iBand].thermal )
		{
			sBand	+= GET_DESC_STR(_TL("At-Sensor Temperature"), CSG_String::Format(SG_T("%.3lf / ln[(%.3lf / %s) + 1.0]"), lsat.band[iBand].K2, lsat.band[iBand].K1, _TL("Radiance")));
			sBand	+= GET_DESC_FLT(_TL("Temperature K1"       ), lsat.band[iBand].K1);
			sBand	+= GET_DESC_FLT(_TL("Temperature K2"       ), lsat.band[iBand].K2);
		}
		else
		{
			sBand	+= GET_DESC_FLT(_TL("Mean Solar Irradiance"), lsat.band[iBand].esun);	// Mean Solar Exoatmospheric Irradiance
			sBand	+= GET_DESC_STR(AC_Method > DOS ? _TL("At-Surface Reflectance") : _TL("At-Sensor Reflectance"), CSG_String::Format(SG_T("%s / %.5lf"), _TL("Radiance"), lsat.band[iBand].K2));

			if( AC_Method > DOS )
			{
				double	ref_mode	= lsat_rad2ref(lsat_qcal2rad(dn_mode[iBand], &lsat.band[iBand]), &lsat.band[iBand]);

				sBand	+= GET_DESC_INT(_TL("Minimum of Darkness DN Cells" ), AC_nCells);
				sBand	+= GET_DESC_INT(_TL("Darkness Digital Number (DN)" ), dn_dark[iBand]);
				sBand	+= GET_DESC_INT(_TL("Mode of DN"                   ), dn_mode[iBand]);
				sBand	+= GET_DESC_FLT(_TL("Mode in Reflectance Histogram"), ref_mode);
			}
		}

		Message_Add(sBand, true);

		//-------------------------------------------------
		pOutput->Set_Name(CSG_String::Format(SG_T("%s [%s]"), pInput->Get_Name(),
			bRadiance ? _TL("Radiance") : lsat.band[iBand].thermal ? _TL("Temperature") : _TL("Reflectance")
		));

		if( lsat.band[iBand].thermal )
		{
			pOutput->Set_Unit(_TL("Kelvin"));
		}

		pOutput->Set_Description(sAll + sBand);
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
CSG_Grid * CLandsat_TOAR::Get_Band_Input(int iBand, int Sensor)
{
	switch( iBand )
	{
	case 0:	return( Parameters(PRM_IN("10"))->asGrid() );
	case 1:	return( Parameters(PRM_IN("20"))->asGrid() );
	case 2:	return( Parameters(PRM_IN("30"))->asGrid() );
	case 3:	return( Parameters(PRM_IN("40"))->asGrid() );
	}

	switch( Sensor )
	{
	case tm4: case tm5:
		switch( iBand )
		{
		case 4:	return( Parameters(PRM_IN("50"))->asGrid() );
		case 5:	return( Parameters(PRM_IN("60"))->asGrid() );
		case 6:	return( Parameters(PRM_IN("70"))->asGrid() );
		}
		break;

	case tm7:
		switch( iBand )
		{
		case 4:	return( Parameters(PRM_IN("50"))->asGrid() );
		case 5:	return( Parameters(PRM_IN("61"))->asGrid() );
		case 6:	return( Parameters(PRM_IN("62"))->asGrid() );
		case 7:	return( Parameters(PRM_IN("80"))->asGrid() );
		}
		break;
	}

	return( NULL );
}

//---------------------------------------------------------
CSG_Grid * CLandsat_TOAR::Get_Band_Output(int iBand, int Sensor)
{
	CSG_Grid	*pInput	= Get_Band_Input(iBand, Sensor);

	if( pInput )
	{
		CSG_Parameter	*pOutput	= NULL;

		switch( iBand )
		{
		case 0:	pOutput = Parameters(PRM_OUT("10"));	break;
		case 1:	pOutput	= Parameters(PRM_OUT("20"));	break;
		case 2:	pOutput	= Parameters(PRM_OUT("30"));	break;
		case 3:	pOutput	= Parameters(PRM_OUT("40"));	break;
		}

		switch( Sensor )
		{
		case tm4: case tm5:
			switch( iBand )
			{
			case 4:	pOutput	= Parameters(PRM_OUT("50"));	break;
			case 5:	pOutput	= Parameters(PRM_OUT("60"));	break;
			case 6:	pOutput	= Parameters(PRM_OUT("70"));	break;
			}
			break;

		case tm7:
			switch( iBand )
			{
			case 4:	pOutput	= Parameters(PRM_OUT("50"));	break;
			case 5:	pOutput	= Parameters(PRM_OUT("61"));	break;
			case 6:	pOutput	= Parameters(PRM_OUT("62"));	break;
			case 7:	pOutput	= Parameters(PRM_OUT("80"));	break;
			}
			break;
		}

		if( pOutput )
		{
			if( !pOutput->asGrid() )
			{
				CSG_Grid	*pGrid	= SG_Create_Grid(pInput);

				if( pGrid && pGrid->is_Valid() && pGrid->Get_System() == pInput->Get_System() )
				{
					pOutput->Set_Value(pGrid);
				}
				else
				{
					if( pGrid )
					{
						delete(pGrid);
					}

					return( NULL );
				}
			}

			pOutput->asGrid()->Set_Name(CSG_String::Format(SG_T("%s [%s]"), pInput->Get_Name(), _TL("Reflectance")));

			return( pOutput->asGrid() );
		}
	}

	return( NULL );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
